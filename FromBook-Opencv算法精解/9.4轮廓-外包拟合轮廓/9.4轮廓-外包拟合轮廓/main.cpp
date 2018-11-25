#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


//��⳵���е��ַ�
void detectPlate()
{
	Mat src = imread("testPlate.jpg", 0);
	Mat colorSrc = imread("testPlate.jpg");
	imshow("ԭͼ", src);

	Mat gBlur;
	GaussianBlur(src, gBlur, Size(3, 3), 0.5);
	imshow("��˹ƽ��", gBlur);
	cout << gBlur.type() << endl;


	//���������ַ��뱳��������ɫ�������Բ����ַ�����ɫ����û�仯�ģ�
	//���ÿ���ַ������Ե���Ľ�����й����(����ͼƬ�ı�Ե�����ܻ�Ƚϻ���)
	//��ʱ����Բ����ж�ֵ����ֱ�ӽ��б�Ե���,��Ȼ����ֵ����Ч���϶�Ҳ����
	/*Mat thresh;
	threshold(gBlur, thresh, 140, 255, THRESH_BINARY);
	imshow("��ֵ��", thresh);*/

	Mat canny;
	Canny(gBlur, canny, 180, 200, 5, true);
	imshow("��Ե���", canny);


	vector<vector<Point> > contours;
	//RETR_TREE������������������״��νṹ�İ�Χ���Ȳ��޳��쳣��
	findContours(canny, contours, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat imgShow = colorSrc.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		//������С��Χ��ĸ߿�Ⱥ����복��ͼƬ������Ƚ���ɸѡ
		//�Ծ���ѡ���������ַ�����
		float areaRatio = 1.0*rect.area() / canny.total();
		float high_width_ratio = rect.height*1.0 / rect.width;
		if (areaRatio > 0.025 && areaRatio < 0.05 && high_width_ratio >1.2 && high_width_ratio <2.5)
		{
			rectangle(imgShow, rect, Scalar(0, 0, 255), 2);
		}
	}
	imshow("�����", imgShow);
}

void detectDog()
{
	Mat src = imread("dog.jpg", 0);
	Mat colorSrc = imread("dog.jpg");
	imshow("ԭͼ", src);

	Mat gBlur;
	GaussianBlur(src, gBlur, Size(3, 3), 0.5);
	imshow("��˹ƽ��", gBlur);
	cout << gBlur.type() << endl;

	Mat thresh;
	//OSTSU�Զ���ֵ�������168�����ǹ��ȱ��ֿ��������ֶ�������ֵ��168Сһ�㣬��֤Ŀ�������������
	threshold(gBlur, thresh, 155, 255,THRESH_BINARY);
	imshow("��ֵ��", thresh); 

	//���п�����(�ȸ�ʴ������)����Ŀ���������ϸС�����İ׵�
	Mat open;
	Mat kelOpen=getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresh, open, MORPH_OPEN, kelOpen);
	imshow("������", open);

	//������(�����ͺ�ʴ)�����������ϸС�ڶ�
	Mat close;
	Mat kelClose = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(open, close, MORPH_CLOSE, kelClose);
	imshow("������", close);

	Mat canny;
	Canny(close, canny, 180, 200, 5, true);
	imshow("��Ե���", canny);


	vector<vector<Point> > contours;
	//RETR_EXTERNALֻ�ҵ���Ϊ�������
	findContours(canny, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat imgShowRect = colorSrc.clone();
	Mat imgShowPloy = colorSrc.clone();
	Mat imgShowContour = colorSrc.clone();
	for (int i = 0; i < contours.size(); i++)
	{

		//����һ�������ǵ�ļ��ϣ�Ȼ�������������ڵ㻭������
		/*int lenC = contours[i].size();
		for (int j = 0; j < lenC; j++)
		{
			Point curPoint = contours[i][j];
			Point nextPoint = contours[i][(j+1)%lenC];

			line(imgShowContour, curPoint, nextPoint, Scalar(0,0,0), 2,7);
		}*/

		//OpenCV�Դ��Ļ������ĺ���-�ϴγ������ù�
		drawContours(imgShowContour, contours, i, Scalar(0, 0, 0), 2);

		//���ݵ�i��������������������Сֱ������
		Rect rect = boundingRect(contours[i]);
		rectangle(imgShowRect, rect, Scalar(0, 255, 0), 1);

		//��������
		vector<Point> ploy;
		//approxPolyDP(ԭ�����㼯�������Ϻ�������㼯����Ͼ��ȣ�����Ķ�����Ƿ���)
		//���ȱ�ʾ��ϵĵ㼯�����ڵ�������룬����ԽС�����ڵ����ԽС����ô���Խ׼ȷ�������ֶ��ı��ֵ���бȽ�
		//approxPolyDP��ʵ���Ƕ�����ԭ�㼯�Ľ�һ�����Ʊ�ʾ
		approxPolyDP(contours[i], ploy, 10,true);
		int len = ploy.size();
		for (int j = 0; j < len; j++)
		{
			Point curPoint = ploy[j];
			Point nextPoint = ploy[(j + 1) %len];

			line(imgShowPloy, curPoint, nextPoint, Scalar(0, 0, 255), 2);
		}
	}
	imshow("�����-��������", imgShowContour);
	imshow("�����-��������", imgShowPloy);
	imshow("�����-��С���ֱ������", imgShowRect);
}
int main()
{


	detectPlate();
	//detectDog();

	waitKey(0);
	return 0;
}