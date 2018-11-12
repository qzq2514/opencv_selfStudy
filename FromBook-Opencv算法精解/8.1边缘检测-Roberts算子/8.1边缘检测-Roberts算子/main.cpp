#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//ԭMat��ֵ���ݣ�Ҫ��Mat &dst���ܴﵽʵ�Ρ��β�ͬʱ�ı�
//����ʹ��OutputArray &dst��
void conv2D(Mat src,Mat &dst,Mat kernel,int ddepth,Point anchor=Point(-1,-1),int borderType=BORDER_DEFAULT)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1); //����˷�ת180��
	
	//same�������,���ͼ��ԭͼͬ��С��ͬ����ͨ��
	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType); 
}
//Ĭ��BORDER_DEFAULT�������BORDER_REFLECT_101�����ֵ��Ϊ4
Mat roberts(Mat src, int ddepth, int x = 1, int y = 0, int borderType = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	Mat robert_135 = (Mat_<float>(2, 2) << 1, 0, 0,-1); // 45�ȷ���ı�Ե
	Mat robert_45 = (Mat_<float>(2, 2) <<  0, 1, -1, 0); // 135�ȷ���ı�Ե
	//cout << "-------------" << endl<<"robert_135:" << endl << robert_135 << endl;
	//����x,yֵѡ����ͬ�ľ����ʽ
	if (x != 0 && y == 0)
		conv2D(src, dst, robert_135, ddepth, Point(0, 0), borderType); //robert_135����˵�ê����(0,0)
	else
		conv2D(src, dst, robert_45,  ddepth, Point(1, 0), borderType); //robert_45����˵�ê����(1,0)
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	//������
	/*Mat src = (Mat_<uchar>(5, 5) << 2, 4, 6, 2, 4,
		5, 2, 7, 2, 5,
		43, 75, 96, 4, 9,
		4, 3, 23, 6, 8,
		94, 2, 6, 12, 8);*/
	//cout << "-------------" << endl << "src:"<<endl<<src << endl;
	
	Mat img_robert1=roberts(src, CV_32FC1, 1, 0); //robert_135���(45�ȷ����Ե���)
	Mat img_robert1_abs;
	//��Ե�����ܳ��ָ�ֵ(img_robert1���ܺ��и�ֵԪ��)��ʹ��convertScaleAbs����ȡͼ��ľ���ֵ
	//dst[i,j]=abs(src[i,j]*alpha+beta),��convertScaleAbs����8λ�޷�������(CV_8U)
	convertScaleAbs(img_robert1, img_robert1_abs, 1, 0);
	imshow("robert_135-45�ȱ�Ե���", img_robert1_abs);

	/*cout << "-------------"<<endl<<"img_robert1:" << endl << img_robert1 << endl;
	cout << "-------------" << endl << "img_robert1_abs:" << endl << img_robert1_abs << endl;
	cout << img_robert1.type() << endl;         //CV_32FC1
	cout << img_robert1_abs.type() << endl;     //CV_8UC1
	cout << img_robert1_abs << endl;*/

	Mat img_robert2 = roberts(src, CV_32FC1, 0,1); //robert_135���(45�ȷ����Ե���)
	Mat img_robert2_abs;
	convertScaleAbs(img_robert2, img_robert2_abs, 1, 0);
	imshow("robert_45-135�ȱ�Ե���", img_robert2_abs);


	//�õ����������Ե������µı�Եǿ��
	//������õ���ƽ��������ʽ����Եǿ�ȵ�ͼƬÿ�������ǲ�ͬ��������
	//Ӧλ������ֵ��ƽ���͵�ƽ����
	Mat img_robert1_pow, img_robert2_pow;
	//��˵������Ϊimg_robert1_abs��img_robert1�ľ���ֵ�����ƽ����һ����
	//����img_robert1��CV_32F,img_robert1_abs��CV_8U,��pow���ı�Mat�����ͣ�sqrtҪ��float����double����
	//��������д��img_robert1,����img_robert1_abs
	pow(img_robert1,2.0,img_robert1_pow);     
	pow(img_robert2, 2.0, img_robert2_pow);

	//cout << "-------------" << endl << "img_robert1_pow:" << endl << img_robert1_pow << endl;
	//cout << "-------------" << endl << "img_robert2_pow:" << endl << img_robert2_pow << endl;
	Mat fullEdge;
	//����ƽ���͵Ŀ���������ȡ����ֵ�����ֵ������ֵ�ĺ͵�����ʹ�ö�������Ե�����б�Ե��ǿ�ķ���
	//��Ϊ��Ե��⣬�õ��ı�Եλ���ǰ�ɫ����CV_8UC1���Ǵ�ֵ-ƫ��255�����Ա�Ե��ǿ���ܹ�����ͬ����
	//��Ե����ı�Ե(��ɫ����)�ϲ����������õ���������ı�Ե
	sqrt(img_robert1_pow + img_robert2_pow, fullEdge); 

	//����fullEdge��ԭͼ-img_robert1_pow + img_robert2_pow����һ��������CV_32FC1,
	//��������ֵ��0-255֮�䣬����Ҫ�任����
	fullEdge.convertTo(fullEdge, CV_8UC1);
	/*cout << fullEdge.type();
	cout << CV_32F << endl;;
	cout << "-------------" << endl << "fullEdge:" << endl << fullEdge << endl;*/
	imshow("fullEdge", fullEdge);

	waitKey(0);
	return 0;
}