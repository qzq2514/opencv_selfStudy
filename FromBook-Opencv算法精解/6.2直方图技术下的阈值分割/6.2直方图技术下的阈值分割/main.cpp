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
	drawGrayHist(hist,"ԭͼ�Ҷ�ͼ");

	Point firstPeakPoint;               //�ҵ���һ������-���Ҷ�ֵ��������������
	minMaxLoc(hist, NULL, NULL, NULL, &firstPeakPoint);
	int firstPeak = firstPeakPoint.x;   //hist��С��Size(256, 1)����������
	cout << "firstPeak:"<<firstPeak << endl;
	Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);  //��ΪҪ��ŵıȽ�ֵ�ϴ��������ʹ��CV_32FC1����CV_64FC1
	
	for (int k = 0; k < 256; k++)
	{
		int hist_k = hist.at<int>(k);    //�Ҷ�ֵΪk��������
		//�����ҵ��ڶ�������Ĺ�ʽ�����ҵ�k,����󻯵Ⱥ��ұߵĹ�ʽ
		measureDists.at<float>(k) = pow(float(k - firstPeak), 2.0)*hist_k;   
		//cout << k << ":" << pow(float(k - firstPeak), 2.0)*hist_k << endl;
	}

	Point secondPeakPoint;              
	minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakPoint);  //������˵�ģ�����ұߵĵ�ʽ
	int secondPeak = secondPeakPoint.x;    
	Point threshLoc;
	int thresh;

	//�ҵ���������֮����С�Ĳ��ȶ�Ӧ�ĻҶ�ֵ
	int maxPeak = firstPeak > secondPeak ? firstPeak : secondPeak;
	int secondMaxPeak = firstPeak < secondPeak ? firstPeak : secondPeak;
	//����ʹ��һ��С���ɣ�ͨ��colRange��������������֮������Сֵ(����ҿ�����)
	minMaxLoc(hist.colRange(secondMaxPeak, maxPeak), NULL, NULL, &threshLoc, NULL);
	//����Ҫע�⣬threshLoc����colRange��Χ�ڵ���Сֵ�±�(��0��ʼ)������Ҫ����secondMaxPeak����������hist����С�±�
	thresh = secondMaxPeak + threshLoc.x+1;  
	return thresh;
}
int main()
{
	Mat src = imread("timg.jpg",0);

	//����ֱ��ͼ�����������ҵ����еĲ�����Ϊ�ָ���ֵ
	//������ֵ�ָ�ķ�ʽ�ʺ�ͼƬ��ֱ��ͼ�д����������ԵĲ����������������ͼ���ֱ��ͼ��һ�����������
	//���������壬���ַ����Ͳ�̫�ʺ�
	int thresh=getFromTwoPeakeOfHist(src);
	cout << "thresh:"<<thresh << endl;
	Mat dst;
	threshold(src, dst, thresh, 255, THRESH_BINARY);

	//����һ��С����(ͻȻ�뵽��)��dst��CV_8UC1�ģ����ڷ�Χ��0-255�ģ�
	//�����������תΪCV_32SC1(��Χ0-2^32-1)
	//��ô�϶�����ʾ��ͼ����ת��CV_32FC1(��Χ0-1)��ʾ��ͼ��һ������
	//src.convertTo(dst, CV_32SC1);
	imshow("timg", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}