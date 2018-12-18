#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat image;
Mat fImage;
Mat hlsImage;
Mat result;

string winName = "�������ͶȺ�����";
int L = 0;
int S = 0;
int width;
int height;
int MAX_VALUE = 100;
void callBack_LS(int, void *);
int main()
{
	image = imread("lena_full.jpg");
	
	imshow("ԭͼ", image);

	width = image.cols;
	height = image.rows;

	image.convertTo(fImage, CV_32FC3, 1.0 / 255, 0);
	namedWindow(winName, WINDOW_AUTOSIZE);

	createTrackbar("���Ͷ�(S)", winName, &S, MAX_VALUE, callBack_LS);
	createTrackbar("����(L)", winName, &L, MAX_VALUE, callBack_LS);

	callBack_LS(0, 0);
	waitKey(0);
	return 0;
}
void callBack_LS(int, void *)
{
	//����һ����ͼ��תΪhls
	//HLS��Χ:   0=<H<=360 , 0=<L<=1, 0=<S<=1 
	cvtColor(fImage, hlsImage, COLOR_BGR2HLS);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			Vec3f hls = hlsImage.at<Vec3f>(r, c);

			double l = (1 + L / double(MAX_VALUE))*hls[1];
			double s = (1 + S / double(MAX_VALUE))*hls[2];
			hls = Vec3f(hls[0],     //���Ȳ���
				l > 1 ? 1 : l,
				s > 1 ? 1 : s);
			hlsImage.at<Vec3f>(r, c) = hls;
		}
	}
	cvtColor(hlsImage, result, COLOR_HLS2BGR);
	imshow(winName, result);
}
