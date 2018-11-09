#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat openImg,closeImg;
int r_open=1, r_close=1;
int iterNUm_open = 1, iterNUm_close = 1;
int MaxR=20,MaxIterNum=20;
Mat src;
void close(int, void*)
{
	Mat sClose = getStructuringElement(MORPH_RECT, Size(2 * r_close + 1, 2 * r_close + 1));

	//cout << r_close << endl;
	morphologyEx(src, closeImg,MORPH_CLOSE, sClose,Point(-1,-1), iterNUm_close);
	imshow("closeImg", closeImg);
}
void open(int, void*)
{
	Mat sOpen = getStructuringElement(MORPH_RECT, Size(2 * r_open + 1, 2 * r_open + 1));

	//cout << r_open << endl;
	morphologyEx(src, openImg, MORPH_OPEN, sOpen, Point(-1, -1), iterNUm_open);
	imshow("openImg", openImg);
}
int main()
{
	src = imread("timg.jpg", 0);
	imshow("src", src);

	Mat binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);
	
	
	//������ͱ����㶼�Ǹ�ʴ�����Ͳ�������ϣ���Ȼ����ʹ��erode,dilate�������
	//����opencv���ṩ��morphologyEx������ֻҪ�ı�ĳ����������ɽ��п�����/������/��̬�ݶ�/��ñ����/��ñ����
	

	//������
	//�����ͣ���ʴ
	//�ܹ�����ɫ�����ڵĺ�ɫ�ն��������ٽ����壬�����Ըı�������������ƽ������߽�
	namedWindow("closeImg", WINDOW_AUTOSIZE);
	createTrackbar("�뾶", "closeImg", &r_close, MaxR, close);
	createTrackbar("��������", "closeImg", &iterNUm_close, MaxIterNum, close);
	close(0, 0);
	
	//������
	//�ȸ�ʴ��������
	//�������Ƚϸߵ�ϸС��������ϸ�㴦�������壬�����Ըı�������������ƽ������߽�
	//Ӧ��Ҳ������ɫ�����ڵİ�ɫС����--�Լ�����⣬����û��д
	namedWindow("openImg", WINDOW_AUTOSIZE);
	createTrackbar("�뾶", "openImg", &r_open, MaxR, open);
	createTrackbar("��������", "openImg", &iterNUm_open, MaxIterNum, open);
	open(0, 0);

	waitKey(0);
	return 0;
}