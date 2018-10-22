#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat I=imread("lena_full.jpg", 0);

	//直方图正规化，就是图像矩阵的min-max归一化，也就是线性变换的一种，
	//即将原图的像素值归一化到指定的范围区间内:
	//O(r,c)=[((Omax - Omin) / (Imax, Imin)]*(I(r,c)-Imin)+Omin
	double Imax, Imin;

	//找到原图的像素最小和最大值，第三，四个参数分别是最大最小像素值所在的点坐标
	minMaxLoc(I, &Imin, &Imax, NULL, NULL);

	cout << "原图:"<<Imin << "--" << Imax << endl;  //0-255
	
	double Omin = 0, Omax = 100;//将目标图的像素值归一化到0-100范围内
	//根据min-max归一化(其实就是一个线性变化的映射)的公式，得到在线性变化下的a,b值
	double a = (Omax - Omin) / (Imax-Imin);
	double b = Omin - a*Imin;   

	//进行线性变化
	//如果是多通道图，那么就对多个通道分别进行正规化操作
	Mat O;

	convertScaleAbs(I, O, a, b);
	imshow("原图", I);
	imshow("直方图正规化图", O);
	minMaxLoc(O, &Imin, &Imax, NULL, NULL);
	cout << "直方图正规化图:" << Imin << "--" << Imax << endl; //0-100


	//opencv自带的正规化函数

	Mat dst;
	//最大最小归一化，如果将下面的参数10改为0，则和上面的效果一致
	normalize(I, dst, 100, 10, NORM_MINMAX, CV_8U);
	
	imshow("normalize_NORM_MINMAX", dst);
	minMaxLoc(dst, &Imin, &Imax, NULL, NULL);
	cout << "normalize_NORM_MINMAX:" << Imin << "--" << Imax << endl; //10-100
	//normalize函数的第5个参数还可以改为NORM_L1,NORM_L2,NORM_INF分别将原图像素值除以范数值，
	//然后进行a,b的线性变化,具体参考图"normalize参数1.jpg"和"normalize参数2.jpg"
	waitKey(0);

	return 0;
}