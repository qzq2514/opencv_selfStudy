#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

Mat src;
Mat FFT;
Mat grayAmpl;
Mat grayAmplCopy;
Point maxLoc;

string windowName = "傅里叶谱的灰度级";
bool drawing_box = false;
Point downPoint;
Rect rectFilter;
bool gotRectFilter=false;
Mat grayAmplitude(Mat ampl)
{
	Mat dst;
	log(ampl + 1, dst);
	normalize(dst, dst, 0, 1, NORM_MINMAX);
	dst.convertTo(dst, CV_8UC1, 255, 0);
	return dst;
}

Mat amplitudeSpectrum(Mat fft)
{
	Mat ampMat;
	vector<Mat> channals;
	split(fft, channals);
	magnitude(channals[0], channals[0], ampMat);
	return ampMat;
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

void mouseRectHandle(int event, int x, int y,int ,void *)
{
	switch (event)
	{
		case CV_EVENT_LBUTTONDOWN:
			drawing_box = true;
			//记录起点
			downPoint = Point(x, y);
			cout << "CV_EVENT_LBUTTONDOWN-----x:" << x << "-y:" << y << endl;
			break;
		case CV_EVENT_MOUSEMOVE:
			if (drawing_box)
			{
				//鼠标移动到起始点的右下角
				if (x >= downPoint.x && y >= downPoint.y)
				{
					rectFilter.x = downPoint.x;
					rectFilter.y = downPoint.y;
					rectFilter.width = x-downPoint.x;
					rectFilter.height = y - downPoint.y;
				}
				//鼠标移动到右上角
				if (x >= downPoint.x && y <= downPoint.y)
				{
					rectFilter.x = downPoint.x;
					rectFilter.y = y;
					rectFilter.width = x - downPoint.x;
					rectFilter.height = downPoint.y - y;
				}
				//鼠标移动到左上角
				if (x <= downPoint.x && y <= downPoint.y)
				{
					rectFilter.x = x;
					rectFilter.y = y;
					rectFilter.width = downPoint.x - x;
					rectFilter.height = downPoint.y - y;
				}
				//鼠标移动到左下角
				if (x <= downPoint.x && y  >= downPoint.y)
				{
					rectFilter.x = x;
					rectFilter.y = downPoint.y;
					rectFilter.width = downPoint.x - x;
					rectFilter.height = y - downPoint.y;
				}
			}
			grayAmplCopy = grayAmpl.clone();
			rectangle(grayAmplCopy, rectFilter, Scalar(0), 1);
			imshow(windowName, grayAmplCopy);
			break;
		case CV_EVENT_LBUTTONUP:
			cout << "CV_EVENT_MBUTTONUP-----x:" << x << "-y:" << y << endl;
			drawing_box = false;
			gotRectFilter = true;
			break;
		default:
			break;
	}
}
int main()
{
	 src= imread("timg.jpg",0);
	imshow("src", src);

	Mat fImage;
	src.convertTo(fImage, CV_32FC1, 1.0, 0.0);

	for (int r = 0; r < fImage.rows; r++)
	{
		for (int c = 0; c < fImage.cols; c++)
		{
			if ((r + c) % 2)
			{
				fImage.at<float>(r, c) *= -1;
			}
		}
	}

	//快速傅立叶变换
	FFT = ff2Image(fImage);

	//傅立叶谱
	Mat amplSpec = amplitudeSpectrum(FFT);

	grayAmpl = grayAmplitude(amplSpec);
	//imshow("原图傅里叶谱的灰度级显示", grayAmpl);

	//傅里叶谱最大的点位置
	minMaxLoc(grayAmpl, NULL, NULL, NULL, &maxLoc);

	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	setMouseCallback(windowName, mouseRectHandle, NULL);

	cout << "---------------------" << endl;
	grayAmplCopy = grayAmpl.clone();
	//自定义频率滤波器-我们画出的矩形宽内滤波器元素值为0,此外为1不动
	//这里只规定一个范围内的滤波器元素值为0，如果下面while(true)内两行不注释，那么在鼠标移动时,傅里叶谱不断变化
	//那样可以得到滤波器多个范围都为0
	while (true)
	{
		/*grayAmpl(rectFilter).setTo(0);
		FFT(rectFilter).setTo(Scalar::all(0));*/

		imshow(windowName, grayAmplCopy);
		if (waitKey(10) == 27)        //按下ESC键后，滤波器绘制完成跳出循环开始绘制自定义滤波器后的图像
			break;	    
	}

	//自定义滤波器绘制完成，相当于模拟自定义的滤波器与傅立叶的点乘，
	//框中的矩形框内滤波器元素值为0，那么FFT矩阵对应的值就为0(两个通道内都为0)
	grayAmpl(rectFilter).setTo(0);
	FFT(rectFilter).setTo(Scalar::all(0));

	Mat result;
	//傅立叶逆变化,取实部
	dft(FFT, result, DFT_SCALE + DFT_INVERSE + DFT_REAL_OUTPUT);

	for (int r = 0; r < result.rows; r++)
	{
		for (int c = 0; c < result.cols; c++)
		{
			if ((r + c) % 2)
			{
				result.at<float>(r, c) *= -1;
			}
		}
	}

	result.convertTo(result, CV_8UC1, 1.0, 0.0);
	result = result(Rect(0, 0, src.cols, src.rows));
	imshow("自定义滤波器后的图像", result);


	//有趣的是，削弱水平方向的频率，则会去除图像中垂直方向的线
	//效果垂直方向的频率，则去除图像中水平方向的线

	waitKey(0);
	return 0;
}