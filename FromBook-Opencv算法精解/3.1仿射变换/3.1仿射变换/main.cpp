#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//����������

	//ͼ����һ�����󣬶������ƽ�ƣ���������ת����ʵ����ԭͼ������е����ݵ�������Է��������пռ�任
	//��������Ϊ������[x;y]�任�������Ϊ[x';y'],Ϊ�˼��㷽�㲢�Ҽ���ƫ�ã����ǽ���������Ϊ
	//[x;y]->[x,;y;1],[x';y']->[x';y';1],���������������3x3�ľ������һ����������[0,0,1]

	//Ϊ�˸��ݱ任ǰ�Ľ���ͱ任��Ľ���õ�������󣬱�������������ԣ��г�6�������飬���
	//6��δ֪��(��3x3��������ǰ����)

	//����ͨ��[0,0],[200,0],[0,200]��������仯����A�õ��������ֱ�Ϊ[0,0],[100,0],[0,100]
	//���������ַ�����øı任�ķ������

	//1.ֱ��ͨ������Point2f���͵Ļ�÷������
	Point2f srcP[] = {Point2f(0, 0), Point2f(200, 0), Point2f(0, 200)};   //�任ǰ��������
	Point2f dstP[] = { Point2f(0, 0), Point2f(100, 0), Point2f(0, 100) };
	Mat A1 = getAffineTransform(srcP, dstP);
	cout << A1 << endl;             //���صķ������ֻ��2x3��С�ģ���ǰ����
	cout << "-----------" << endl;

	//2.�ѵ�Ž�Mat������,ͨ��getAffineTransform�������ط������A2
	//ע��A2����������CV_64F,������CV_32F
	Mat srcM = (Mat_<float>(3,2) << 0, 0, 200, 0, 0, 200);   //ÿһ��Ϊһ�������
	Mat dstM = (Mat_<float>(3, 2) << 0, 0, 100, 0, 0, 100);
	Mat A2 = getAffineTransform(srcM, dstM);
	cout << A2 << endl;
	cout << "-----------" << endl;


	//֮ǰѧ��Mat����ĳ˷���þ������Կ���ͨ������˷���÷������
	//��ͨ�������ķ���任������ϵõ����ӵķ���任
	Mat s = (Mat_<float>(3, 3) << 0.5, 0, 0, 0, 0.5, 0, 0, 0, 1);  //���ž���
	Mat t = (Mat_<float>(3, 3) << 1, 0, 100, 0, 1, 200, 0, 0, 1);   //ƽ�ƾ���  (�������ۿ���)
	Mat A3;	
	//gemm(s, t, 1.0, Mat(), 0, A3, 0);  //A3=1.0*s*t+0*Mat()����ʾ�Ƚ���ƽ�ƣ��ٽ������ű任
	A3 = t*s;       //���ֱ�ʾ�����ţ���ƽ�ƣ���Ϊ[x';y';1]=A3*[x;y;1],�����Ǵ������������
	                //���������ϵķ��������򵥷���˳���й�ϵ
	cout << A3 << endl;
	cout << "-----------" << endl;

	//������������ű�����ͬʱ(��Sx=Sy�ȱ�����)�����Ƚ��������ٽ�����ת��
	//�����Ƚ�����ת���ٽ������ţ��õ�����ͬ�ķ�����󣬿���ͨ��getRotationMatrix2D������÷������
	//getRotationMatrix2D(p1,angle,scale)�в����ֱ��ʾ��ת���ģ���ת����(�Ƕȣ����ǻ���),���ű���
	Mat A4 = getRotationMatrix2D(Point2f(40, 50), 30, 0.5); //��getAffineTransformһ����ͬ������2x3��CV_64F�ľ���
	cout << A4 << endl;



	//��ֵ����
	//��ֵ�������Ϊ�˼����ڷ���仯ʱ��Ŀ��ͼ�����޷���ԭͼ��׼ȷ�����Ԫ��ֵ
	//���磬����ԭ��(0,0)Ϊ���ĵ㣬��������Ϊ2���ķ����任���У�
	//fo(2x,2y)=fi(x,y)������Ŀ��ͼ��������Ϊ(3,3)������ֵ����˵Ϊԭͼ��������ֵΪ(1.5,1.5)��
	//��ֵ�������±�����û��С����������Ҫ�ҵ����ʵ�ֵȥ����fi(1.5,1.5),��ֵ��������ҵ��������ʵ�ֵ
	
	Mat src = imread("lena_full.jpg",1);
	imshow("ԭͼ", src);
	
	int w = src.cols;
	int h = src.rows;
	cout << w << ":" << h << endl;  //330:706

	Mat AA1 = (Mat_<float>(2, 3) << 0.5, 0, 0, 0, 0.5, 0);   //����任����-��С����
	Mat d1;
	//������С����,��ָͼ�����Ч������СΪԭ��1/2������imshow������ͼƬ��Ȼ�Ǻ�ԭͼͬ����С��
	//�������Ч�������ͨ��ָ��Ϊ������ɫֵ(BORDER_CONSTANT����)����ͨ��Scalar(B,G,R)����ָ�������ɫ
	warpAffine(src, d1, AA1,Size(w,h),INTER_NEAREST,BORDER_CONSTANT,Scalar(100,120,200));
	//INTER_NEAREST:����ڲ�ֵ����ĳС�������Ԫ��ֵ������������������Ԫ��ֵ
	cout << d1.cols << ":" << d1.rows << endl;
	imshow("��С����", d1);    //330:706

	Mat AA2 = (Mat_<float>(2, 3) << 0.5, 0, w/4.0, 0, 0.5, h/4.0);   //����任����-��С������ƽ��
	Mat d2;
	//INTER_NEAREST��˫���Բ�ֵ����С������㸽�����ĸ���������������ֵ���м�Ȩ���
	warpAffine(src, d2, AA2, Size(w, h), INTER_NEAREST, BORDER_CONSTANT, Scalar(100, 120, 200));
	imshow("��С������ƽ��", d2);

	Mat AA3 = getRotationMatrix2D(Point2f(w / 2, h / 2), 30, 1);
	Mat d3;
	warpAffine(d2, d3, AA3, Size(w, h));
	imshow("��С����ƽ�ƺ���������ʱ����ת30��", d3);
	

	//�򻯰�����
	Mat d4;
	//����Ϊԭ����һ��
	resize(src, d4, Size(w / 2, h / 2),0,0,INTER_NEAREST);
	//resize(src, d4, Size(), 0.5, 0.5, INTER_NEAREST);   //ͬ��Ч��,Size���������������fx,fy����ߵ����ű���
	imshow("�򻯰���������", d4);//resize�õ���Ŀ��ͼֻ������Ч��������Ҳ�Ͳ���Ҫ������Ч����


	//�򻯰���ת��opencv3.x����ת����rotate,���汾������2.x�Ͳ�ʵ����
	cout << "CV_VERSION:"<<CV_VERSION << endl;// ???������3.1.0��,���ˣ��Ժ��ʹ��getRotationMatrix2D������
	Mat dst;
	transpose(src, dst);
	flip(dst, dst, 0);      //rotate�����ã�����ʹ��ת�ü��Ͼ���ת����ת��Ч��-��ʱ����ת90��
	imshow("��ת", dst);
	waitKey(0);
	//system("pause");
	return 0;
}