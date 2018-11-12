#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;

void conv2D(Mat src, Mat &dst, Mat kernel, int ddepth, Point anchor = Point(-1, -1), 
	int borderType = BORDER_DEFAULT)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1);
	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType);
}
void sepConv2D_Y_X(Mat src, Mat &dst, int ddepth, Mat kernelY, Mat kernelX, 
	Point anchor = Point(-1, -1), int bordertype = BORDER_DEFAULT)
{
	Mat src_kerY;
	//使用离散卷积的性质，分别在y方向和x方向进行卷积
	conv2D(src, src_kerY, kernelY, ddepth, anchor, bordertype);
	conv2D(src_kerY, dst, kernelX, ddepth, anchor, bordertype);
}
Mat prewitt(Mat src, int ddepth, int x=1, int y = 0, int bordertype = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	if (x != 0 && y == 0)
	{
		//Prewitt_x垂直方向边缘检测，卷积核是[[1 0 -1],
		//                                    [1 0 -1],
		//                                    [1 0 -1]]
		//这是一个可分离卷积可分离为[[1],[1],[1]]和[1,0-1]
		Mat prewitt_x_y = (Mat_<float>(3, 1) << 1, 1, 1);
		Mat prewitt_x_x = (Mat_<float>(1, 3) << 1, 0, -1);
		//这里先对那个分离后的卷积核进行卷积都行
		//prewitt_x_y表示对垂直方向进行非归一化(即非归一化的均值平滑)，有着平滑的效果
		//prewitt_x_x则是进行水平方向的差分计算边缘
		sepConv2D_Y_X(src, dst, ddepth, prewitt_x_y, prewitt_x_x, Point(-1, -1), bordertype);
	}
	else
	{
		//Prewitt_y水平方向边缘检测，卷积核是[[ 1  1  1],
		//                                    [ 0  0  0],
		//                                    [-1 -1 -1]]
		//这是一个可分离卷积可分离为[[1,1,1]和[[1],[0],[-1]]
		Mat prewitt_y_y = (Mat_<float>(1, 3) << 1, 1, 1);
		Mat prewitt_y_x = (Mat_<float>(3, 1) << 1, 0, -1);
		//同样Prewitt水平方向检测卷积核也是可分离的，这里对哪个分离的子卷积核先进行卷积都行
		//且prewitt_y_y表示对水平方向进行非归一化，有着平滑的效果
		//prewitt_y_x则是进行垂直方向的差分计算边缘
		sepConv2D_Y_X(src, dst, ddepth, prewitt_y_x,prewitt_y_y, Point(-1, -1), bordertype);
	}
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg",0);
	imshow("src", src);

	/*Mat src = (Mat_<uchar>(5, 5) << 2, 4, 6, 2, 4,
		5, 2, 7, 2, 5,
		43, 75, 96, 4, 9,
		4, 3, 23, 6, 8,
		94, 2, 6, 12, 8);
	cout << "-------------" << endl << "src:" << endl << src << endl;*/

	Mat img_prewitt_x,abs_img_prewitt_x;  //检测列边缘-垂直方向的边缘
	//解决"8.1边缘检测-Roberts算子"中一个没注意的问题
	//因为边缘检测中存在减法，可能导致最后结果中有负数，所以这里可以用CV_32FC1暂时保存负数
	//但是不能用CV_8UC1保存，因为其范围是0-255的正整数，不然负数会被截断为0
	img_prewitt_x=prewitt(src, CV_32FC1, 1, 0);   
	//img_prewitt_x是CV_32FC1,abs_img_prewitt_x是CV_8UC1
	convertScaleAbs(img_prewitt_x, abs_img_prewitt_x, 1.0, 0);
	imshow("垂直方向边缘检测", abs_img_prewitt_x);

	Mat img_prewitt_y, abs_img_prewitt_y;  //检测行边缘-水平方向的边缘
	img_prewitt_y = prewitt(src, CV_32FC1, 0,1);
	convertScaleAbs(img_prewitt_y, abs_img_prewitt_y, 1.0, 0);
	imshow("水平方向边缘检测", abs_img_prewitt_y);

	Mat img_prewitt_x2, img_prewitt_y2;
	pow(img_prewitt_x, 2.0, img_prewitt_x2);
	pow(img_prewitt_y, 2.0, img_prewitt_y2);

	Mat edge;
	sqrt(img_prewitt_x2 + img_prewitt_y2, edge);
	//同样，因为img_prewitt_x，img_prewitt_y都是CV_32FC1，但是保存的值都是0-255,
	//进行幂运算pow后得到的结果也是CV_32FC1，所以变为CV_8UC1才能正确显示结果
	edge.convertTo(edge, CV_8UC1); 
	imshow("edge", edge);
	waitKey(0);


	return 0;
}