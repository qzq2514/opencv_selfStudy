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



	//插值运算
	//插值运算就是为了计算在仿射变化时，目标图像中无法由原图像准确仿射的元素值
	//例如，在以原点(0,0)为中心点，放缩比例为2倍的放缩变换中有：
	//fo(2x,2y)=fi(x,y)，所以目标图像中坐标为(3,3)的像素值按理说为原图像中坐标值为(1.5,1.5)处
	//的值，但是下标索引没有小数啊，所以要找到合适的值去当作fi(1.5,1.5),插值运算就是找到这样合适的值
	
	Mat src = imread("lena_full.jpg",1);
	imshow("原图", src);
	
	int w = src.cols;
	int h = src.rows;
	cout << w << ":" << h << endl;  //330:706

	Mat AA1 = (Mat_<float>(2, 3) << 0.5, 0, 0, 0, 0.5, 0);   //仿射变换矩阵-缩小两倍
	Mat d1;
	//这里缩小两倍,是指图像的有效区域缩小为原来1/2，但是imshow出来的图片仍然是和原图同样大小的
	//其余非有效区域可以通过指定为常量颜色值(BORDER_CONSTANT参数)，并通过Scalar(B,G,R)给定指定的填充色
	warpAffine(src, d1, AA1,Size(w,h),INTER_NEAREST,BORDER_CONSTANT,Scalar(100,120,200));
	//INTER_NEAREST:最近邻插值，即某小数坐标的元素值等于最近的整点坐标的元素值
	cout << d1.cols << ":" << d1.rows << endl;
	imshow("缩小两倍", d1);    //330:706

	Mat AA2 = (Mat_<float>(2, 3) << 0.5, 0, w/4.0, 0, 0.5, h/4.0);   //仿射变换矩阵-缩小两倍再平移
	Mat d2;
	//INTER_NEAREST：双线性插值，用小数坐标点附近的四个整数坐标点的像素值进行加权求和
	warpAffine(src, d2, AA2, Size(w, h), INTER_NEAREST, BORDER_CONSTANT, Scalar(100, 120, 200));
	imshow("缩小两倍再平移", d2);

	Mat AA3 = getRotationMatrix2D(Point2f(w / 2, h / 2), 30, 1);
	Mat d3;
	warpAffine(d2, d3, AA3, Size(w, h));
	imshow("缩小两倍平移后再中心逆时针旋转30度", d3);
	

	//简化版缩放
	Mat d4;
	//缩放为原来的一半
	resize(src, d4, Size(w / 2, h / 2),0,0,INTER_NEAREST);
	//resize(src, d4, Size(), 0.5, 0.5, INTER_NEAREST);   //同样效果,Size后面的两个参数是fx,fy即宽高的缩放比例
	imshow("简化版缩放两倍", d4);//resize得到的目标图只包括有效区域，所以也就不需要填充非有效区域


	//简化版旋转，opencv3.x有旋转函数rotate,本版本好像是2.x就不实验了
	
	cout << "CV_VERSION:"<<CV_VERSION << endl;// ???明明是3.1.0啊,算了，以后就使用getRotationMatrix2D函数吧
	waitKey(0);
	//system("pause");
	return 0;
}