#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat I=imread("lena_full.jpg",0);

	//4.3ֱ��ͼ���滯
	//����ͼ������min-max��һ�������Ա任��һ��,����ԭͼ������ֵ��һ����ָ���ķ�Χ������:
	//O(r,c)=[((Omax - Omin) / (Imax, Imin)]*(I(r,c)-Imin)+Omin
	double Imax, Imin;

	//�ҵ�ԭͼ��������С�����ֵ���������ĸ������ֱ��������С����ֵ���ڵĵ�����
	minMaxLoc(I, &Imin, &Imax, NULL, NULL);

	cout << "ԭͼ:"<<Imin << "--" << Imax << endl;  //0-255
	
	double Omin = 0, Omax = 100;//��Ŀ��ͼ������ֵ��һ����0-100��Χ��
	//����min-max��һ��(��ʵ����һ�����Ա仯��ӳ��)�Ĺ�ʽ���õ������Ա仯�µ�a,bֵ
	double a = (Omax - Omin) / (Imax-Imin);
	double b = Omin - a*Imin;   

	//�������Ա仯
	//����Ƕ�ͨ��ͼ����ô�ͶԶ��ͨ���ֱ�������滯����
	Mat O;

	convertScaleAbs(I, O, a, b);
	imshow("ԭͼ", I);
	imshow("ֱ��ͼ���滯ͼ", O);
	minMaxLoc(O, &Imin, &Imax, NULL, NULL);
	cout << "ֱ��ͼ���滯ͼ:" << Imin << "--" << Imax << endl; //0-100


	//opencv�Դ������滯����

	Mat dst;
	//�����С��һ�������������Ĳ���10��Ϊ0����������Ч��һ��
	normalize(I, dst, 100, 10, NORM_MINMAX, CV_8U);
	
	imshow("normalize_NORM_MINMAX", dst);
	minMaxLoc(dst, &Imin, &Imax, NULL, NULL);
	cout << "normalize_NORM_MINMAX:" << Imin << "--" << Imax << endl; //10-100
	//normalize�����ĵ�5�����������Ը�ΪNORM_L1,NORM_L2,NORM_INF�ֱ�ԭͼ����ֵ���Է���ֵ��
	//Ȼ�����a,b�����Ա仯,����ο�ͼ"normalize����1.jpg"��"normalize����2.jpg"


	//4.4٤��任
	//�Ƚ�ͼ��Ҷ�ֵ������0-1��Χ�ڣ�Ȼ�������ָ��Ϊr��������
	//gamma>1, ����������Ҷȷ�Χ�����죬�ϰ�������Ҷȷ�Χ��ѹ���ĸ�����ͼ������䰵��
	//gamma<1, ����������Ҷȷ�Χ��ѹ�����ϰ�������Ҷȷ�Χ������Ľ�����ͼ�����������
	Mat gammaImg;
	
	//���Ա任�Ƚ�ԭͼ�Ҷ�ֵ��Χ�䵽0-1.0
	I.convertTo(gammaImg, CV_64F, 1.0 / 255, 0);
	double gamma = 0.5;
	pow(gammaImg, gamma, gammaImg);   //����gamma = 0.5;��٤��任(ͼ���������)

	gammaImg.convertTo(gammaImg, CV_8U, 255, 0);  //����Ҫ�����CV_8U,��Ҫ����ɸ����ͣ���Ȼ��Ȼ���ᱨ������ͼƬ�޷���ʾ
	imshow("gamma=0.5�任", gammaImg);            //��ͨ�������CV_8UC3
	waitKey(0);

	return 0;
}