#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;


int main()
{
	//点集准备和可视化
	vector<Point> points;
	points.push_back(Point(0, 0));
	points.push_back(Point(50, 30));
	points.push_back(Point(100, 0));
	points.push_back(Point(100, 100));

	Mat canvas = Mat::zeros(Size(200, 200), CV_8U);
 
	//可以通过首尾相连的方式把轮廓画出来
	/*int len = points.size();
	for (int i = 0; i <len; i++)
	{
		line(canvas, points[i], points[(i + 1) % len], Scalar(255), 1);
	}*/
	
	//也可以用opencv自带的polylines画点集构成的多边形
	//polylines(画布,点集,是否闭集，颜色线条粗度)
	polylines(canvas, points, true, Scalar(255), 1);
	
	//突然想到使用drawContours或者fillPoly画出自定义轮廓，可以运行~
	//vector<vector<Point> >contours;
	//contours.push_back(points);
	//drawContours(canvas, contours, 0, Scalar(255), -1);
	//fillPoly(canvas, contours, Scalar(255));    //该句与上局相互替代
	
	//1.轮廓周长和面积
	//arcLength(点集，是否闭合)，contourArea(点集)
	//同样，点集可以有三种形式:元素为Point的vector,N*2的单通道Mat，N*1的双通道Mat
	double length1 = arcLength(points, false);
	double length2 = arcLength(points, true);
	double area = contourArea(points);
	cout << "首尾不相连轮廓周长：" << length1 << endl;
	cout << "首尾相连轮廓周长：" << length2 << endl;
	cout << "轮廓包围区域面积:" << area << endl;
	cout << "---------------------------" << endl;

	//2.点和轮廓的关系-pointPolygonTest函数
	vector<Point> testPoints;
	testPoints.push_back(Point(70, 40));
	testPoints.push_back(Point(100, 50));
	testPoints.push_back(Point(40, 80));
	for (int i = 0; i < testPoints.size(); i++)
	{
		circle(canvas, testPoints[i], 3, Scalar(255), CV_FILLED);

		//pointPolygonTest(代表轮廓的点集,测试点,是否计算坐标点到轮廓距离-measureDist)
		//measureDist=false时，返回值-1,0,1分别表示测试点在轮廓外、轮廓上、轮廓内
		//measureDist=true时，返回值正负仍然定性表示测试点与轮廓位置关系，绝对值表示测试点到最近轮廓的距离
		double dist=pointPolygonTest(points, testPoints[i], true);
		cout << "dist " << i << ":" << dist << endl;
	}
	cout << "---------------------------" << endl;

	//3.凸包缺陷
	vector<Point> contour;
	contour.push_back(Point(20, 20));
	contour.push_back(Point(50, 70));
	contour.push_back(Point(20, 120));
	contour.push_back(Point(120, 120));
	contour.push_back(Point(100, 70));
	contour.push_back(Point(120, 20));
	//计算轮廓的凸包
	vector<int> hull;
	//convexHull计算轮廓的凸包，其中之前用过的convexHull第二个参数类型为vector<Point>表示构成凸包的点
	//但是这里第二个参数表示构成凸包的点在原轮廓中的下标
	convexHull(contour, hull, false, true);
	//计算凸包的缺陷
	vector<Vec4i> defects;
    //convexityDefects(原轮廓，凸包点下标集合，缺陷信息)
	//defects每个元素是一个长度为4的向量分别缺陷起点、缺陷终点、最远点的索引及其到凸包的距离(还要除以256)
	convexityDefects(contour, hull, defects);
	//打印凸包的缺陷
	for (int i = 0; i < defects.size(); i++)
	{
		cout << defects[i] << endl;
	}

	imshow("canvas", canvas);
	waitKey(0);
	return 0;
}