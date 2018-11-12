#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;

//����׳ˣ�Ϊ��������˹������׼��
int factorial(int n)
{
	int fac = 1;
	if (n == 0) return fac;
	for (int i = 1; i <= n; i++)
		fac *= i;
	return fac;
}

//����Ϊn�ķǹ�һ���ĸ�˹ƽ�����ӣ���n-1�Ķ���ʽչ��ϵ�������㣬�����P144
Mat getPascalSmooth(int n)
{
	Mat pascalSmooth = Mat::zeros(Size(n, 1), CV_32FC1);
	for (int i = 0; i <= n-1; i++)
	{
		//����n-1����չ��ʽ��ϵ��
		int v = factorial(n - 1) / (factorial(i)*factorial(n - 1 - i));
		pascalSmooth.at<float>(i) = v;
	}
	return pascalSmooth;//�ǹ�һ���ĸ�˹ƽ������(!!��һ���ĸ�˹������Ҫ����ÿ��Ԫ�س���2��n-1�η�)
}
Mat getPascalDiff(int n)
{
	Mat pascalDiff = Mat::zeros(Size(n, 1), CV_32FC1);

	//���㳤��Ϊn�Ĳ�����Ӽ����������:
	//1.�ȼ��㳤��Ϊn-1�ĸ�˹����
	Mat pascalSmooth_pre = getPascalSmooth(n-1);

	//2.����˹�������ಹ0��Ȼ������֣���������Ԫ�ز�ֵ��Ϊ������ӵ�ֵ
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
	//ʹ����ɢ��������ʣ��ֱ���y�����x������о��
	conv2D(src, src_kerY, kernelY, ddepth, anchor, bordertype);
	conv2D(src_kerY, dst, kernelX, ddepth, anchor, bordertype);
}

Mat sobel(Mat src, int ddepth, int x , int y , int winSize,int bordertype)
{
	Mat dst;
	CV_Assert(winSize>=3 && winSize%2 == 1);   //��֤����˴�СΪ���ڵ���3������

	//�õ�ָ�����ȵĸ�˹���ӺͲ������-����1��winSize��
	Mat pascalDiff = getPascalDiff(winSize);
	Mat pascalSmooth = getPascalSmooth(winSize);

	if (x != 0 && y == 0)
	{   //���д�ֱ�����ϵļ��-��Sobel��ֱ��Ե���
		//�Ƚ��д�ֱ�����ƽ��(�ǹ�һ���ĸ�˹����)��Ȼ��ˮƽ����Ĳ��
		sepConv2D_Y_X(src, dst, ddepth, pascalSmooth.t(), pascalDiff, Point(-1, -1), bordertype);
	}
	else
	{
		//����ˮƽ�����ϵļ��-��Sobelˮƽ��Ե���
		//�Ƚ���ˮƽ�����ƽ��(�ǹ�һ���ĸ�˹����)��Ȼ��ֱ����Ĳ��
		sepConv2D_Y_X(src, dst, ddepth, pascalSmooth, pascalDiff.t(), Point(-1, -1), bordertype);
	}
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	//Sobel��Prewitt����ˮƽ�ʹ�ֱ����ı�Ե���,�������ֱ�Ե���ľ���˶��ǿɷ����,���Է���Ϊƽ�����ӺͲ������
	//��ͬ������Prewitt���÷ǹ�һ���ľ�ֵƽ����ƽ��ͼƬ����Sobel���÷ǹ�һ���ĸ�˹���Ӷ�ͼƬ����ƽ��
	//����Ĳ��������һ����
	Mat img_Sobel_x, abs_img_Sobel_x;
	img_Sobel_x = sobel(src, CV_32FC1, 1, 0, 3, BORDER_DEFAULT);
	convertScaleAbs(img_Sobel_x, abs_img_Sobel_x, 1.0, 0);
	imshow("��ֱ�����Ե���", abs_img_Sobel_x);

	Mat img_Sobel_y, abs_img_Sobel_y;
	img_Sobel_y = sobel(src, CV_32FC1, 0, 1, 3, BORDER_DEFAULT);
	convertScaleAbs(img_Sobel_y, abs_img_Sobel_y, 1.0, 0);
	imshow("ˮƽ�����Ե���", abs_img_Sobel_y);

	//����ʹ����һ�ֱ�Եǿ�ȵļ���-��ֵ�������ǶԲ�ͬ�������ľ���ֵ�ļ�Ȩ���
	Mat edge;
	addWeighted(abs_img_Sobel_x, 0.5, abs_img_Sobel_y, 0.5, 0, edge);
	edge.convertTo(edge, CV_8UC1);
	imshow("edge", edge);

	//opencv�Դ���Sobel��Ե���
	Mat dstOpencv;
	//Sobel(ԭͼ, Ŀ��ͼ, ddepth, dx, dy, ksize, ����ϵ��, ƽ��ϵ��, BORDER_DEFAULT);
	//dx!=0 :ˮƽ���
	//dx=0.dy!=0����ֱ���
	Sobel(src, dstOpencv, -1, 1, 0, 3, 1.0, 0.0, BORDER_DEFAULT);
	imshow("dstOpencv", dstOpencv);   
	//����һ�����⣬ֻ�ܼ�⵽���ο���ұߺ��±ߣ�����Ҫ��Դ���￴

	waitKey(0);
	return 0;
}