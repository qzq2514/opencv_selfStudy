#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
int main()
{

	Mat src1 = Mat::zeros(Size(200, 200), CV_8UC1);

	//ʹ��CV_FILLED�����ܹ������������Σ���Ȼֻ��һ�������ο�
	rectangle(src1,Rect(50,50,100,100),Scalar(255),CV_FILLED);  
	//src1(Rect(50, 50, 100, 100)).setTo(Scalar(255));//Ҳ����ʹ��setTo��ʽ��������ֵ
	imshow("src1", src1);

	Mat src2 = Mat::zeros(Size(200, 200), CV_8U);
	circle(src2, Point(150, 100), 50, Scalar(255), CV_FILLED);
	imshow("src2", src2);


	Mat mat_and;        
	bitwise_and(src1, src2, mat_and);    //������С��ͬ��ͼ��λ����������
	imshow("mat_and", mat_and);

	Mat mat_or;
	bitwise_or(src1, src2, mat_or);      //������С��ͬ��ͼ��λ���л�����
	imshow("mat_or", mat_or);

	waitKey(0);
	return 0;
}