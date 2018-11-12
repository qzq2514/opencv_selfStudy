#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//原Mat是值传递，要用Mat &dst才能达到实参、形参同时改变
//或者使用OutputArray &dst，
void conv2D(Mat src,Mat &dst,Mat kernel,int ddepth,Point anchor=Point(-1,-1),int borderType=BORDER_DEFAULT)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1); //卷积核翻转180度
	
	//same卷积操作,结果图和原图同大小，同数量通道
	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType); 
}
//默认BORDER_DEFAULT卷积，即BORDER_REFLECT_101卷积，值都为4
Mat roberts(Mat src, int ddepth, int x = 1, int y = 0, int borderType = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	Mat robert_135 = (Mat_<float>(2, 2) << 1, 0, 0,-1); // 45度方向的边缘
	Mat robert_45 = (Mat_<float>(2, 2) <<  0, 1, -1, 0); // 135度方向的边缘
	//cout << "-------------" << endl<<"robert_135:" << endl << robert_135 << endl;
	//根据x,y值选定不同的卷积方式
	if (x != 0 && y == 0)
		conv2D(src, dst, robert_135, ddepth, Point(0, 0), borderType); //robert_135卷积核的锚点在(0,0)
	else
		conv2D(src, dst, robert_45,  ddepth, Point(1, 0), borderType); //robert_45卷积核的锚点在(1,0)
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	//测试用
	/*Mat src = (Mat_<uchar>(5, 5) << 2, 4, 6, 2, 4,
		5, 2, 7, 2, 5,
		43, 75, 96, 4, 9,
		4, 3, 23, 6, 8,
		94, 2, 6, 12, 8);*/
	//cout << "-------------" << endl << "src:"<<endl<<src << endl;
	
	Mat img_robert1=roberts(src, CV_32FC1, 1, 0); //robert_135卷积(45度方向边缘检测)
	Mat img_robert1_abs;
	//边缘检测可能出现负值(img_robert1可能含有负值元素)，使用convertScaleAbs函数取图像的绝对值
	//dst[i,j]=abs(src[i,j]*alpha+beta),且convertScaleAbs返回8位无符号整型(CV_8U)
	convertScaleAbs(img_robert1, img_robert1_abs, 1, 0);
	imshow("robert_135-45度边缘检测", img_robert1_abs);

	/*cout << "-------------"<<endl<<"img_robert1:" << endl << img_robert1 << endl;
	cout << "-------------" << endl << "img_robert1_abs:" << endl << img_robert1_abs << endl;
	cout << img_robert1.type() << endl;         //CV_32FC1
	cout << img_robert1_abs.type() << endl;     //CV_8UC1
	cout << img_robert1_abs << endl;*/

	Mat img_robert2 = roberts(src, CV_32FC1, 0,1); //robert_135卷积(45度方向边缘检测)
	Mat img_robert2_abs;
	convertScaleAbs(img_robert2, img_robert2_abs, 1, 0);
	imshow("robert_45-135度边缘检测", img_robert2_abs);


	//得到两个方向边缘检测结合下的边缘强度
	//这里采用的是平方根的形式，边缘强度的图片每个像素是不同方向检测后对
	//应位置像素值的平方和的平方根
	Mat img_robert1_pow, img_robert2_pow;
	//虽说这里因为img_robert1_abs是img_robert1的绝对值结果，平方后都一样，
	//但是img_robert1是CV_32F,img_robert1_abs是CV_8U,且pow不改变Mat的类型，sqrt要用float或者double类型
	//所以这里写成img_robert1,而非img_robert1_abs
	pow(img_robert1,2.0,img_robert1_pow);     
	pow(img_robert2, 2.0, img_robert2_pow);

	//cout << "-------------" << endl << "img_robert1_pow:" << endl << img_robert1_pow << endl;
	//cout << "-------------" << endl << "img_robert2_pow:" << endl << img_robert2_pow << endl;
	Mat fullEdge;
	//除了平方和的开方，还有取绝对值的最大值、绝对值的和等其他使用多个方向边缘检测进行边缘增强的方法
	//因为边缘检测，得到的边缘位置是白色，在CV_8UC1中是大值-偏向255，所以边缘增强后能够将不同方向
	//边缘检测后的边缘(白色区域)合并起来进而得到各个方向的边缘
	sqrt(img_robert1_pow + img_robert2_pow, fullEdge); 

	//这里fullEdge和原图-img_robert1_pow + img_robert2_pow类型一样，都是CV_32FC1,
	//但是其存的值是0-255之间，所以要变换类型
	fullEdge.convertTo(fullEdge, CV_8UC1);
	/*cout << fullEdge.type();
	cout << CV_32F << endl;;
	cout << "-------------" << endl << "fullEdge:" << endl << fullEdge << endl;*/
	imshow("fullEdge", fullEdge);

	waitKey(0);
	return 0;
}