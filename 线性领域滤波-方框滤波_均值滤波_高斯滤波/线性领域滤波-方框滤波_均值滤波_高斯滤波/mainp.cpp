#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>           //滤波必须引用
#include<iostream>

using namespace cv;
using namespace std;

Mat image, dstImage1, dstImage2, dstImage3;
int kn_box=3, kn_mean=3, kn_gaussian = 3;
static void on_BoxFilter(int, void*);
static void on_MeanBlur(int, void*);
static void on_GaussianBlur(int, void*);

//具体理论见算法笔记里的图片
int main()
{
	image = imread("lena_full.jpg");
	

	dstImage1 = image.clone();              //克隆三个原图备份
	dstImage2 = image.clone();
	dstImage3 = image.clone();

	namedWindow("原图窗口",1);
	imshow("原图窗口",image);

	//1.方框滤波
	namedWindow("方框滤波",1);
	createTrackbar("内核值","方框滤波",&kn_box,40,on_BoxFilter);   //40是核宽高的最大值
	on_BoxFilter(kn_box, 0);

	//2.均值滤波
	namedWindow("均值滤波", 1);
	createTrackbar("内核值", "均值滤波", &kn_mean, 40, on_MeanBlur);   //40是核宽高的最大值
	on_MeanBlur(kn_mean, 0);

	//3.高斯滤波
	namedWindow("高斯滤波", 1);
	createTrackbar("内核值", "高斯滤波", &kn_gaussian, 40, on_GaussianBlur);   //40是核宽高的最大值
	on_GaussianBlur(kn_gaussian, 0);

	waitKey();
	return 0;
}
static void on_BoxFilter(int, void*)
{
	//原图，输出图片，输出图像深度，核大小，锚点(即被平滑的那个点,(-1,-1)表示中心点)
	boxFilter(image,dstImage1,-1,Size(kn_box+1, kn_box + 1),Point(-1,-1)); //核宽高都加一，因为TrackBar从0开始，而核宽高必须从1开始
	imshow("方框滤波",dstImage1);
}

static void on_MeanBlur(int, void*)
{
	//原图，输出图片，核大小，锚点(即被平滑的那个点,(-1,-1)表示中心点)
	blur(image, dstImage2, Size(kn_mean + 1, kn_mean + 1), Point(-1, -1));
	imshow("均值滤波", dstImage2);
}

static void on_GaussianBlur(int, void*)
{
	//原图，输出图片，核大小，锚点(即被平滑的那个点,(-1,-1)表示中心点)
	GaussianBlur(image, dstImage3, Size(kn_gaussian *2 + 1, kn_gaussian *2 + 1), 0,0);
	//高斯核的宽高可以不同，但必须为奇数，第4,5个w为高斯函数在x,y方向的偏差
	imshow("高斯滤波", dstImage3);
}
