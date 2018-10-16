#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat I;
Mat pI;
Point2f IPoint, pIPoint;
int i = 0, j = 0;
Point2f src[4];
Point2f dst[4];
Scalar scalars[4] = { Scalar(0,0,0),Scalar(0,0,255),Scalar(0,255,0),Scalar(255,255,255)};
void mouse_I(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	    case CV_EVENT_LBUTTONDOWN:   //鼠标按下事件
			
			IPoint = Point2f(x, y);
			src[i] = IPoint;
			cout << "mouse_I鼠标事件....." << src[i] << endl;
			//在图片I上画圆，以src[i]为圆心，5为半径，scalars[i]是圆的颜色，3是圆的线条粗细,6是线条类型
			circle(I, src[i], 5, scalars[i], 3,6);
			i += 1;
		    break;
		default:
			break;
	}
}
void mouse_pI(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:   //鼠标按下事件
		pIPoint = Point2f(x, y);
		dst[j] = pIPoint;
		cout << "mouse_pI鼠标事件....." << dst[j] << endl;
		circle(pI, dst[j], 5, scalars[j], 3);
		j += 1;
		break;
	default:
		break;
	}
}
int handle1()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	int w = I.cols;
	int h = I.rows;
	Mat src = (Mat_<float>(4, 2) << 0, 0, w - 1, 0, 0, h - 1, w - 1, h - 1);
	Mat dst = (Mat_<float>(4, 2) << 50, 50, w/3, 50, 50, h - 1, w - 1, h - 1);


	//相比较与仿射变换，投影变换需要四对坐标点(即src，dst都是4*2矩阵)，和getAffineTransform一样传入坐标对，
	//且坐标对的数据类型也是float,返回的变换矩阵为3*3，数据类型是CV_64F(getAffineTransform返回2*3矩阵)，
	Mat p = getPerspectiveTransform(src, dst);

	Mat result;
	//投影变换函数warpPerspective参数意义和warpAffine一样:
	//参数：原图像，结果图像，变换矩阵，图片大小(包括非有效区域),插值算法，非有效区域填充方法，填充颜色值
	warpPerspective(I, result, p, pI.size(), INTER_NEAREST, BORDER_CONSTANT, Scalar(100, 120, 200));

	imshow("Origin pic", I);
	imshow("After perspective", result);

	waitKey(0);
	return 0;
}
int handle2()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	pI = 255 * Mat::ones(I.size(), CV_8UC3);   //目标图初始化为空白图
	cout << pI.rows << "-" << pI.cols << endl;
	//在原图窗口上定义鼠标事件
	namedWindow("I", WINDOW_AUTOSIZE);    //第二个参数表示窗口是否可以改变
	setMouseCallback("I", mouse_I, NULL); //(WINDOW_NORMAL=0可改变)(WINDOW_AUTOSIZE=1不可改变)

	//在输出窗口上定义鼠标事件
	namedWindow("pI", 1);
	setMouseCallback("pI", mouse_pI, NULL);

	imshow("I", I);
	imshow("pI", pI);
	while (!(i == 4 && j == 4))
	{
		imshow("I", I);
		imshow("pI", pI);
		if (waitKey(50) == 'q')
			break;
	}

	cout << "投影特征点选取完毕...." << endl;
	imshow("I", I);
	imshow("pI", pI);

	/*for (int n = 0; n < 4; n++)
	{
		cout << src[n] << ":" << dst[n] << endl;
	}*/

	//移除鼠标事件
	setMouseCallback("I", NULL, NULL);
	setMouseCallback("pI", NULL, NULL);

	
	Mat p = getPerspectiveTransform(src, dst);
	/*cout << "投影变换矩阵:" << endl;
	cout << p << endl;*/


	//进行投影变化
	Mat result;
	warpPerspective(I, result, p, pI.size());
	imshow("After perspective", result);//显示变换结果

	waitKey(0);
	return 0;
}
int main()
{
	 //投影变换：给定四对CV_32F类型数据点,即4*2，得到CV_64F类型的变换矩阵(3*3)
//复习:仿射变换：....三..................,..3*2，........................(3*2)
	//handle1();        //程序中就给定了投影变换的四组坐标对
	handle2();          //通过在画面中手动点击鼠标获取四对坐标对
	
	
	return 0;
}