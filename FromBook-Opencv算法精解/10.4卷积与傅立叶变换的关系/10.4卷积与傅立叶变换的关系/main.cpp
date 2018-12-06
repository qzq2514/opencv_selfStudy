#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
Mat ConvFull(Mat I, Mat k)
{
	Mat kCopy = k.clone();
	int kRow = k.rows;
	int kCol = k.cols;
	
	//全卷积后的尺寸和Img_padded一样
	//卷积核尺寸为偶数的，则右侧和下侧多一个单位(最好为奇数，不然有很多麻烦)
	Mat Img_padded;
	copyMakeBorder(I, Img_padded, (kRow - 1) / 2, kRow / 2, (kCol - 1) / 2, kCol / 2, BORDER_CONSTANT, Scalar::all(0));
	flip(k, kCopy, -1);    //卷积核旋转180度

	Mat convFull;
	//filter2D内部做得是same卷积，但是通过之前的padding后便可以达到全卷积的效果
	//要指定BORDER_CONSTANT使用0作为边界填充
	filter2D(Img_padded, convFull,-1, kCopy,Point(-1, -1),0.0,BORDER_CONSTANT);

	//cout << convFull << endl;
	return convFull;
}
int main()
{
	//先亮出公式，便于时候查询
	//I*K   <<====>> FT_Ip.*FT_Kp  
	Mat Img = (Mat_<double>(8, 8) << 34, 56, 1, 0, 255, 230, 45, 12,
									0, 201, 101, 125, 52, 12, 124, 12,
									3, 41, 42, 40, 12, 90, 123, 45,
									5, 245, 98, 32, 34, 234, 90, 123,
									12, 12, 10, 41, 56, 89, 189, 5,
									112, 87, 12, 45, 78, 45, 10, 1,
									42, 123, 234, 12, 12, 21, 56, 43,
									1, 2, 45, 123, 10, 44, 123, 90);

	Mat kernel= (Mat_<double>(3,3) << 1, 0, -1, 1, 0, 1, 1, 0, -1);

	//原图与核的全卷积-留着后面用
	Mat convFull=ConvFull(Img, kernel);   //Mat传参会影响原值

	//原图的傅立叶变换
	Mat FT_Img;
	dft(Img, FT_Img, DFT_COMPLEX_OUTPUT);

	//卷积核的傅立叶变换
	Mat FT_Ker;
	dft(kernel, FT_Ker, DFT_COMPLEX_OUTPUT);

	//对原图填充至全卷积的尺寸(右侧和下侧填充0)
	Mat Img_padding;
	copyMakeBorder(Img, Img_padding, 0,  kernel.rows - 1, 0,  kernel.cols - 1, BORDER_CONSTANT, Scalar::all(0));

	//对填充的原图进行傅立叶变换
	Mat FT_Img_padding;
	dft(Img_padding, FT_Img_padding, DFT_COMPLEX_OUTPUT);

	//对卷积核填充至全卷积的尺寸(右侧和下侧填充0)
	Mat Ker_padding;
	copyMakeBorder(kernel, Ker_padding, 0, Img.rows - 1, 0, Img.cols - 1, BORDER_CONSTANT, Scalar::all(0));

	//对填充的卷积核进行傅立叶变换
	Mat FT_Ker_padding;
	dft(Ker_padding, FT_Ker_padding, DFT_COMPLEX_OUTPUT);

	Mat ImgPadFT_mul_KetPadFT;
	//mulSpectrums完成矩阵对应的复数相乘，结果仍为双通道的矩阵
	mulSpectrums(FT_Img_padding, FT_Ker_padding, ImgPadFT_mul_KetPadFT, DFT_ROWS);


	Mat  IFFT2;
	dft(ImgPadFT_mul_KetPadFT, IFFT2, DFT_REAL_OUTPUT+DFT_INVERSE+DFT_SCALE);
	//cout << IFFT2 << endl;
	//cout<< convFull << endl;  //从这里分别打印IFFT2和convFull可以看出两者是相等的(除去傅立叶变换后IFFT2出现精度丢失的情况)
	//以上则证明原图和卷积核的全卷积等于(原图全填充的傅立叶)点乘(卷积核全填充的傅立叶)后的傅立叶逆变换(取实部)
	//即I*K   <<====>> FT_Ip.*FT_Kp   
	//(I,K为原图和卷积核,小p表示填充形式，FT表示傅立叶形式，*表示全卷积,.*为点乘,<<====>>表示互为傅立叶逆变换)


	Mat convFullFT;
	dft(convFull, convFullFT, DFT_COMPLEX_OUTPUT);
	cout << convFullFT << endl;
	cout << "----------------------" << endl;
	cout << ImgPadFT_mul_KetPadFT << endl;//从这里也可以看出来，全卷积的傅立叶变换等于(填充原图的傅立叶)和(填充卷积核的傅立叶)的点乘
										  //和上面说的是一个意思
	system("pause");
	return 0;
}
