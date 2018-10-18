#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat I;
Mat pI;
Point2f IPoint, pIPoint;
int i = 0, j = 0;
Point2f src[4];
Point2f dst[4];
Scalar scalars[4] = { Scalar(0,0,0),Scalar(0,0,255),Scalar(0,255,0),Scalar(255,255,255)};
void mouse_I(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	    case CV_EVENT_LBUTTONDOWN:   //��갴���¼�
			
			IPoint = Point2f(x, y);
			src[i] = IPoint;
			cout << "mouse_I����¼�....." << src[i] << endl;
			//��ͼƬI�ϻ�Բ����src[i]ΪԲ�ģ�5Ϊ�뾶��scalars[i]��Բ����ɫ��3��Բ��������ϸ,6����������
			circle(I, src[i], 5, scalars[i], 3,6);
			i += 1;
		    break;
		default:
			break;
	}
}
void mouse_pI(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:   //��갴���¼�
		pIPoint = Point2f(x, y);
		dst[j] = pIPoint;
		cout << "mouse_pI����¼�....." << dst[j] << endl;
		circle(pI, dst[j], 5, scalars[j], 3);
		j += 1;
		break;
	default:
		break;
	}
}
int handle1()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	int w = I.cols;
	int h = I.rows;
	Mat src = (Mat_<float>(4, 2) << 0, 0, w - 1, 0, 0, h - 1, w - 1, h - 1);
	Mat dst = (Mat_<float>(4, 2) << 50, 50, w/3, 50, 50, h - 1, w - 1, h - 1);


	//��Ƚ������任��ͶӰ�任��Ҫ�Ķ������(��src��dst����4*2����)����getAffineTransformһ����������ԣ�
	//������Ե���������Ҳ��float,���صı任����Ϊ3*3������������CV_64F(getAffineTransform����2*3����)��
	Mat p = getPerspectiveTransform(src, dst);

	Mat result;
	//ͶӰ�任����warpPerspective���������warpAffineһ��:
	//������ԭͼ�񣬽��ͼ�񣬱任����ͼƬ��С(��������Ч����),��ֵ�㷨������Ч������䷽���������ɫֵ
	warpPerspective(I, result, p, pI.size(), INTER_NEAREST, BORDER_CONSTANT, Scalar(100, 120, 200));

	imshow("Origin pic", I);
	imshow("After perspective", result);

	waitKey(0);
	return 0;
}
int handle2()
{
	I = imread("prespective.jpg", 1);
	if (I.empty())
	{
		cout << "Empty pic....." << endl;
		return -1;
	}

	pI = 255 * Mat::ones(I.size(), CV_8UC3);   //Ŀ��ͼ��ʼ��Ϊ�հ�ͼ
	cout << pI.rows << "-" << pI.cols << endl;
	//��ԭͼ�����϶�������¼�
	namedWindow("I", WINDOW_AUTOSIZE);    //�ڶ���������ʾ�����Ƿ���Ըı�
	setMouseCallback("I", mouse_I, NULL); //(WINDOW_NORMAL=0�ɸı�)(WINDOW_AUTOSIZE=1���ɸı�)

	//����������϶�������¼�
	namedWindow("pI", 1);
	setMouseCallback("pI", mouse_pI, NULL);

	imshow("I", I);
	imshow("pI", pI);
	while (!(i == 4 && j == 4))
	{
		imshow("I", I);
		imshow("pI", pI);
		if (waitKey(50) == 'q')
			break;
	}

	cout << "ͶӰ������ѡȡ���...." << endl;
	imshow("I", I);
	imshow("pI", pI);

	/*for (int n = 0; n < 4; n++)
	{
		cout << src[n] << ":" << dst[n] << endl;
	}*/

	//�Ƴ�����¼�
	setMouseCallback("I", NULL, NULL);
	setMouseCallback("pI", NULL, NULL);

	
	Mat p = getPerspectiveTransform(src, dst);
	/*cout << "ͶӰ�任����:" << endl;
	cout << p << endl;*/


	//����ͶӰ�仯
	Mat result;
	warpPerspective(I, result, p, pI.size());
	imshow("After perspective", result);//��ʾ�任���

	waitKey(0);
	return 0;
}
void handle3()
{
	//�ѿ��������Ϊ������
	//cartToPolar��������ԭ��(0,0)Ϊ���Ľ��м�����任
	//�������������(1,1)Ϊ���Ľ��м�����任��Ҫ�Ƚ�����ת����ԭ�㣬
	//Ȼ�����cartToPolar���м�����任���������α�ʾ(�ѿ����µĺ����꼯�������꼯��������ϵ��r,theta,�Ƿ�ʹ�ýǶ���)
	Mat x = (Mat_<float>(3, 3) << 0, 1, 2, 0, 1, 2, 0, 1, 2) - 1;
	Mat y = (Mat_<float>(3, 3) << 0, 0, 0, 1, 1, 1, 2, 2, 2) - 1;
	Mat r, theta;
	cartToPolar(x, y, r, theta, true);
	cout << r << endl;
	cout << theta << endl;     //�ڵѿ�������ϵ�о���任������ͬ�ĵ㣬�ڼ�����ϵ��λ��ͬһ��ֱ��(��Ϊr������ͬ)
	system("pause");
}
void handle4()
{
	//polarToCart������cartToPolar�����������,���Եѿ���ϵ��ԭ��(0,0)Ϊ���ĵѿ��������Ϊ������
	Mat angle = (Mat_<float>(2, 2) << 30,31,30,31) ;
	Mat r = (Mat_<float>(2, 2) <<10,10,11,11);
	Mat x, y;
	polarToCart(r, angle, x, y, true);
	x += -12;      //��ΪpolarToCart��õ����Եѿ��������е�ԭ��(0,0)Ϊ�任����
	y += 15;       //����任������(-12,15)����ô���൱�ڰ������ѿ�������ϵƽ�Ƶ�(-12,15)
	cout << x << endl;//��ʵ����"������->�ѿ�������"�������
	cout << y << endl;//�ѿ�������->������:�任�����Ƶ�(0,0),����cartToPolar
	system("pause");  //������->�ѿ�������:����polarToCart,�������(0,0)Ϊ�����ƶ�����ʵ�任����
}

