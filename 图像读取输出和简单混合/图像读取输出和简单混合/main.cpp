#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat scene = imread("secne.jpg");
	namedWindow("�羰ͼ");
	imshow("�羰ͼ",scene);

	Mat logo = imread("logo.jpg");
	namedWindow("logoͼ");
	imshow("logoͼ", logo);
	
	cout << scene.cols << "-" << scene.rows << endl;
	cout << "qzq2514" << endl;
	Mat sceneROI;
	//��ȡͼƬ��ROIʱ��Ҫ��֤ROI��Χ���ᳬ��ͼƬ��ԭ��Χ
	//sceneROI = scene(Rect(80, 35, logo.cols, logo.rows));           //��һ�ַ���:����ROI������������ϽǺ�������;��εĳ���

	sceneROI = scene(Range(35, 35 + logo.rows), Range(80, 80 + logo.cols)); //�ڶ��ַ���:���θ���ROI�����������ͺ����귶Χ
	addWeighted(sceneROI,0.5,logo,0.05,0.,sceneROI);   //����ͼƬ�ļ򵥻�ϣ����к���Ĳ����Ǹ��ԵĻ��Ȩ�أ������������gamma,��ʾƫ��
	    //����Ҫע�����sceneROI��ʾscene��ROI�������Ǹ��ݾ��ο��scene���н�ͼ�����µ�matͼƬ����������ϵĽ����ֵ��sceneROI,
		//�ͻ��Զ����õ�ԭͼƬ�ľ��ο�Χ��

	namedWindow("���ͼ");
	imshow("���ͼ", scene);

	imwrite("��һ�����ͼƬ.jpg",scene);

	waitKey();
	return 0;
}