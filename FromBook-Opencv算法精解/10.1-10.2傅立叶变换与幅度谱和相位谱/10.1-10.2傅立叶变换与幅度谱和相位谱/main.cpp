#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

void FourierDemo(Mat src)
{

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
void DemoFastFourier(Mat src)
{
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
}

//幅度谱和相位谱的灰度级展示
Mat grayShow(Mat spectrum)
{
	//为了更好的显示幅度谱，先进行对数运算，然后归一化后乘以255并保存为CV_8U类型
	Mat dst;
	log(spectrum + 1, dst);

	//注意这里因为使用NORM_MINMAX参数，表明采用最小-最大归一化到指定范围[a,b]
	//而具体的范围又第三个参数-alpha和第四个参数-beta决定。
	//具体的a=min(a,b),b=max(a,b),所以这里写成1,0也是可以的
	normalize(dst, dst, 0, 1, NORM_MINMAX);
	dst.convertTo(dst, CV_8UC1, 255, 0);   //从0-1范围直接乘以255放缩到0-255范围
	return dst;
}
Mat getPhase(Mat src)
{
	Mat tempI;
	src.convertTo(tempI, CV_64FC1); //傅立叶变换的图像是folat或者double类型

	Mat F;
	//输出复数形式的傅立叶变换矩阵(双通道)，以便进行后面幅度谱和相位谱的计算
	dft(tempI, F, DFT_COMPLEX_OUTPUT);


	//得到傅立叶变换后的复数矩阵
	vector<Mat> dftChannals;
	split(F, dftChannals);      
	//imshow("傅立叶变换的实部", dftChannals[0]);
	//imshow("傅立叶变换的虚部", dftChannals[1]);
	//cout << "实虚部通道的类型:" << dftChannals[0].type() << "-" << dftChannals[1].type() << endl;  //CV_64F
																						   //求相位谱
	Mat phase; //幅度谱，相位谱，傅立叶变换矩阵的大小都是与原图大小相等的
	phase.create(tempI.size(), CV_64FC1);
	for (int r = 0; r < phase.rows; r++)
	{
		for (int c = 0; c < phase.cols; c++)
		{
			double real = dftChannals[0].at<double>(r, c);
			double imaginary = dftChannals[1].at<double>(r, c);
			phase.at<double>(r, c) = atan2(imaginary, real);
		}
	}
	return phase;
}
Mat getAmplitude(Mat src)
{
	Mat tempI;
	src.convertTo(tempI, CV_64FC1); //傅立叶变换的图像是folat或者double类型

	Mat F;
	//输出复数形式的傅立叶变换矩阵(双通道)，以便进行后面幅度谱和相位谱的计算
	dft(tempI, F, DFT_COMPLEX_OUTPUT);



	//得到傅立叶变换后的复数矩阵
	vector<Mat> dftChannals;
	split(F, dftChannals);      //release版本下
	//imshow("傅立叶变换的实部", dftChannals[0]);
	//imshow("傅立叶变换的虚部", dftChannals[1]);
	//cout << "实虚部通道的类型:" << dftChannals[0].type() << "-" << dftChannals[1].type() << endl;  //CV_64F
	
	//计算幅度谱，幅度谱每个位置等于傅立叶矩阵实部与虚部的平方和的平方根
	Mat amplitude;
	magnitude(dftChannals[0], dftChannals[1], amplitude);
	//cout << "幅度谱类型:" << amplitude.type() << endl;   //CV_64F
	//imshow("幅度谱(频率谱)", amplitude);     //纯白的
	
	return amplitude;
}
int main()
{
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("原图", src);

	//傅立叶变换
	//FourierDemo(src);
	//快速傅立叶变换
	//DemoFastFourier(src);


	//计算图像的幅度谱(即频率谱)和相位谱
	//由傅立叶变化可知F(0,0)是整个傅立叶变化矩阵中最大的，其等于输入矩阵f的所有像素值之和
	//同时因为幅度谱Amplitude是虚部和实部平方和的开根号，所以Amplitude(0,0)=F(0,0)
	//所以幅度谱中最大的值是Amplitude(0,0)在左上角，为了便于观察，将最大值移动到幅度谱的中心
	//这时候将原图中每个像素值乘以(r+c)^(-1)
	int rows = src.rows;
	int cols = src.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			//由于uchar范围是0-255,所以乘以-1后变成负数的像素会溢出，然后实际保存的值就是(256+该负数)
			if ((r + c) % 2 == 1)
				src.at<uchar>(r, c) *= -1;
		}
	}
	imshow("原图*(-1)", src);

	//得到幅度谱
	Mat amplitude=getAmplitude(src);   //这里是纯粹的幅度谱-CV_64F
	Mat amplitudeGray = grayShow(amplitude);   
	imshow("幅度谱的灰度级展示", amplitudeGray);
	//得到相位谱
	Mat phase = getPhase(src);
	Mat phaseGray = grayShow(phase);
	imshow("相位谱的灰度级展示", phaseGray);

	cout << "------------------------------------------------" << endl;
	//下面为个人测试:改变幅度谱中频率较高的部分，然后与相位谱合并后进行傅立叶逆变换
	//这里会使得图片中高频部分被去除，各部分灰度级相似
	//注意这里最好在一开始得到的原幅度谱上去除高频部分,而不要用灰度级展示的幅度谱去除高频部分
	rectangle(amplitude, Rect(265, 595, 50, 50), Scalar(0), -1);
	//如果不去除频率较高的部分，那么就相当于没做变化，直接按照傅立叶逆变化的步骤还原回原图
	
	//根据幅度谱和相位谱得到傅立叶变化的复数矩阵-实数部分和虚数部分
	Mat realMat = Mat(amplitude.size(), CV_64FC1);
	Mat imaginayrMat = Mat(amplitude.size(), CV_64FC1);
	for (int r = 0; r < realMat.rows; r++)
	{
		for (int c = 0; c < realMat.cols; c++)
		{
			//用纯粹的幅度谱还原实部虚部，而不要用灰度级显示的幅度谱
			//同样，也是用纯粹的相位谱还原时序不，不要用其灰度级展示
			//灰度级展示的目的仅仅是为了展示
			double a = amplitude.at<double>(r, c);
			double p= phase.at<double>(r, c);
			realMat.at<double>(r, c) = a*cos(p);
			imaginayrMat.at<double>(r, c) = a*sin(p);
		}
	}
	/*imshow("实数部分2", realMat);
	imshow("虚数部分2", imaginayrMat);
	imwrite("real.jpg", realMat);
	imwrite("imaginary.jpg", imaginayrMat);*/

	//根据得到的实部和虚部合并得到傅立叶变换的复数矩阵
	Mat invF;
	vector<Mat> invFChannals;
	invFChannals.push_back(realMat);
	invFChannals.push_back(imaginayrMat);
	merge(invFChannals, invF);

	//进行傅立叶逆变换
	Mat invRes;
	dft(invF, invRes, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	invRes.convertTo(invRes, CV_8UC1);
	
	//再乘-1还原原图
	for (int r = 0; r < invRes.rows; r++)
	{
		for (int c = 0; c < invRes.cols; c++)
		{
			if ((r + c) % 2 == 1)
				invRes.at<uchar>(r, c) *= -1;
		}
	}

	imshow("invRes", invRes);
	imwrite("invRes.jpg", invRes);


	waitKey(0);
	return 0;
}