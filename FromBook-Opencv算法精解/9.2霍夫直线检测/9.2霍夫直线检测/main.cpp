#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<map>

using namespace std;
using namespace cv;

//霍夫直线检测核心代码
//霍夫空间函数关系为:rho=x*cos(theta)+y*sin(theta),
//假定rho为原点到原笛卡尔坐标系中经过(x,y)点直线的垂线距离(书中假定长度在一二象限为正，三四象限为负,范围为[-L,L]，其中L为固定点(x,y)到原点的距离)
//theta角度为垂线和x轴正向夹角更好(0-360度)
//霍夫函数就表示经过固定点的直线的rho和theta的关系
map<vector<int>, vector<Point>> HoughLine(Mat I, Mat &accumulator, float stepTheta = 1, float stepRho = 1)
{
	int rows = I.rows;
	int cols = I.cols;
	//可能出现的最大的出现距离，
	int L = round(sqrt(pow(rows - 1, 2.0) + pow(cols - 1, 2.0))) + 1;

	//初始化投票器accumulator，每个坐标的值表示在霍夫空间内有多少个点的霍夫曲线经过该坐标
	//其横坐标为theta,纵坐标为垂线长度
	//但是我觉得垂线就为长度(恒正)，角度为垂线和x轴正向夹角更好(0-360度)
	int numTheta = int(180.0 / stepTheta);
	int numRho = int(2 * L / stepRho + 1);
	accumulator = Mat::zeros(Size(numTheta, numRho), CV_32SC1);

	//初始化map,用于存储共线的点
	map<vector<int>, vector<Point>> lines;

	for (int i = 0; i <numRho; i++)
	{
		for (int j = 0; j <numTheta; j++)
		{
			//原本vector<int>(j, i):创建一个vector，元素个数为j,且值均为i
			//但是这里j,i分别表示霍夫空间中theta和垂线长度，也就是vector<int>(j, i)就表示了一个霍夫空间的坐标
			//vector<Point>()表示霍夫曲线经过(j, i)点的边缘点集合
			//换个角度，就是霍夫空间中(j, i)代表的直线经过的边缘点集合
			lines.insert(make_pair(vector<int>(j, i), vector<Point>()));
		}
	}

	//投票计数
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			//对每一个边缘点找到其霍夫空间的(角度，垂线长度)的坐标对集合
			if (I.at<uchar>(Point(x, y)) == 255)
			{
				for (int m = 0; m < numTheta; m++)
				{
					//对每个角度，计算垂线长度,注意角度要变成弧度制
					//这里根据书上说的，可以为负值
					float rho = x*(cos(stepTheta*m / 180.0 * CV_PI)) + y*(sin(stepTheta*m / 180.0*CV_PI));
					//计算投票到哪个(角度，垂线长度)坐标对
					//即该图像的某个边缘点(x,y)的霍夫曲线经过该点(m,n)
					//因为rho在[-L,L]之间，而我们定义的vector<int>(j, i)中j,i根据其原本的C++意义都是整数
					//所以要加个L
					int n = int(round(rho + L) / stepRho);
					accumulator.at<int>(n, m) += 1;
					lines.at(vector<int>(m, n)).push_back(Point(x, y));
				}
			}
		}
	}
	return lines;
}
Mat drawLines(Mat src, Mat accu, map<vector<int>, vector<Point>> lines, int vote = 200)
{
	//在霍夫空间内，画出至少穿过vote个点的直线
	Mat showHough = src.clone();
	for (int r = 0; r < accu.rows - 1; r++)
	{
		for (int c = 0; c < accu.cols - 1; c++)
		{
			int current = accu.at<int>(r, c);
			if (current > vote)
			{
				int lt = accu.at<int>(r - 1, c - 1);//左上
				int t = accu.at<int>(r - 1, c);//正上
				int rt = accu.at<int>(r - 1, c + 1);//右上
				int l = accu.at<int>(r, c - 1);//左
				int right = accu.at<int>(r, c + 1);//右
				int lb = accu.at<int>(r + 1, c - 1);//左下
				int b = accu.at<int>(r - 1, c);//下
				int rb = accu.at<int>(r + 1, c + 1);//下
													//判断该位置是不是局部最大值,保证一条直线边缘上就画一一条直线就行，不要重复描线
				if (current > lt&& current > t && current > rt
					&&current > l && current > right&&
					current > lb && current > b&& current > rb)
				{
					vector<Point> line = lines.at(vector<int>(c, r));
					int len = line.size();
					cv::line(showHough, line[0], line[len - 1], Scalar(255), 0);
				}
			}
		}
	}
	return showHough;
}

int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	//霍夫直线检测前先进行边缘检测
	Mat edge;
	Canny(src, edge, 50, 180, 3, true);
	cout << edge.type() << endl;     //CV_8UC1
	imshow("edge", edge);

	//开始霍夫直线检测
	Mat accu;   //投票器
	map<vector<int>, vector<Point>> lines;
	lines = HoughLine(edge, accu);

	double maxValue;
	minMaxLoc(accu, NULL, &maxValue, NULL, NULL);
	Mat grayAccu;
	accu.convertTo(grayAccu, CV_32FC1, 1 / maxValue);   //grayAccu是CV_32FC1，范围[0,1]符合要求


	grayAccu.convertTo(grayAccu, CV_8UC1, 255);    //此时grayAccu变为CV_8UC1，范围[0,255]符合要求
	grayAccu = grayAccu.t();
	imshow("投票器灰度级显示", grayAccu);
	imwrite("投票器灰度级显示.jpg", grayAccu);

	Mat showHough = drawLines(src, accu, lines, 150);
	imshow("霍夫直线检测", showHough);
	waitKey(0);

	/*map<vector<int>, int> lines;
	lines.insert(make_pair(vector<int>(1, 2), 2));
	lines.insert(make_pair(vector<int>(3, 4), 4));
	lines.insert(make_pair(vector<int>(3, 5), 5));
	lines.insert(make_pair(vector<int>(9, 2), 7));
	cout << lines.at(vector<int>(9, 2)) << endl;

	system("pause");*/
	return 0;
}