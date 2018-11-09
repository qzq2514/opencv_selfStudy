#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int r = 1;
int MAXR = 20;
Mat binCopy;
Mat dilateImg;

void callBackFunc(int, void *)
{
	//���ﴴ��һ����ֱ�������Բ״�ṹԪ
	Mat sDilate = getStructuringElement(MORPH_ELLIPSE, Size(1, 2 * r + 1));   //��ȡ�ṹԪ

	dilate(binCopy, dilateImg, sDilate, Point(-1, -1));    //Ĭ������һ��
	imshow("dilateImg", dilateImg);
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	Mat binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);

	//��ʴ-��ɫ�����С
	//ԭ������ֵƽ���ǽ����صľ��������ڵ���ֵ��ֵ�������أ�
	//����ʴ�������ǽ������ڵ���Сֵ��ֵ�������أ���θ�ʴ���������򲻽����Ǿ��Σ�
	//��������Բ�Σ�ʮ�ּܣ���̬ѧ�����г����ֺ�Ϊ�ṹԪ��ͨ��getStructuringElement���ָ����״�ʹ�С�ĽṹԪ
	Mat sErode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat erodeImg;
	erode(binImg, erodeImg, sErode, Point(-1, -1), 1);//���и�ʴ����:ԭͼ��Ŀ��ͼ���ṹԪ��ê�㣬��ʴ��������
	imshow("erodeImg", erodeImg); //��Ϊȡ��Сֵ������ͼƬ�����ȵ͵�������



	//����-��ɫ������
	//��ʴ��ȡ�ṹԪ����Сֵ������ǡ���෴��ȡ�ṹԪ�е����ֵ�����԰�ɫ������
    //����ʹ�ö�̬�ķ�ʽ��ʾ���ͽ��-ʹ��createTrackbar�������Կ��ӻ��ĵ��ڽṹԪ�뾶��С
	binCopy = binImg.clone();
	namedWindow("dilateImg",WINDOW_AUTOSIZE);   //����һ�����ӻ�����
	//��ͬ���Ŀ��ӻ������д��������������ڵ��ڽṹԪ�뾶,r�ǲ��ϱ仯�ĵ�ǰ�뾶��MAXR�ǽ��������ֵ
	createTrackbar("�뾶", "dilateImg", &r, MAXR, callBackFunc);
	callBackFunc(0, 0);  //Ҫ��ִ��һ��callBackFunc��������Ȼ���涼������,��ô��������
	
	waitKey(0);
	return 0;
}