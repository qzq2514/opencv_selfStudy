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

	//�����һ���Ҷ�ֱ��ͼ�����ÿ���Ҷ�ֵ�ĸ���
	histogram.convertTo(normHist, CV_32FC1, 1.0 / src.total(), 0.0); 

	//�����ۼ�ֱ��ͼ(����ۻ���)��һ���ۼӾ�-����1��256��
	Mat zeroCumnMoment = Mat::zeros(Size(256, 1), CV_32FC1);
	Mat oneCumnMoment = Mat::zeros(1,256, CV_32FC1);

	for (int i = 0; i < 256; i++)
	{
		if (i == 0) 
		{
			zeroCumnMoment.at<float>(i) = normHist.at<float>(i);
			//��ʱoneCumnMoment��ʵ����0����֤��ʽͳһ����������д
			oneCumnMoment.at<float>(i) = i*normHist.at<float>(i);  
		}
		else
		{
			zeroCumnMoment.at<float>(i) = zeroCumnMoment.at<float>(i-1) + normHist.at<float>(i);
			oneCumnMoment.at<float>(i) = oneCumnMoment.at<float>(i-1) + i*normHist.at<float>(i);
		}
	}

	//������䷽��
	Mat variance = Mat::zeros(1, 256, CV_32FC1);
	float mean = oneCumnMoment.at<float>(255);   //��ʵ��������ͼƬ��ƽ���Ҷ�ֵ
	//cout << "mean:" << mean << endl;

	for (int i = 0; i < 255; i++)
	{
		float zeroCumnMomentt_i = zeroCumnMoment.at<float>(i);
		float oneCumnMoment_i = oneCumnMoment.at<float>(i);

		//�ų���ĸΪ0�����
		if (zeroCumnMomentt_i == 0 || zeroCumnMomentt_i == 1)
			variance.at<float>(i) == 0;
		else
		{
			float confficient = zeroCumnMomentt_i*(1.0 - zeroCumnMomentt_i);   //��ʽ�еķ�ĸ
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
	Mat myDst;   //�ҵ���ֵ�󣬾�ʹ���Դ���threshold��ֵ�ָ������
	threshold(src, myDst, myThresh, 255, THRESH_BINARY);
	imshow("myDst", myDst);


	
	Mat dst;
	double thresh = 0;
	//ʹ���Դ�����ֵ�ָ��,ͨ��ָ��������THRESH_OTSU + THRESH_BINARY���ɴﵽostu�Զ�ѡȡȫ����ֵ
	//ϵͳ�Դ���OTSU��ֵ�ָ�ֻ֧��8λͼ
	thresh=threshold(src, dst, thresh, 255, THRESH_OTSU + THRESH_BINARY);
	cout << "thresh:" << thresh << endl;     //�����Լ�д�ĺ�ϵͳ�Դ���otsu��ֵ�ָ����������ֵ��һ����
	imshow("dst", dst);

	waitKey(0);
	return 0;
}