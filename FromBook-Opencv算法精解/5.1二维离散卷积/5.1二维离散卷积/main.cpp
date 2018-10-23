#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//边界填充
	Mat src = (Mat_<uchar>(3, 3) << 5, 1, 7, 1, 5, 9, 2, 6, 2);

	Mat dst;
	//copyMakeBorder函数对图片进行边界填充
	//原图，结果图，上下左右四个边界的填充宽度，填充方式
	//对于填充方式，常见的有:
	//BORDER_CONSTANT:常数填充
	//BORDER_REPLICATE:边界复制
	//BORDER_REFLECT:图像的边界边作为轴进行镜像复制
	//BORDER_WRAP:平铺复制
	//具体效果可代入不同的参数查看
	copyMakeBorder(src, dst, 2, 2, 2, 2, BORDER_REFLECT_101);
	cout << dst << endl;

	//卷积运算
	Mat I = (Mat_<float>(2, 2) << 1, 2, 3, 4);  //原图像矩阵
	Mat K = (Mat_<float>(2, 2) << -1, -2, 2, 1);//卷积核,其类型必须是CV_32F或者CV_64F

	Mat O;
	//参照书上的步骤，卷积第一步要将卷积逆时针旋转180度
	flip(K, K, -1);
	//第二步进行卷积运算,opencv进行的都是same卷积运算，即输出图像尺寸和原图像矩阵一致
	//filter2D(原图，输出图，ddepth,锚点,delta,填充方式)
	//其中ddepth表示输出图像的位深，具体位深(ddepth)与depth关系见"ddepth与depth关系.png"
	//锚点取自于卷积核K的一个像素位置，锚点在原图的每个位置跑，然后计算现在整个卷积核和原图的卷机值，作为输出图像对应位置的像素值
	filter2D(I,O,-1,K,Point(0,0),0.0,BORDER_CONSTANT);

	cout << O << endl;


	system("pause");
	//waitKey(0);
	return 0;
}