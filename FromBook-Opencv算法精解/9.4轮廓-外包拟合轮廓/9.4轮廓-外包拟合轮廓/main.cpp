#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("testPlate.jpg", 0);
	Mat colorSrc = imread("testPlate.jpg");
	imshow("ԭͼ", src);

	Mat gBlur;
	GaussianBlur(src, gBlur, Size(3, 3), 0.5);
	imshow("��˹ƽ��", gBlur);
	cout << gBlur.type()<<endl;

	Mat thresh;
	threshold(gBlur, thresh, 140, 255, THRESH_BINARY);
	imshow("��ֵ��", thresh);
	Mat canny;
	Canny(thresh, canny, 180, 200, 5, true);
	imshow("��Ե���", canny);


	vector<vector<Point> > contours;
	//RETR_TREE������������������״��νṹ�İ�Χ���Ȳ��޳��쳣��
	findContours(canny, contours, RETR_TREE,CV_CHAIN_APPROX_SIMPLE);

	Mat imgShow = colorSrc.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect= boundingRect(contours[i]);

		//������С��Χ��ĸ߿�Ⱥ����복��ͼƬ������Ƚ���ɸѡ
		//�Ծ���ѡ���������ַ�����
		float areaRatio = 1.0*rect.area() / canny.total();
		float high_width_ratio = rect.height*1.0 / rect.width;
		if (areaRatio > 0.025 && areaRatio < 0.1 && high_width_ratio >1.2 && high_width_ratio <2.5)
		{
			rectangle(imgShow, rect, Scalar(0, 0, 255), 2);
		}
	}
	imshow("�����", imgShow);


	waitKey(0);
	return 0;
}