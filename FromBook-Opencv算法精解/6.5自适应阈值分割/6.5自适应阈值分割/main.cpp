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
	case MEAN:     //��ֵƽ��
		boxFilter(src, smooth, CV_32FC1, Size(2 * radius + 1, 2 * radius + 1));
		break;
	case GAUSS:     //��˹ƽ��
		GaussianBlur(src, smooth, Size(2 * radius + 1, 2 * radius + 1), 0, 0);
		break;
	case MEDIAN:     //��ֵƽ��
		medianBlur(src, smooth, 2 * radius + 1);
		break;
	default:
		break;
	}
	//cout << src.size() << endl;
	//cout << smooth.size() << endl;

	src.convertTo(src, CV_32FC1);
	smooth.convertTo(smooth, CV_32FC1);   //��תΪfloat���ͣ����ں������ϵ��float���͵�ratio
										  //(1.0 - ratio)*smooth����ֵ����(����ԭͼ��С��ͬ��Ȼ��ԭͼÿ����������ֵ�����Ӧ��Ԫ��ֵ���бȽ�)
	Mat diff = src - (1.0 - ratio)*smooth;

	Mat out = Mat::zeros(diff.size(), CV_8UC1);

	for (int r = 0; r < out.rows; r++)
	{
		for (int c = 0; c < out.cols; c++)
		{
			if (diff.at<float>(r, c) > 0)      //ԭͼĳλ�ûҶ�ֵ������ֵ�����Ӧ�ĻҶ�ֵ
				out.at<uchar>(r, c) = 255;
		}
	}
	return out;
}
int main()
{
	Mat src = imread("adaptive.jpg", 0);
	imshow("src", src);

	//��ͼƬ�ĸ����ֹ�����������ȣ�ʹ��ȫ����ֵ�ָ�Ч�����ã�
	//��������ʹ��ǿ�Ƶ�ȫ����ֵ�ָ������ֵ����Ϊ���٣�Ч��������
	/*Mat dst;
	threshold(src, dst, 130, 255, THRESH_BINARY);
	imshow("dst", dst);*/

	//���ڹ��ղ����Ե���Ƭ��ʹ�þֲ���ֵ�ָ�Ч���Ϻã�ÿ�����ض���һ����Ӧ����ֵ
	Mat myDst;
	//ʹ������Ӧ�ֲ���ֵ�ָ����ƽ����������þ�ֵ�������ƣ����ߴ��Сʱ(5x5,7x7)Ч��������
	//���ž���˳ߴ���Ч���ͱ�ã��ָ����ǰ������ҲԽ��Խ����
	myDst = adaptiveThresh(src, 50, 0.15, MEDIAN);
	imshow("myDst", myDst);

	Mat dst2;
	//ϵͳ�Դ�������Ӧ��ֵ�ָ�
	//ԭͼ(����CV_8U)��Ŀ��ͼ�����ֵ��ƽ����ʽ(ֻ�о�ֵƽ���͸�˹ƽ��)����ֵ����ʽ,ƽ�����ӳߴ�(����),����ϵ��(ratio)
	adaptiveThreshold(src, dst2, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY,101,0.15);
	imshow("dst2", dst2);

	waitKey(0);
	return 0;
}