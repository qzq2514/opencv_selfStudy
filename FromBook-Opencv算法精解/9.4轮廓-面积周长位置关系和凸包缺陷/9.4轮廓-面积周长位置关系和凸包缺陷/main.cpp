#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;


int main()
{
	//�㼯׼���Ϳ��ӻ�
	vector<Point> points;
	points.push_back(Point(0, 0));
	points.push_back(Point(50, 30));
	points.push_back(Point(100, 0));
	points.push_back(Point(100, 100));

	Mat canvas = Mat::zeros(Size(200, 200), CV_8U);
 
	//����ͨ����β�����ķ�ʽ������������
	/*int len = points.size();
	for (int i = 0; i <len; i++)
	{
		line(canvas, points[i], points[(i + 1) % len], Scalar(255), 1);
	}*/
	
	//Ҳ������opencv�Դ���polylines���㼯���ɵĶ����
	//polylines(����,�㼯,�Ƿ�ռ�����ɫ�����ֶ�)
	polylines(canvas, points, true, Scalar(255), 1);
	
	//ͻȻ�뵽ʹ��drawContours����fillPoly�����Զ�����������������~
	//vector<vector<Point> >contours;
	//contours.push_back(points);
	//drawContours(canvas, contours, 0, Scalar(255), -1);
	//fillPoly(canvas, contours, Scalar(255));    //�þ����Ͼ��໥���
	
	//1.�����ܳ������
	//arcLength(�㼯���Ƿ�պ�)��contourArea(�㼯)
	//ͬ�����㼯������������ʽ:Ԫ��ΪPoint��vector,N*2�ĵ�ͨ��Mat��N*1��˫ͨ��Mat
	double length1 = arcLength(points, false);
	double length2 = arcLength(points, true);
	double area = contourArea(points);
	cout << "��β�����������ܳ���" << length1 << endl;
	cout << "��β���������ܳ���" << length2 << endl;
	cout << "������Χ�������:" << area << endl;
	cout << "---------------------------" << endl;

	//2.��������Ĺ�ϵ-pointPolygonTest����
	vector<Point> testPoints;
	testPoints.push_back(Point(70, 40));
	testPoints.push_back(Point(100, 50));
	testPoints.push_back(Point(40, 80));
	for (int i = 0; i < testPoints.size(); i++)
	{
		circle(canvas, testPoints[i], 3, Scalar(255), CV_FILLED);

		//pointPolygonTest(���������ĵ㼯,���Ե�,�Ƿ��������㵽��������-measureDist)
		//measureDist=falseʱ������ֵ-1,0,1�ֱ��ʾ���Ե��������⡢�����ϡ�������
		//measureDist=trueʱ������ֵ������Ȼ���Ա�ʾ���Ե�������λ�ù�ϵ������ֵ��ʾ���Ե㵽��������ľ���
		double dist=pointPolygonTest(points, testPoints[i], true);
		cout << "dist " << i << ":" << dist << endl;
	}
	cout << "---------------------------" << endl;

	//3.͹��ȱ��
	vector<Point> contour;
	contour.push_back(Point(20, 20));
	contour.push_back(Point(50, 70));
	contour.push_back(Point(20, 120));
	contour.push_back(Point(120, 120));
	contour.push_back(Point(100, 70));
	contour.push_back(Point(120, 20));
	//����������͹��
	vector<int> hull;
	//convexHull����������͹��������֮ǰ�ù���convexHull�ڶ�����������Ϊvector<Point>��ʾ����͹���ĵ�
	//��������ڶ���������ʾ����͹���ĵ���ԭ�����е��±�
	convexHull(contour, hull, false, true);
	//����͹����ȱ��
	vector<Vec4i> defects;
    //convexityDefects(ԭ������͹�����±꼯�ϣ�ȱ����Ϣ)
	//defectsÿ��Ԫ����һ������Ϊ4�������ֱ�ȱ����㡢ȱ���յ㡢��Զ����������䵽͹���ľ���(��Ҫ����256)
	convexityDefects(contour, hull, defects);
	//��ӡ͹����ȱ��
	for (int i = 0; i < defects.size(); i++)
	{
		cout << defects[i] << endl;
	}

	imshow("canvas", canvas);
	waitKey(0);
	return 0;
}