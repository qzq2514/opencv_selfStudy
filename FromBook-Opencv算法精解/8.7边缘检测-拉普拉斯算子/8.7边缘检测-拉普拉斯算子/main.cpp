#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//������˹���Ӿ��Ǽ���ê�㴦����ˮƽ�ʹ�ֱ������ĸ����ڵ��ƽ��ֵ֮��Ĳ�ֵ(ֻ�����һ��4�ı���)
	//�䲻��Sobel���Ӻ�Prewitt����һ�����и�ƽ���Ĺ���
	//�����������˹���Ӽ�"������˹����.jpg"
	Mat src = imread("lena_full.jpg", 0);
	imshow("src", src);

	Mat dst;
	Laplacian(src, dst,CV_32FC1,5,1.0,0);
	cout << dst.type() << endl;   //CV_32FC1
	convertScaleAbs(dst, dst, 1, 0);
	imshow("dst", dst);


	//�Ƚ��и�˹ƽ���ٽ���������˹��Ե��⣬�ܹ����ٱ�Ե���������
	Mat GaussAndLaplac;
	GaussianBlur(src, src, Size(7, 1), 1, 0);
	cout << src.type() << endl;     //GaussianBlur���Mat����CV_8UC1

	Laplacian(src, GaussAndLaplac, CV_32FC1, 5, 1.0, 0);
	//��ʵsize=1ʱʹ�õ���L0-��ʽ��������˹����
	//size=4ʹ��l4��ʽ��������˹����
	cout << GaussAndLaplac.type() << endl;     //CV_32FC1

	convertScaleAbs(GaussAndLaplac, GaussAndLaplac, 1, 0);
	cout << GaussAndLaplac.type() << endl;     //CV_8UC1

	//imshow("GaussAndLaplac", GaussAndLaplac);   
	system("pause");
	waitKey(0);
	return 0;
}