//ԭͼ���任����(�ѿ�������ϵ��),Ŀ��ͼ��С����Сr����Сtheta��r������theta����
Mat polar(Mat I, Point2f center, Size size, float minr=0,float mintheta=0,float thetaStep=1.0/4,float rStep=1.0)
{
	Mat ri = Mat::zeros(Size(1, size.height), CV_32FC1);    //���r����

	for (int i = 0; i < size.height; i++)
	{
		ri.at<float>(i, 0) = minr + i*rStep;      //����r����
	}
	Mat r = repeat(ri, 1, size.width);     //��ri����(height*1)��ֱ�����ظ�һ�Σ�ˮƽ�����ظ�width,
											//����height*width��С�ľ���(������ˮƽ����)
	
	Mat thetaj = Mat::zeros(Size(size.width, 1), CV_32FC1);
	for (int j = 0; j < size.width; j++)
	{
		thetaj .at<float>(0,j)= mintheta + thetaStep*j;
	}

	Mat theta = repeat(thetaj,size.height, 1);    //theat�䵱y����(�����괹ֱ����)

	Mat x, y;
	polarToCart(r, theta, x, y, true);

	x += center.x;
	y += center.y;

	//dst(i,j)=src(cx+(min_r+r_step*i)*cos(min_theta+theta_stpe*j), 
	          //   cy+(min_r+r_step*i)*sin(min_theta+theta_stpe*j))  ����Ϊdst(i,j)=src(x,y)
	//���Ƚ�ԭͼ�о������ĵ�min_r+r_step*i���н�Ϊmin_theta+theta_stpe*j�ĵ�(x,y)ӳ��ֵĿ��ͼ��������(min_r+r_step*i,min_theta+theta_stpe*j)
	//Ȼ���ٽ��з�����ƽ�Ʊ任��(min_r+r_step*i,min_theta+theta_stpe*j)�任������Ŀ��ͼ��(i,j)λ��

	Mat dst = 125 * Mat::ones(size, CV_8UC1);
	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			float xij = x.at<float>(i, j);
			float yij = y.at<float>(i, j);
			int nearestx = int(round(xij));
			int nearesty = int(round(yij));
			
			if (nearestx >= 0 && nearestx < I.cols && nearesty >= 0 && nearesty < I.rows)
			{
				dst.at<uchar>(i, j) = I.at<uchar>(nearestx, nearesty);
			}
		}
	}
	return dst;
}
void handle5()
{
	Mat src = imread("circlePlate.jpg", 1);
	if (src.empty())
	{
		cout << "circlePlateͼƬΪ��......" << endl;
		return;
	}

	float thetaStep=1.0;
	float minr = 100;
	Size size(int(500 / thetaStep), 250);    
	Mat dst = polar(src, Point2f(200, 250), size, minr);

	//ˮƽ��ת
	flip(dst, dst, 0);
	imshow("ԭͼ", src);
	imshow("������任��", dst);
	waitKey(0);
}

int main()
{
	 //ͶӰ�任�������Ķ�CV_32F�������ݵ�,��4*2���õ�CV_64F���͵ı任����(3*3)
//��ϰ:����任��....��..................,..3*2��........................(3*2)
	//handle1();        //�����о͸�����ͶӰ�任�����������
	//handle2();          //ͨ���ڻ������ֶ��������ȡ�Ķ������
	
	
	//������͵ѿ��������ת��
	//1.�ѿ��������Ϊ������
	//handle3();

	//2.�������Ϊ�ѿ�������
	//handle4();


	handle5();
	return 0;
}