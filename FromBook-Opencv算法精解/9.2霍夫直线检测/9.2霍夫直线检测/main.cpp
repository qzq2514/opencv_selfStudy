#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<map>

using namespace std;
using namespace cv;

//����ֱ�߼����Ĵ���
//����ռ亯����ϵΪ:rho=x*cos(theta)+y*sin(theta),
//�ٶ�rhoΪԭ�㵽ԭ�ѿ�������ϵ�о���(x,y)��ֱ�ߵĴ��߾���(���мٶ�������һ������Ϊ������������Ϊ��,��ΧΪ[-L,L]������LΪ�̶���(x,y)��ԭ��ľ���)
//theta�Ƕ�Ϊ���ߺ�x������нǸ���(0-360��)
//�������ͱ�ʾ�����̶����ֱ�ߵ�rho��theta�Ĺ�ϵ
map<vector<int>, vector<Point>> HoughLine(Mat I, Mat &accumulator, float stepTheta = 1, float stepRho = 1)
{
	int rows = I.rows;
	int cols = I.cols;
	//���ܳ��ֵ����ĳ��־��룬
	int L = round(sqrt(pow(rows - 1, 2.0) + pow(cols - 1, 2.0))) + 1;

	//��ʼ��ͶƱ��accumulator��ÿ�������ֵ��ʾ�ڻ���ռ����ж��ٸ���Ļ������߾���������
	//�������Ϊtheta,������Ϊ���߳���
	//�����Ҿ��ô��߾�Ϊ����(����)���Ƕ�Ϊ���ߺ�x������нǸ���(0-360��)
	int numTheta = int(180.0 / stepTheta);
	int numRho = int(2 * L / stepRho + 1);
	accumulator = Mat::zeros(Size(numTheta, numRho), CV_32SC1);

	//��ʼ��map,���ڴ洢���ߵĵ�
	map<vector<int>, vector<Point>> lines;

	for (int i = 0; i <numRho; i++)
	{
		for (int j = 0; j <numTheta; j++)
		{
			//ԭ��vector<int>(j, i):����һ��vector��Ԫ�ظ���Ϊj,��ֵ��Ϊi
			//��������j,i�ֱ��ʾ����ռ���theta�ʹ��߳��ȣ�Ҳ����vector<int>(j, i)�ͱ�ʾ��һ������ռ������
			//vector<Point>()��ʾ�������߾���(j, i)��ı�Ե�㼯��
			//�����Ƕȣ����ǻ���ռ���(j, i)�����ֱ�߾����ı�Ե�㼯��
			lines.insert(make_pair(vector<int>(j, i), vector<Point>()));
		}
	}

	//ͶƱ����
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			//��ÿһ����Ե���ҵ������ռ��(�Ƕȣ����߳���)������Լ���
			if (I.at<uchar>(Point(x, y)) == 255)
			{
				for (int m = 0; m < numTheta; m++)
				{
					//��ÿ���Ƕȣ����㴹�߳���,ע��Ƕ�Ҫ��ɻ�����
					//�����������˵�ģ�����Ϊ��ֵ
					float rho = x*(cos(stepTheta*m / 180.0 * CV_PI)) + y*(sin(stepTheta*m / 180.0*CV_PI));
					//����ͶƱ���ĸ�(�Ƕȣ����߳���)�����
					//����ͼ���ĳ����Ե��(x,y)�Ļ������߾����õ�(m,n)
					//��Ϊrho��[-L,L]֮�䣬�����Ƕ����vector<int>(j, i)��j,i������ԭ����C++���嶼������
					//����Ҫ�Ӹ�L
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
	//�ڻ���ռ��ڣ��������ٴ���vote�����ֱ��
	Mat showHough = src.clone();
	for (int r = 0; r < accu.rows - 1; r++)
	{
		for (int c = 0; c < accu.cols - 1; c++)
		{
			int current = accu.at<int>(r, c);
			if (current > vote)
			{
				int lt = accu.at<int>(r - 1, c - 1);//����
				int t = accu.at<int>(r - 1, c);//����
				int rt = accu.at<int>(r - 1, c + 1);//����
				int l = accu.at<int>(r, c - 1);//��
				int right = accu.at<int>(r, c + 1);//��
				int lb = accu.at<int>(r + 1, c - 1);//����
				int b = accu.at<int>(r - 1, c);//��
				int rb = accu.at<int>(r + 1, c + 1);//��
													//�жϸ�λ���ǲ��Ǿֲ����ֵ,��֤һ��ֱ�߱�Ե�Ͼͻ�һһ��ֱ�߾��У���Ҫ�ظ�����
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

	//����ֱ�߼��ǰ�Ƚ��б�Ե���
	Mat edge;
	Canny(src, edge, 50, 180, 3, true);
	cout << edge.type() << endl;     //CV_8UC1
	imshow("edge", edge);

	//��ʼ����ֱ�߼��
	Mat accu;   //ͶƱ��
	map<vector<int>, vector<Point>> lines;
	lines = HoughLine(edge, accu);

	double maxValue;
	minMaxLoc(accu, NULL, &maxValue, NULL, NULL);
	Mat grayAccu;
	accu.convertTo(grayAccu, CV_32FC1, 1 / maxValue);   //grayAccu��CV_32FC1����Χ[0,1]����Ҫ��


	grayAccu.convertTo(grayAccu, CV_8UC1, 255);    //��ʱgrayAccu��ΪCV_8UC1����Χ[0,255]����Ҫ��
	grayAccu = grayAccu.t();
	imshow("ͶƱ���Ҷȼ���ʾ", grayAccu);
	imwrite("ͶƱ���Ҷȼ���ʾ.jpg", grayAccu);

	Mat showHough = drawLines(src, accu, lines, 150);
	imshow("����ֱ�߼��", showHough);
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