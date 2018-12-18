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

string windowName = "����Ҷ�׵ĻҶȼ�";
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

	//�²���Ҳಹ�㣬�����ٸ���Ҷ�任
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
			//��¼���
			downPoint = Point(x, y);
			cout << "CV_EVENT_LBUTTONDOWN-----x:" << x << "-y:" << y << endl;
			break;
		case CV_EVENT_MOUSEMOVE:
			if (drawing_box)
			{
				//����ƶ�����ʼ������½�
				if (x >= downPoint.x && y >= downPoint.y)
				{
					rectFilter.x = downPoint.x;
					rectFilter.y = downPoint.y;
					rectFilter.width = x-downPoint.x;
					rectFilter.height = y - downPoint.y;
				}
				//����ƶ������Ͻ�
				if (x >= downPoint.x && y <= downPoint.y)
				{
					rectFilter.x = downPoint.x;
					rectFilter.y = y;
					rectFilter.width = x - downPoint.x;
					rectFilter.height = downPoint.y - y;
				}
				//����ƶ������Ͻ�
				if (x <= downPoint.x && y <= downPoint.y)
				{
					rectFilter.x = x;
					rectFilter.y = y;
					rectFilter.width = downPoint.x - x;
					rectFilter.height = downPoint.y - y;
				}
				//����ƶ������½�
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

	//���ٸ���Ҷ�任
	FFT = ff2Image(fImage);

	//����Ҷ��
	Mat amplSpec = amplitudeSpectrum(FFT);

	grayAmpl = grayAmplitude(amplSpec);
	//imshow("ԭͼ����Ҷ�׵ĻҶȼ���ʾ", grayAmpl);

	//����Ҷ�����ĵ�λ��
	minMaxLoc(grayAmpl, NULL, NULL, NULL, &maxLoc);

	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	setMouseCallback(windowName, mouseRectHandle, NULL);

	cout << "---------------------" << endl;
	grayAmplCopy = grayAmpl.clone();
	//�Զ���Ƶ���˲���-���ǻ����ľ��ο����˲���Ԫ��ֵΪ0,����Ϊ1����
	//����ֻ�涨һ����Χ�ڵ��˲���Ԫ��ֵΪ0���������while(true)�����в�ע�ͣ���ô������ƶ�ʱ,����Ҷ�ײ��ϱ仯
	//�������Եõ��˲��������Χ��Ϊ0
	while (true)
	{
		/*grayAmpl(rectFilter).setTo(0);
		FFT(rectFilter).setTo(Scalar::all(0));*/

		imshow(windowName, grayAmplCopy);
		if (waitKey(10) == 27)        //����ESC�����˲��������������ѭ����ʼ�����Զ����˲������ͼ��
			break;	    
	}

	//�Զ����˲���������ɣ��൱��ģ���Զ�����˲����븵��Ҷ�ĵ�ˣ�
	//���еľ��ο����˲���Ԫ��ֵΪ0����ôFFT�����Ӧ��ֵ��Ϊ0(����ͨ���ڶ�Ϊ0)
	grayAmpl(rectFilter).setTo(0);
	FFT(rectFilter).setTo(Scalar::all(0));

	Mat result;
	//����Ҷ��仯,ȡʵ��
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
	imshow("�Զ����˲������ͼ��", result);


	//��Ȥ���ǣ�����ˮƽ�����Ƶ�ʣ����ȥ��ͼ���д�ֱ�������
	//Ч����ֱ�����Ƶ�ʣ���ȥ��ͼ����ˮƽ�������

	waitKey(0);
	return 0;
}