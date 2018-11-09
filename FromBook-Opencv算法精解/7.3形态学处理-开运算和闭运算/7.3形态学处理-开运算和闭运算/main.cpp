#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat openImg,closeImg;
int r_open=1, r_close=1;
int iterNUm_open = 1, iterNUm_close = 1;
int MaxR=20,MaxIterNum=20;
Mat src;
void close(int, void*)
{
	Mat sClose = getStructuringElement(MORPH_RECT, Size(2 * r_close + 1, 2 * r_close + 1));

	//cout << r_close << endl;
	morphologyEx(src, closeImg,MORPH_CLOSE, sClose,Point(-1,-1), iterNUm_close);
	imshow("closeImg", closeImg);
}
void open(int, void*)
{
	Mat sOpen = getStructuringElement(MORPH_RECT, Size(2 * r_open + 1, 2 * r_open + 1));

	//cout << r_open << endl;
	morphologyEx(src, openImg, MORPH_OPEN, sOpen, Point(-1, -1), iterNUm_open);
	imshow("openImg", openImg);
}
int main()
{
	src = imread("timg.jpg", 0);
	imshow("src", src);

	Mat binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);
	
	
	//开运算和闭运算都是腐蚀和膨胀操作的组合，当然可以使用erode,dilate函数组合
	//但是opencv还提供了morphologyEx函数，只要改变某个参数，便可进行开运算/闭运算/形态梯度/顶帽运算/底帽运算
	

	//闭运算
	//先膨胀，后腐蚀
	//能够填充白色物体内的黑色空洞，连接临近物体，不明显改变物体面积情况下平滑物体边界
	namedWindow("closeImg", WINDOW_AUTOSIZE);
	createTrackbar("半径", "closeImg", &r_close, MaxR, close);
	createTrackbar("迭代次数", "closeImg", &iterNUm_close, MaxIterNum, close);
	close(0, 0);
	
	//开运算
	//先腐蚀，后膨胀
	//消除亮度较高的细小区域，在纤细点处分离物体，不明显改变物体面积情况下平滑物体边界
	//应该也能填充黑色区域内的白色小物体--自己的理解，书上没有写
	namedWindow("openImg", WINDOW_AUTOSIZE);
	createTrackbar("半径", "openImg", &r_open, MaxR, open);
	createTrackbar("迭代次数", "openImg", &iterNUm_open, MaxIterNum, open);
	open(0, 0);

	waitKey(0);
	return 0;
}