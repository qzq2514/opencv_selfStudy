#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat topHatImg,bottomHatImg;
int r_topHat =1, r_bottomHat =1;
int iterNUm_topHat = 1, iterNUm_bottomHat = 1;
int MaxR=20,MaxIterNum=20;
Mat binImg;
void close(int, void*)
{
	Mat sClose = getStructuringElement(MORPH_RECT, Size(2 * r_bottomHat + 1, 2 * r_bottomHat + 1));

	morphologyEx(binImg, bottomHatImg,MORPH_BLACKHAT, sClose,Point(-1,-1), iterNUm_bottomHat);
	imshow("bottomHatImg", bottomHatImg);
}
void topHat(int, void*)
{
	Mat sOpen = getStructuringElement(MORPH_RECT, Size(2 * r_topHat + 1, 2 * r_topHat + 1));

	morphologyEx(binImg, topHatImg, MORPH_TOPHAT, sOpen, Point(-1, -1), iterNUm_topHat);
	imshow("topHatImg", topHatImg);
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);
	
	
	//底帽运算和顶帽运算都是开运算和闭运算的组合
	
	

	//底帽运算
	//又称黑帽运算，闭运算结果-原图
	namedWindow("bottomHatImg", WINDOW_AUTOSIZE);
	createTrackbar("半径", "bottomHatImg", &r_bottomHat, MaxR, close);
	createTrackbar("迭代次数", "bottomHatImg", &iterNUm_bottomHat, MaxIterNum, close);
	close(0, 0);
	
	//顶帽运算
	//原图-开运算结果
	//因为开运算可以消除暗背景下的较亮区域，那么用原图减去开运算结果，那么就可以得到原图中较亮的区域
	//开运算的暗背景下的较亮区域变黑(灰度值变小)，这时原图对应的位置减去灰度值变小的区域，就会得到灰度值大的区域(变亮)
	namedWindow("topHatImg", WINDOW_AUTOSIZE);
	createTrackbar("半径", "topHatImg", &r_topHat, MaxR, topHat);
	createTrackbar("迭代次数", "topHatImg", &iterNUm_topHat, MaxIterNum, topHat);
	topHat(0, 0);


	//形态学梯度
	//膨胀结果-腐蚀结果
	//有边缘检测的效果
	Mat gradientImg;
	Mat s = getStructuringElement(MORPH_RECT, Size(2 * 2 + 1, 2 * 2 + 1));
	morphologyEx(binImg, gradientImg, MORPH_GRADIENT, s, Point(-1, -1), 1);
	imshow("gradientImg", gradientImg);

	waitKey(0);
	return 0;
}