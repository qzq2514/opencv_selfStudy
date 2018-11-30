#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

void FourierDemo()
{
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("src", src);

	Mat fI;
	src.convertTo(fI, CV_64F);    //傅立叶变换图像类型是double型，这里只是变换了类型，没有换数值
								  //imshow("fI", fI);             //纯白

	Mat F;
	/*开始进行傅立叶变换
	DFT_COMPLEX_INPUT:输出复数形式
	DFT_REAL_OUTPUT:只输出实数形式
	*/
	dft(fI, F, DFT_COMPLEX_OUTPUT);
	cout << "type:" << F.type() << "----" << "channels:" << F.channels() << endl;
	//imshow("F", F);
	//傅立叶逆变换，只取实部

	Mat iF;
	dft(F, iF, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);

	Mat res;
	iF.convertTo(res, CV_8U);   //之前CV_8UC1变CV_64FC1，变类型，数值范围没有变，逆变换时也要变类型，不变范围
	imshow("傅立叶逆变换结果", res);    //这时候原图经过傅立叶变换再进行傅立叶逆变换得到的图片与原图一致
}

//输入需要变换的图像(CV_64F类型),输出傅立叶变换的复数形式矩阵
//快速傅立叶变换
void fft2(Mat &I, Mat &F)
{
	Mat src = I.clone();
	int rows = src.rows;
	int cols = src.cols;
	
    //opencv中实现的快速傅立叶算法是针对行数和列数均满足可以分解为(2^p)*(3^q)*(5^r)
	//getOptimalDFTSize则返回不小参数，且可以分解为(2^p)*(3^q)*(5^r)的整数
	int rPadding = getOptimalDFTSize(rows);
	int cPadding = getOptimalDFTSize(cols);

	Mat f;
	//将原图src填充成rPadding*cPadding大小
	copyMakeBorder(src, f, 0, rPadding - rows, 0, cPadding, BORDER_CONSTANT, Scalar::all(0));
	
	//进行快速傅立叶变换(其实还是调用的dft函数，只是图像大小是被填充过的)
	dft(f, F, DFT_COMPLEX_OUTPUT);
}
void DemoFastFourier()
{
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("src", src);

	Mat fI;
	src.convertTo(fI, CV_64F);

	Mat F;
	fft2(fI, F);

	//同样，快速傅立叶变换后也能够进行逆变换然后得到与原图一样的图像
	Mat iF;
	//傅立叶逆变化，取实部
	dft(F, iF, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE);
	//傅立叶变化和逆变换都是得到与变换钱同大小的图像，
	//所以这里进行剪裁后得到与一开始src图像大小的图像
	Mat res = iF(Rect(0, 0, src.cols, src.rows));
	res.convertTo(res, CV_8U);//同样要进行类型转换
	imshow("res", res);

	waitKey(0);
}
int main()
{
	//FourierDemo();
	DemoFastFourier();
	return 0;
}