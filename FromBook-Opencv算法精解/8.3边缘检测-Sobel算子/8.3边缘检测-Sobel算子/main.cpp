#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;

//计算阶乘，为后面计算高斯算子做准备
int factorial(int n)
{
	int fac = 1;
	if (n == 0) return fac;
	for (int i = 1; i <= n; i++)
		fac *= i;
	return fac;
}

//长度为n的非归一化的高斯平滑算子，用n-1的二项式展开系数来计算，详见书P144
Mat getPascalSmooth(int n)
{
	Mat pascalSmooth = Mat::zeros(Size(n, 1), CV_32FC1);
	for (int i = 0; i <= n-1; i++)
	{
		//计算n-1二项展开式的系数
		int v = factorial(n - 1) / (factorial(i)*factorial(n - 1 - i));
		pascalSmooth.at<float>(i) = v;
	}
	return pascalSmooth;//非归一化的高斯平滑算子(!!归一化的高斯算子则要其中每个元素除以2的n-1次方)
}
Mat getPascalDiff(int n)
{
	Mat pascalDiff = Mat::zeros(Size(n, 1), CV_32FC1);

	//计算长度为n的差分算子计算过程如下:
	//1.先计算长度为n-1的高斯算子
	Mat pascalSmooth_pre = getPascalSmooth(n-1);

	//2.将高斯算子两侧补0，然后后向差分，计算相邻元素差值作为差分算子的值
	for (int i = 0; i < n; i++)
	{
		if (i == 0)
			pascalDiff.at<float>(i) = 1;
		else if(i == n - 1)
			pascalDiff.at<float>(i) = -1;
		else
			pascalDiff.at<float>(i) = pascalSmooth_pre.at<float>(i) - pascalSmooth_pre.at<float>(i - 1);
			
	}
	return pascalDiff;
}

void conv2D(Mat src, Mat &dst, Mat kernel, int ddepth, Point anchor = Point(-1, -1),
	int borderType = BORDER_DEFAULT)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1);
	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType);
}

void sepConv2D_Y_X(Mat src, Mat &dst, int ddepth, Mat kernelY, Mat kernelX,
	Point anchor = Point(-1, -1), int bordertype = BORDER_DEFAULT)
{
	Mat src_kerY;
	//使用离散卷积的性质，分别在y方向和x方向进行卷积
	conv2D(src, src_kerY, kernelY, ddepth, anchor, bordertype);
	conv2D(src_kerY, dst, kernelX, ddepth, anchor, bordertype);
}

Mat sobel(Mat src, int ddepth, int x , int y , int winSize,int bordertype)
{
	Mat dst;
	CV_Assert(winSize>=3 && winSize%2 == 1);   //保证卷积核大小为大于等于3的奇数

	//得到指定长度的高斯算子和差分算子-都是1行winSize行
	Mat pascalDiff = getPascalDiff(winSize);
	Mat pascalSmooth = getPascalSmooth(winSize);

	if (x != 0 && y == 0)
	{   //进行垂直方向上的检测-即Sobel垂直边缘检测
		//先进行垂直方向的平滑(非归一化的高斯算子)，然后水平方向的差分
		sepConv2D_Y_X(src, dst, ddepth, pascalSmooth.t(), pascalDiff, Point(-1, -1), bordertype);
	}
	else
	{
		//进行水平方向上的检测-即Sobel水平边缘检测
		//先进行水平方向的平滑(非归一化的高斯算子)，然后垂直方向的差分
		sepConv2D_Y_X(src, dst, ddepth, pascalSmooth, pascalDiff.t(), Point(-1, -1), bordertype);
	}
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	//Sobel和Prewitt都是水平和垂直方向的边缘检测,并且两种边缘检测的卷积核都是可分离的,可以分离为平滑算子和差分算子
	//不同点在于Prewitt是用非归一化的均值平滑来平滑图片，而Sobel则用非归一化的高斯算子对图片进行平滑
	//后面的差分算子是一样的
	Mat img_Sobel_x, abs_img_Sobel_x;
	img_Sobel_x = sobel(src, CV_32FC1, 1, 0, 3, BORDER_DEFAULT);
	convertScaleAbs(img_Sobel_x, abs_img_Sobel_x, 1.0, 0);
	imshow("垂直方向边缘检测", abs_img_Sobel_x);

	Mat img_Sobel_y, abs_img_Sobel_y;
	img_Sobel_y = sobel(src, CV_32FC1, 0, 1, 3, BORDER_DEFAULT);
	convertScaleAbs(img_Sobel_y, abs_img_Sobel_y, 1.0, 0);
	imshow("水平方向边缘检测", abs_img_Sobel_y);

	//这里使用另一种边缘强度的计算-插值法，就是对不同方向卷积的绝对值的加权求和
	Mat edge;
	addWeighted(abs_img_Sobel_x, 0.5, abs_img_Sobel_y, 0.5, 0, edge);
	edge.convertTo(edge, CV_8UC1);
	imshow("edge", edge);

	//opencv自带的Sobel边缘检测
	Mat dstOpencv;
	//Sobel(原图, 目标图, ddepth, dx, dy, ksize, 放缩系数, 平移系数, BORDER_DEFAULT);
	//dx!=0 :水平检测
	//dx=0.dy!=0：垂直检测
	Sobel(src, dstOpencv, -1, 1, 0, 3, 1.0, 0.0, BORDER_DEFAULT);
	imshow("dstOpencv", dstOpencv);   
	//这里一个问题，只能检测到矩形框的右边和下边，具体要到源码里看

	waitKey(0);
	return 0;
}