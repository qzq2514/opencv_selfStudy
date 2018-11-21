#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>


using namespace std;
using namespace cv;


Mat dstHist;//�õ���ֱ��ͼ��Ϣ
int org_size = 256;
void getHistCalwithOpencv(Mat src)
{
	int dims = 1;//�õ���ֱ��ͼ��ά�� �Ҷ�ͼ��ά��Ϊ1
	float hranges1[2] = { 0, 255 };
	const float *ranges[1] = { hranges1 };  // ������ҪΪconst���ͣ���ά��������ָ��ÿ������ķ�Χ  
	int size = org_size;//ֱ��ͼ������������� �������걻�ֳ�256��
	int channels[1] = { 0 };
	//calcHist��������ͼ���ֱ��ͼ  
	//��������:
	//&src:�������Ҷ�ֱ��ͼ��ͼƬ(�����Ƕ���ͼƬ,��ͬʱͳ�ƶ��ͼƬ�Ĳ�ͬ�Ҷ�ֵ�����ظ�������ֱ��ͼ)
	//1:����ͼƬ�ĸ���(���������ͼƬ��ͬ�ĻҶ�ֱ��ͼ����ô�������1��src��һ������)
	//channels:��Ҫ�����ͼƬ��ͨ��������������2��ͼ�񣬵�һ��ͼ����0��1��2������channel��
	//�ڶ���ͼ��ֻ��0һ��channel����ô�����һ����4��channes(�Ⱥ�ֱ�Ϊ��һ��ͼ�������ͨ���͵ڶ���ͼ��ĵ�0��ͨ��)��
	//eg.�������int channels[3] = {3, 2, 0}����ô�ͱ�ʾ��ʹ�õڶ���ͼ��ĵ�һ��ͨ���͵�һ��ͼ��ĵ�2�͵�0��ͨ��������ֱ��ͼ
	//Mat():���룬������һ��8λ��CV_8U�������飬�������Ĵ�С�ĺ�arrays[i]�Ĵ�С��ͬ����ֻ��maskֵΪ1�ĵ��Ӧ�ĻҶ�ͼ�ĵ���������ֱ��ͼ��
	//dstHist:����ĻҶ�ֱ��ͼ
	//dims:�Ҷ�ͼ��ά��
	//size:��ÿһά��ֱ��ͼ�ĸ������򵥰�ֱ��ͼ����һ��һ���������Ļ�������ÿһά�������ĸ���:
	//ranges:��������ͳ�Ƶķ�Χ����ÿһά�Ҷȵ����½硣
	//���� float rang1[] = {0, 20};float rang2[] = {30, 40};  
	//const float*rangs[] = {rang1, rang2};��ô���Ƕ�0��20��30��40��Χ��ֵ����ͳ��
	calcHist(&src, 1, channels, Mat(), dstHist, dims, &size, ranges);

	//cout << "dstHist.size():"<<dstHist.size()<<endl;   //[1*256] ����ΪcalcHist�����е�size����ʾ���㽫0-255�Ҷ�ֵ"�ȷ�"��256�����䣬ͳ��ÿ�������������
	//cout << dstHist << endl;
}
void myCalHistfromOnePic(Mat src)
{
	//opencv�Դ���calcHist�õ���dstHist����Ԫ����float,Ϊ�˺���ͳһ����������Ҳ����32float����
	dstHist = Mat::zeros(Size(org_size,1), CV_32FC1);//[1*256]��С,д��Size(1,org_size)Ҳ�У�
													 //����Ϊ�˺�opencv���Դ�������calcHist���ص�dstHistͳһ������Ҳд�ɵ��е�

	int rows = src.rows;
	int cols = src.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int ind = int(src.at<uchar>(r, c));
			dstHist.at<int>(0, ind)++;
			//dstHist.at<int>(ind)++;   //���л��ߵ��е�Mat����ֱ������������Ԫ��
		}    
	}
}
int main()
{
	Mat src = imread("lena_full.jpg",0);
	if (src.empty())
	{
		cout << "Empty picture...." << endl;
		return -1;
	}

	imshow("ԭ�Ҷ�ͼ",src);
	     

	//4.1�Ҷ�ֱ��ͼ
	getHistCalwithOpencv(src);  //ͨ��opencv�Դ���ͳ�ƻҶ�ֵ��Ϣ�ĺ���calcHist��ûҶ���Ϣ
	//myCalHistfromOnePic(src); //��д�ļ�ͳ�Ƶ�ͨ��ͼ��ĻҶ�


	int scale = 2; //ÿscale����һ��ֱ����
	//Mat(����������)
	Mat dstImage(org_size, org_size* scale, CV_8U, Scalar(0)); //���������Ҷ�ֱ��ͼ(Scalar(0)��ʾ��ɫ����ͼ)
	//��ȡ���ֵ����Сֵ  
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0); //�ҵ�ֱ��ͼ�лҶȵ����ֵ����Сֵ 

    //���Ƴ�ֱ��ͼ  
	int hpt = saturate_cast<int>(0.9 * org_size);//��ֹ�������������֮�󣬽��С����Сֵ����תΪ��Сֵ������������ֵ���򷵻����ֵ
	        //eg.saturate_cast<uchar>���ǲ���Ϊ����������0,����255����255
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);     //�Ҷ�Ϊi��������,
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);  //��ֹ���
		//�����������������ӣ�ע��ͼ��������������������������
		//����һ�����Ӿ���һ��ֱ��
		line(dstImage, Point(i*scale, org_size - 1), Point(i*scale, org_size - realValue), Scalar(255));
	}
	imshow("һάֱ��ͼ", dstImage);


	//4.2���Ա任
	Mat I = (Mat_<uchar>(2, 2) << 0, 200, 23, 4);

	//����1.
	Mat O1;
	I.convertTo(O1, CV_8UC1, 2.0, 5); 
	//ִ�еı任Ϊ:O1(r,c)=saturate_cast<uchar>(2.0*I(r,c)+5)
	//����ϵ��2.0�������ͼ��Աȶȣ�5�������ͼ�������,
	cout << O1 << endl;
	cout << "--------------" << endl;

	Mat O2=2.0*I+ 5;     //ֱ�ӽ�ͼ����г˷������ﷵ�ص�ͼ���ԭͼ���������ͣ�������CV_8UC1
	cout << O2 << endl;  //����������ص�ĳ������ֵ����255,�򷵻�255���͵�һ�ַ���һ��(���ǵ�һ�ַ����ǿ���ָ�����ͼ�������Ԫ������)
	cout << "--------------" << endl;

	Mat O3;
	I.convertTo(I, CV_32FC1, 2.0, 5);
	convertScaleAbs(I, O3, 2.5, 5);     //convertScaleAbs�����Ľ��O3ΪCV_8UC1
	cout << O3 << endl;
	cout << "--------------" << endl;
	cout << I.type() << endl;
	cout << O3.type() << endl;

	//���˶����������ͼ�������ͬ������ƫ�õ����Ա任���⣬��������Բ�ͬ�ĻҶ�����ѡ��ͬ�Ĳ�����ƫ��
	//�������Ը���̶ȵļӴ�ͼ��ĶԱȶ�
	waitKey(0);
	return 0;
}