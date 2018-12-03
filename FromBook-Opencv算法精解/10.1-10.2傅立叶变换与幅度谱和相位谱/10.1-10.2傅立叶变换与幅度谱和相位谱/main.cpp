#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

void FourierDemo(Mat src)
{

	Mat fI;
	src.convertTo(fI, CV_64F);    //����Ҷ�任ͼ��������double�ͣ�����ֻ�Ǳ任�����ͣ�û�л���ֵ
								  //imshow("fI", fI);             //����

	Mat F;
	/*��ʼ���и���Ҷ�任
	DFT_COMPLEX_INPUT:���������ʽ
	DFT_REAL_OUTPUT:ֻ���ʵ����ʽ
	*/
	dft(fI, F, DFT_COMPLEX_OUTPUT);
	cout << "type:" << F.type() << "----" << "channels:" << F.channels() << endl;
	//imshow("F", F);
	//����Ҷ��任��ֻȡʵ��

	Mat iF;
	dft(F, iF, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);

	Mat res;
	iF.convertTo(res, CV_8U);   //֮ǰCV_8UC1��CV_64FC1�������ͣ���ֵ��Χû�б䣬��任ʱҲҪ�����ͣ����䷶Χ
	imshow("����Ҷ��任���", res);    //��ʱ��ԭͼ��������Ҷ�任�ٽ��и���Ҷ��任�õ���ͼƬ��ԭͼһ��
}

//������Ҫ�任��ͼ��(CV_64F����),�������Ҷ�任�ĸ�����ʽ����
//���ٸ���Ҷ�任
void fft2(Mat &I, Mat &F)
{
	Mat src = I.clone();
	int rows = src.rows;
	int cols = src.cols;

	//opencv��ʵ�ֵĿ��ٸ���Ҷ�㷨�����������������������Էֽ�Ϊ(2^p)*(3^q)*(5^r)
	//getOptimalDFTSize�򷵻ز�С�������ҿ��Էֽ�Ϊ(2^p)*(3^q)*(5^r)������
	int rPadding = getOptimalDFTSize(rows);
	int cPadding = getOptimalDFTSize(cols);

	Mat f;
	//��ԭͼsrc����rPadding*cPadding��С
	copyMakeBorder(src, f, 0, rPadding - rows, 0, cPadding, BORDER_CONSTANT, Scalar::all(0));

	//���п��ٸ���Ҷ�任(��ʵ���ǵ��õ�dft������ֻ��ͼ���С�Ǳ�������)
	dft(f, F, DFT_COMPLEX_OUTPUT);
}
void DemoFastFourier(Mat src)
{
	Mat fI;
	src.convertTo(fI, CV_64F);

	Mat F;
	fft2(fI, F);

	//ͬ�������ٸ���Ҷ�任��Ҳ�ܹ�������任Ȼ��õ���ԭͼһ����ͼ��
	Mat iF;
	//����Ҷ��仯��ȡʵ��
	dft(F, iF, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE);
	//����Ҷ�仯����任���ǵõ���任Ǯͬ��С��ͼ��
	//����������м��ú�õ���һ��ʼsrcͼ���С��ͼ��
	Mat res = iF(Rect(0, 0, src.cols, src.rows));
	res.convertTo(res, CV_8U);//ͬ��Ҫ��������ת��
	imshow("res", res);
}

