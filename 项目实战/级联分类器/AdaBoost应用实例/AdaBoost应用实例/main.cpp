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
	//42个样本数据
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
	//42行2列，与trainingData矩阵绑定,即trainingDataMat和trainingData指向统一内存空间，
	//改变其一会影响另一个
	Mat trainingDataMat(42, 2, CV_32FC1, trainingData);


	//数据标签-最好使用整型类型的，不要使用float,同样把int数组绑定到Mat中
	int labels[42] = { 'R','R','R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
		'R','R','R','R','R','R','R','R','R','R','R','R','R',
		'B','B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		'B','B','B','B','B' };
	Mat labelsMat = Mat(42, 1, CV_32S, labels);



	//AdaBoos算法又分为Discrete AdaBoost、Real AdaBoost、LogitBoost AdaBoost、Gentle AdaBoost
	//其中Discrete AdaBoost每个弱分类器预测结果是离散的{-1,1}然后通过各自的权重加权求和得到强分类器的预测结果(连续),再通过sign得到强分类器的离散预测结果
	//而Real AdaBoost每个弱分类器预测结果是连续[0,1]然后直接将每个弱分类器的连续型预测结果相加，再sign得到离散分类结果
	Ptr<Boost> boost = Boost::create();
	boost->setBoostType(Boost::DISCRETE);     //使用Real AdaBoost(REAL)或者Discrete AdaBoost(DISCRETE)
	boost->setWeakCount(200);             //弱分类器的个数
	boost->setWeightTrimRate(0.95);      //裁剪率:样本权值从大到小排序，权值从高到低不断累加，小于该阈值的后面的样本被剪裁不用于下一个弱分类器的训练
										 //样本总权重为1，这里的阈值不能设置太小，不然太多样本被剪裁导致后面训练样本不足
	boost->setMaxDepth(10);        //每个弱分类器的是一个决策树，这里规定每个决策树的最大深度
	boost->setUseSurrogates(false);
	boost->setPriors(Mat());
	//训练数据，ROW_SAMPLE规定行为样本
	Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	//开始训练
	boost->train(tData);


	//预测:同样，要将数据封装进Mat
	float myData[2] = { 50,45 };
	Mat myDataMat(2, 1, CV_32FC1, myData);
	double r = boost->predict(myDataMat);
	cout << "预测结果:" << char(r) << endl;


	//训练和预测数据显示
	Mat canv = Mat::zeros(Size(130, 100), CV_8UC3);

	//画出分界面
	Vec3b blue(205, 182, 141), red(184, 169, 238);
	for (int r = 0; r < canv.rows; r++)
	{
		for (int c = 0; c < canv.cols; c++)
		{
			//之前ROW_SAMPLE规定行为样本，所以这里是(1,2)不是(2,1)
			//但是这里要注意，因为之前样本都是(横坐标，纵坐标的形式)
			//所以这里将界面中每个点变为样本应该是(c,r)，而不是(r,c)
			//c代表列号，即x，r代表行号，即y
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