#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>


using namespace std;
using namespace cv;
enum LPFFILTER
{
	ILP_FILTER = 0,
	BLP_FILTER = 1,
	GLP_FILTER = 2
};


Mat src;   //原图
Mat FFT;   //快速傅立叶变换
Point maxLoc;  //傅立叶谱最大值处坐标
int radius = 20;  //截断频率
const int MAX_RADIUS = 100;
Mat LPFilter;         //高通滤波器
Mat FFT_LP;          //原图的快速傅立叶矩阵FFT与高通滤波器LPFilter相乘结果
Mat FFT_LP_amlp;      //FFT_LP的傅里叶谱
int lpType;           //高通滤波器类型
const int MAX_LPTYPE = 2;
Mat flpSpectrum;    //傅立叶谱的灰度级
Mat result;			//高通滤波后的原图结果
string windowName = "高通滤波";

//得到高通滤波后的滤波器 -只与原图中心坐标、频率截断和n有关，与原图的具体像素无关
Mat createLPFilter(Size size, Point center, float radius, int type, int n = 2)
{
	Mat lpFilter = Mat::zeros(size, CV_32FC1);
	//cout << "size:" << size << endl;
	//cout << "lpFilter.size():" << lpFilter.size() << endl;

	int rows = size.height;
	int cols = size.width;
	if (radius < 0)
		return lpFilter;

	//理想高通滤波器
	if (type == ILP_FILTER)
	{
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				float dist = sqrt(pow(r - center.y, 2) + pow(c - center.x, 2));
				if (dist < radius)
					lpFilter.at<float>(r, c) = 0;
				else
					lpFilter.at<float>(r, c) = 1;
			}
		}
	}

	//巴特沃斯高通滤波器
	if (type == BLP_FILTER)
	{
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{

				float dist = sqrt(pow(r - center.y, 2) + pow(c - center.x, 2));
				lpFilter.at<float>(r, c) = 1.0-1.0 / (1.0 + pow(dist / radius, 2.0*n));
			}
		}
	}

	//高斯高通滤波
	if (type == GLP_FILTER)
	{
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				float dist = sqrt(pow(r - center.y, 2) + pow(c - center.x, 2));

				lpFilter.at<float>(r, c) = 1.0 - exp(-pow(dist, 2) / (2 * pow(radius, 2.0)));
			}
		}
	}

	return lpFilter;

}


Mat ff2Image(Mat I)
{
	int rows = I.rows;
	int cols = I.cols;

	int rPadding = getOptimalDFTSize(rows);
	int cPadding = getOptimalDFTSize(cols);

	//下侧和右侧补零，并快速傅立叶变换
	Mat Ip;
	copyMakeBorder(I, Ip, 0, rPadding - rows, 0, cPadding - cols, BORDER_CONSTANT, Scalar::all(0));

	Mat fft;
	dft(Ip, fft, DFT_COMPLEX_OUTPUT);
	return fft;
}

