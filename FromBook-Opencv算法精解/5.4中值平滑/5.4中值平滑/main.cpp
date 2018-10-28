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
			//��õ�ǰ��ֵ����˿�ס��ԭͼλ��,ע��Ҫ��¡clone
			Mat curRegion = padding(Rect(w - ww, h - hh, kW, kH)).clone();  

			//1,1�ֱ��ʾreshape����ͼ�����ͨ������������
			//����Ѿ���˿�ס������reshape��һ��������
			curRegion = curRegion.reshape(1, 1);

			//����������������
			cv::sort(curRegion, curRegion, CV_SORT_EVERY_ROW);
			uchar midVal = curRegion.at<uchar>(midInd); //ȡ��ֵ
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
	//ʹ��Release���У���Ϊsplit������Release��ʹ��vector<Mat>+resize���� Mat planes[3]
	Mat src = imread("lena_full.jpg", 0);

	//��ԭͼ����ӽ�������(��ͼ���ڴ���ϵͳ�����ڽ�������ԭ�򣬵���ͼ���г��ֹ����İ׵���ߺڵ�)
	Mat noiseImg= src.clone();
	int noiseNum = 4000;
	for (int n = 1; n <= noiseNum; n++)
	{
		int randR = rand() % noiseImg.rows;
		int randC = rand() % noiseImg.cols;
		noiseImg.at<uchar>(randR, randC) = 0;
	}

	//��ͼ�������ֵƽ��������ͼ���д��ڵĽ�������
	Mat myMedianImg = myMedianBlur(noiseImg, Size(3, 3));

	imshow("Origin", src);
	imshow("noiseImg", noiseImg);
	imshow("myMedianBlur", myMedianImg);


	//ʹ���Դ�����ֵƽ������
	Mat srcColor = imread("lena_full.jpg", 1);

	Mat noiseColor = srcColor.clone();
	Mat medianImg;
	//����ɫͼƬ��ӽ�������
	for (int n = 1; n <= noiseNum; n++)
	{
		int randR = rand() % noiseColor.rows;
		int randC = rand() % noiseColor.cols;
		cout << randR << "-" << randC << endl;
		noiseColor.at<Vec3b>(randR, randC) = Vec3b(10,10,10);
	}
	//opencv�Դ�����ֵƽ�����Դ����ͨ��ͼ��
	medianBlur(noiseColor, medianImg, 3);
	imshow("noiseColor", noiseColor);
	imshow("medianImgWithOpencv", medianImg);


	//ʹ���Լ��ĵ�ͨ����ֵƽ�����Զ�ͨ��split���ÿ��ͨ��ͼ�������ֵƽ��
	Mat noiseColor1 = noiseColor.clone();
	vector<Mat> planes;
	planes.resize(3);    //Release��ʹ��vector<Mat>+resize���� Mat planes[3]
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