#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

Mat calGrayHist(Mat src)
{
	Mat hist = Mat::zeros(Size(256, 1), CV_32SC1);

	int rows = src.rows;
	int cols = src.cols;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int pixelVal = (int)src.at<uchar>(r, c);
			hist.at<int>(pixelVal) += 1;
		}
	}
	return hist;
}

int otsuThreshold(Mat src)
{
	Mat histogram = calGrayHist(src);

	Mat normHist;

	//计算归一化灰度直方图，存放每个灰度值的概率
	histogram.convertTo(normHist, CV_32FC1, 1.0 / src.total(), 0.0); 

	//计算累加直方图(零阶累积矩)和一阶累加矩-都是1行256列
	Mat zeroCumnMoment = Mat::zeros(Size(256, 1), CV_32FC1);
	Mat oneCumnMoment = Mat::zeros(1,256, CV_32FC1);

	for (int i = 0; i < 256; i++)
	{
		if (i == 0) 
		{
			zeroCumnMoment.at<float>(i) = normHist.at<float>(i);
			//此时oneCumnMoment其实就是0，保证形式统一，所以这样写
			oneCumnMoment.at<float>(i) = i*normHist.at<float>(i);  
		}
		else
		{
			zeroCumnMoment.at<float>(i) = zeroCumnMoment.at<float>(i-1) + normHist.at<float>(i);
			oneCumnMoment.at<float>(i) = oneCumnMoment.at<float>(i-1) + i*normHist.at<float>(i);
		}
	}

	//计算类间方差
	Mat variance = Mat::zeros(1, 256, CV_32FC1);
	float mean = oneCumnMoment.at<float>(255);   //其实就是整张图片的平均灰度值
	//cout << "mean:" << mean << endl;

	for (int i = 0; i < 255; i++)
	{
		float zeroCumnMomentt_i = zeroCumnMoment.at<float>(i);
		float oneCumnMoment_i = oneCumnMoment.at<float>(i);

		//排除分母为0的情况
		if (zeroCumnMomentt_i == 0 || zeroCumnMomentt_i == 1)
			variance.at<float>(i) == 0;
		else
		{
			float confficient = zeroCumnMomentt_i*(1.0 - zeroCumnMomentt_i);   //公式中的分母
			variance.at<float>(i) = pow(mean*zeroCumnMomentt_i - oneCumnMoment_i, 2.0) / confficient;
		}
	}

	Point pointThresh;
	minMaxLoc(variance, NULL, NULL, NULL, &pointThresh);
	int otsuThresh = pointThresh.x;
	return otsuThresh;
}

int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	int myThresh = otsuThreshold(src);
	cout << "myThresh:" << myThresh << endl;
	Mat myDst;   //找到阈值后，就使用自带的threshold阈值分割函数即可
	threshold(src, myDst, myThresh, 255, THRESH_BINARY);
	imshow("myDst", myDst);


	
	Mat dst;
	double thresh = 0;
	//使用自带的阈值分割函数,通过指定参数：THRESH_OTSU + THRESH_BINARY即可达到ostu自动选取全局阈值
	//系统自带的OTSU阈值分割只支持8位图
	thresh=threshold(src, dst, thresh, 255, THRESH_OTSU + THRESH_BINARY);
	cout << "thresh:" << thresh << endl;     //看出自己写的和系统自带的otsu阈值分割算出来的阈值是一样的
	imshow("dst", dst);

	waitKey(0);
	return 0;
}