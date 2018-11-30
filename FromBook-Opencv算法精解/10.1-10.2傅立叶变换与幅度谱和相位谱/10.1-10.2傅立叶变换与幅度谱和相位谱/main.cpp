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
void DemoFastFourier()
{
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("src", src);

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

	waitKey(0);
}
int main()
{
	//FourierDemo();
	DemoFastFourier();
	return 0;
}