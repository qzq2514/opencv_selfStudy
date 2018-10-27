#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat fastMeanBlur(Mat src, Size size, int BorderTpye, Scalar value = Scalar())
{
	int khei = size.height;
	int kwid = size.width;

	//��֤�˵Ŀ�߶�������
	CV_Assert(khei % 2 == 1 && kwid % 2 == 1);

	//����˴��ڰ뾶
	int hh = (khei - 1) / 2;
	int ww = (kwid - 1) / 2;

	//����˴������
	float area = float(khei*kwid);

	//Ϊ��ʡȥ�ڼ���ʱ���б߽��жϣ�������б߽�����
	Mat padding;
	copyMakeBorder(src, padding, hh, hh, ww, ww, BorderTpye, value);
	imshow("padding", padding);
	cout << "padding_size" << padding.size() << endl;

	cout << padding.type() << endl;
	//����ͼ�����,�õ���inteͼ��ĵ�(r,c)��λ�õ�Ԫ��ֵ�Ǹ�λ�����Ͻǵľ��������Ԫ�غ�
	//�õ���inte���������к���������ಹ���ͼ���������õ���ͼ���С��(padding.height+1)*(padding.width+1)
	Mat inte;
	integral(padding, inte, CV_64FC1);
	//imshow("inte", inte);
	cout << "inte_size" << inte.size() << endl;

	/*cout << inte.at<double>(50,100) << endl;
	cout << inte.at<double>(230, 240) << endl;*/
	//cout << inte.type() << endl;    //5(CV_32FC1)

	int rows = src.rows;
	int cols = src.cols;
	int r = 0, c = 0;
	Mat fastMeanImg = Mat::zeros(src.size(), CV_64FC1);

	for (int h = hh; h < hh + rows; h++)
	{
		for (int w = ww; w < ww + cols; w++)
		{
			double bottomRight = inte.at<double>(h + hh + 1, w + ww + 1);
			double topLeft = inte.at<double>(h - hh , w - ww );
			double bottomLeft = inte.at<double>(h + hh + 1, w - ww);
			double topRight = inte.at<double>(h - hh , w + ww + 1);
			double value= (bottomRight + topLeft - bottomLeft - topRight) / area;
			//cout << bottomRight<<"-"<< topLeft<<"-"<< bottomLeft<<"-"<< topRight<<"-"<<value<<endl;
			fastMeanImg.at<double>(r, c) = value;
			c++;
		}
		r++;
		c = 0;
	}
	//cout << fastMeanImg << endl;              //����ͼ������Ҳ��������ֵ����255,���Ǿ�����ʾ���ף��㲻������ѧ
	return fastMeanImg;
}
int main()
{
	/*Mat src = imread("lena_smaller.jpg",0);

	Mat dst = fastMeanBlur(src,Size(5,5),BORDER_DEFAULT,Scalar());
	imshow("Origin", src);
	imshow("fastMeanBlur", dst);*/

	//opencv�Դ���������ֵƽ���ĺ���boxFilter��blur
	//�Ҷ��ڶ�ͨ����ͼ�񣬷ֱ���ÿ��ͨ���Ͻ���ƽ��
	Mat src1= imread("lena_full.jpg", 1);
	Mat dst1;
	boxFilter(src1, dst1, -1, Size(5, 5), Point(-1, -1));
	imshow("src1", src1);
	imshow("boxFilter", dst1);

	Mat src2 = imread("lena_full.jpg", 1);
	Mat dst2;
	blur(src2, dst2, Size(5, 5), Point(-1, -1));
	imshow("src1", src2);
	imshow("blur", dst2);

	waitKey(0);
	return 0;
}