Mat amplitudeSpectrum(Mat fft)
{
	//复习:幅度谱/傅里叶谱就是傅里叶矩阵实部虚部的平方和的平方根
	Mat ampMat;
	vector<Mat> channals;
	split(fft, channals);
	magnitude(channals[0], channals[0], ampMat);
	return ampMat;
}
Mat grayAmplitude(Mat ampl)
{
	Mat dst;
	log(ampl + 1, dst);
	normalize(dst, dst, 0, 1, NORM_MINMAX);
	dst.convertTo(dst, CV_8UC1, 255, 0);
	return dst;
}
void callback_lpFilter(int, void*)
{
	//7.根据中心位置，频率截断和n得到高通滤波器
	LPFilter = createLPFilter(FFT.size(), maxLoc, radius, lpType, 2);

	//8.高通滤波器与原图的快速傅立叶变换矩阵相乘得到高通滤波下的傅立叶变换
	FFT_LP.create(FFT.size(), FFT.type());
	//cout << "FFT_LP.size():" << FFT_LP.size() << endl;
	for (int r = 0; r < FFT_LP.rows; r++)
	{
		for (int c = 0; c < FFT_LP.cols; c++)
		{
			Vec2f FFT_rc = FFT.at<Vec2f>(r, c);
			float LPFilter_rc = LPFilter.at<float>(r, c);
			FFT_LP.at<Vec2f>(r, c) = FFT_rc*LPFilter_rc;
		}
	}

	//傅立叶谱/ 频谱图并没有在高通滤波中有实际的变换操作，
	//只是用来展示一些效果和找到频谱图的中心位置以构建高通滤波器
	//真正操作的其实就是傅立叶变换矩阵

	//额外2.相乘后的高通傅里叶矩阵的频谱图/傅里叶谱的灰度级展示
	FFT_LP_amlp = amplitudeSpectrum(FFT_LP);
	FFT_LP_amlp = grayAmplitude(FFT_LP_amlp);
	imshow("高通傅里叶谱灰度级展示", FFT_LP_amlp);

	//9.高通滤波下的傅立叶矩阵进行傅里叶逆变换
	dft(FFT_LP, result, DFT_SCALE + DFT_REAL_OUTPUT + DFT_INVERSE);

	//下面其实就是原图进行快速傅立叶变换前的逆操作
	//10.乘以(-1)^(r+c)
	for (int r = 0; r < result.rows; r++)
	{
		for (int c = 0; c < result.cols; c++)
		{
			if ((r + c) % 2)
				result.at<float>(r, c) *= -1;
		}
	}

	//11.变回CV_8UC1
	result.convertTo(result, CV_8UC1, 1.0, 0);

	//12.截取左上部分，保证与元输入图像大小相同
	result = result(Rect(0, 0, src.cols, src.rows));
	imshow(windowName, result);
}
int main()
{
	//傅里叶谱中白色部分是图像的低频部分，即灰度值变换缓慢的区域，
	//黑色部分是高频部分，即灰度值变换快速的区域，如边缘
	//高通滤波即过滤掉图像中高频部分，保留低频部分，类似于平滑操作，最后图像趋于平滑

	//高通滤波的滤波器模版与对应的低通滤波模版和为1 ，所以本程序直接复制的“11.2.2低通滤波”的代码
	//只是在滤波器生成createLPFilter函数中用1减去对应的低通模版即得到高通滤波器

	//1.读入原图
	src = imread("lena_full.jpg", 0);
	imshow("src", src);

	//2.原图变为CV_32FC1类型
	Mat fI;
	src.convertTo(fI, CV_32FC1, 1.0, 0.0);
	//3.图像每个值乘(-1)^(r+c),以便进行后面的灰度级展示
	for (int r = 0; r < fI.rows; r++)
	{
		for (int c = 0; c < fI.cols; c++)
		{
			if ((r + c) % 2)
				fI.at<float>(r, c) *= -1;
		}
	}
	//4.图像补0并进行傅立叶变换
	FFT = ff2Image(fI);
	//cout << "FFT.size():" << FFT.size() << endl;

	//5.获得傅里叶谱/ 幅度谱
	Mat amplSpec = amplitudeSpectrum(FFT);
	cout << amplSpec.channels() << endl;

	//额外1.傅里叶谱的灰度级展示
	Mat grayAmpl = grayAmplitude(amplSpec);
	imshow("原图傅里叶谱的灰度级显示", grayAmpl);

	//6.获得傅立叶频谱图中最大值所在坐标-即最亮(频率最低)处
	minMaxLoc(grayAmpl, NULL, NULL, NULL, &maxLoc);
	cout << "maxLoc:" << maxLoc << endl;


	namedWindow(windowName, WINDOW_AUTOSIZE);
	createTrackbar("高通类型", windowName, &lpType, MAX_LPTYPE, callback_lpFilter);
	createTrackbar("半径", windowName, &radius, MAX_RADIUS, callback_lpFilter);
	callback_lpFilter(0, 0);

	waitKey(0);
	return 0;
}