//�����׺���λ�׵ĻҶȼ�չʾ
Mat grayShow(Mat spectrum)
{
	//Ϊ�˸��õ���ʾ�����ף��Ƚ��ж������㣬Ȼ���һ�������255������ΪCV_8U����
	Mat dst;
	log(spectrum + 1, dst);

	//ע��������Ϊʹ��NORM_MINMAX����������������С-����һ����ָ����Χ[a,b]
	//������ķ�Χ�ֵ���������-alpha�͵��ĸ�����-beta������
	//�����a=min(a,b),b=max(a,b),��������д��1,0Ҳ�ǿ��Ե�
	normalize(dst, dst, 0, 1, NORM_MINMAX);
	dst.convertTo(dst, CV_8UC1, 255, 0);   //��0-1��Χֱ�ӳ���255������0-255��Χ
	return dst;
}
Mat getPhase(Mat src)
{
	Mat tempI;
	src.convertTo(tempI, CV_64FC1); //����Ҷ�任��ͼ����folat����double����

	Mat F;
	//���������ʽ�ĸ���Ҷ�任����(˫ͨ��)���Ա���к�������׺���λ�׵ļ���
	dft(tempI, F, DFT_COMPLEX_OUTPUT);


	//�õ�����Ҷ�任��ĸ�������
	vector<Mat> dftChannals;
	split(F, dftChannals);      
	//imshow("����Ҷ�任��ʵ��", dftChannals[0]);
	//imshow("����Ҷ�任���鲿", dftChannals[1]);
	//cout << "ʵ�鲿ͨ��������:" << dftChannals[0].type() << "-" << dftChannals[1].type() << endl;  //CV_64F
																						   //����λ��
	Mat phase; //�����ף���λ�ף�����Ҷ�任����Ĵ�С������ԭͼ��С��ȵ�
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
	src.convertTo(tempI, CV_64FC1); //����Ҷ�任��ͼ����folat����double����

	Mat F;
	//���������ʽ�ĸ���Ҷ�任����(˫ͨ��)���Ա���к�������׺���λ�׵ļ���
	dft(tempI, F, DFT_COMPLEX_OUTPUT);



	//�õ�����Ҷ�任��ĸ�������
	vector<Mat> dftChannals;
	split(F, dftChannals);      //release�汾��
	//imshow("����Ҷ�任��ʵ��", dftChannals[0]);
	//imshow("����Ҷ�任���鲿", dftChannals[1]);
	//cout << "ʵ�鲿ͨ��������:" << dftChannals[0].type() << "-" << dftChannals[1].type() << endl;  //CV_64F
	
	//��������ף�������ÿ��λ�õ��ڸ���Ҷ����ʵ�����鲿��ƽ���͵�ƽ����
	Mat amplitude;
	magnitude(dftChannals[0], dftChannals[1], amplitude);
	//cout << "����������:" << amplitude.type() << endl;   //CV_64F
	//imshow("������(Ƶ����)", amplitude);     //���׵�
	
	return amplitude;
}
int main()
{
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("ԭͼ", src);

	//����Ҷ�任
	//FourierDemo(src);
	//���ٸ���Ҷ�任
	//DemoFastFourier(src);


	//����ͼ��ķ�����(��Ƶ����)����λ��
	//�ɸ���Ҷ�仯��֪F(0,0)����������Ҷ�仯���������ģ�������������f����������ֵ֮��
	//ͬʱ��Ϊ������Amplitude���鲿��ʵ��ƽ���͵Ŀ����ţ�����Amplitude(0,0)=F(0,0)
	//���Է�����������ֵ��Amplitude(0,0)�����Ͻǣ�Ϊ�˱��ڹ۲죬�����ֵ�ƶ��������׵�����
	//��ʱ��ԭͼ��ÿ������ֵ����(r+c)^(-1)
	int rows = src.rows;
	int cols = src.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			//����uchar��Χ��0-255,���Գ���-1���ɸ��������ػ������Ȼ��ʵ�ʱ����ֵ����(256+�ø���)
			if ((r + c) % 2 == 1)
				src.at<uchar>(r, c) *= -1;
		}
	}
	imshow("ԭͼ*(-1)", src);

	//�õ�������
	Mat amplitude=getAmplitude(src);   //�����Ǵ���ķ�����-CV_64F
	Mat amplitudeGray = grayShow(amplitude);   
	imshow("�����׵ĻҶȼ�չʾ", amplitudeGray);
	//�õ���λ��
	Mat phase = getPhase(src);
	Mat phaseGray = grayShow(phase);
	imshow("��λ�׵ĻҶȼ�չʾ", phaseGray);

	cout << "------------------------------------------------" << endl;
	//����Ϊ���˲���:�ı��������Ƶ�ʽϸߵĲ��֣�Ȼ������λ�׺ϲ�����и���Ҷ��任
	//�����ʹ��ͼƬ�и�Ƶ���ֱ�ȥ���������ֻҶȼ�����
	//ע�����������һ��ʼ�õ���ԭ��������ȥ����Ƶ����,����Ҫ�ûҶȼ�չʾ�ķ�����ȥ����Ƶ����
	rectangle(amplitude, Rect(265, 595, 50, 50), Scalar(0), -1);
	//�����ȥ��Ƶ�ʽϸߵĲ��֣���ô���൱��û���仯��ֱ�Ӱ��ո���Ҷ��仯�Ĳ��軹ԭ��ԭͼ
	
	//���ݷ����׺���λ�׵õ�����Ҷ�仯�ĸ�������-ʵ�����ֺ���������
	Mat realMat = Mat(amplitude.size(), CV_64FC1);
	Mat imaginayrMat = Mat(amplitude.size(), CV_64FC1);
	for (int r = 0; r < realMat.rows; r++)
	{
		for (int c = 0; c < realMat.cols; c++)
		{
			//�ô���ķ����׻�ԭʵ���鲿������Ҫ�ûҶȼ���ʾ�ķ�����
			//ͬ����Ҳ���ô������λ�׻�ԭʱ�򲻣���Ҫ����Ҷȼ�չʾ
			//�Ҷȼ�չʾ��Ŀ�Ľ�����Ϊ��չʾ
			double a = amplitude.at<double>(r, c);
			double p= phase.at<double>(r, c);
			realMat.at<double>(r, c) = a*cos(p);
			imaginayrMat.at<double>(r, c) = a*sin(p);
		}
	}
	/*imshow("ʵ������2", realMat);
	imshow("��������2", imaginayrMat);
	imwrite("real.jpg", realMat);
	imwrite("imaginary.jpg", imaginayrMat);*/

	//���ݵõ���ʵ�����鲿�ϲ��õ�����Ҷ�任�ĸ�������
	Mat invF;
	vector<Mat> invFChannals;
	invFChannals.push_back(realMat);
	invFChannals.push_back(imaginayrMat);
	merge(invFChannals, invF);

	//���и���Ҷ��任
	Mat invRes;
	dft(invF, invRes, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	invRes.convertTo(invRes, CV_8UC1);
	
	//�ٳ�-1��ԭԭͼ
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