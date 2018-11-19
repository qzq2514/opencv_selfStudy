#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//�㼯�ı�ʾ��ʽ:
	//1.N��2�д�С��Mat��ÿһ�д���һ���������(x,y)
	//Mat points = (Mat_<float>(5, 2) << 190, 150, 70, 180, 40, 164, 50, 120, 270, 150);

	//2.vector<Point2f>���ͻ���vector<Point2>��ŵ㼯
	vector<Point2f> points;
	points.push_back(Point(190, 15));
	points.push_back(Point(70, 180));
	points.push_back(Point(40, 164));
	points.push_back(Point(50, 120));
	points.push_back(Point(270, 150));

	//3.�㼯�����N��1�е�˫ͨ��Mat��
	/*Mat points = (Mat_<Vec2f>(5, 1) << Vec2f(190, 150), Vec2f(70, 180), Vec2f(40, 164),
		                               Vec2f(50, 120), Vec2f(270, 150));*/


	//��ɫ����ϻ����㣬���п��ӻ�
	Mat img = Mat::zeros(Size(400, 400), CV_8UC3);
	for (int i = 0; i < points.size(); i++)
	{
		Point p = points[i];
		img.at<Vec3b>(p.y, p.x) = Vec3b(255, 255, 255);
	}

	//minAreaRect-���ݵ㼯�õ���С��ת����
	RotatedRect rRect = minAreaRect(points);
	cout << "��С��ת��������:" << rRect.center << endl;
	cout << "��С��ת���νǶ�:" << rRect.angle << endl;
	cout << "��С��ת���γߴ�:" << rRect.size << endl;

	//�������ת���ε��ĸ���������
	Mat vertices;
	boxPoints(rRect, vertices);
	cout << vertices << endl;
	cout << "-------------------------" << endl;

	//������С��ת����
	for (int i = 0; i < 4; i++)
	{
		Point curP = vertices.row(i);
		cout << curP << endl;
		Point nextPoint= vertices.row((i+1)%4);
		line(img, curP, nextPoint, Scalar(255,0,0),2);
	}


	// minEnclosingCircle-���ݵ㼯�õ���С���Բ��
	Point2f center;
	float radius;
	minEnclosingCircle(points, center, radius);//�����pointsͬ�����������ֵ㼯��ʽ
	circle(img, center, radius, Scalar(0, 255, 0), 2);   //������С�������


	// boundingRect-���ݵ㼯�õ���С���ֱ������
	//Mat(points)��ʽ�ǽ�vector<Point2f>�㼯ת���� һ��N*1����ͨ��Mat
	//ͬ������ʹ��������ʽ�ĵ㼯�õ���С���ֱ������
	Rect rect = boundingRect(Mat(points));
	//Rect rect = boundingRect(points);   //һ���vector<Point2f>�㼯�����С���ֱ������

	rectangle(img, rect, Scalar(0, 0, 255), 2);

	imshow("��С����㼯", img);
	waitKey(0);
	return 0;
}