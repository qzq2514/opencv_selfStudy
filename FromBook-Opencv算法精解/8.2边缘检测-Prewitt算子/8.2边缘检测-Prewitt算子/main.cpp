#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;

void conv2D(Mat src, Mat &dst, Mat kernel, int ddepth, Point anchor = Point(-1, -1), 
	int borderType = BORDER_DEFAULT)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1);
	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType);
}
void sepConv2D_Y_X(Mat src, Mat &dst, int ddepth, Mat kernelY, Mat kernelX, 
	Point anchor = Point(-1, -1), int bordertype = BORDER_DEFAULT)
{
	Mat src_kerY;
	//ʹ����ɢ��������ʣ��ֱ���y�����x������о��
	conv2D(src, src_kerY, kernelY, ddepth, anchor, bordertype);
	conv2D(src_kerY, dst, kernelX, ddepth, anchor, bordertype);
}
Mat prewitt(Mat src, int ddepth, int x=1, int y = 0, int bordertype = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	if (x != 0 && y == 0)
	{
		//Prewitt_x��ֱ�����Ե��⣬�������[[1 0 -1],
		//                                    [1 0 -1],
		//                                    [1 0 -1]]
		//����һ���ɷ������ɷ���Ϊ[[1],[1],[1]]��[1,0-1]
		Mat prewitt_x_y = (Mat_<float>(3, 1) << 1, 1, 1);
		Mat prewitt_x_x = (Mat_<float>(1, 3) << 1, 0, -1);
		//�����ȶ��Ǹ������ľ���˽��о������
		//prewitt_x_y��ʾ�Դ�ֱ������зǹ�һ��(���ǹ�һ���ľ�ֵƽ��)������ƽ����Ч��
		//prewitt_x_x���ǽ���ˮƽ����Ĳ�ּ����Ե
		sepConv2D_Y_X(src, dst, ddepth, prewitt_x_y, prewitt_x_x, Point(-1, -1), bordertype);
	}
	else
	{
		//Prewitt_yˮƽ�����Ե��⣬�������[[ 1  1  1],
		//                                    [ 0  0  0],
		//                                    [-1 -1 -1]]
		//����һ���ɷ������ɷ���Ϊ[[1,1,1]��[[1],[0],[-1]]
		Mat prewitt_y_y = (Mat_<float>(1, 3) << 1, 1, 1);
		Mat prewitt_y_x = (Mat_<float>(3, 1) << 1, 0, -1);
		//ͬ��Prewittˮƽ����������Ҳ�ǿɷ���ģ�������ĸ�������Ӿ�����Ƚ��о������
		//��prewitt_y_y��ʾ��ˮƽ������зǹ�һ��������ƽ����Ч��
		//prewitt_y_x���ǽ��д�ֱ����Ĳ�ּ����Ե
		sepConv2D_Y_X(src, dst, ddepth, prewitt_y_x,prewitt_y_y, Point(-1, -1), bordertype);
	}
	return dst;
}
int main()
{
	Mat src = imread("timg.jpg",0);
	imshow("src", src);

	/*Mat src = (Mat_<uchar>(5, 5) << 2, 4, 6, 2, 4,
		5, 2, 7, 2, 5,
		43, 75, 96, 4, 9,
		4, 3, 23, 6, 8,
		94, 2, 6, 12, 8);
	cout << "-------------" << endl << "src:" << endl << src << endl;*/

	Mat img_prewitt_x,abs_img_prewitt_x;  //����б�Ե-��ֱ����ı�Ե
	//���"8.1��Ե���-Roberts����"��һ��ûע�������
	//��Ϊ��Ե����д��ڼ��������ܵ�����������и������������������CV_32FC1��ʱ���渺��
	//���ǲ�����CV_8UC1���棬��Ϊ�䷶Χ��0-255������������Ȼ�����ᱻ�ض�Ϊ0
	img_prewitt_x=prewitt(src, CV_32FC1, 1, 0);   
	//img_prewitt_x��CV_32FC1,abs_img_prewitt_x��CV_8UC1
	convertScaleAbs(img_prewitt_x, abs_img_prewitt_x, 1.0, 0);
	imshow("��ֱ�����Ե���", abs_img_prewitt_x);

	Mat img_prewitt_y, abs_img_prewitt_y;  //����б�Ե-ˮƽ����ı�Ե
	img_prewitt_y = prewitt(src, CV_32FC1, 0,1);
	convertScaleAbs(img_prewitt_y, abs_img_prewitt_y, 1.0, 0);
	imshow("ˮƽ�����Ե���", abs_img_prewitt_y);

	Mat img_prewitt_x2, img_prewitt_y2;
	pow(img_prewitt_x, 2.0, img_prewitt_x2);
	pow(img_prewitt_y, 2.0, img_prewitt_y2);

	Mat edge;
	sqrt(img_prewitt_x2 + img_prewitt_y2, edge);
	//ͬ������Ϊimg_prewitt_x��img_prewitt_y����CV_32FC1�����Ǳ����ֵ����0-255,
	//����������pow��õ��Ľ��Ҳ��CV_32FC1�����Ա�ΪCV_8UC1������ȷ��ʾ���
	edge.convertTo(edge, CV_8UC1); 
	imshow("edge", edge);
	waitKey(0);


	return 0;
}