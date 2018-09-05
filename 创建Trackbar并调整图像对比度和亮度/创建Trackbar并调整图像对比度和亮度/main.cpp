#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int g_nContrastValue;
int g_nBrightValue;
Mat g_srcImage, g_dstImage;

void ContrastAndBright(int, void*)    //�ص�����ԭ�ͱ�����void XXXX(int,void*)
{
	namedWindow("ԭͼImage", 1);
	for (int x=0;x<g_srcImage.rows;x++)
	{
		for (int y = 0; y < g_srcImage.cols; y++)
		{
			for (int c = 0; c < 3; c++)
			{
				//�ı�ͼƬÿ��λ����ÿ��ͨ�������أ��Աȶ���Ϊϵ��������Ϊƫ��
				g_dstImage.at<Vec3b>(x, y)[c] = saturate_cast<uchar>((g_nContrastValue*0.01)*g_srcImage.at<Vec3b>(x, y)[c] + g_nBrightValue);
			}
		}
	}

	imshow("ԭͼImage", g_srcImage);
	imshow("Ч��ͼImage", g_dstImage);
}

int main()
{
	g_srcImage = imread("lena_full.jpg");
	if (!g_srcImage.data) { cout << "�Ҳ���lena_full.jpgͼƬ" << endl; return false; }
	g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());   //�����ԭͼͬ����С��ͼƬ�������ڴ��Ч��ͼ

	g_nContrastValue = 80;
	g_nBrightValue = 80;             //����ԱȶȺ����ȳ�ʼֵ

	namedWindow("Ч��ͼImage", 1);   //WINDOW_AUTOSIZE=1,���ܸı䴰�ڴ�С


	
	//createTrackbar�������α�ʾTrackbar���ƺ������Ĵ������ƣ�Ȼ���ʾbar�ϵ�ǰֵ�ı��������ֵ������ǻص�����
	//��ɵ�ǰֵ��������ص������У����ﴫ�뵱ǰֵ�ı���ʱ��Ҫʹ�����ô��ݣ���������tracbar, �ͻ���Ÿı����ֵ
	createTrackbar("�Աȶ�:","Ч��ͼImage",&g_nContrastValue,300,ContrastAndBright);
	createTrackbar("��  ��:", "Ч��ͼImage", &g_nBrightValue, 200, ContrastAndBright);
	
	ContrastAndBright(g_nContrastValue, 0);  //Ҫ��������������һ�䣬��Ȼû�У���һ��ʼ������ʾͼƬ�����Ǹı�trackbar�Ż���ʾͼƬ
	//ContrastAndBright(g_nBrightValue, 0);
	
	waitKey();
	return 0;
}