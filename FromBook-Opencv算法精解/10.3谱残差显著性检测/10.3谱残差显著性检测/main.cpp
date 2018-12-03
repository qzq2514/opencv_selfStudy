#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;
Mat fft2Image(Mat fImage)
{
	CV_Assert(fImage.type() == CV_32FC1 || fImage.type() == CV_64FC1);
	CV_Assert(fImage.channels() == 1);


	//快速傅立叶变换填充需要
	int row = fImage.rows;
	int col = fImage.cols;
	int rPadding = getOptimalDFTSize(row);
	int cPadding = getOptimalDFTSize(col);

	Mat padded,dst;
	copyMakeBorder(fImage, padded, 0, rPadding - row, 0, cPadding - col, BORDER_CONSTANT, Scalar::all(0));
	//傅立叶变换，这里得到双通道含虚部实部的Mat
	dft(padded, dst, DFT_COMPLEX_OUTPUT);
	return dst;
}
Mat getAmplitude(Mat fft)
{
	CV_Assert(fft.channels() == 2);
	vector<Mat> channals;
	split(fft, channals);

	Mat amplitude;
	magnitude(channals[0], channals[1], amplitude);
	return amplitude;
}
Mat getPhase(Mat fft)
{
	CV_Assert(fft.channels() == 2);
	vector<Mat> channals;
	split(fft, channals);

	Mat phaseImg;
	phaseImg.create(fft.size(), CV_64FC1);

	for (int r = 0; r < phaseImg.rows; r++)
	{
		for (int c = 0; c < phaseImg.cols; c++)
		{
			double real = channals[0].at<double>(r, c);
			double imaginary = channals[1].at<double>(r, c);
			phaseImg.at<double>(r, c)= atan2(imaginary ,real);
		}
	}
	return phaseImg;
}
int main()
{
	Mat src = imread("lena_full.jpg",0);
	imshow("原图",src);

	Mat fImage;
	src.convertTo(fImage, CV_64FC1, 1.0 / 255);

	cout << fImage.type() << "--" << CV_32FC1 << "--" << CV_64FC1 << endl;
	//1.傅立叶变换得到双通道虚实补矩阵fft
	Mat fft=fft2Image(fImage);

	//2.幅度谱
	Mat amplitude= getAmplitude(fft);
	
	//3.幅度谱的log运算
	Mat logAmplitude;
	log(amplitude + 1.0, logAmplitude);

	//4.幅度谱log运算的均值平滑
	Mat meanLogAmplitude;
	blur(amplitude, meanLogAmplitude, Size(3, 3), Point(-1, -1));

	//5.计算残差谱=log(幅度谱)-meanBlur(log(幅度谱))
	Mat specRes = logAmplitude - meanLogAmplitude;

	//6.残差谱指数运算
	Mat expSpecRes;
	exp(specRes, expSpecRes);

	//7.相位谱
	Mat phase = getPhase(fft);
	
	//8.正弦余弦相位谱-用于后面和新幅度谱(残差谱)计算傅立叶矩阵的实部虚部
	Mat cosPhase(phase.size(), CV_64FC1);
	Mat sinPhase(phase.size(), CV_64FC1);
	for (int r = 0; r < phase.rows; r++)
	{
		for (int c = 0; c < phase.cols; c++)
		{
			cosPhase.at<double>(r, c) = cos(phase.at<double>(r, c));
			sinPhase.at<double>(r, c) = sin(phase.at<double>(r, c));
		}
	}

	//9.残差谱作为幅度谱计算的新傅立叶矩阵
	Mat real = expSpecRes.mul(cosPhase);   
	Mat imaginary = expSpecRes.mul(sinPhase);

	//10.合并实部虚部得到新傅立叶矩阵
	vector<Mat> realAndImag;
	realAndImag.push_back(real);
	realAndImag.push_back(imaginary);
	Mat complex;
	merge(realAndImag, complex);


	//11.新傅立叶矩矩阵下的傅立叶逆变换
	Mat invFFT;
	//注意之前傅立叶逆变换是：DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE只取实部，然后再将invFFT2变为CV_8UC1就变回原图
	//但是这里不仅仅是获得实部，而是复数形式的原图，所以invFFT是双通道的
	dft(complex, invFFT, DFT_COMPLEX_OUTPUT+DFT_INVERSE);

	//12.傅立叶逆变化幅度谱
	Mat invFFTAmp=getAmplitude(invFFT);
	
	//13.平方运算
	pow(invFFTAmp, 2.0, invFFTAmp);

	//14.高斯平滑
	GaussianBlur(invFFTAmp, invFFTAmp, Size(11, 11), 2.5);
	//cout << invFFTAmp.type() << endl;     //CV_64FC1;

	//15.显著性显示
	normalize(invFFTAmp, invFFTAmp, 0, 1.0, NORM_MINMAX);

	//16.提升对比度
	pow(invFFTAmp, 0.5, invFFTAmp);

	//17.之前src->fImage从CV_8UC1变为CV_64FC1时乘以1.0/255,为了保证数值统一，这里要乘以255
	Mat saliencyMap;
	invFFTAmp.convertTo(saliencyMap,CV_8UC1, 255);
	//显著性检测能抽取图像信息中、最具差异的部分或者最感兴趣或者首先关注的部分
	//结果图像中，白色部分就是差异性最明显的部分

	imshow("显著性检测", saliencyMap);
	waitKey(0);
	return 0;
}