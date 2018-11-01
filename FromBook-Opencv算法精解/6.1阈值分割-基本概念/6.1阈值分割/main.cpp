#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
int main()
{
	//阈值分割就是将输入图像分割成多个区域，每个区域的像素有相似性
	//一般阈值分割将灰度图的每个像素与阈值比较，将其变化为0或者255，所以阈值分割又常乘坐图像的二值化处理
	//阈值分割处理主要是根据灰度值信息提取前景
	//阈值分割分为:全局阈值分割和自适应局部阈值分割
	Mat src = (Mat_<uchar>(5, 3) << 123, 234, 68, 33, 51,
									17, 48, 98, 234, 129,
									89, 27, 45, 167, 134);
	double thresh = 150;
	Mat dst;

	threshold(src,dst,thresh,255,THRESH_BINARY);
	cout << dst << endl;
	//THRESH_BINARY(0):  大于阈值，置为maxVal,否则为0
	//THRESH_BINARY_INV(1):大于阈值，置为0,否则为maxVal
	//THRESH_TRUNC(2):大于阈值，截断置为阈值，否则不变
	//THRESH_TOZERO(3):大于阈值，不变，否则为0
	//THRESH_TOZERO_INV(4):大于阈值，置为0.否则不变

	double ostuThresh = 0;
	Mat dst_Ostu;
	//这里THRESH_OTSU表示自动计算阈值(使用OTSU算法)，计算出阈值后再利用THRESH_BINARY方式进行阈值分割
	//当然这里也可以采用此时THRESH_OTSU+THRESH_BINARY_INV，表示自动计算阈值后采用THRESH_BINARY_INV方式进行阈值分割，都可以
	//threshold函数会返回自动计算的阈值
	ostuThresh =threshold(src, dst_Ostu, ostuThresh, 255, THRESH_OTSU + THRESH_BINARY);
	cout << "ostuThresh:" << ostuThresh << endl;
	cout << dst_Ostu << endl;
	
	double triThresh = 0;
	Mat dst_Tri;
	//同样，THRESH_TRIANGLE也自动计算阈值(使用triangle算法)，然后利用THRESH_BINARY方式进行阈值分割，这里同样返回
	//此时函数还是会返回自动计算的阈值
	triThresh = threshold(src, dst_Tri, triThresh, 255, THRESH_TRIANGLE + THRESH_BINARY);
	cout << "triThresh:" << triThresh << endl;
	cout << dst_Tri << endl;

	//以上提到的都是全局阈值，即原始图片的像素与一个固定的阈值进行比较(无论是手动指定还是OTSU或者triangle算法自动该计算的阈值)
	//而局部阈值下，原图每个像素都有自己相对应的阈值，即会得到一个与原图大小相等的阈值矩阵，然后逐位比较
	//比较简单的是将该位置像素的领域位置的平均值作为该位置比较的阈值
	system("pause");
	return 0;
}
