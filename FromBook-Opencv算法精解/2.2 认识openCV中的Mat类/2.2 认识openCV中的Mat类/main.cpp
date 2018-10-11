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
			int *v = (int *)(m5.data + m5.step[0] * r + m5.step[1] * c); //data���Ա�ʾ����ͼ�������׵�ַ,����һ����ֵ�ĵ�ַ(���ǵ�һ��Ԫ��)
			cout << *v << ",";											   //step[0]��ʾͼ��ÿһ�е��ֽ���
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
		//const int * ptr = m5.ptr<int>(0);  //ͨ��
		const int * ptr = (int *)m5.data;    //data���Եõ�ͼ���һ����ֵ�ĵ�ַ����Ϊ��ͨ����һ��Ԫ�ؾ���һ����ֵ����������
											 //(int *)data��ptr<int>(0)��Ч����һ����

		for (int c = 0; c < m5.total(); c++)  //m5.total()�͵���m5.rows*m5.cols,�������ɷ���ͼ����������Ԫ��
			cout << ptr[c] << ",";
		cout << endl;
	}
	cout << "----------------" << endl;

	//2.2.5������Vec
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
	                                 //���Ƶ�ͨ��һ�����ٹ����ͨ��ͼ��������Ƕ�ͨ��ͼ���Ԫ��������
									 //��������Mat_<int>������Mat_<Vec3f>
									 //ͬ�����������ƹ��쵥ͨ����������һ��(Mat���캯����create������)�����ͨ����ֻ����Ҫ��
									 //����CV_32FC1�ĳ�CV_32FC3����
	
	//2.2.7���ʶ�ͨ��Mat�е�ֵ
	for (int r = 0; r < mm.rows; r++)
	{
		for (int c = 0; c < mm.cols; c++)
		{
			cout << mm.at<Vec3f>(r, c) << ",";  //������ͨ��һ����ͨ��at���ʶ�ͨ��ͼ��ֻ����Ԫ�ش�float,int���Vec3f֮��

			//cout << mm.at<Vec3f>(Point(c, r)) << ",";  //ͬ������ʹ��Point�������Ԫ��

			/*Vec3f *ptr = (Vec3f*)(mm.data + r*mm.step[0] + c*mm.step[1]);
			cout << *ptr << ",";*/               //ͬ��ʹ���׵�ַ���step���Խ���Ԫ��ֵ����
		}                                      //step[0]:ÿһ����ռ�ֽ���
		cout << endl;						   //step[1]:ÿ��Ԫ����ռ�ֽ���(����mm.elemSize()=mm.elemSize1()*mm.channels())
											   //����mm.elemSize1():ÿ��Ԫ����ÿ����ֵ��ռ�ֽ���
	}
	cout << "========qq" << endl;
	for (int r = 0; r < mm.rows; r++)
	{
		Vec3f *ptr = mm.ptr<Vec3f>(r);   //�͵�ͨ����const int * ptr = m5.ptr<int>(r)һ���õ�ָ���е��׵�ַ
		for (int c = 0; c < mm.cols; c++)//ͬ��Ҫע�⽫��ͨ����intԪ�ر��Vex3f֮���Ԫ��
		{
			cout << ptr[c] << "," ;
		}
		cout << endl;
	}
	cout << "========" << endl;

	if (mm.isContinuous())
	{
		Vec3f *ptr = mm.ptr<Vec3f>(0);        //�͵�ͨ��һ��������ж�ͼ����������(��ÿһ��֮��û�м��)
		//Vec3f *ptr = (Vec3f *)mm.data;         //�͵�ͨ��һ����������ֵָ������ǿתΪ��Ԫ��ָ�����ͼ�����ͬ����Ч��

		for (int n = 0; n < mm.total(); n++)  //��ô����ͨ����Ԫ�صĵ�ַ����һ��ѭ���������е�Ԫ��
			cout << ptr[n] << endl;
	}

	cout << "========" << endl;

	//����ͨ��
	//����ͼ���ǽ��������ͼ�����������Ԫ�صĵ�һ��ֵ��ɵ�һͨ�����ڶ���ֵ��ɵڶ���ͨ��
	//�����õ�n����ԭͼ��С��ͬ�ĵ�ͨ������n����ԭͼ���ͨ������
	//vector<Mat> planes_split;    //���ʹ��������ʽ����Ҫ��C++��vector��ŷ����Ķ����ͨ������
	Mat planes_split[3];
	split(mm, planes_split);

	for (int r = 0; r < planes_split[0].rows; r++)
	{
		for (int c = 0; c < planes_split[0].cols; c++)
		{
			cout << planes_split[0].at<float>(r, c) << ",";
		}
		cout << endl;
	}
	cout << "=========" << endl;

	//�ϲ�ͨ��
	//�������������������ͨ����ͬλ�õ�ֵ���һ����������Ϊ�ϲ���ͼ���һ������Ԫ��
	Mat planes0 = (Mat_<int>(2,3) << 1,2,3,4,5,6);
	Mat planes1 = (Mat_<int>(2, 3) << 11, 12, 13, 14, 15, 16);
	Mat planes2 = (Mat_<int>(2, 3) << 21, 22, 23, 24, 25, 26);
	Mat planes[] = { planes0 ,planes1,planes2 };
	Mat mat;
	merge(planes, 3, mat);

	for (int r = 0; r < mat.rows; r++)
	{
		Vec3i *ptr = mat.ptr<Vec3i>(r);
		for (int c = 0; c < mat.cols; c++)
		{
			cout << ptr[c] << ",";
		}
		cout << endl;
	}
	cout << "========" << endl;


	//2.2.8���Mat��ĳһ�����ֵ
	//mm����ͨ��Vec3f����
	Mat mc = mm.col(2);     //���ĳһ��Ԫ��->>��������1
	Mat mr = mm.row(1);     //���ĳһ��Ԫ��->>��������1,ע�ⷵ�صĵ��к͵��У���Ȼ��һ��Mat����
	
	for (int r = 0; r < mc.rows; r++)
	{
		cout << mc.at<Vec3f>(r,0)<<"-";   //��������һ��СBug,ԭ��mc��ȡ��mm��Ԫ����Vec3f������Ͱ��ո��е�at<Vec3f>���з�����û�����
		//cout << mc.at<float>(r, 0) << "-"; //�����������at<float>(r, 0)���򷵻�ÿһ�еĵ�һ��ֵ(���ǵ�һ��Ԫ��)
	}                                    
	cout << endl << "========" << endl;

	for (int c = 0; c < mr.cols; c++)
	{
		cout << mr.at<Vec3f>(0, c) << "-";  //��ȷ����
		//cout << mr.at<float>(0, c) << "-";  //ͬ������������ʣ����Ƿ���ֵ����ʾ���ʵ�һ�еĵ�c��ֵ
	}

	cout << endl<<"=======" << endl;
	Mat matrix = (Mat_<int>(5, 5) << 1,  2,  3,  4,  5,
									 6,  7,  8,  9, 10,
									11, 12, 13, 14, 15,
									16, 17, 18, 19, 20,
									21, 22, 23, 24, 25);

	//Mat r_range = matrix.rowRange(Range(2, 4));  //Range(2, 4)������ҿ����䣬�����ص�2�к͵�3�е�Ԫ��
	Mat r_range = matrix.rowRange(2, 4);           //rowRange����һ�����غ�������ֱ�Ӵ���������������˵�
												   //ͬ����ѡȡ����������У�����ʹ��colRange����
	for (int r = 0; r < r_range.rows; r++)
	{
		for (int c = 0; c < r_range.cols; c++)
		{
			cout << r_range.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << endl << "=======" << endl;

	r_range.at<int>(0, 0) = 10000;            //��Ҫע����ǣ�֮ǰ��row,col,rowRange,colRange�������صľ�����ָ��ԭ����ģ������ڷ��صľ������޸�
	cout << matrix.at<int>(2, 0) << endl;     //Ҳ��Ӱ�쵽ԭͼ�����

	matrix.at<int>(2, 0) = 9999;              //�ı�ԭ����Ҳ��Ӱ��rowRange�ȷ��صľ��󣬼�������ָ��ͬһ���ڴ�ռ䣬���߻���Ӱ��
	cout << r_range.at<int>(0, 0) << endl;

	r_range.at<int>(0, 0) = 11;               //�Ļ�ԭ����Ԫ��ֵ

	//r_range = matrix.rowRange(2, 4).clone();  //����Mat���clone()�������Կ�¡�ͱ���ԭͼ����������ڱ��ݵľ�����������ô�޸�
	Mat r_range_copy;						  //������Ӱ��ԭͼ�����
	matrix.rowRange(2, 4).copyTo(r_range_copy);  //�������ֱ��copyTo(r_range),���ǻ�Ӱ�쵽ԭͼ����󣬲�֪��Ϊʲô����
	r_range_copy.at<int>(0, 0) = 66666;
	cout << matrix.at<int>(2, 0) << endl;     

	//ʹ�þ����ȡͼ������������������
	cout << "=======roi1" << endl;
	Mat roi1 = matrix(Rect(Point(0, 1), Point(4, 3)));   //���϶������굽���¶�������
	for (int r = 0; r < roi1.rows; r++)                  //ע�⣺֮ǰ�ᵽ����Point�����������α�ʾ�кź��к�
	{													 //��Σ���Ҫע����Ƿ��صľ���ͼ�񲻰����ڶ���������ʾ�ĵ����ڵ��к���
		for (int c = 0; c < roi1.cols; c++)              //�൱�ڻ�������ҿ�����
		{
			cout << roi1.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << roi1.rows << ".." << roi1.cols << endl;     //��Ϊ������ҿ����䣬���Է��صľ�����2��4��

	cout << "=======roi2" << endl;
	Mat roi2 = matrix(Rect(0,1,4,2));         //��������(�кţ��к�)�Ϳ�(����)����(����)
	for (int r = 0; r < roi2.rows; r++)
	{
		for (int c = 0; c < roi2.cols; c++)
		{
			cout << roi2.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << "=======roi3" << endl;
	Mat roi3 = matrix(Rect(Point(0, 1),Size(4,2)));  //���������Size��ʾ�Ŀ��(֮ǰ�ᵽ����Size�����������α�ʾ���)
	for (int r = 0; r < roi3.rows; r++)
	{
		for (int c = 0; c < roi3.cols; c++)
		{
			cout << roi3.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	//��row,col,rowRange,colRangeһ����ʹ�þ���Rect��ȡ�ľ���ͬ����ָ��ԭ����ģ��ı���������һ������Ӱ����һ������
	//ͬ���ģ��뵥���ı�roi��ֵ����Ҫclone()����copyTo()
	cout << "end........" << endl;
	system("pause");
	return 0;
}