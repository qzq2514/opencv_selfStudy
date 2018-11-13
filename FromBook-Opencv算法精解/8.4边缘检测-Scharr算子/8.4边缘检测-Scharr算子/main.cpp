#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

void conv2D(Mat src, Mat &dst, int ddepth, Mat kernel, int borderType)
{
	flip(kernel, kernel, -1);
	filter2D(src, dst, ddepth, kernel, Point(-1, -1));
}
Mat scharr(Mat src, int ddepth, int x, int y, int borderType = BORDER_DEFAULT)
{
	Mat dst;
	CV_Assert(!(x == 0 && y == 0));
	Mat ker_scharr_x = (Mat_<float>(3, 3) << 3, 0, -3, 10, 0, -10, 3, 0, -3);
	Mat ker_scharr_y = (Mat_<float>(3, 3) << 3, 10, 3, 0, 0, 0, -3, -10, -3);
	if (x == 1 && y == 0) //��ֱ��Ե���
	{
		//��������д��һ����scharr����Ҳ�ǿɷ����
		//��ֱ�����Ե�����Է���Ϊ:
		//Mat scharr_x_x=(Mat_<float>(3,1)<<3,10,3);        ��ֱ�����ƽ������
		//Mat scharr_x_y = (Mat_<float>(1, 3) << 1, 0, -1); ˮƽ����Ĳ������

		//��ȻҲ����ֱ��ʹ�ò������scharr����ֱ�Ӿ�������������һ��
		conv2D(src, dst, ddepth, ker_scharr_x, BORDER_DEFAULT);
	}
	else //ˮƽ��Ե���
	{
		//conv2D(src, dst, ddepth, ker_scharr_y, BORDER_DEFAULT);


		Mat scharr_y_y = (Mat_<float>(1, 3) << 3, 10, 3);    //ˮƽ�����ƽ������
		Mat scharr_y_x = (Mat_<float>(3, 1) << 1, 0, -1);    //��ֱ����Ĳ������
		conv2D(src, dst, ddepth, scharr_y_y, BORDER_DEFAULT);
		conv2D(dst, dst, ddepth, scharr_y_x, BORDER_DEFAULT);

	}


	//����scharr���ӻ�������չ����������
	//Mat scharr_135 = (Mat_<float>(3, 3) << 10,  3,  0,
	//										  3,  0, -3,
	//										  0, -3, -10); //���45�ȷ���ı�Ե

	//Mat scharr_45 = (Mat_<float>(3, 3) <<   0,  3, 10,
	//										  -3,  0,  3,
	//										 -10, -3,  0); //���135�ȷ���ı�Ե
	//������������������Ĳ��ܷ���

	return dst; 
}
int main()
{
	Mat src = imread("timg.jpg",0);
	imshow("src", src);

	Mat img_scharr_x, img_scharr_x_abs;
	img_scharr_x = scharr(src, CV_32FC1, 1, 0);
	convertScaleAbs(img_scharr_x, img_scharr_x_abs, 1.0, 0.0);
	imshow("scharr��ֱ���",img_scharr_x_abs);

	Mat img_scharr_y, img_scharr_y_abs;
	img_scharr_y = scharr(src, CV_32FC1,0,1);
	convertScaleAbs(img_scharr_y, img_scharr_y_abs, 1.0, 0.0);
	imshow("scharrˮƽ���", img_scharr_y_abs);


	//����ʹ����һ�ֱ�Եǿ�ȵļ���-��λȡ���ֵ
	Mat edge= img_scharr_x_abs.clone();
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (img_scharr_y_abs.at<uchar>(r, c) > edge.at<uchar>(r, c))
				edge.at<uchar>(r, c) = img_scharr_y_abs.at<uchar>(r, c);
		}
	}
	imshow("edge", edge);


	//opencv�Դ���Scharr��Ե��⣬��Sobelһ����ͬ�����ھ��ο���ϱ�Ե�����Ե���Ч������
	Mat dstOpencv;
	Scharr(src, dstOpencv, CV_8UC1,1,0, 1, 0 );
	imshow("dstOpencv", dstOpencv);
	waitKey(0);
	return 0;
}