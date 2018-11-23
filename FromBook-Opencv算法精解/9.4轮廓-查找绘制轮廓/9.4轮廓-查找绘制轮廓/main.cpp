#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src = imread("contours.png",0);

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

	src = src > 1;
	namedWindow("Source", 1);
	imshow("Source", src);

	//一个轮廓是点的集合，vector<Point>,而contours存放的是轮廓，即其是轮廓的集合，就是vector<vector<Point> >类型
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	//找到二值化图的边缘轮廓，轮廓集合放在contours，各个轮廓之间的层级关系放在hierarchy
	//第三个参数是提取轮廓的模式:
	/*
cv::RETR_EXTERNAL：表示只提取最外面的轮廓；
cv::RETR_LIST：表示提取所有轮廓并将其放入列表；全部在同一级
cv::RETR_CCOMP:表示提取所有轮廓并将组织成一个两层结构，其中顶层轮廓是外部轮廓，第二层轮廓是“洞”的轮廓；
cv::RETR_TREE：表示提取所有轮廓并组织成轮廓嵌套的完整层级结构。
详见"findContours-mode参数.png"
第四个参数是轮廓呈现方式，可忽略*/
	findContours(src, contours, hierarchy,
		RETR_TREE, CV_CHAIN_APPROX_SIMPLE);



	//hierarchy参数表示每个轮廓的层级关系，其是一个M*4的矩阵，M表示轮廓数量，每行的四个参数有以下意义:
	/*
第一个数:表示同一级轮廓的下个轮廓的编号,如果这一级轮廓没有下一个轮廓,一般是这一级轮廓的最后一个的时候,则为-1
第二个数:表示同一级轮廓的上个轮廓的编号,如果这一级轮廓没有上一个轮廓,一般是这一级轮廓的第一个的时候,则为-1
第三个数:表示该轮廓包含的下一级轮廓的第一个的编号,假如没有,则为-1
第四个数: 表示该轮廓的上一级轮廓的编号,假如没有上一级,则为-1*/
	//这里用一个vector<Vec4i>变量表示层级关系是符合的，具体可见"hierarchy参数详解.png"
	/*for (int i = 0; i < hierarchy.size(); i++)
	{
	cout << hierarchy[i] << endl;
	cout << "----------" << endl;
	}*/

	cout << "轮廓数量:" << contours.size() << endl;
	int idx = 0;
	//不断找出最外层轮廓并随机填充颜色，hierarchy[idx][0]即为不断找当前点的同级的下一个轮廓，返回-1表示没有当前层便轮廓遍历结束
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//画出轮廓(原图，轮廓集合，要画的轮廓id，画的颜色，画的方式-这里是填充，画框等级-缺省值2147483647)
		//画的方式thickness=-1表示颜色填充，否则表示画线条，此时表示线条粗细
		/*画框等级为:
		如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓。如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种*/
		cout << "idx:" << idx << endl;
		drawContours(dst, contours, idx, color, -1, 8, hierarchy,0);
	}
	//画出每个轮廓(每个轮廓是有序点的集合，直接依次连接每个相邻的点就行)
	Scalar colors[4] = { Scalar(0,0,255) ,Scalar(0,255,0) ,Scalar(255,0,0) ,Scalar(255,255,0) };
	for (int j = 0; j < contours.size(); j++)
	{
		int len = contours[j].size();
		for (int i = 0; i < len; i++)
		{
			Point start = contours[j][i];
			Point end = contours[j][(i + 1) % len];
			line(dst, start, end, colors[j]);
		}
	}
	
	namedWindow("Components", 1);
	imshow("Components", dst);
	waitKey(0);
}