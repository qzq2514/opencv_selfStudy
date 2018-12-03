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


	//���ٸ���Ҷ�任�����Ҫ
	int row = fImage.rows;
	int col = fImage.cols;
	int rPadding = getOptimalDFTSize(row);
	int cPadding = getOptimalDFTSize(col);

	Mat padded,dst;
	copyMakeBorder(fImage, padded, 0, rPadding - row, 0, cPadding - col, BORDER_CONSTANT, Scalar::all(0));
	//����Ҷ�任������õ�˫ͨ�����鲿ʵ����Mat
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
	imshow("ԭͼ",src);

	Mat fImage;
	src.convertTo(fImage, CV_64FC1, 1.0 / 255);

	cout << fImage.type() << "--" << CV_32FC1 << "--" << CV_64FC1 << endl;
	//1.����Ҷ�任�õ�˫ͨ����ʵ������fft
	Mat fft=fft2Image(fImage);

	//2.������
	Mat amplitude= getAmplitude(fft);
	
	//3.�����׵�log����
	Mat logAmplitude;
	log(amplitude + 1.0, logAmplitude);

	//4.������log����ľ�ֵƽ��
	Mat meanLogAmplitude;
	blur(amplitude, meanLogAmplitude, Size(3, 3), Point(-1, -1));

	//5.����в���=log(������)-meanBlur(log(������))
	Mat specRes = logAmplitude - meanLogAmplitude;

	//6.�в���ָ������
	Mat expSpecRes;
	exp(specRes, expSpecRes);

	//7.��λ��
	Mat phase = getPhase(fft);
	
	//8.����������λ��-���ں�����·�����(�в���)���㸵��Ҷ�����ʵ���鲿
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

	//9.�в�����Ϊ�����׼�����¸���Ҷ����
	Mat real = expSpecRes.mul(cosPhase);   
	Mat imaginary = expSpecRes.mul(sinPhase);

	//10.�ϲ�ʵ���鲿�õ��¸���Ҷ����
	vector<Mat> realAndImag;
	realAndImag.push_back(real);
	realAndImag.push_back(imaginary);
	Mat complex;
	merge(realAndImag, complex);


	//11.�¸���Ҷ�ؾ����µĸ���Ҷ��任
	Mat invFFT;
	//ע��֮ǰ����Ҷ��任�ǣ�DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALEֻȡʵ����Ȼ���ٽ�invFFT2��ΪCV_8UC1�ͱ��ԭͼ
	//�������ﲻ�����ǻ��ʵ�������Ǹ�����ʽ��ԭͼ������invFFT��˫ͨ����
	dft(complex, invFFT, DFT_COMPLEX_OUTPUT+DFT_INVERSE);

	//12.����Ҷ��仯������
	Mat invFFTAmp=getAmplitude(invFFT);
	
	//13.ƽ������
	pow(invFFTAmp, 2.0, invFFTAmp);

	//14.��˹ƽ��
	GaussianBlur(invFFTAmp, invFFTAmp, Size(11, 11), 2.5);
	//cout << invFFTAmp.type() << endl;     //CV_64FC1;

	//15.��������ʾ
	normalize(invFFTAmp, invFFTAmp, 0, 1.0, NORM_MINMAX);

	//16.�����Աȶ�
	pow(invFFTAmp, 0.5, invFFTAmp);

	//17.֮ǰsrc->fImage��CV_8UC1��ΪCV_64FC1ʱ����1.0/255,Ϊ�˱�֤��ֵͳһ������Ҫ����255
	Mat saliencyMap;
	invFFTAmp.convertTo(saliencyMap,CV_8UC1, 255);
	//�����Լ���ܳ�ȡͼ����Ϣ�С���߲���Ĳ��ֻ��������Ȥ�������ȹ�ע�Ĳ���
	//���ͼ���У���ɫ���־��ǲ����������ԵĲ���

	imshow("�����Լ��", saliencyMap);
	waitKey(0);
	return 0;
}