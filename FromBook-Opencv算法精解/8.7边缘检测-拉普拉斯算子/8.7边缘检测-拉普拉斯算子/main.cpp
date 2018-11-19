#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//拉普拉斯算子就是计算锚点处与其水平和垂直方向的四个相邻点的平均值之间的插值(只是相差一个4的倍数)
	//其不响Sobel算子和Prewitt算子一样先有个平滑的过程
	//具体的拉普拉斯算子见"拉普拉斯算子.jpg"
	Mat src = imread("lena_full.jpg", 0);
	imshow("src", src);

	Mat dst;
	Laplacian(src, dst,CV_32FC1,5,1.0,0);
	cout << dst.type() << endl;   //CV_32FC1
	convertScaleAbs(dst, dst, 1, 0);
	imshow("dst", dst);


	//先进行高斯平滑再进行拉普拉斯边缘检测，能够减少边缘检测后的噪声
	Mat GaussAndLaplac;
	GaussianBlur(src, src, Size(7, 1), 1, 0);
	cout << src.type() << endl;     //GaussianBlur后的Mat还是CV_8UC1

	Laplacian(src, GaussAndLaplac, CV_32FC1, 5, 1.0, 0);
	//其实size=1时使用的是L0-形式的拉普拉斯算子
	//size=4使用l4形式的拉普拉斯算子
	cout << GaussAndLaplac.type() << endl;     //CV_32FC1

	convertScaleAbs(GaussAndLaplac, GaussAndLaplac, 1, 0);
	cout << GaussAndLaplac.type() << endl;     //CV_8UC1

	//imshow("GaussAndLaplac", GaussAndLaplac);   
	system("pause");
	waitKey(0);
	return 0;
}
