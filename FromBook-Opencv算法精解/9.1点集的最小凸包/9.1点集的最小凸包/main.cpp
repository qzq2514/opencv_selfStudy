#include <iostream>
#include<cstdlib>
#include <opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<time.h> 

using namespace std;
using namespace cv;


int main()
{
	//最小凸包
	//随机产生40个坐标点
	RNG rnger(cv::getTickCount());
	int N=20;
	Mat points(Size(2, N), CV_32FC1);//点最好都用float类型
	rnger.fill(points, RNG::UNIFORM, cv::Scalar::all(100), cv::Scalar::all(300));


	//展示点集
	Mat canv = Mat::zeros(Size(400, 400), CV_8UC1);
	for (int n = 0; n < N; n++)
	{
		float x = points.at<float>(n, 0);
		float y = points.at<float>(n, 1);
		Point2f p(x, y);
		circle(canv, p, 1, Scalar(255), CV_FILLED);
	}

	//得到凸包点集(即最外侧的点)
	vector<Point2f> hull;
	convexHull(points,hull);
	int len = hull.size();
	for (int i = 0; i < len; i++)
	{
		Point curP = hull[i];
		Point nextP = hull[(i+1)% len];
		line(canv, curP, nextP, Scalar(255), 1);
	}
	imshow("canv", canv);

	cout << "-----------------------------------" << endl;

	//最小外包三角形
	Mat points2 = (Mat_<float>(5, 2) << 190, 150, 70, 180, 40, 164, 50, 120, 270, 150);
	cout << points2.type() << endl;  //CV_32FC1
	points2=points2.reshape(2, 5);   //reshape对Mat矩阵进行重塑，变成2通道，行数为5的双通道Mat(列数自动计算为1)
	cout << points2.type() << endl;  //CV_32FC2


	Mat canv2 = Mat::zeros(Size(400, 400), CV_8UC1);
	for (int n = 0; n < points2.rows; n++)
	{
		Vec2f vec=points2.at<Vec2f>(n, 0);
		cout << vec << endl;

		Point2f p(vec[0], vec[1]);
		circle(canv2, p, 1, Scalar(255), CV_FILLED);
	}

	//保存最小外包三角形的三个顶点
	vector<Point2f> trangles;
	//minEnclosingTriangle找到最小外包三角形，并返回该三角形面积
	//和之前最小外包旋转，直立矩形和圆不同，minEnclosingTriangle接收的点集只有N*1的双通道Mat
	//和Vector<Point>,Vector<Point2f>，不接收N*2的单通道Mat
	double area=minEnclosingTriangle(points2, trangles);
	cout << area << endl;

	int len2 = trangles.size();
	for (int i = 0; i < len2; i++)
	{
		Point curP = trangles[i];
		Point nextP = trangles[(i + 1) % len2];
		line(canv2, curP, nextP, Scalar(255), 1);
	}

	imshow("canv2", canv2);
	waitKey(0);
	return 0;
}