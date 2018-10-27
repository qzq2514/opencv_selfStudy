#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>
#define PI 3.14159265


using namespace std;
using namespace cv;

Mat getGaussianKernel_1D(float sigma, Size size, int dtype)
{
	Mat GaussianKernel_1D = Mat::zeros(size, dtype);
	
	//cout << size.width << "-" << size.height << endl;
	//要求必须是垂直方向或者水平方向的一维卷积核，且长度必须为奇数
	CV_Assert((size.height % 2 == 1 && size.width % 2 == 1) && (size.height == 1 || size.width == 1));

	int len = size.height != 1 ? size.height : size.width;    //取不为一的作为长度
	float mean= (len - 1)/2.0;
	float sum=0;
	for (int i = 0; i < len; i++)
	{
		float val = 1 / (sqrt(2 * PI)*sigma)*exp(-pow(i- mean, 2) / (2 * sigma*sigma));
		GaussianKernel_1D.at<double>(i) = val;
		sum += val;
	}
	//最后注意进行归一化处理
	GaussianKernel_1D = GaussianKernel_1D / sum;
	return GaussianKernel_1D;
}
Mat myGaussianBlur(Mat src,Size size,float sigmaX, float sigmaY)
{
	Mat dst;

	//根据高斯卷积核离散性质，将其分为水平方向和垂直方向的一维卷积核
	//这里使用自己写的getGaussianKernel_1D和opencv自带的getGaussianKernel函数得到一维的卷积核
	//注意，卷积核数值类型必须是CV_32F或者CV_64F
	Mat GaussianX = getGaussianKernel_1D(sigmaX,Size(size.width,1), CV_32F);
	Mat GaussianY = getGaussianKernel(size.height, sigmaY, CV_32F);   //默认是垂直方向的一维卷积核
	/*cout << GaussianX << endl;
	cout << GaussianY << endl;*/   //对比可以看到，两种的卷积结果是一样的
	
	//Mat Gaussian2D;
	//两个一维卷积核进行卷积运算得到二维的卷积核-不可用，因为opencv中的卷积运算filter2D为same卷积
	//flip(GaussianY, GaussianY, -1);   //先进行转置，即逆时针旋转180度
	//filter2D(GaussianX, Gaussian2D, CV_32F, GaussianY, Point(0, size.height - 1),0.0, BORDER_CONSTANT);
	//cout << Gaussian2D << endl;

	//根据高斯核的离散卷积性质，这里先进行水平方向的卷积，然后进行垂直方向的卷积
	//注意卷积之前要进行卷积核的吗、逆时针旋转180度
	flip(GaussianX, GaussianX, -1);
	cout << src.depth() << endl;   //0(CV_8U)
	filter2D(src, dst, -1, GaussianX, Point(-1, -1), 0.0, BORDER_DEFAULT);

	flip(GaussianY, GaussianY, -1);
	filter2D(dst, dst, -1, GaussianY, Point(-1, -1), 0.0, BORDER_DEFAULT);
	return dst;
}
int main()
{
	Mat src1 = imread("lena_full.jpg", 1);
	//Mat dst1=myGaussianBlur(src1,Size(5,5),1.1,1.1);   //卷积核为5x5,且水平和垂直方向的标准差都等于1.1
	imshow("Origin", src1);
	//imshow("myGaussianBlur", dst1);

	Mat dst2;
	//opencv自带的高斯模糊，参数意义依次为(原图，结果图，核大小，水平和数值方向的标准差)
	//其中，核较小时，对标准差不敏感，平滑效果差别不大
	//核较大时，对标准差敏感，平滑效果差别大
	GaussianBlur(src1, dst2, Size(5, 5), 1.1, 1.1);   
	imshow("GaussianBlur", dst2);


	//特么的，这里不能同时运行myGaussianBlur和GaussianBlur，本以为是同时对原图图片引用操作造成，
	//然后clone原图分别运行也不行，特么出鬼了
	waitKey(0);
	return 0;
}