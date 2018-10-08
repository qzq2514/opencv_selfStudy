#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//2.2.2���쵥ͨ��Mat����
	Mat m1 = Mat(2, 3, CV_32FC1);   //����2��3��ͼ�����ÿ��Ԫ����32λfloat����,��ͨ��

	Mat m2 = Mat(Size(3, 2), CV_32FC1);    //��Size����ͼ������С����ʽ:Size(����������)

	Mat m3;
	m3.create(Size(3, 2), CV_32FC1);      //ʹ��Mat���create��������ͼ��

	Mat ones = Mat::ones(2, 3, CV_32FC1); //����ȫ1����(��ɫ)
	Mat zeros = Mat::zeros(2, 3, CV_32FC1); //����ȫ0����(��ɫ)

	Mat m4 = (Mat_<int>(2, 3) << 1, 2, 3, 4, 5, 6);  //���ٴ���2��3�У�Ԫ��Ϊint��С��ͼ�����
	cout << "---------------------" << endl;

	//2.2.3��õ�ͨ��Mat�������Ϣ
	cout << m4.rows << endl;
	cout << m4.cols << endl;              //ͼ����������(��)������(��)
	cout << m4.channels() << endl;        //ͼ��ͨ����-1
	cout << m4.total() << endl;           //ͼ������������(��ͨ�����޹�)
	cout << m4.dims << endl;              //ͼ��ά��-��ͨ��ͼ���Ƕ�ά��

	Size s = m4.size();   //[3*2]-����ǰ�����ں�
	cout << s << endl;
	cout << "---------------------" << endl;

	//2.2.4���ʵ�ͨ��Mat�����ֵ
	Mat m5 = (Mat_<int>(3, 2) << 1, 2, 3, 4, 5, 6);
	for (int r = 0; r < m5.rows; r++)
	{
		for (int c = 0; c < m5.cols; c++)
		{
			//cout << m5.at<int>(r, c) << ",";  //1.ʹ��Mat�����at�������ʣ�<int>��ʾ��ͨ������������ΪCV_32SC1
											  //����r��c�е�ͼ��Ԫ��

			//cout << m5.at<int>(Point(c, r)) << ",";     //2.ʹ��������ʽ����ͼ������Ԫ�أ�
													//��һ��Ԫ���������꣬�ڶ���Ԫ����������

			                                           //3.ʹ��step��data����ͼ�����
			int v = *((int *)(m5.data + m5.step[0] * r + m5.step[1] * c)); //data���Ա�ʾ����ͼ�������׵�ַ
			cout << v << ",";											   //step[0]��ʾͼ��ÿһ�е��ֽ���
		}									                               //step[1]��ʾͼ��ÿһ��Ԫ����ռ�ֽ���
		cout << endl;
	}

	cout << "=======" << endl;
	for (int r = 0; r < m5.rows; r++)
	{
		const int * ptr = m5.ptr<int>(r);     //4.ptr�������ָ���е����׵�ַ
		for (int c = 0; c < m5.cols; c++)
		{
			cout << ptr[c] << ",";           //ͨ�����׵�ַָ����ʸ��е�c��Ԫ��
		}
		cout << endl;
	}

	cout << "=======" << endl;
	//����ÿһ�е�ֵ���洢���������ڴ��У�����������֮����ܻ��м��
	//ʹ��isContinuous()�����ж�������֮���ǲ���������
	if (m5.isContinuous())
	{
		const int * ptr = m5.ptr<int>(0);
		for (int c = 0; c < m5.total(); c++)  //m5.total()�͵���m5.rows*m5.cols,��ͨ���£��������ɷ���ͼ����������Ԫ��
			cout << ptr[c] << ",";
		cout << endl;
	}
	cout << "----------------" << endl;

	//2.2.6������Vec
	//��ͨ��ͼ���Ԫ������ֵ������1,2...
	//��ͨ��ͼ���Ԫ��������(Vec),�����ĳ�����ͼ���ͨ����

	Vec<int, 3> vi(21, 22, 23);
	cout << vi[0] << "," << vi[1] << endl;  //ͨ���±����������Ԫ��
	cout << vi.rows << "," << vi.cols << endl;  //ͬ�����Եõ�����������������

	/*OpenCVΪ�����������ȡ�˱���������:
     typedef Vec<uchar,3> Vec3b
	 typedef Vec<int,2> Vec2i
	 typedef Vec<float,4> Vec4f
	 typedef Vec<double,3> Vec3d
	 */
	cout << "-------------------" << endl;

	//2.2.6 �����ͨ��Mat����
	Mat mm = (Mat_<Vec3f>(2, 3) << Vec3f(1, 2, 3), Vec3f(4, 5, 6), Vec3f(7, 8, 9),
								   Vec3f(10, 11, 12), Vec3f(13, 14, 15), Vec3f(16, 17, 18));  
	                                 //���Ƶ�ͨ�����ٹ����ͨ��ͼ��������Ƕ�ͨ��ͼ���Ԫ��������
									 //��������Mat_<int>������Mat_<Vec3f>
									 //ͬ�����������ƹ��쵥ͨ����������һ�������ͨ����ֻ����Ҫ��
									 //����CV_32FC1�ĳ�CV_32FC3����



	cout << "end........" << endl;
	system("pause");
	return 0;
}