#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;
#define put_equal cout << "=======" << endl;
int main()
{

	Mat src1 = (Mat_<int>(2, 3) << 23, 123, 90, 100, 250, 0);
	Mat src2 = (Mat_<int>(2, 3) << 125, 150, 60, 100, 10, 40);
	
	//�ӷ�����
	//1.ֱ��"+"���мӷ�
	//��ʱ����������ӣ�Ҫ��֤�������������������ͬ������ʱ��Ҫ������ֵ���������
	Mat dst_add1 = src1 + src2;    
	dst_add1 = dst_add1 + 1000.3;   //������ĳ����ֵ��ӣ����ص�Mat������������������������ͬ
	
	for (int r = 0; r < dst_add1.rows; r++)
	{
		for (int c = 0; c < dst_add1.cols; c++)
		{
			cout << dst_add1.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat src3 = (Mat_<float>(2, 3) << 125, 150, 60, 100, 10, 40);
	Mat dst_add2;
	//ʹ��opencv�ļӷ�����add,����ʵ��ͼ�����ӣ���ʱ�򲢲�Ҫ������ͼ�����������һ�£�
	//ͬʱ���Բ����������ָ����������,Ĭ�ϵ�type=-1��
	//��ʾ�����ͼ�������������ԭ��������ӵ�ͼ�����������һ��
	//������ʱ���Ҫ��ԭ������ͼ�������������ͬ
	add(src1, src3, dst_add2,Mat(), CV_64FC1);

	for (int r = 0; r < dst_add2.rows; r++)
	{
		for (int c = 0; c < dst_add2.cols; c++)
		{
			cout << dst_add2.at<double>(r, c) << ",";   //ע����ǣ�ָ���������������64F,���ʾ��double,������float
		}
		cout << endl;
	}
	put_equal;

	Vec3f v1 = Vec3f(1, 2, 3);
	Vec3f v2 = Vec3f(10, 20, 30);
	Vec3f v3 = v1 + v2;    //�ӷ�ͬ������ʵ�����������
	cout << v3 << endl;;

	put_equal;

	//�������
	//�������ͬ������ʹ��"-"���ط�ʵ�֣���������ע������ͼӷ�"+"һ��
	Mat dst_sub;

	//�ͼӷ�һ��������ͬ��Ҳ��opencv����-subtract,ע������һ����
	subtract(src1, src3, dst_sub, Mat(), CV_64FC1);
	for (int r = 0; r < dst_sub.rows; r++)
	{
		for (int c = 0; c < dst_sub.cols; c++)
		{
			cout << dst_sub.at<double>(Point(c,r)) << ",";   //ע����ǣ�ָ���������������64F,���ʾ��double,������float
		}
		cout << endl;
	}
	put_equal;

	//������
	//ʹ��Mat�ĳ�Ա����mul���о����ˣ�ͬ��Ҫ�������������������һ��
	//���������������ʾ�����˺�Ҫ����һ��ϵ��
	Mat dst_mul1=src1.mul(src2,2.0);
	//Mat dst_mul1 = src1*src2;        //������ʹ��"*"����Mat�ĵ��,������ʾ����˷��������ǵ��
	//Mat dst_mul1 = src1.mul(2.0);    //�ͼӼ���һ�������󻹿��Ժ�һ��������������
	for (int r = 0; r < dst_sub.rows; r++)
	{
		int *ptr = dst_mul1.ptr<int>(r);
		for (int c = 0; c < dst_sub.cols; c++)
		{
			cout << ptr[c] << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat dst_mul2;
	//ͬ������ʹ��opencv�ĺ���-multiplyʵ��Mat��ˣ�1.0ͬ����ʾϵ��
	//�����add,subtract�������÷�һ��
	multiply(src1, src3, dst_mul2,1.0, CV_32FC1);
	int *ptr = dst_mul1.ptr<int>(0);

	for (int n = 0; n < dst_mul2.total(); n++)
	{
		
		cout << ptr[n] << ",";
	}
	cout << endl;

	put_equal;
	
	//������
	//ͬ������ʹ��"/"ʵ��Mat�ĵ����Ҫ�������������һ��
	Mat dst_div1 = src2/src1;      //���ﲻ�����������ǵõ�С������������
								   //��������Ϊ0���򷵻�0����������40/0������0
	for (int r = 0; r < dst_div1.rows; r++)
	{
		for (int c = 0; c < dst_div1.cols; c++)
		{
			int *val = (int*)(dst_div1.data + r*dst_div1.step[0] + c*dst_div1.step[1]);
			cout << *val << ",";
		}
		cout << endl;
	}

	put_equal;
	Mat dst_div2;
	//ʹ��opencv�Դ���divideʵ�־�����,��������Ϊ0���򷵻�0����������40/0������0
	//�����ϵ��2.0����Ϊ����������dst_div2=2.0*(src3/src1)
	divide(src3,src1, dst_div2, 2.0, CV_32FC1);

	for (int r = 0; r < dst_div2.rows; r++)
	{
		for (int c = 0; c < dst_div2.cols; c++)
		{
			cout << dst_div2.at<float>(r, c) << ",";
		}
		cout << endl;
	}

	put_equal;

	//����˷�
	//���������Դ����еľ���˷�������Ҫ�������������ͱ�����float����double 
	Mat srcMul1 = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat srcMul2 = (Mat_<float>(3, 2) << 6, 5, 4, 3, 2, 1);
	Mat dstMul = srcMul1*srcMul2; 
	for (int r = 0; r < dstMul.rows; r++)
	{
		for (int c = 0; c < dstMul.cols; c++)
		{
			cout << dstMul.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;
	
	//˫ͨ��Mat�ĳ˷�
	//˫ͨ������ÿ��Ԫ��������Ϊһ����������Vec2f(1, 2)��ʾ����:1+2i
	//֮�������ÿ����ֵ����һ��������Ȼ����и����ĳ˷������磺
	//dstMu2[0,0]=srcMul3[0,0]*srcMul4[0,0]=(1+2i)*(10+20i)=-30+40i,Ҳ�����������е�[-30,40]
	Mat srcMul3 = (Mat_<Vec2f>(2, 1) << Vec2f(1, 2), Vec2f(3, 4));
	Mat srcMul4 = (Mat_<Vec2f>(1, 2) << Vec2f(10, 20), Vec2f(5, 15));
	Mat dstMu2 = srcMul3*srcMul4;    //ֱ��ʹ��"*"���о���˷�
	for (int r = 0; r < dstMu2.rows; r++)
	{
		for (int c = 0; c < dstMu2.cols; c++)
		{
			cout << dstMu2.at<Vec2f>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	Mat dstMu3;
	gemm(srcMul3,srcMul4, 0.5, dstMu2,1.2, dstMu3,0);
	//0.5��alpha,1.2��beta,������ʾ:dstMu3=0.5*srcMul3*srcMul4+1.2*dstMu2
	//����0��flag,����srcMul3��srcMul4��dstMu3�Ƿ�ת�����������㣬
	//���ʾ��ͬ�������ʽ�����忴��P48����ͼƬ"gemm����flags��������.jpg"
	//��"*"���о���˷���ͬ��gemmҲֻ���յ�ͨ��/˫ͨ����float/double

	for (int r = 0; r < dstMu3.rows; r++)
	{
		for (int c = 0; c < dstMu3.cols; c++)
		{
			cout << dstMu3.at<Vec2f>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	//ָ���Ͷ�������
	//����ָ���Ͷ��������Mat�������������ͱ�����float/double���ǵ�ͨ����
	Mat src_log = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat dst_log;
	log(src_log, dst_log); //log�ĳ�exp���Խ��ж�������

	for (int r = 0; r < dst_log.rows; r++)
	{
		for (int c = 0; c < dst_log.cols; c++)
		{
			cout << dst_log.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;
	
	//��ָ���Ϳ�ƽ������
	//sqrt�������������ֻ����float/double,��pow�����κ��������͵�Mat
	Mat src_pow = (Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6);
	Mat dst_pow;
	pow(src_pow,2, dst_pow); //��ָ������
	sqrt(src_pow,dst_pow);   //��ƽ������

	for (int r = 0; r < dst_pow.rows; r++)
	{
		for (int c = 0; c < dst_pow.cols; c++)
		{
			cout << dst_pow.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	put_equal;

	system("pause");
	return 0;
}