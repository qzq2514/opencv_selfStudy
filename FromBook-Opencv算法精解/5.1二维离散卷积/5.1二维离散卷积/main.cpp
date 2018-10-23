#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//�߽����
	Mat src = (Mat_<uchar>(3, 3) << 5, 1, 7, 1, 5, 9, 2, 6, 2);

	Mat dst;
	//copyMakeBorder������ͼƬ���б߽����
	//ԭͼ�����ͼ�����������ĸ��߽������ȣ���䷽ʽ
	//������䷽ʽ����������:
	//BORDER_CONSTANT:�������
	//BORDER_REPLICATE:�߽縴��
	//BORDER_REFLECT:ͼ��ı߽����Ϊ����о�����
	//BORDER_WRAP:ƽ�̸���
	//����Ч���ɴ��벻ͬ�Ĳ����鿴
	copyMakeBorder(src, dst, 2, 2, 2, 2, BORDER_REFLECT_101);
	cout << dst << endl;

	//�������
	Mat I = (Mat_<float>(2, 2) << 1, 2, 3, 4);  //ԭͼ�����
	Mat K = (Mat_<float>(2, 2) << -1, -2, 2, 1);//�����,�����ͱ�����CV_32F����CV_64F

	Mat O;
	//�������ϵĲ��裬�����һ��Ҫ�������ʱ����ת180��
	flip(K, K, -1);
	//�ڶ������о������,opencv���еĶ���same������㣬�����ͼ��ߴ��ԭͼ�����һ��
	//filter2D(ԭͼ�����ͼ��ddepth,ê��,delta,��䷽ʽ)
	//����ddepth��ʾ���ͼ���λ�����λ��(ddepth)��depth��ϵ��"ddepth��depth��ϵ.png"
	//ê��ȡ���ھ����K��һ������λ�ã�ê����ԭͼ��ÿ��λ���ܣ�Ȼ�����������������˺�ԭͼ�ľ��ֵ����Ϊ���ͼ���Ӧλ�õ�����ֵ
	filter2D(I,O,-1,K,Point(0,0),0.0,BORDER_CONSTANT);

	cout << O << endl;


	system("pause");
	//waitKey(0);
	return 0;
}