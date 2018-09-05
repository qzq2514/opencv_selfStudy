#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

bool ROI_AddImage()
{
	Mat srcImage = imread("scene.jpg");
	Mat logoImage = imread("logo.jpg");

	if (!srcImage.data) {cout << "'scene.jpg'ͼ�񲻴���" << endl; return false;}
	if (!logoImage.data) { cout << "'logo.jpg'ͼ�񲻴���" << endl; return false; }

	Mat imageROI = srcImage(Rect(500, 200, logoImage.cols, logoImage.rows));

	Mat mask = imread("logo.jpg", 0);     //������Ĥ(�����ǻҶ�ͼ)
	//mask������ģ����ģ����0��1��ɵ�һ��������ͼ�񡣵���ĳһ������Ӧ����ģʱ��
	//1ֵ���򣨰�ɫ���򣩱����������ε�0ֵ����(��ɫ����)���������ڼ�����

	logoImage.copyTo(imageROI);     //����Ĥ������ԭͼ��ROI����
	//1��image.copyTo(imageROI)�������ǰ�image������ճ����imageROI��
	//2��image.copyTo(imageROI��mask), �����ǰ�mask��image�ص��Ժ��mask������ֵΪ1����ɫ����
	//   �ĵ��Ӧ��image�еĵ�ֱ�Ӹ��Ƹ�imageROI�����mask������ֵΪ0�ĵ��Ӧ��image�еĵ�����


	namedWindow("1.����OIʵ��ͼ�����");    //��֮ǰ��˵���ˣ���ROI�����ϵĲ�����������ԭͼ�ϵĲ���
	imshow("1.����OIʵ��ͼ�����", srcImage);   //��������ͨ��logoImage.copyTo������imageROI���ͻ�Ӱ��ԭͼsrcImage

	return true;
}

//����ͼƬ���Ի��
bool linearBlending()
{
	double alphaVal = 0.5;
	double betaVal;

	Mat srcImage1, srcImage2, dstImage;

	srcImage1 = imread("logo.jpg");
	srcImage2 = imread("samelogo.jpg");
	if (!srcImage1.data) { cout << "'scene.jpg'ͼ�񲻴���" << endl; return false; }
	if (!srcImage2.data) { cout << "'samelogo.jpg'ͼ�񲻴���" << endl; return false; }

	betaVal = 1.0 - alphaVal;
	addWeighted(srcImage1, alphaVal, srcImage2, betaVal, 0.0, dstImage);   //������ӵ�����ͼƬһ��Ҫ����ͬ�ߴ�

	//dst = src1[I]*alpha+ src2[I]*beta + gamma;        //gamma��addWeighted�����ĵ��������,��dstImage��ʾ������ͼƬ
	namedWindow("���Ի�ϴ���1", 1);
	imshow("���Ի�ϴ���1", srcImage1);

	namedWindow("���Ի�ϴ���2", 1);
	imshow("���Ի�ϴ���2", dstImage);

	return true;
}
int main()
{
	linearBlending();
	waitKey();
	return 0;
}