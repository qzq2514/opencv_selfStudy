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

	Mat Img_padded;
	copyMakeBorder(I, Img_padded, (kRow - 1) / 2, kRow / 2, (kCol - 1) / 2, kCol / 2, BORDER_CONSTANT, Scalar::all(0));
	cout <<"ȫ����:"<<endl<< Img_padded << endl;
	cout << "------------------" << endl;
	
	flip(k, kCopy, -1);    //�������ת180��

	Mat convFull;
	filter2D(Img_padded, convFull, -1, kCopy, Point(-1, -1), 0.0, BORDER_CONSTANT);

	//cout << convFull << endl;
	return convFull;
}
int main()
{
	//��������ʽ������ʱ���ѯ
	//I*K   <<====>> FT_Ip.*FT_Kp  
	Mat Img = (Mat_<double>(8, 8) << 
		34, 56, 1, 0, 255, 230, 45, 12,
		0, 201, 101, 125, 52, 12, 124, 12,
		3, 41, 42, 40, 12, 90, 123, 45,
		5, 245, 98, 32, 34, 234, 90, 123,
		12, 12, 10, 41, 56, 89, 189, 5,
		112, 87, 12, 45, 78, 45, 10, 1,
		42, 123, 234, 12, 12, 21, 56, 43,
		1, 2, 45, 123, 10, 44, 123, 90);

	Mat kernel = (Mat_<double>(3, 3) << 1, 0, -1, 1, 0, 1, 1, 0, -1);

	//������û�õ������ǿ��Բ���ȫ�����same����Ĺ�ϵ(��ͬ��䷽ʽ�£�same�����ȫ����Ĳ���)
	/*Mat convFull = ConvFull(Img, kernel);
	cout <<"ȫ���:"<<endl<< convFull << endl;   */

	//ԭͼ���
	int imgR = Img.rows;
	int imgC = Img.cols;

	//����˿��
	int kerR = kernel.rows;
	int kerC = kernel.cols;

	//����˰뾶
	int radiusR = (kerR - 1) / 2;
	int radiusC = (kerC - 1) / 2;


	//ԭͼ�߽����-����ʹ���Ա߽�Ϊ��ľ������
	//��ʱ���ͼ��С��(imgR+kerR-1)*(imgC+kerC-1)
	Mat Img_padded;
	copyMakeBorder(Img, Img_padded, radiusR, radiusR, radiusC, radiusC, BORDER_DEFAULT);

	//���������ٸ���Ҷ�任����������
	//ע�������Ǹ���Img_padded.rows + kerR - 1�������Ǹ���Img_padded.rows�õ����ٸ���Ҷ�任�ĳ���
	int rowPadd = getOptimalDFTSize(Img_padded.rows + kerR - 1);
	int colPadd = getOptimalDFTSize(Img_padded.cols + kerC - 1);

	//���ٸ���Ҷ�任������Ҳ���²����0����
	Mat Img_padded_zeros, ker_zeros;
	copyMakeBorder(Img_padded, Img_padded_zeros, 0, rowPadd - Img_padded.rows, 0, colPadd - Img_padded.cols, BORDER_CONSTANT, Scalar::all(0));
	copyMakeBorder(kernel, ker_zeros, 0, rowPadd - kernel.rows, 0, colPadd - kernel.cols, BORDER_CONSTANT, Scalar::all(0));
	//Img_padded_zeros��ker_zeros�ߴ���һ���ģ�����rowPadd*colPadd

	//���ٸ���Ҷ�任
	Mat FFT_ImgPaddZero, FFT_KerZero;
	dft(Img_padded_zeros, FFT_ImgPaddZero, DFT_COMPLEX_OUTPUT);
	dft(ker_zeros, FFT_KerZero, DFT_COMPLEX_OUTPUT);

	//�������ٸ���Ҷ�任�ĵ��
	Mat Img_ker_FFT;
	mulSpectrums(FFT_ImgPaddZero, FFT_KerZero, Img_ker_FFT, DFT_ROWS);

	//����Ҷ�任��ֻȡʵ��
	Mat invFFT;
	dft(Img_ker_FFT, invFFT, DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);

	Mat sameConv_FFT = invFFT(Rect(kerC - 1, kerR - 1, imgC + kerC - 1, imgR + kerR - 1));
	cout <<"����Ҷ�任�õ�������:"<<endl<< sameConv_FFT << endl;


	Mat kerFlip;
	flip(kernel, kerFlip, -1);
	Mat sameConv;
	//ע������ʹ��filter2D����same�������䷽ʽҪ��Img->Img_padded����䷽ʽ��ͬ
	filter2D(Img, sameConv, -1, kerFlip, Point(-1, -1),0.0, BORDER_DEFAULT);
	cout << "same���:" << endl << sameConv <<endl;

    //���ˣ����Կ�������Ҷ�任������ֱ��same��������Ľ����һ�������Կ���ʹ�ÿ��ٸ���Ҷ�任�õ�same���
	//��ϰ:��֮ǰ"10.4����븵��Ҷ�任�Ĺ�ϵ"�����У����Ի��ڸ���Ҷ�任�õ�ͼ����˵�full������


	//�ܽ�:���ˣ�����Ҷ�任���½ڽ���
	//֮ǰ��ͼ��ƽ���ͱ�Ե��ⶼ��ͳ��Ϊ�ռ����˲�(���ؿռ��ڵĲ���)����ͨ������������Խ��ռ���ľ������
	//ת����Ƶ������ɡ�
	system("pause");
	return 0;
}