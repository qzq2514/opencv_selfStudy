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
	
	//ȫ�����ĳߴ��Img_paddedһ��
	//����˳ߴ�Ϊż���ģ����Ҳ���²��һ����λ(���Ϊ��������Ȼ�кܶ��鷳)
	Mat Img_padded;
	copyMakeBorder(I, Img_padded, (kRow - 1) / 2, kRow / 2, (kCol - 1) / 2, kCol / 2, BORDER_CONSTANT, Scalar::all(0));
	flip(k, kCopy, -1);    //�������ת180��

	Mat convFull;
	//filter2D�ڲ�������same���������ͨ��֮ǰ��padding�����Դﵽȫ�����Ч��
	//Ҫָ��BORDER_CONSTANTʹ��0��Ϊ�߽����
	filter2D(Img_padded, convFull,-1, kCopy,Point(-1, -1),0.0,BORDER_CONSTANT);

	//cout << convFull << endl;
	return convFull;
}
int main()
{
	//��������ʽ������ʱ���ѯ
	//I*K   <<====>> FT_Ip.*FT_Kp  
	Mat Img = (Mat_<double>(8, 8) << 34, 56, 1, 0, 255, 230, 45, 12,
									0, 201, 101, 125, 52, 12, 124, 12,
									3, 41, 42, 40, 12, 90, 123, 45,
									5, 245, 98, 32, 34, 234, 90, 123,
									12, 12, 10, 41, 56, 89, 189, 5,
									112, 87, 12, 45, 78, 45, 10, 1,
									42, 123, 234, 12, 12, 21, 56, 43,
									1, 2, 45, 123, 10, 44, 123, 90);

	Mat kernel= (Mat_<double>(3,3) << 1, 0, -1, 1, 0, 1, 1, 0, -1);

	//ԭͼ��˵�ȫ���-���ź�����
	Mat convFull=ConvFull(Img, kernel);   //Mat���λ�Ӱ��ԭֵ

	//ԭͼ�ĸ���Ҷ�任
	Mat FT_Img;
	dft(Img, FT_Img, DFT_COMPLEX_OUTPUT);

	//����˵ĸ���Ҷ�任
	Mat FT_Ker;
	dft(kernel, FT_Ker, DFT_COMPLEX_OUTPUT);

	//��ԭͼ�����ȫ����ĳߴ�(�Ҳ���²����0)
	Mat Img_padding;
	copyMakeBorder(Img, Img_padding, 0,  kernel.rows - 1, 0,  kernel.cols - 1, BORDER_CONSTANT, Scalar::all(0));

	//������ԭͼ���и���Ҷ�任
	Mat FT_Img_padding;
	dft(Img_padding, FT_Img_padding, DFT_COMPLEX_OUTPUT);

	//�Ծ���������ȫ����ĳߴ�(�Ҳ���²����0)
	Mat Ker_padding;
	copyMakeBorder(kernel, Ker_padding, 0, Img.rows - 1, 0, Img.cols - 1, BORDER_CONSTANT, Scalar::all(0));

	//�����ľ���˽��и���Ҷ�任
	Mat FT_Ker_padding;
	dft(Ker_padding, FT_Ker_padding, DFT_COMPLEX_OUTPUT);

	Mat ImgPadFT_mul_KetPadFT;
	//mulSpectrums��ɾ����Ӧ�ĸ�����ˣ������Ϊ˫ͨ���ľ���
	mulSpectrums(FT_Img_padding, FT_Ker_padding, ImgPadFT_mul_KetPadFT, DFT_ROWS);


	Mat  IFFT2;
	dft(ImgPadFT_mul_KetPadFT, IFFT2, DFT_REAL_OUTPUT+DFT_INVERSE+DFT_SCALE);
	//cout << IFFT2 << endl;
	//cout<< convFull << endl;  //������ֱ��ӡIFFT2��convFull���Կ�����������ȵ�(��ȥ����Ҷ�任��IFFT2���־��ȶ�ʧ�����)
	//������֤��ԭͼ�;���˵�ȫ�������(ԭͼȫ���ĸ���Ҷ)���(�����ȫ���ĸ���Ҷ)��ĸ���Ҷ��任(ȡʵ��)
	//��I*K   <<====>> FT_Ip.*FT_Kp   
	//(I,KΪԭͼ�;����,Сp��ʾ�����ʽ��FT��ʾ����Ҷ��ʽ��*��ʾȫ���,.*Ϊ���,<<====>>��ʾ��Ϊ����Ҷ��任)


	Mat convFullFT;
	dft(convFull, convFullFT, DFT_COMPLEX_OUTPUT);
	cout << convFullFT << endl;
	cout << "----------------------" << endl;
	cout << ImgPadFT_mul_KetPadFT << endl;//������Ҳ���Կ�������ȫ����ĸ���Ҷ�任����(���ԭͼ�ĸ���Ҷ)��(������˵ĸ���Ҷ)�ĵ��
										  //������˵����һ����˼
	system("pause");
	return 0;
}
