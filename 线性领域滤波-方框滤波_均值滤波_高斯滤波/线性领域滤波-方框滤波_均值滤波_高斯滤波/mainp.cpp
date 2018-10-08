#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>           //�˲���������
#include<iostream>

using namespace cv;
using namespace std;

Mat image, dstImage1, dstImage2, dstImage3;
int kn_box=3, kn_mean=3, kn_gaussian = 3;
static void on_BoxFilter(int, void*);
static void on_MeanBlur(int, void*);
static void on_GaussianBlur(int, void*);

//�������ۼ��㷨�ʼ����ͼƬ
int main()
{
	image = imread("lena_full.jpg");
	

	image.copyTo(dstImage1);              //copyTo��clone����������Ḵ������ԭͼ����
	dstImage2 = image.clone();			  //���ֻ��ֱ��dstImage1=image������ǳ�������ᵼ����������ͷָ��ͬһ��ͼ���ڴ棬һ���ı䣬��һ��Ҳ���Ÿı�
	dstImage3 = image.clone();

	namedWindow("ԭͼ����",1);
	imshow("ԭͼ����",image);

	//1.�����˲�
	namedWindow("�����˲�",1);
	createTrackbar("�ں�ֵ","�����˲�",&kn_box,40,on_BoxFilter);   //40�Ǻ˿�ߵ����ֵ
	on_BoxFilter(kn_box, 0);

	//2.��ֵ�˲�
	namedWindow("��ֵ�˲�", 1);
	createTrackbar("�ں�ֵ", "��ֵ�˲�", &kn_mean, 40, on_MeanBlur);   //40�Ǻ˿�ߵ����ֵ
	on_MeanBlur(kn_mean, 0);

	//3.��˹�˲�
	namedWindow("��˹�˲�", 1);
	createTrackbar("�ں�ֵ", "��˹�˲�", &kn_gaussian, 40, on_GaussianBlur);   //40�Ǻ˿�ߵ����ֵ
	on_GaussianBlur(kn_gaussian, 0);

	waitKey();
	return 0;
}
static void on_BoxFilter(int, void*)
{
	//ԭͼ�����ͼƬ�����ͼ����ȣ��˴�С��ê��(����ƽ�����Ǹ���,(-1,-1)��ʾ���ĵ�)
	boxFilter(image,dstImage1,-1,Size(kn_box+1, kn_box + 1),Point(-1,-1)); //�˿�߶���һ����ΪTrackBar��0��ʼ�����˿�߱����1��ʼ
	imshow("�����˲�",dstImage1);
	imshow("ԭͼ����", image);
}

static void on_MeanBlur(int, void*)
{
	//ԭͼ�����ͼƬ���˴�С��ê��(����ƽ�����Ǹ���,(-1,-1)��ʾ���ĵ�)
	blur(image, dstImage2, Size(kn_mean + 1, kn_mean + 1), Point(-1, -1));
	imshow("��ֵ�˲�", dstImage2);
}

static void on_GaussianBlur(int, void*)
{
	//ԭͼ�����ͼƬ���˴�С��ê��(����ƽ�����Ǹ���,(-1,-1)��ʾ���ĵ�)
	GaussianBlur(image, dstImage3, Size(kn_gaussian *2 + 1, kn_gaussian *2 + 1), 0,0);
	//��˹�˵Ŀ�߿��Բ�ͬ��������Ϊ��������4,5��wΪ��˹������x,y�����ƫ��
	imshow("��˹�˲�", dstImage3);
}
