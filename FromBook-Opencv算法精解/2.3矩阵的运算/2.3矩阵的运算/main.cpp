#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;
#define put_equal cout << "=======" << endl;
int main()
{

	Mat src1 = (Mat_<int>(2, 3) << 23, 123, 90, 100, 250, 0);
	Mat src2 = (Mat_<int>(2, 3) << 125, 150, 60, 100, 10, 40);
	
	//加法运算
	//1.直接"+"进行加法
	//此时两个矩阵相加，要保证两个矩阵的数据类型相同，且有时候要考虑数值溢出的问题
	Mat dst_add1 = src1 + src2;    
	dst_add1 = dst_add1 + 1000.3;   //矩阵与某个数值相加，返回的Mat数据类型与矩阵的数据类型相同
	
	for (int r = 0; r < dst_add1.rows; r++)
	{
		for (int c = 0; c < dst_add1.cols; c++)
		{
			cout << dst_add1.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat src3 = (Mat_<float>(2, 3) << 125, 150, 60, 100, 10, 40);
	Mat dst_add2;
	//使用opencv的加法函数add,可以实现图像的相加，这时候并不要求两张图像的数据类型一致，
	//同时可以并对输出函数指定数据类型,默认的type=-1，
	//表示输出的图像的数据类型与原来两张相加的图像的数据类型一致
	//所以这时候就要求原来两张图像的数据类型相同
	add(src1, src3, dst_add2,Mat(), CV_64FC1);

	for (int r = 0; r < dst_add2.rows; r++)
	{
		for (int c = 0; c < dst_add2.cols; c++)
		{
			cout << dst_add2.at<double>(r, c) << ",";   //注意的是，指定输出数据类型是64F,则表示是double,而不是float
		}
		cout << endl;
	}
	put_equal;

	Vec3f v1 = Vec3f(1, 2, 3);
	Vec3f v2 = Vec3f(10, 20, 30);
	Vec3f v3 = v1 + v2;    //加法同样可以实现向量的相加
	cout << v3 << endl;;

	put_equal;

	//矩阵减法
	//矩阵减法同样可以使用"-"重载符实现，其条件和注意事项和加法"+"一样
	Mat dst_sub;

	//和加法一样，减法同样也有opencv函数-subtract,注意事项一样的
	subtract(src1, src3, dst_sub, Mat(), CV_64FC1);
	for (int r = 0; r < dst_sub.rows; r++)
	{
		for (int c = 0; c < dst_sub.cols; c++)
		{
			cout << dst_sub.at<double>(Point(c,r)) << ",";   //注意的是，指定输出数据类型是64F,则表示是double,而不是float
		}
		cout << endl;
	}
	put_equal;

	//矩阵点乘
	//使用Mat的成员函数mul进行矩阵点乘，同样要求两个矩阵的数据类型一致
	//这里第三个参数表示矩阵点乘后还要乘以一个系数
	Mat dst_mul1=src1.mul(src2,2.0);
	//Mat dst_mul1 = src1*src2;        //不可以使用"*"进行Mat的点乘,这样表示矩阵乘法，而不是点乘
	//Mat dst_mul1 = src1.mul(2.0);    //和加减法一样，矩阵还可以和一个常数进行运算
	for (int r = 0; r < dst_sub.rows; r++)
	{
		int *ptr = dst_mul1.ptr<int>(r);
		for (int c = 0; c < dst_sub.cols; c++)
		{
			cout << ptr[c] << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat dst_mul2;
	//同样可以使用opencv的函数-multiply实现Mat相乘，1.0同样表示系数
	//其余和add,subtract函数的用法一致
	multiply(src1, src3, dst_mul2,1.0, CV_32FC1);
	int *ptr = dst_mul1.ptr<int>(0);

	for (int n = 0; n < dst_mul2.total(); n++)
	{
		
		cout << ptr[n] << ",";
	}
	cout << endl;

	put_equal;
	
	//矩阵点除
	//同样可以使用"/"实现Mat的点除，要求矩阵数据类型一致
	Mat dst_div1 = src2/src1;      //这里不是整除，而是得到小数后四舍五入
								   //遇到除数为0，则返回0，例如这里40/0，返回0
	for (int r = 0; r < dst_div1.rows; r++)
	{
		for (int c = 0; c < dst_div1.cols; c++)
		{
			int *val = (int*)(dst_div1.data + r*dst_div1.step[0] + c*dst_div1.step[1]);
			cout << *val << ",";
		}
		cout << endl;
	}

	put_equal;
	Mat dst_div2;
	//使用opencv自带的divide实现矩阵点除,遇到除数为0，则返回0，例如这里40/0，返回0
	//这里的系数2.0是作为乘数，即：dst_div2=2.0*(src3/src1)
	divide(src3,src1, dst_div2, 2.0, CV_32FC1);

	for (int r = 0; r < dst_div2.rows; r++)
	{
		for (int c = 0; c < dst_div2.cols; c++)
		{
			cout << dst_div2.at<float>(r, c) << ",";
		}
		cout << endl;
	}

	put_equal;

	//矩阵乘法
	//类似于线性代数中的矩阵乘法，这里要求矩阵的数据类型必须是float后者double 
	Mat srcMul1 = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat srcMul2 = (Mat_<float>(3, 2) << 6, 5, 4, 3, 2, 1);
	Mat dstMul = srcMul1*srcMul2; 
	for (int r = 0; r < dstMul.rows; r++)
	{
		for (int c = 0; c < dstMul.cols; c++)
		{
			cout << dstMul.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;
	
	//双通道Mat的乘法
	//双通道矩阵将每个元素向量作为一个复数，如Vec2f(1, 2)表示复数:1+2i
	//之后矩阵中每个数值都是一个复数，然后进行复数的乘法，例如：
	//dstMu2[0,0]=srcMul3[0,0]*srcMul4[0,0]=(1+2i)*(10+20i)=-30+40i,也就是输出结果中的[-30,40]
	Mat srcMul3 = (Mat_<Vec2f>(2, 1) << Vec2f(1, 2), Vec2f(3, 4));
	Mat srcMul4 = (Mat_<Vec2f>(1, 2) << Vec2f(10, 20), Vec2f(5, 15));
	Mat dstMu2 = srcMul3*srcMul4;    //直接使用"*"进行矩阵乘法
	for (int r = 0; r < dstMu2.rows; r++)
	{
		for (int c = 0; c < dstMu2.cols; c++)
		{
			cout << dstMu2.at<Vec2f>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat dstMu3;
	gemm(srcMul3,srcMul4, 0.5, dstMu2,1.2, dstMu3,0);
	//0.5是alpha,1.2是beta,上述表示:dstMu3=0.5*srcMul3*srcMul4+1.2*dstMu2
	//最后的0是flag,控制srcMul3，srcMul4，dstMu3是否转置来进行运算，
	//其表示不同的组合形式，具体看书P48或者图片"gemm函数flags参数意义.jpg"
	//与"*"进行矩阵乘法相同，gemm也只接收单通道/双通道，float/double

	for (int r = 0; r < dstMu3.rows; r++)
	{
		for (int c = 0; c < dstMu3.cols; c++)
		{
			cout << dstMu3.at<Vec2f>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	//指数和对数运算
	//进行指数和对数运算的Mat矩阵，其数据类型必须是float/double且是单通道的
	Mat src_log = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat dst_log;
	log(src_log, dst_log); //log改成exp可以进行对数运算

	for (int r = 0; r < dst_log.rows; r++)
	{
		for (int c = 0; c < dst_log.cols; c++)
		{
			cout << dst_log.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;
	
	//幂指数和开平方运算
	//sqrt函数的输入矩阵只接受float/double,而pow接受任何数据类型的Mat
	Mat src_pow = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat dst_pow;
	pow(src_pow,2, dst_pow); //幂指数运算
	sqrt(src_pow,dst_pow);   //开平方运算

	for (int r = 0; r < dst_pow.rows; r++)
	{
		for (int c = 0; c < dst_pow.cols; c++)
		{
			cout << dst_pow.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	system("pause");
	return 0;
}