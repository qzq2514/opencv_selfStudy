#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat edgeFull;
int thresh;
void conv2D(Mat src, Mat &dst, Mat kernel, Point anchor = Point(-1, -1), int bordertype = BORDER_DEFAULT)
{
	flip(kernel, kernel, -1);
	filter2D(src, dst,CV_32FC1, kernel,anchor,0.0,bordertype);
}
Mat kirsch(Mat src, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT)
{
	Mat dst;
	vector<Mat> edges;
	edges.clear();

	//Kirsch算子由8个卷积核构成，分别如下
	Mat dst1;
	Mat ker1 = (Mat_<float>(3, 3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);
	conv2D(src, dst1, ker1);
	convertScaleAbs(dst1, dst1);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst1);

	Mat dst2;
	Mat ker2 = (Mat_<float>(3, 3) << -3, -3, -3, -3, 0, -3, 5, 5, 5);
	conv2D(src, dst2, ker2);
	convertScaleAbs(dst2, dst2);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst2);

	Mat dst3;
	Mat ker3 = (Mat_<float>(3, 3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
	conv2D(src, dst3, ker3);
	convertScaleAbs(dst3, dst3);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst3);

	Mat dst4;
	Mat ker4 = (Mat_<float>(3, 3) << -3, -3, -3, 5, 0, -3, 5, 5, -3);
	conv2D(src, dst4, ker4);
	convertScaleAbs(dst4, dst4);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst4);

	Mat dst5;
	Mat ker5 = (Mat_<float>(3, 3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
	conv2D(src, dst5, ker5);
	convertScaleAbs(dst5, dst5);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst5);

	Mat dst6;
	Mat ker6 = (Mat_<float>(3, 3) << 5, -3, -3, 5, 0, -3, 5, -3, -3);
	conv2D(src, dst6, ker6);
	convertScaleAbs(dst6, dst6);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst6);

	Mat dst7;
	Mat ker7 = (Mat_<float>(3, 3) << -3, -3, -3, -3, 0, 5, -3, 5, 5);
	conv2D(src, dst7, ker7);
	convertScaleAbs(dst7, dst7);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst7);

	Mat dst8;
	Mat ker8 = (Mat_<float>(3, 3) << 5, 5, -3, 5, 0, -3, -3, -3, -3);
	conv2D(src, dst8, ker8);
	convertScaleAbs(dst8, dst8);      //由CV_32FC1变为CV_8UC1
	edges.push_back(dst8);

	dst = edges[0].clone();

	//计算八个方向边缘检测的边缘强度-这里使用取最大绝对值的边缘强度计算方法
	for (int i = 1; i < edges.size(); i++)
	{
		/*stringstream ss;
		ss << "Debug" << i;
		imshow(ss.str(), edges[i]);
		waitKey(0);*/
		max(dst, edges[i], dst);       //使用自带的max函数就能计算逐位比较去最大值的功能
	}
	return dst;
}
void adjustThresh(int, void*)
{
	Mat threshImg;
	threshold(edgeFull, threshImg, thresh, 255, THRESH_BINARY);
	imshow("调整阈值", threshImg);
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	edgeFull= kirsch(src);
	imshow("kirsch边缘检测", edgeFull);


	namedWindow("调整阈值", WINDOW_AUTOSIZE);
	createTrackbar("阈值", "调整阈值", &thresh, 255,adjustThresh );
	adjustThresh(0, 0);

	//Robinson算子同样由8个卷积核构成，具体见"Kirsch算子和Robinson算子.jpg"
	//利用Robinson算子进行边缘检测的步骤与Kirsch边缘检测无大差别
	waitKey(0);
	return 0;
}