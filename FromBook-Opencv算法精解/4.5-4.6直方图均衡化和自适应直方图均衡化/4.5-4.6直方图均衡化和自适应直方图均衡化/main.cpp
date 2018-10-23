#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat srcHist,dstHist;
Mat getEqualHistImage(Mat src) {
	Mat dstImage;
	CV_Assert(src.type() == CV_8UC1);  //ֱ��ͼ���⻯����ʹ��CV_8UC1���͵�ͼƬ

	//����opencv�Դ��ļ���Ҷ�ֱ��ͼ����calcHist�Ĳ���
	int dims = 1;
	float hranges1[2] = { 0, 255 };   //ͳ�ƻҶ�ֵ��0-255��ÿ�����ص����
	const float *ranges[1] = { hranges1 };  
	int size = 256;    //ֱ��ͼ������,
	int channels[1] = { 0 };  //�Եڼ���ͨ��������ֱ��ͼ����

	//calcHist�������ֱ��ͼͳ����ϢsrcHist
	calcHist(&src, 1, channels, Mat(), srcHist, dims, &size, ranges);

	//cout << dstHist << endl;             
	//cout << dstHist.size() << endl;   //[1*256]����256��ʾͳ�Ƶ�����������calcHist�����е�size��Ӧ
	//									//�����ʾ��0-255��Χ�ֳ�256�����䣬ͳ������ֵ��ÿ�������������
	//cout << dstHist.type();       //5(CV_32F)
	//cout << "CV_32SC1:" << CV_32SC1 << endl; //4

	Mat sumHist=Mat::zeros(Size(size,1),CV_32SC1);   //�ۼ�ֱ��ͼ-����ۼӵ�����ֵ����
	sumHist.at<int>(0) = srcHist.at<int>(0);    //dstHist��CV_32F,sumHist��CV_32SC1,����32bit
	//sumHist.at<int>(i)���ʾԭͼ������С�ڵ���i�ĸ���
	for (int i = 1; i < size; i++) sumHist.at<int>(i) = sumHist.at<int>(i - 1) + srcHist.at<float>(i);
	//cout << sumHist << endl;

	//�ҵ�ԭ����ͼ��Ҷ�ֵp�����ͼ��Ҷ�ֵq��ӳ��
	Mat outPut_q = Mat::zeros(Size(256, 1), CV_8UC1);
	float cofficient = 256.0 / (src.total());
	for (int p = 0; p < 256; p++)
	{
		float q = cofficient*sumHist.at<int>(p) - 1;   //����ԭ�Ҷ�ֵp�����⻯��ͼ��Ҷ�ֵq��ӳ��
		if (q >= 0) outPut_q.at<uchar>(p) = uchar(floor(q));//�γ�ӳ��
		else outPut_q.at<uchar>(p) = 0;
	}

	dstImage = Mat::zeros(src.size(), CV_8UC1);
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			int p = src.at<uchar>(r, c);
			dstImage.at<uchar>(r, c) = outPut_q.at<uchar>(p);
		}
	}

	calcHist(&dstImage, 1, channels, Mat(), dstHist, dims, &size, ranges);
	return dstImage;
}
//����ֱ��ͼͳ����ϢHist,[1*256]
Mat getGrayHist(Mat Hist)
{
	int scale = 2;    //��󻭵����ӵļ��
	Mat HistPlate(256, 256 * scale, CV_8U, Scalar(0)); //���������Ҷ�ֱ��ͼ(Scalar(0)��ʾ��ɫ����ͼ)
	
    //��ȡֱ��ͼ������ֵ���������ֵ����Сֵ  
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(Hist, &minValue, &maxValue, 0, 0); //�ҵ�ֱ��ͼ�лҶȵ����ֵ����Сֵ 

													//���Ƴ�ֱ��ͼ  
	int hpt = saturate_cast<int>(0.9 * 256);//��ֹ�������������֮�󣬽��С����Сֵ����תΪ��Сֵ������������ֵ���򷵻����ֵ
												 //eg.saturate_cast<uchar>���ǲ���Ϊ����������0,����255����255
	for (int i = 0; i < 256; i++)
	{
		float binValue = Hist.at<float>(i);     //�Ҷ�Ϊi��������,
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);  //��ֹ���
																		//�����������������ӣ�ע��ͼ��������������������������
																		//����һ�����Ӿ���һ��ֱ��
		line(HistPlate, Point(i*scale, 256 - 1), Point(i*2, 256 - realValue), Scalar(255));
	}
	return HistPlate;
}
int main()
{
	//4.4ֱ��ͼ���⻯
	//4.3��ֱ��ͼ���滯ֻ�ǽ�ͼ�����������ͬʱ�������Ա任���������ڵķֲ����ǲ���ģ�
	//����ֱ��ͼ���⻯�����ͼ���ڵ����طֲ���ʹ�����ؽ�Ϊ���ȵķֲ����������ط�Χ֮��
	//���幫ʽ���"ֱ��ͼ���⻯����1.jpg"��"ֱ��ͼ���⻯����2.jpg"
	Mat src = imread("lena_full.jpg",0);

	Mat dstImage= getEqualHistImage(src);

	imshow("ԭͼ", src);
	Mat srcHistPlate = getGrayHist(srcHist);
	imshow("ԭͼֱ��ͼ", srcHistPlate);


	imshow("���⻯���ͼƬ", dstImage);
	Mat dstHistPlate = getGrayHist(dstHist);
	imshow("���⻯��ֱ��ͼ", dstHistPlate);
	
	//4.5���ƶԱȶȵ�����Ӧֱ��ͼ���⻯
	//����Ӧֱ��ͼ���⻯��ͼ���Ϊ���ص�����Ȼ��ֱ����ֱ��ͼ���⻯
	//����һ����Ϊ�˷�ֹÿһ���ָ������ֱ��ͼ���⻯�����е�����Ӱ�죬���ƶԱȶȣ��������ƶԱȶȵģ�
	//�ͽ������Ĳ��־��ȷֲ�������ֱ��ͼ������
	Ptr<CLAHE> clahe = createCLAHE(40, Size(8, 8));   //Ĭ�����ƶԱȶ�Ϊ40,�ָ��ÿһ�����СΪ8*8
	Mat claheDst;
	clahe->apply(src, claheDst);
	imshow("���ƶԱȶȵ�����Ӧֱ��ͼ���⻯", claheDst);

	waitKey(0);
	return 0;
}