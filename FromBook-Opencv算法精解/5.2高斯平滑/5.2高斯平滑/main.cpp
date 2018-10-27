#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>
#define PI 3.14159265


using namespace std;
using namespace cv;

Mat getGaussianKernel_1D(float sigma, Size size, int dtype)
{
	Mat GaussianKernel_1D = Mat::zeros(size, dtype);
	
	//cout << size.width << "-" << size.height << endl;
	//Ҫ������Ǵ�ֱ�������ˮƽ�����һά����ˣ��ҳ��ȱ���Ϊ����
	CV_Assert((size.height % 2 == 1 && size.width % 2 == 1) && (size.height == 1 || size.width == 1));

	int len = size.height != 1 ? size.height : size.width;    //ȡ��Ϊһ����Ϊ����
	float mean= (len - 1)/2.0;
	float sum=0;
	for (int i = 0; i < len; i++)
	{
		float val = 1 / (sqrt(2 * PI)*sigma)*exp(-pow(i- mean, 2) / (2 * sigma*sigma));
		GaussianKernel_1D.at<double>(i) = val;
		sum += val;
	}
	//���ע����й�һ������
	GaussianKernel_1D = GaussianKernel_1D / sum;
	return GaussianKernel_1D;
}
Mat myGaussianBlur(Mat src,Size size,float sigmaX, float sigmaY)
{
	Mat dst;

	//���ݸ�˹�������ɢ���ʣ������Ϊˮƽ����ʹ�ֱ�����һά�����
	//����ʹ���Լ�д��getGaussianKernel_1D��opencv�Դ���getGaussianKernel�����õ�һά�ľ����
	//ע�⣬�������ֵ���ͱ�����CV_32F����CV_64F
	Mat GaussianX = getGaussianKernel_1D(sigmaX,Size(size.width,1), CV_32F);
	Mat GaussianY = getGaussianKernel(size.height, sigmaY, CV_32F);   //Ĭ���Ǵ�ֱ�����һά�����
	/*cout << GaussianX << endl;
	cout << GaussianY << endl;*/   //�Աȿ��Կ��������ֵľ�������һ����
	
	//Mat Gaussian2D;
	//����һά����˽��о������õ���ά�ľ����-�����ã���Ϊopencv�еľ������filter2DΪsame���
	//flip(GaussianY, GaussianY, -1);   //�Ƚ���ת�ã�����ʱ����ת180��
	//filter2D(GaussianX, Gaussian2D, CV_32F, GaussianY, Point(0, size.height - 1),0.0, BORDER_CONSTANT);
	//cout << Gaussian2D << endl;

	//���ݸ�˹�˵���ɢ������ʣ������Ƚ���ˮƽ����ľ����Ȼ����д�ֱ����ľ��
	//ע����֮ǰҪ���о���˵�����ʱ����ת180��
	flip(GaussianX, GaussianX, -1);
	cout << src.depth() << endl;   //0(CV_8U)
	filter2D(src, dst, -1, GaussianX, Point(-1, -1), 0.0, BORDER_DEFAULT);

	flip(GaussianY, GaussianY, -1);
	filter2D(dst, dst, -1, GaussianY, Point(-1, -1), 0.0, BORDER_DEFAULT);
	return dst;
}
int main()
{
	Mat src1 = imread("lena_full.jpg", 1);
	//Mat dst1=myGaussianBlur(src1,Size(5,5),1.1,1.1);   //�����Ϊ5x5,��ˮƽ�ʹ�ֱ����ı�׼�����1.1
	imshow("Origin", src1);
	//imshow("myGaussianBlur", dst1);

	Mat dst2;
	//opencv�Դ��ĸ�˹ģ����������������Ϊ(ԭͼ�����ͼ���˴�С��ˮƽ����ֵ����ı�׼��)
	//���У��˽�Сʱ���Ա�׼����У�ƽ��Ч����𲻴�
	//�˽ϴ�ʱ���Ա�׼�����У�ƽ��Ч������
	GaussianBlur(src1, dst2, Size(5, 5), 1.1, 1.1);   
	imshow("GaussianBlur", dst2);


	//��ô�ģ����ﲻ��ͬʱ����myGaussianBlur��GaussianBlur������Ϊ��ͬʱ��ԭͼͼƬ���ò�����ɣ�
	//Ȼ��cloneԭͼ�ֱ�����Ҳ���У���ô������


	//��˹ƽ����˼��:
	//��ˮƽ�ʹ�ֱ����ı�׼��һ������ô�õ������������һά��˹����һ���ģ��������ڹ�ʽ����
	//i- mean�Ĵ��ڣ��������ĸ�˹���е�Ԫ��ֵԽ�����м�Խ��ͬ���õ��Ķ�ά�����Ҳ���м����(���⣬
	//���ˮƽ�ʹ�ֱ����ı�׼��һ�£���ô�õ��Ķ�ά��˹���ǶԳƵ�)���������о��ʱ�����������Ϊê��(Point(-1,-1))
	//��ô�м�����ֵ��Ȩ����󣬶���Χ�������Ը�˹�ֲ�����ʽӰ������������ֵ(�����м�ԽԶӰ��ԽС)
	waitKey(0);
	return 0;
}