#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

enum METHOD { MEAN, GAUSS, MEDIAN };

Mat adaptiveThresh(Mat src, int radius, float ratio, METHOD method = MEAN)
{
	Mat smooth;
	switch (method)
	{
	case MEAN:     //均值平滑
		boxFilter(src, smooth, CV_32FC1, Size(2 * radius + 1, 2 * radius + 1));
		break;
	case GAUSS:     //高斯平滑
		GaussianBlur(src, smooth, Size(2 * radius + 1, 2 * radius + 1), 0, 0);
		break;
	case MEDIAN:     //中值平滑
		medianBlur(src, smooth, 2 * radius + 1);
		break;
	default:
		break;
	}
	//cout << src.size() << endl;
	//cout << smooth.size() << endl;

	src.convertTo(src, CV_32FC1);
	smooth.convertTo(smooth, CV_32FC1);   //都转为float类型，便于后面乘以系数float类型的ratio
										  //(1.0 - ratio)*smooth是阈值矩阵(其与原图大小相同，然后原图每个像素与阈值矩阵对应的元素值进行比较)
	Mat diff = src - (1.0 - ratio)*smooth;

	Mat out = Mat::zeros(diff.size(), CV_8UC1);

	for (int r = 0; r < out.rows; r++)
	{
		for (int c = 0; c < out.cols; c++)
		{
			if (diff.at<float>(r, c) > 0)      //原图某位置灰度值大于阈值矩阵对应的灰度值
				out.at<uchar>(r, c) = 255;
		}
	}
	return out;
}
int main()
{
	Mat src = imread("adaptive.jpg", 0);
	imshow("src", src);

	//当图片的各部分光照相过不均匀，使用全局阈值分割效果不好，
	//例如下面使用强制的全局阈值分割，无论阈值设置为多少，效果都不好
	/*Mat dst;
	threshold(src, dst, 130, 255, THRESH_BINARY);
	imshow("dst", dst);*/

	//对于光照不明显的照片，使用局部阈值分割效果较好，每个像素都有一个对应的阈值
	Mat myDst;
	//使用自适应局部阈值分割，其中平滑算子与采用均值算子类似，当尺寸较小时(5x5,7x7)效果都不好
	//随着卷积核尺寸变大，效果就变好，分割出的前景物体也越来越完整
	myDst = adaptiveThresh(src, 50, 0.15, MEDIAN);
	imshow("myDst", myDst);

	Mat dst2;
	//系统自带的自适应阈值分割
	//原图(仅限CV_8U)，目标图，最大值，平滑方式(只有均值平滑和高斯平滑)，二值化方式,平滑算子尺寸(奇数),比例系数(ratio)
	adaptiveThreshold(src, dst2, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY,101,0.15);
	imshow("dst2", dst2);

	waitKey(0);
	return 0;
}