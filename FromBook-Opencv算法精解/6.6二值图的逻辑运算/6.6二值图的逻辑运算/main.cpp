#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
int main()
{

	Mat src1 = Mat::zeros(Size(200, 200), CV_8UC1);

	//使用CV_FILLED参数能够画出整个矩形，不然只是一个矩阵形框
	rectangle(src1,Rect(50,50,100,100),Scalar(255),CV_FILLED);  
	//src1(Rect(50, 50, 100, 100)).setTo(Scalar(255));//也可以使用setTo方式设置像素值
	imshow("src1", src1);

	Mat src2 = Mat::zeros(Size(200, 200), CV_8U);
	circle(src2, Point(150, 100), 50, Scalar(255), CV_FILLED);
	imshow("src2", src2);


	Mat mat_and;        
	bitwise_and(src1, src2, mat_and);    //两个大小相同的图按位进行与运算
	imshow("mat_and", mat_and);

	Mat mat_or;
	bitwise_or(src1, src2, mat_or);      //两个大小相同的图按位进行或运算
	imshow("mat_or", mat_or);

	waitKey(0);
	return 0;
}