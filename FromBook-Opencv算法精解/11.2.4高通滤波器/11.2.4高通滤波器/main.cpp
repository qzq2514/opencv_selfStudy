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


Mat src;   //ԭͼ
Mat FFT;   //���ٸ���Ҷ�任
Point maxLoc;  //����Ҷ�����ֵ������
int radius = 20;  //�ض�Ƶ��
const int MAX_RADIUS = 100;
Mat LPFilter;         //��ͨ�˲���
Mat FFT_LP;          //ԭͼ�Ŀ��ٸ���Ҷ����FFT���ͨ�˲���LPFilter��˽��
Mat FFT_LP_amlp;      //FFT_LP�ĸ���Ҷ��
int lpType;           //��ͨ�˲�������
const int MAX_LPTYPE = 2;
Mat flpSpectrum;    //����Ҷ�׵ĻҶȼ�
Mat result;			//��ͨ�˲����ԭͼ���
string windowName = "��ͨ�˲�";

//�õ���ͨ�˲�����˲��� -ֻ��ԭͼ�������ꡢƵ�ʽضϺ�n�йأ���ԭͼ�ľ��������޹�
Mat createLPFilter(Size size, Point center, float radius, int type, int n = 2)
{
	Mat lpFilter = Mat::zeros(size, CV_32FC1);
	//cout << "size:" << size << endl;
	//cout << "lpFilter.size():" << lpFilter.size() << endl;

	int rows = size.height;
	int cols = size.width;
	if (radius < 0)
		return lpFilter;

	//�����ͨ�˲���
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

	//������˹��ͨ�˲���
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

	//��˹��ͨ�˲�
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

	//�²���Ҳಹ�㣬�����ٸ���Ҷ�任
	Mat Ip;
	copyMakeBorder(I, Ip, 0, rPadding - rows, 0, cPadding - cols, BORDER_CONSTANT, Scalar::all(0));

	Mat fft;
	dft(Ip, fft, DFT_COMPLEX_OUTPUT);
	return fft;
}

Mat amplitudeSpectrum(Mat fft)
{
	//��ϰ:������/����Ҷ�׾��Ǹ���Ҷ����ʵ���鲿��ƽ���͵�ƽ����
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
	//7.��������λ�ã�Ƶ�ʽضϺ�n�õ���ͨ�˲���
	LPFilter = createLPFilter(FFT.size(), maxLoc, radius, lpType, 2);

	//8.��ͨ�˲�����ԭͼ�Ŀ��ٸ���Ҷ�任������˵õ���ͨ�˲��µĸ���Ҷ�任
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

	//����Ҷ��/ Ƶ��ͼ��û���ڸ�ͨ�˲�����ʵ�ʵı任������
	//ֻ������չʾһЩЧ�����ҵ�Ƶ��ͼ������λ���Թ�����ͨ�˲���
	//������������ʵ���Ǹ���Ҷ�任����

	//����2.��˺�ĸ�ͨ����Ҷ�����Ƶ��ͼ/����Ҷ�׵ĻҶȼ�չʾ
	FFT_LP_amlp = amplitudeSpectrum(FFT_LP);
	FFT_LP_amlp = grayAmplitude(FFT_LP_amlp);
	imshow("��ͨ����Ҷ�׻Ҷȼ�չʾ", FFT_LP_amlp);

	//9.��ͨ�˲��µĸ���Ҷ������и���Ҷ��任
	dft(FFT_LP, result, DFT_SCALE + DFT_REAL_OUTPUT + DFT_INVERSE);

	//������ʵ����ԭͼ���п��ٸ���Ҷ�任ǰ�������
	//10.����(-1)^(r+c)
	for (int r = 0; r < result.rows; r++)
	{
		for (int c = 0; c < result.cols; c++)
		{
			if ((r + c) % 2)
				result.at<float>(r, c) *= -1;
		}
	}

	//11.���CV_8UC1
	result.convertTo(result, CV_8UC1, 1.0, 0);

	//12.��ȡ���ϲ��֣���֤��Ԫ����ͼ���С��ͬ
	result = result(Rect(0, 0, src.cols, src.rows));
	imshow(windowName, result);
}
int main()
{
	//����Ҷ���а�ɫ������ͼ��ĵ�Ƶ���֣����Ҷ�ֵ�任����������
	//��ɫ�����Ǹ�Ƶ���֣����Ҷ�ֵ�任���ٵ��������Ե
	//��ͨ�˲������˵�ͼ���и�Ƶ���֣�������Ƶ���֣�������ƽ�����������ͼ������ƽ��

	//��ͨ�˲����˲���ģ�����Ӧ�ĵ�ͨ�˲�ģ���Ϊ1 �����Ա�����ֱ�Ӹ��Ƶġ�11.2.2��ͨ�˲����Ĵ���
	//ֻ�����˲�������createLPFilter��������1��ȥ��Ӧ�ĵ�ͨģ�漴�õ���ͨ�˲���

	//1.����ԭͼ
	src = imread("lena_full.jpg", 0);
	imshow("src", src);

	//2.ԭͼ��ΪCV_32FC1����
	Mat fI;
	src.convertTo(fI, CV_32FC1, 1.0, 0.0);
	//3.ͼ��ÿ��ֵ��(-1)^(r+c),�Ա���к���ĻҶȼ�չʾ
	for (int r = 0; r < fI.rows; r++)
	{
		for (int c = 0; c < fI.cols; c++)
		{
			if ((r + c) % 2)
				fI.at<float>(r, c) *= -1;
		}
	}
	//4.ͼ��0�����и���Ҷ�任
	FFT = ff2Image(fI);
	//cout << "FFT.size():" << FFT.size() << endl;

	//5.��ø���Ҷ��/ ������
	Mat amplSpec = amplitudeSpectrum(FFT);
	cout << amplSpec.channels() << endl;

	//����1.����Ҷ�׵ĻҶȼ�չʾ
	Mat grayAmpl = grayAmplitude(amplSpec);
	imshow("ԭͼ����Ҷ�׵ĻҶȼ���ʾ", grayAmpl);

	//6.��ø���ҶƵ��ͼ�����ֵ��������-������(Ƶ�����)��
	minMaxLoc(grayAmpl, NULL, NULL, NULL, &maxLoc);
	cout << "maxLoc:" << maxLoc << endl;


	namedWindow(windowName, WINDOW_AUTOSIZE);
	createTrackbar("��ͨ����", windowName, &lpType, MAX_LPTYPE, callback_lpFilter);
	createTrackbar("�뾶", windowName, &radius, MAX_RADIUS, callback_lpFilter);
	callback_lpFilter(0, 0);

	waitKey(0);
	return 0;
}