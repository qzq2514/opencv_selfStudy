#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

void conv2D(Mat src, Mat &dst, int ddepth, Mat kernel, int borderType)
{
	flip(kernel, kernel, -1);
	filter2D(src, dst, ddepth, kernel, Point(-1, -1));
}
Mat scharr(Mat src, int ddepth, int x, int y, int borderType = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	Mat ker_scharr_x = (Mat_<float>(3, 3) << 3, 0, -3, 10, 0, -10, 3, 0, -3);
	Mat ker_scharr_y = (Mat_<float>(3, 3) << 3, 10, 3, 0, 0, 0, -3, -10, -3);
	if (x == 1 && y == 0) //垂直边缘检测
	{
		//不像书上写的一样，scharr算子也是可分离的
		//垂直方向边缘检测可以分离为:
		//Mat scharr_x_x=(Mat_<float>(3,1)<<3,10,3);        垂直方向的平滑算子
		//Mat scharr_x_y = (Mat_<float>(1, 3) << 1, 0, -1); 水平方向的差分算子

		//当然也可以直接使用不分离的scharr算子直接卷积，像下面这句一样
		conv2D(src, dst, ddepth, ker_scharr_x, BORDER_DEFAULT);
	}
	else //水平边缘检测
	{
		//conv2D(src, dst, ddepth, ker_scharr_y, BORDER_DEFAULT);


		Mat scharr_y_y = (Mat_<float>(1, 3) << 3, 10, 3);    //水平方向的平滑算子
		Mat scharr_y_x = (Mat_<float>(3, 1) << 1, 0, -1);    //垂直方向的差分算子
		conv2D(src, dst, ddepth, scharr_y_y, BORDER_DEFAULT);
		conv2D(dst, dst, ddepth, scharr_y_x, BORDER_DEFAULT);

	}


	//此外scharr算子还可以扩展到其他方向：
	//Mat scharr_135 = (Mat_<float>(3, 3) << 10,  3,  0,
	//										  3,  0, -3,
	//										  0, -3, -10); //检测45度方向的边缘

	//Mat scharr_45 = (Mat_<float>(3, 3) <<   0,  3, 10,
	//										  -3,  0,  3,
	//										 -10, -3,  0); //检测135度方向的边缘
	//但是这两个算子是真的不能分离

	return dst; 
}
int main()
{
	Mat src = imread("timg.jpg",0);
	imshow("src", src);

	Mat img_scharr_x, img_scharr_x_abs;
	img_scharr_x = scharr(src, CV_32FC1, 1, 0);
	convertScaleAbs(img_scharr_x, img_scharr_x_abs, 1.0, 0.0);
	imshow("scharr垂直检测",img_scharr_x_abs);

	Mat img_scharr_y, img_scharr_y_abs;
	img_scharr_y = scharr(src, CV_32FC1,0,1);
	convertScaleAbs(img_scharr_y, img_scharr_y_abs, 1.0, 0.0);
	imshow("scharr水平检测", img_scharr_y_abs);


	//这里使用另一种边缘强度的计算-按位取最大值
	Mat edge= img_scharr_x_abs.clone();
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (img_scharr_y_abs.at<uchar>(r, c) > edge.at<uchar>(r, c))
				edge.at<uchar>(r, c) = img_scharr_y_abs.at<uchar>(r, c);
		}
	}
	imshow("edge", edge);


	//opencv自带的Scharr边缘检测，和Sobel一样，同样对于矩形框的上边缘和左边缘检测效果不好
	Mat dstOpencv;
	Scharr(src, dstOpencv, CV_8UC1,1,0, 1, 0 );
	imshow("dstOpencv", dstOpencv);
	waitKey(0);
	return 0;
}