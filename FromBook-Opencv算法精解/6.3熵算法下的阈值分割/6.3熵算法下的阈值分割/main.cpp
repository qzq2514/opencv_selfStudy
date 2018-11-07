#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

Mat calGrayHist(Mat src)
{
	Mat hist = Mat::zeros(Size(256, 1), CV_32SC1);

	int rows = src.rows;
	int cols = src.cols;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int pixelVal = (int)src.at<uchar>(r, c);
			hist.at<int>(pixelVal) += 1;
		}
	}
	return hist;
}
int entropyThreshold(Mat img)
{
	assert(img.type() == CV_8UC1);

	int rows = img.rows;
	int cols = img.cols;

	Mat garyHist = calGrayHist(img);

	//这里要将garyHist类型变为CV_32FC1浮点型，然后下面归一化的时候，保证结果为小数
	//因为下面归一化直接除法，返回的Mat和原图类型一致
	garyHist.convertTo(garyHist, CV_32FC1);

	Mat normalHist = garyHist / (float)img.total();   //灰度个数归一化，得到表示每个灰度值概率的归一化灰度直方图

													  //第一步:计算累加概率直方图
	Mat cumuHist = Mat::zeros(1, 256, CV_32FC1);
	cumuHist.at<float>(0) = normalHist.at<float>(0);
	for (int k = 1; k < 256; k++)
		cumuHist.at<float>(k) = cumuHist.at<float>(k - 1) + normalHist.at<float>(k);

	//cout << cumuHist << endl;

	//第二步:计算各个灰度级的熵(即灰度值小于t的所有灰度值的熵之和)
	Mat entropy = Mat::zeros(1, 256, CV_32FC1);

	//求熵之和，单独计算第一个值，要考虑熵为0情况，因为求熵时有一个对数函数
	//if (fabs(normalHist.at<float>(0) - 0) < 1e-10)   //浮点数比较是不是等于0，最好用这种方法
	if (normalHist.at<float>(0) == 0)
		entropy.at<float>(0) = 0;
	else
		entropy.at<float>(0) = -normalHist.at<float>(0)*log10f(normalHist.at<float>(0));

	for (int k = 1; k < 256; k++)
	{
		if (normalHist.at<float>(k) == 0)
			entropy.at<float>(k) = entropy.at<float>(k - 1);    //当前灰度熵为0
		else
			entropy.at<float>(k) = entropy.at<float>(k - 1) - normalHist.at<float>(k)*log10f(normalHist.at<float>(k));
	}
	//cout << entropy << endl;

	//第三步:找阈值
	double ft1 = 0, ft2 = 0;
	double totalEntropy = entropy.at<float>(255);

	Mat FT = Mat::zeros(1, 256, CV_64FC1);
	for (int k = 0; k < 256; k++)
	{
		double t1, t2;
		double cumuHist_k = cumuHist.at<float>(k);
		double entropy_k = entropy.at<float>(k);

		//书上有错误，并不是找到cumuHist中指定范围的最大值，而是normalHist概率直方图中的最大值
		//具体详见"熵算法阈值分割算法.png"
		minMaxLoc(normalHist.colRange(0, k + 1), NULL, &t1, NULL, NULL);
		minMaxLoc(normalHist.colRange(k + 1, 256), NULL, &t2, NULL, NULL);
		//cout << t1 << "-" << t2 << endl;

		//排除一系列使得等式不能计算的条件
		if (t1 == 0 || t1 == 1 || cumuHist_k == 0 || cumuHist_k == 1 || totalEntropy == 0 || cumuHist_k == 1)
			ft1 = 0;
		else
			ft1 = entropy_k / totalEntropy*(log10f(cumuHist_k) / log10f(t1));

		if (t2 == 0 || t2 == 1 || cumuHist_k == 1 || cumuHist_k == 0)
			ft2 = 0;
		else
		{
			if (totalEntropy == 0)
				ft2 = log10(1 - cumuHist_k) / log10f(t2);
			else
				ft2 = (1 - entropy_k / totalEntropy)*(log10f(1 - cumuHist_k) / log10f(t2));
		}
		//cout << ft1 << "-" << ft2 << endl;
		FT.at<double>(k) = ft1 + ft2;
	}
	//cout << FT << endl;
	Point threshPoint;
	minMaxLoc(FT, NULL, NULL, NULL, &threshPoint);

	int thresh = threshPoint.x;
	return thresh;
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	int thresh = entropyThreshold(src);
	cout << "thresh:" << thresh << endl;

	Mat dst;   //找到阈值后，就使用自带的threshold阈值分割函数即可
	threshold(src, dst, thresh, 255, THRESH_BINARY);

	imshow("dst", dst);
	waitKey(0);
	return 0;
}