#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void drawGrayHist(Mat hist,string name)
{
	double maxV;
	minMaxLoc(hist, NULL, &maxV, NULL, NULL);
	//cout << maxV << endl;
	int canvaHight=300,lineMaxheight=saturate_cast<int>( 0.9*canvaHight);
	Mat canva = Mat::zeros(Size(256*2, canvaHight), CV_8U);

	for (int pix = 0; pix < 256; pix++)
	{
		int realHeigh = hist.at<int>(pix);
		int drawHeight = saturate_cast<int>(realHeigh*lineMaxheight / maxV);
		//cout << drawHeight << endl;
		line(canva, Point(pix * 2, canvaHight), Point(pix * 2, canvaHight - drawHeight), Scalar(255));
	}
	imshow(name, canva);

}
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
int getFromTwoPeakeOfHist(const Mat image)
{
	Mat hist = calGrayHist(image);
	drawGrayHist(hist,"原图灰度图");

	Point firstPeakPoint;               //找到第一个波峰-即灰度值数出现最多的像素
	minMaxLoc(hist, NULL, NULL, NULL, &firstPeakPoint);
	int firstPeak = firstPeakPoint.x;   //hist大小是Size(256, 1)，即行向量
	cout << "firstPeak:"<<firstPeak << endl;
	Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);  //因为要存放的比较值较大，这里最好使用CV_32FC1甚至CV_64FC1
	
	for (int k = 0; k < 256; k++)
	{
		int hist_k = hist.at<int>(k);    //灰度值为k的像素数
		//这是找到第二个波峰的公式，即找到k,以最大化等号右边的公式
		measureDists.at<float>(k) = pow(float(k - firstPeak), 2.0)*hist_k;   
		//cout << k << ":" << pow(float(k - firstPeak), 2.0)*hist_k << endl;
	}

	Point secondPeakPoint;              
	minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakPoint);  //像上面说的，最大化右边的等式
	int secondPeak = secondPeakPoint.x;    
	Point threshLoc;
	int thresh;

	//找到两个波峰之间最小的波谷对应的灰度值
	int maxPeak = firstPeak > secondPeak ? firstPeak : secondPeak;
	int secondMaxPeak = firstPeak < secondPeak ? firstPeak : secondPeak;
	//这里使用一个小技巧，通过colRange限制在两个波峰之间找最小值(左闭右开区间)
	minMaxLoc(hist.colRange(secondMaxPeak, maxPeak), NULL, NULL, &threshLoc, NULL);
	//这里要注意，threshLoc是在colRange范围内的最小值下标(从0开始)，所以要加上secondMaxPeak才是在整个hist的最小下标
	thresh = secondMaxPeak + threshLoc.x+1;  
	return thresh;
}
int main()
{
	Mat src = imread("timg.jpg",0);

	//根据直方图的两个波峰找到其中的波谷作为分割阈值
	//这种阈值分割的方式适合图片的直方图中存在两个明显的波峰的情况，而大多数图像的直方图不一定会存在明显
	//的两个波峰，这种方法就不太适合
	int thresh=getFromTwoPeakeOfHist(src);
	cout << "thresh:"<<thresh << endl;
	Mat dst;
	threshold(src, dst, thresh, 255, THRESH_BINARY);

	//这里一个小测试(突然想到的)，dst是CV_8UC1的，其内范围是0-255的，
	//所以如果将其转为CV_32SC1(范围0-2^32-1)
	//那么肯定是显示黑图，跟转成CV_32FC1(范围0-1)显示白图是一个道理
	//src.convertTo(dst, CV_32SC1);
	imshow("timg", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}