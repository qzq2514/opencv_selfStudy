#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
int main()
{
	//��ֵ�ָ���ǽ�����ͼ��ָ�ɶ������ÿ�������������������
	//һ����ֵ�ָ�Ҷ�ͼ��ÿ����������ֵ�Ƚϣ�����仯Ϊ0����255��������ֵ�ָ��ֳ�����ͼ��Ķ�ֵ������
	//��ֵ�ָ����Ҫ�Ǹ��ݻҶ�ֵ��Ϣ��ȡǰ��
	//��ֵ�ָ��Ϊ:ȫ����ֵ�ָ������Ӧ�ֲ���ֵ�ָ�
	Mat src = (Mat_<uchar>(5, 3) << 123, 234, 68, 33, 51,
									17, 48, 98, 234, 129,
									89, 27, 45, 167, 134);
	double thresh = 150;
	Mat dst;

	threshold(src,dst,thresh,255,THRESH_BINARY);
	cout << dst << endl;
	//THRESH_BINARY(0):  ������ֵ����ΪmaxVal,����Ϊ0
	//THRESH_BINARY_INV(1):������ֵ����Ϊ0,����ΪmaxVal
	//THRESH_TRUNC(2):������ֵ���ض���Ϊ��ֵ�����򲻱�
	//THRESH_TOZERO(3):������ֵ�����䣬����Ϊ0
	//THRESH_TOZERO_INV(4):������ֵ����Ϊ0.���򲻱�

	double ostuThresh = 0;
	Mat dst_Ostu;
	//����THRESH_OTSU��ʾ�Զ�������ֵ(ʹ��OTSU�㷨)���������ֵ��������THRESH_BINARY��ʽ������ֵ�ָ�
	//��Ȼ����Ҳ���Բ��ô�ʱTHRESH_OTSU+THRESH_BINARY_INV����ʾ�Զ�������ֵ�����THRESH_BINARY_INV��ʽ������ֵ�ָ������
	//threshold�����᷵���Զ��������ֵ
	ostuThresh =threshold(src, dst_Ostu, ostuThresh, 255, THRESH_OTSU + THRESH_BINARY);
	cout << "ostuThresh:" << ostuThresh << endl;
	cout << dst_Ostu << endl;
	
	double triThresh = 0;
	Mat dst_Tri;
	//ͬ����THRESH_TRIANGLEҲ�Զ�������ֵ(ʹ��triangle�㷨)��Ȼ������THRESH_BINARY��ʽ������ֵ�ָ����ͬ������
	//��ʱ�������ǻ᷵���Զ��������ֵ
	triThresh = threshold(src, dst_Tri, triThresh, 255, THRESH_TRIANGLE + THRESH_BINARY);
	cout << "triThresh:" << triThresh << endl;
	cout << dst_Tri << endl;

	//�����ᵽ�Ķ���ȫ����ֵ����ԭʼͼƬ��������һ���̶�����ֵ���бȽ�(�������ֶ�ָ������OTSU����triangle�㷨�Զ��ü������ֵ)
	//���ֲ���ֵ�£�ԭͼÿ�����ض����Լ����Ӧ����ֵ������õ�һ����ԭͼ��С��ȵ���ֵ����Ȼ����λ�Ƚ�
	//�Ƚϼ򵥵��ǽ���λ�����ص�����λ�õ�ƽ��ֵ��Ϊ��λ�ñȽϵ���ֵ
	system("pause");
	return 0;
}
