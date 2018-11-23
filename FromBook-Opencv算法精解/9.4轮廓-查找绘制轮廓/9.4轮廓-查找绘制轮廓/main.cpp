#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src = imread("contours.png",0);

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

	src = src > 1;
	namedWindow("Source", 1);
	imshow("Source", src);

	//һ�������ǵ�ļ��ϣ�vector<Point>,��contours��ŵ��������������������ļ��ϣ�����vector<vector<Point> >����
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	//�ҵ���ֵ��ͼ�ı�Ե�������������Ϸ���contours����������֮��Ĳ㼶��ϵ����hierarchy
	//��������������ȡ������ģʽ:
	/*
cv::RETR_EXTERNAL����ʾֻ��ȡ�������������
cv::RETR_LIST����ʾ��ȡ������������������б�ȫ����ͬһ��
cv::RETR_CCOMP:��ʾ��ȡ��������������֯��һ������ṹ�����ж����������ⲿ�������ڶ��������ǡ�������������
cv::RETR_TREE����ʾ��ȡ������������֯������Ƕ�׵������㼶�ṹ��
���"findContours-mode����.png"
���ĸ��������������ַ�ʽ���ɺ���*/
	findContours(src, contours, hierarchy,
		RETR_TREE, CV_CHAIN_APPROX_SIMPLE);



	//hierarchy������ʾÿ�������Ĳ㼶��ϵ������һ��M*4�ľ���M��ʾ����������ÿ�е��ĸ���������������:
	/*
��һ����:��ʾͬһ���������¸������ı��,�����һ������û����һ������,һ������һ�����������һ����ʱ��,��Ϊ-1
�ڶ�����:��ʾͬһ���������ϸ������ı��,�����һ������û����һ������,һ������һ�������ĵ�һ����ʱ��,��Ϊ-1
��������:��ʾ��������������һ�������ĵ�һ���ı��,����û��,��Ϊ-1
���ĸ���: ��ʾ����������һ�������ı��,����û����һ��,��Ϊ-1*/
	//������һ��vector<Vec4i>������ʾ�㼶��ϵ�Ƿ��ϵģ�����ɼ�"hierarchy�������.png"
	/*for (int i = 0; i < hierarchy.size(); i++)
	{
	cout << hierarchy[i] << endl;
	cout << "----------" << endl;
	}*/

	cout << "��������:" << contours.size() << endl;
	int idx = 0;
	//�����ҳ��������������������ɫ��hierarchy[idx][0]��Ϊ�����ҵ�ǰ���ͬ������һ������������-1��ʾû�е�ǰ���������������
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//��������(ԭͼ���������ϣ�Ҫ��������id��������ɫ�����ķ�ʽ-��������䣬����ȼ�-ȱʡֵ2147483647)
		//���ķ�ʽthickness=-1��ʾ��ɫ��䣬�����ʾ����������ʱ��ʾ������ϸ
		/*����ȼ�Ϊ:
		����ȼ�Ϊ0�����Ƶ��������������Ϊ1��������������������ͬ�ļ��������������ֵΪ2�����е�����������ȼ�Ϊ2����������ͬ�����������е�һ���������������*/
		cout << "idx:" << idx << endl;
		drawContours(dst, contours, idx, color, -1, 8, hierarchy,0);
	}
	//����ÿ������(ÿ�������������ļ��ϣ�ֱ����������ÿ�����ڵĵ����)
	Scalar colors[4] = { Scalar(0,0,255) ,Scalar(0,255,0) ,Scalar(255,0,0) ,Scalar(255,255,0) };
	for (int j = 0; j < contours.size(); j++)
	{
		int len = contours[j].size();
		for (int i = 0; i < len; i++)
		{
			Point start = contours[j][i];
			Point end = contours[j][(i + 1) % len];
			line(dst, start, end, colors[j]);
		}
	}
	
	namedWindow("Components", 1);
	imshow("Components", dst);
	waitKey(0);
}