#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat topHatImg,bottomHatImg;
int r_topHat =1, r_bottomHat =1;
int iterNUm_topHat = 1, iterNUm_bottomHat = 1;
int MaxR=20,MaxIterNum=20;
Mat binImg;
void close(int, void*)
{
	Mat sClose = getStructuringElement(MORPH_RECT, Size(2 * r_bottomHat + 1, 2 * r_bottomHat + 1));

	morphologyEx(binImg, bottomHatImg,MORPH_BLACKHAT, sClose,Point(-1,-1), iterNUm_bottomHat);
	imshow("bottomHatImg", bottomHatImg);
}
void topHat(int, void*)
{
	Mat sOpen = getStructuringElement(MORPH_RECT, Size(2 * r_topHat + 1, 2 * r_topHat + 1));

	morphologyEx(binImg, topHatImg, MORPH_TOPHAT, sOpen, Point(-1, -1), iterNUm_topHat);
	imshow("topHatImg", topHatImg);
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);
	
	
	//��ñ����Ͷ�ñ���㶼�ǿ�����ͱ���������
	
	

	//��ñ����
	//�ֳƺ�ñ���㣬��������-ԭͼ
	namedWindow("bottomHatImg", WINDOW_AUTOSIZE);
	createTrackbar("�뾶", "bottomHatImg", &r_bottomHat, MaxR, close);
	createTrackbar("��������", "bottomHatImg", &iterNUm_bottomHat, MaxIterNum, close);
	close(0, 0);
	
	//��ñ����
	//ԭͼ-��������
	//��Ϊ��������������������µĽ���������ô��ԭͼ��ȥ������������ô�Ϳ��Եõ�ԭͼ�н���������
	//������İ������µĽ���������(�Ҷ�ֵ��С)����ʱԭͼ��Ӧ��λ�ü�ȥ�Ҷ�ֵ��С�����򣬾ͻ�õ��Ҷ�ֵ�������(����)
	namedWindow("topHatImg", WINDOW_AUTOSIZE);
	createTrackbar("�뾶", "topHatImg", &r_topHat, MaxR, topHat);
	createTrackbar("��������", "topHatImg", &iterNUm_topHat, MaxIterNum, topHat);
	topHat(0, 0);


	//��̬ѧ�ݶ�
	//���ͽ��-��ʴ���
	//�б�Ե����Ч��
	Mat gradientImg;
	Mat s = getStructuringElement(MORPH_RECT, Size(2 * 2 + 1, 2 * 2 + 1));
	morphologyEx(binImg, gradientImg, MORPH_GRADIENT, s, Point(-1, -1), 1);
	imshow("gradientImg", gradientImg);

	waitKey(0);
	return 0;
}