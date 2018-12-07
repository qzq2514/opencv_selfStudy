#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\ml\ml.hpp>


#include<iostream>
using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
	//42����������
	float trainingData[42][2] = { { 40,55 },{ 35,35 },{ 55,15 },{ 45,25 },{ 10,10 },
	{ 15,15 },{ 40,10 },
	{ 35,15 },{ 30,50 },{ 100,20 },{ 45,65 },{ 20,35 },
	{ 80,20 },{ 90,5 } ,
	{ 95,35 },{ 80,65 },{ 15,55 },{ 25,65 },{ 85,35 },
	{ 85,55 },{ 95,70 },
	{ 105,50 },{ 115,65 },{ 110,25 },{ 120,45 },{ 15,45 },
	{ 55,30 },{ 60,65 },{ 95,60 },{ 25,40 },{ 75,45 },
	{ 105,35 },{ 65,10 },
	{ 50,50 },{ 40,35 },{ 70,55 },{ 80,30 },{ 95,45 },
	{ 60,20 },{ 70,30 },
	{ 65,45 },{ 85,40 } };
	//42��2�У���trainingData�����,��trainingDataMat��trainingDataָ��ͳһ�ڴ�ռ䣬
	//�ı���һ��Ӱ����һ��
	Mat trainingDataMat(42, 2, CV_32FC1, trainingData);


	//���ݱ�ǩ-���ʹ���������͵ģ���Ҫʹ��float,ͬ����int����󶨵�Mat��
	int labels[42] = { 'R','R','R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
		'R','R','R','R','R','R','R','R','R','R','R','R','R',
		'B','B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		'B','B','B','B','B' };
	Mat labelsMat = Mat(42, 1, CV_32S, labels);



	//AdaBoos�㷨�ַ�ΪDiscrete AdaBoost��Real AdaBoost��LogitBoost AdaBoost��Gentle AdaBoost
	//����Discrete AdaBoostÿ����������Ԥ��������ɢ��{-1,1}Ȼ��ͨ�����Ե�Ȩ�ؼ�Ȩ��͵õ�ǿ��������Ԥ����(����),��ͨ��sign�õ�ǿ����������ɢԤ����
	//��Real AdaBoostÿ����������Ԥ����������[0,1]Ȼ��ֱ�ӽ�ÿ������������������Ԥ������ӣ���sign�õ���ɢ������
	Ptr<Boost> boost = Boost::create();
	boost->setBoostType(Boost::DISCRETE);     //ʹ��Real AdaBoost(REAL)����Discrete AdaBoost(DISCRETE)
	boost->setWeakCount(200);             //���������ĸ���
	boost->setWeightTrimRate(0.95);      //�ü���:����Ȩֵ�Ӵ�С����Ȩֵ�Ӹߵ��Ͳ����ۼӣ�С�ڸ���ֵ�ĺ�������������ò�������һ������������ѵ��
										 //������Ȩ��Ϊ1���������ֵ��������̫С����Ȼ̫�����������õ��º���ѵ����������
	boost->setMaxDepth(10);        //ÿ��������������һ��������������涨ÿ����������������
	boost->setUseSurrogates(false);
	boost->setPriors(Mat());
	//ѵ�����ݣ�ROW_SAMPLE�涨��Ϊ����
	Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	//��ʼѵ��
	boost->train(tData);


	//Ԥ��:ͬ����Ҫ�����ݷ�װ��Mat
	float myData[2] = { 50,45 };
	Mat myDataMat(2, 1, CV_32FC1, myData);
	double r = boost->predict(myDataMat);
	cout << "Ԥ����:" << char(r) << endl;


	//ѵ����Ԥ��������ʾ
	Mat canv = Mat::zeros(Size(130, 100), CV_8UC3);

	//�����ֽ���
	Vec3b blue(205, 182, 141), red(184, 169, 238);
	for (int r = 0; r < canv.rows; r++)
	{
		for (int c = 0; c < canv.cols; c++)
		{
			//֮ǰROW_SAMPLE�涨��Ϊ����������������(1,2)����(2,1)
			//��������Ҫע�⣬��Ϊ֮ǰ��������(�����꣬���������ʽ)
			//�������ｫ������ÿ�����Ϊ����Ӧ����(c,r)��������(r,c)
			//c�����кţ���x��r�����кţ���y
			Mat sampleMat = (Mat_<float>(1, 2) << c, r);  
			double pred = boost->predict(sampleMat);
			if (pred == 'R')
				canv.at<Vec3b>(r, c) = red;
			else
				canv.at<Vec3b>(r, c) = blue;
		}
	}


	for (int r = 0; r < canv.rows; r++)
	{
		if (labels[r] == 'R')
			circle(canv, Point(trainingData[r][0], trainingData[r][1]), 1, Scalar(0, 0, 255), 2);
		else if (labels[r] == 'B')
			circle(canv, Point(trainingData[r][0], trainingData[r][1]), 1, Scalar(255, 0, 0), 2);

	}
	circle(canv, Point(myData[0], myData[1]), 1, Scalar(255, 255, 255), 2);
	imshow("canv", canv);
	waitKey(0);

	return 0;
}