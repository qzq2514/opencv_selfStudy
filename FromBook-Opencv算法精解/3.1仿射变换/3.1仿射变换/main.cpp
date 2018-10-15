#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//计算仿射矩阵

	//图像是一个矩阵，对其进行平移，放缩，旋转，其实就是原图像矩阵中的数据点坐标乘以仿射矩阵进行空间变换
	//点坐标视为列向量[x;y]变换后的坐标为[x';y'],为了计算方便并且加上偏置，我们将点坐标视为
	//[x;y]->[x,;y;1],[x';y']->[x';y';1],仿射矩阵完整的是3x3的矩阵，最后一行是行向量[0,0,1]

	//为了根据变换前的结果和变换后的结果得到仿射矩阵，必须有三组坐标对，列出6个方程组，解出
	//6个未知数(即3x3仿射矩阵的前两行)

	//例如通过[0,0],[200,0],[0,200]经过仿射变化矩阵A得到的坐标点分别为[0,0],[100,0],[0,100]
	//有以下两种方法获得改变换的仿射矩阵

	//1.直接通过三对Point2f类型的获得仿射矩阵
	Point2f srcP[] = {Point2f(0, 0), Point2f(200, 0), Point2f(0, 200)};   //变换前的三个点
	Point2f dstP[] = { Point2f(0, 0), Point2f(100, 0), Point2f(0, 100) };
	Mat A1 = getAffineTransform(srcP, dstP);
	cout << A1 << endl;             //返回的仿射矩阵只是2x3大小的，即前两行
	cout << "-----------" << endl;

	//2.把点放进Mat类型中,通过getAffineTransform函数返回仿射矩阵A2
	//注意A2数据类型是CV_64F,而不是CV_32F
	Mat srcM = (Mat_<float>(3,2) << 0, 0, 200, 0, 0, 200);   //每一行为一个坐标点
	Mat dstM = (Mat_<float>(3, 2) << 0, 0, 100, 0, 0, 100);
	Mat A2 = getAffineTransform(srcM, dstM);
	cout << A2 << endl;
	cout << "-----------" << endl;


	//之前学过Mat矩阵的乘法获得矩阵，所以可以通过矩阵乘法获得仿射矩阵
	//即通过基本的仿射变换进行组合得到复杂的仿射变换
	Mat s = (Mat_<float>(3, 3) << 0.5, 0, 0, 0, 0.5, 0, 0, 0, 1);  //缩放矩阵
	Mat t = (Mat_<float>(3, 3) << 1, 0, 100, 0, 1, 200, 0, 0, 1);   //平移矩阵  (具体理论看书)
	Mat A3;	
	//gemm(s, t, 1.0, Mat(), 0, A3, 0);  //A3=1.0*s*t+0*Mat()，表示先进行平移，再进行缩放变换
	A3 = t*s;       //这种表示先缩放，再平移，因为[x';y';1]=A3*[x;y;1],矩阵是从右向左运算的
	                //并且最后组合的仿射矩阵与简单仿射顺序有关系
	cout << A3 << endl;
	cout << "-----------" << endl;

	//横纵坐标的缩放比例相同时(即Sx=Sy等比缩放)，则先进行缩放再进行旋转，
	//或者先进行旋转，再进行缩放，得到是相同的仿射矩阵，可以通过getRotationMatrix2D函数获得仿射矩阵
	//getRotationMatrix2D(p1,angle,scale)中参数分别表示旋转中心，旋转度数(角度，不是弧度),缩放比例
	Mat A4 = getRotationMatrix2D(Point2f(40, 50), 30, 0.5); //和getAffineTransform一样，同样返回2x3的CV_64F的矩阵
	cout << A4 << endl;

	system("pause");
	return 0;
}