#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

bool ROI_AddImage()
{
	Mat srcImage = imread("scene.jpg");
	Mat logoImage = imread("logo.jpg");

	if (!srcImage.data) {cout << "'scene.jpg'图像不存在" << endl; return false;}
	if (!logoImage.data) { cout << "'logo.jpg'图像不存在" << endl; return false; }

	Mat imageROI = srcImage(Rect(500, 200, logoImage.cols, logoImage.rows));

	Mat mask = imread("logo.jpg", 0);     //加载掩膜(必须是灰度图)
	//mask：即掩模。掩模是由0和1组成的一个二进制图像。当在某一功能中应用掩模时，
	//1值区域（白色区域）被处理，被屏蔽的0值区域(黑色区域)不被包括在计算中

	logoImage.copyTo(imageROI);     //将掩膜拷贝到原图的ROI区域
	//1、image.copyTo(imageROI)，作用是把image的内容粘贴到imageROI；
	//2、image.copyTo(imageROI，mask), 作用是把mask和image重叠以后把mask中像素值为1（白色区域）
	//   的点对应的image中的点直接复制给imageROI，后把mask中像素值为0的点对应的image中的点舍弃


	namedWindow("1.利用OI实现图像叠加");    //在之前就说过了，在ROI区域上的操作，就是在原图上的操作
	imshow("1.利用OI实现图像叠加", srcImage);   //所以这里通过logoImage.copyTo拷贝到imageROI，就会影响原图srcImage

	return true;
}

//两张图片线性混合
bool linearBlending()
{
	double alphaVal = 0.5;
	double betaVal;

	Mat srcImage1, srcImage2, dstImage;

	srcImage1 = imread("logo.jpg");
	srcImage2 = imread("samelogo.jpg");
	if (!srcImage1.data) { cout << "'scene.jpg'图像不存在" << endl; return false; }
	if (!srcImage2.data) { cout << "'samelogo.jpg'图像不存在" << endl; return false; }

	betaVal = 1.0 - alphaVal;
	addWeighted(srcImage1, alphaVal, srcImage2, betaVal, 0.0, dstImage);   //线性相加的两个图片一定要是相同尺寸

	//dst = src1[I]*alpha+ src2[I]*beta + gamma;        //gamma是addWeighted函数的第五个参数,而dstImage表示最后输出图片
	namedWindow("线性混合窗口1", 1);
	imshow("线性混合窗口1", srcImage1);

	namedWindow("线性混合窗口2", 1);
	imshow("线性混合窗口2", dstImage);

	return true;
}
int main()
{
	linearBlending();
	waitKey();
	return 0;
}