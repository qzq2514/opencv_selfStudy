#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
Mat myMedianBlur(Mat src, Size size)
{
	CV_Assert(src.type() == CV_8UC1);
	int kH = size.height;
	int kW = size.width;
	CV_Assert(kH % 2 == 1 && kW % 2 == 1);

	int hh = (kH - 1) / 2;
	int ww = (kW - 1) / 2;

	Mat padding;
	copyMakeBorder(src, padding, hh, hh, ww, ww, BORDER_REFLECT_101);

	int rows = src.rows;
	int cols = src.cols;

	int r = 0,c = 0,midInd=(kW*kH-1)/2;
	Mat medianImg=Mat(src.size(),CV_8UC1);

	for (int h = hh; h < hh + rows; h++)
	{
		for (int w = ww; w < ww + cols; w++)
		{
			//获得当前中值卷积核框住的原图位置,注意要克隆clone
			Mat curRegion = padding(Rect(w - ww, h - hh, kW, kH)).clone();  

			//1,1分别表示reshape后新图像的新通道数和新行数
			//这里把卷积核框住的区域reshape成一个行向量
			curRegion = curRegion.reshape(1, 1);

			//对行向量进行排序
			cv::sort(curRegion, curRegion, CV_SORT_EVERY_ROW);
			uchar midVal = curRegion.at<uchar>(midInd); //取中值
			medianImg.at<uchar>(r, c) = midVal;
			c++;
		}
		r++;
		c = 0;
	}
	return medianImg;
}
int main()
{
	//使用Release运行，因为split函数在Release下使用vector<Mat>+resize或者 Mat planes[3]
	Mat src = imread("lena_full.jpg", 0);

	//给原图像添加椒盐噪声(即图像在传输系统中由于解码误差等原因，导致图像中出现孤立的白点或者黑点)
	Mat noiseImg= src.clone();
	int noiseNum = 4000;
	for (int n = 1; n <= noiseNum; n++)
	{
		int randR = rand() % noiseImg.rows;
		int randC = rand() % noiseImg.cols;
		noiseImg.at<uchar>(randR, randC) = 0;
	}

	//对图像进行中值平滑，消除图像中存在的椒盐噪声
	Mat myMedianImg = myMedianBlur(noiseImg, Size(3, 3));

	imshow("Origin", src);
	imshow("noiseImg", noiseImg);
	imshow("myMedianBlur", myMedianImg);


	//使用自带的中值平滑函数
	Mat srcColor = imread("lena_full.jpg", 1);

	Mat noiseColor = srcColor.clone();
	Mat medianImg;
	//给彩色图片添加椒盐噪声
	for (int n = 1; n <= noiseNum; n++)
	{
		int randR = rand() % noiseColor.rows;
		int randC = rand() % noiseColor.cols;
		cout << randR << "-" << randC << endl;
		noiseColor.at<Vec3b>(randR, randC) = Vec3b(10,10,10);
	}
	//opencv自带的中值平滑可以处理多通道图像
	medianBlur(noiseColor, medianImg, 3);
	imshow("noiseColor", noiseColor);
	imshow("medianImgWithOpencv", medianImg);


	//使用自己的单通道中值平滑，对多通道split后的每个通道图像进行中值平滑
	Mat noiseColor1 = noiseColor.clone();
	vector<Mat> planes;
	planes.resize(3);    //Release下使用vector<Mat>+resize或者 Mat planes[3]
	split(noiseColor1, planes);
	planes[0] = myMedianBlur(planes[0], Size(3, 3));
	planes[1] = myMedianBlur(planes[1], Size(3, 3));
	planes[2] = myMedianBlur(planes[2], Size(3, 3));
	Mat medianImg1;
	merge(planes, medianImg1);
	imshow("medianMutiImgWithMyBlur", medianImg1);

	waitKey(0);
	return 0;
}