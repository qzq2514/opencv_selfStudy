#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//点集的表示形式:
	//1.N行2列大小的Mat，每一行代表一个点的坐标(x,y)
	//Mat points = (Mat_<float>(5, 2) << 190, 150, 70, 180, 40, 164, 50, 120, 270, 150);

	//2.vector<Point2f>类型或者vector<Point2>存放点集
	vector<Point2f> points;
	points.push_back(Point(190, 15));
	points.push_back(Point(70, 180));
	points.push_back(Point(40, 164));
	points.push_back(Point(50, 120));
	points.push_back(Point(270, 150));

	//3.点集存放在N行1列的双通道Mat中
	/*Mat points = (Mat_<Vec2f>(5, 1) << Vec2f(190, 150), Vec2f(70, 180), Vec2f(40, 164),
		                               Vec2f(50, 120), Vec2f(270, 150));*/


	//黑色面板上画出点，进行可视化
	Mat img = Mat::zeros(Size(400, 400), CV_8UC3);
	for (int i = 0; i < points.size(); i++)
	{
		Point p = points[i];
		img.at<Vec3b>(p.y, p.x) = Vec3b(255, 255, 255);
	}

	//minAreaRect-根据点集得到最小旋转矩形
	RotatedRect rRect = minAreaRect(points);
	cout << "最小旋转矩形中心:" << rRect.center << endl;
	cout << "最小旋转矩形角度:" << rRect.angle << endl;
	cout << "最小旋转矩形尺寸:" << rRect.size << endl;

	//计算出旋转矩形的四个顶点坐标
	Mat vertices;
	boxPoints(rRect, vertices);
	cout << vertices << endl;
	cout << "-------------------------" << endl;

	//画出最小旋转矩形
	for (int i = 0; i < 4; i++)
	{
		Point curP = vertices.row(i);
		cout << curP << endl;
		Point nextPoint= vertices.row((i+1)%4);
		line(img, curP, nextPoint, Scalar(255,0,0),2);
	}


	// minEnclosingCircle-根据点集得到最小外包圆形
	Point2f center;
	float radius;
	minEnclosingCircle(points, center, radius);//这里的points同样可以用三种点集形式
	circle(img, center, radius, Scalar(0, 255, 0), 2);   //画出最小外包矩形


	// boundingRect-根据点集得到最小外包直立矩形
	//Mat(points)形式是将vector<Point2f>点集转化成 一个N*1的三通道Mat
	//同样可以使用三种形式的点集得到最小外包直立矩形
	Rect rect = boundingRect(Mat(points));
	//Rect rect = boundingRect(points);   //一般的vector<Point2f>点集获得最小外包直立矩形

	rectangle(img, rect, Scalar(0, 0, 255), 2);

	imshow("最小外包点集", img);
	waitKey(0);
	return 0;
}