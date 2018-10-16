#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat I;
Mat pI;
Point2f IPoint, pIPoint;
int i = 0, j = 0;
Point2f src[4];
Point2f dst[4];
Scalar scalars[4] = { Scalar(0,0,0),Scalar(0,0,255),Scalar(0,255,0),Scalar(255,255,255)};
void mouse_I(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	    case CV_EVENT_LBUTTONDOWN:   //��갴���¼�
			
			IPoint = Point2f(x, y);
			src[i] = IPoint;
			cout << "mouse_I����¼�....." << src[i] << endl;
			//��ͼƬI�ϻ�Բ����src[i]ΪԲ�ģ�5Ϊ�뾶��scalars[i]��Բ����ɫ��3��Բ��������ϸ,6����������
			circle(I, src[i], 5, scalars[i], 3,6);
			i += 1;
		    break;
		default:
			break;
	}
}
void mouse_pI(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:   //��갴���¼�
		pIPoint = Point2f(x, y);
		dst[j] = pIPoint;
		cout << "mouse_pI����¼�....." << dst[j] << endl;
		circle(pI, dst[j], 5, scalars[j], 3);
		j += 1;
		break;
	default:
		break;
	}
}
int handle1()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	int w = I.cols;
	int h = I.rows;
	Mat src = (Mat_<float>(4, 2) << 0, 0, w - 1, 0, 0, h - 1, w - 1, h - 1);
	Mat dst = (Mat_<float>(4, 2) << 50, 50, w/3, 50, 50, h - 1, w - 1, h - 1);


	//��Ƚ������任��ͶӰ�任��Ҫ�Ķ������(��src��dst����4*2����)����getAffineTransformһ����������ԣ�
	//������Ե���������Ҳ��float,���صı任����Ϊ3*3������������CV_64F(getAffineTransform����2*3����)��
	Mat p = getPerspectiveTransform(src, dst);

	Mat result;
	//ͶӰ�任����warpPerspective���������warpAffineһ��:
	//������ԭͼ�񣬽��ͼ�񣬱任����ͼƬ��С(��������Ч����),��ֵ�㷨������Ч������䷽���������ɫֵ
	warpPerspective(I, result, p, pI.size(), INTER_NEAREST, BORDER_CONSTANT, Scalar(100, 120, 200));

	imshow("Origin pic", I);
	imshow("After perspective", result);

	waitKey(0);
	return 0;
}
int handle2()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	pI = 255 * Mat::ones(I.size(), CV_8UC3);   //Ŀ��ͼ��ʼ��Ϊ�հ�ͼ
	cout << pI.rows << "-" << pI.cols << endl;
	//��ԭͼ�����϶�������¼�
	namedWindow("I", WINDOW_AUTOSIZE);    //�ڶ���������ʾ�����Ƿ���Ըı�
	setMouseCallback("I", mouse_I, NULL); //(WINDOW_NORMAL=0�ɸı�)(WINDOW_AUTOSIZE=1���ɸı�)

	//����������϶�������¼�
	namedWindow("pI", 1);
	setMouseCallback("pI", mouse_pI, NULL);

	imshow("I", I);
	imshow("pI", pI);
	while (!(i == 4 && j == 4))
	{
		imshow("I", I);
		imshow("pI", pI);
		if (waitKey(50) == 'q')
			break;
	}

	cout << "ͶӰ������ѡȡ���...." << endl;
	imshow("I", I);
	imshow("pI", pI);

	/*for (int n = 0; n < 4; n++)
	{
		cout << src[n] << ":" << dst[n] << endl;
	}*/

	//�Ƴ�����¼�
	setMouseCallback("I", NULL, NULL);
	setMouseCallback("pI", NULL, NULL);

	
	Mat p = getPerspectiveTransform(src, dst);
	/*cout << "ͶӰ�任����:" << endl;
	cout << p << endl;*/


	//����ͶӰ�仯
	Mat result;
	warpPerspective(I, result, p, pI.size());
	imshow("After perspective", result);//��ʾ�任���

	waitKey(0);
	return 0;
}
int main()
{
	 //ͶӰ�任�������Ķ�CV_32F�������ݵ�,��4*2���õ�CV_64F���͵ı任����(3*3)
//��ϰ:����任��....��..................,..3*2��........................(3*2)
	//handle1();        //�����о͸�����ͶӰ�任�����������
	handle2();          //ͨ���ڻ������ֶ��������ȡ�Ķ������
	
	
	return 0;
}