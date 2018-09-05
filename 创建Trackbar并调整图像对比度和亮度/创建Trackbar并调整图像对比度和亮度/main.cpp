#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int g_nContrastValue;
int g_nBrightValue;
Mat g_srcImage, g_dstImage;

void ContrastAndBright(int, void*)    //回调函数原型必须是void XXXX(int,void*)
{
	namedWindow("原图Image", 1);
	for (int x=0;x<g_srcImage.rows;x++)
	{
		for (int y = 0; y < g_srcImage.cols; y++)
		{
			for (int c = 0; c < 3; c++)
			{
				//改变图片每个位置上每个通道的像素，对比度作为系数，亮度为偏差
				g_dstImage.at<Vec3b>(x, y)[c] = saturate_cast<uchar>((g_nContrastValue*0.01)*g_srcImage.at<Vec3b>(x, y)[c] + g_nBrightValue);
			}
		}
	}

	imshow("原图Image", g_srcImage);
	imshow("效果图Image", g_dstImage);
}

int main()
{
	g_srcImage = imread("lena_full.jpg");
	if (!g_srcImage.data) { cout << "找不到lena_full.jpg图片" << endl; return false; }
	g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());   //定义和原图同样大小的图片给，用于存放效果图

	g_nContrastValue = 80;
	g_nBrightValue = 80;             //定义对比度和亮度初始值

	namedWindow("效果图Image", 1);   //WINDOW_AUTOSIZE=1,不能改变窗口大小


	
	//createTrackbar参数依次表示Trackbar名称和所处的窗口名称，然后表示bar上当前值的变量和最大值，最后是回调函数
	//会吧当前值变量传入回调函数中，这里传入当前值的变量时，要使用引用传递，后面拉动tracbar, 就会跟着改变变量值
	createTrackbar("对比度:","效果图Image",&g_nContrastValue,300,ContrastAndBright);
	createTrackbar("亮  度:", "效果图Image", &g_nBrightValue, 200, ContrastAndBright);
	
	ContrastAndBright(g_nContrastValue, 0);  //要有这两句中至少一句，不然没有，即一开始不会显示图片，除非改变trackbar才会显示图片
	//ContrastAndBright(g_nBrightValue, 0);
	
	waitKey();
	return 0;
}