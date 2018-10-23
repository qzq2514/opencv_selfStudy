#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat srcHist,dstHist;
Mat getEqualHistImage(Mat src) {
	Mat dstImage;
	CV_Assert(src.type() == CV_8UC1);  //直方图均衡化必须使用CV_8UC1类型的图片

	//配置opencv自带的计算灰度直方图函数calcHist的参数
	int dims = 1;
	float hranges1[2] = { 0, 255 };   //统计灰度值在0-255的每个像素点个数
	const float *ranges[1] = { hranges1 };  
	int size = 256;    //直方图区间数,
	int channels[1] = { 0 };  //对第几个通道数进行直方图计算

	//calcHist函数获得直方图统计信息srcHist
	calcHist(&src, 1, channels, Mat(), srcHist, dims, &size, ranges);

	//cout << dstHist << endl;             
	//cout << dstHist.size() << endl;   //[1*256]其中256表示统计的区间数，和calcHist函数中的size对应
	//									//这里表示将0-255范围分成256个区间，统计像素值在每个区间的像素数
	//cout << dstHist.type();       //5(CV_32F)
	//cout << "CV_32SC1:" << CV_32SC1 << endl; //4

	Mat sumHist=Mat::zeros(Size(size,1),CV_32SC1);   //累加直方图-存放累加的像素值个数
	sumHist.at<int>(0) = srcHist.at<int>(0);    //dstHist是CV_32F,sumHist是CV_32SC1,都是32bit
	//sumHist.at<int>(i)则表示原图中像素小于等于i的个数
	for (int i = 1; i < size; i++) sumHist.at<int>(i) = sumHist.at<int>(i - 1) + srcHist.at<float>(i);
	//cout << sumHist << endl;

	//找到原输入图像灰度值p到输出图像灰度值q的映射
	Mat outPut_q = Mat::zeros(Size(256, 1), CV_8UC1);
	float cofficient = 256.0 / (src.total());
	for (int p = 0; p < 256; p++)
	{
		float q = cofficient*sumHist.at<int>(p) - 1;   //计算原灰度值p到均衡化后图像灰度值q的映射
		if (q >= 0) outPut_q.at<uchar>(p) = uchar(floor(q));//形成映射
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
//传入直方图统计信息Hist,[1*256]
Mat getGrayHist(Mat Hist)
{
	int scale = 2;    //最后画的柱子的间隔
	Mat HistPlate(256, 256 * scale, CV_8U, Scalar(0)); //用来画出灰度直方图(Scalar(0)表示黑色背景图)
	
    //获取直方图中像素值个数的最大值和最小值  
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(Hist, &minValue, &maxValue, 0, 0); //找到直方图中灰度的最大值和最小值 

													//绘制出直方图  
	int hpt = saturate_cast<int>(0.9 * 256);//防止溢出，当运算完之后，结果小于最小值，则转为最小值，结果超出最大值，则返回最大值
												 //eg.saturate_cast<uchar>则是参数为负数，返回0,超出255返回255
	for (int i = 0; i < 256; i++)
	{
		float binValue = Hist.at<float>(i);     //灰度为i的像素数,
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);  //防止溢出
																		//根据像素数画出柱子，注意图像的坐标中纵坐标是向下增大的
																		//这里一个柱子就是一条直线
		line(HistPlate, Point(i*scale, 256 - 1), Point(i*2, 256 - realValue), Scalar(255));
	}
	return HistPlate;
}
int main()
{
	//4.4直方图均衡化
	//4.3的直方图正规化只是将图像的整体像素同时进行线性变换，其像素内的分布还是不变的，
	//但是直方图均衡化，会改图像内的像素分布，使得像素较为均匀的分布在整个像素范围之内
	//具体公式详见"直方图均衡化理论1.jpg"和"直方图均衡化理论2.jpg"
	Mat src = imread("lena_full.jpg",0);

	Mat dstImage= getEqualHistImage(src);

	imshow("原图", src);
	Mat srcHistPlate = getGrayHist(srcHist);
	imshow("原图直方图", srcHistPlate);


	imshow("均衡化后的图片", dstImage);
	Mat dstHistPlate = getGrayHist(dstHist);
	imshow("均衡化后直方图", dstHistPlate);
	
	//4.5限制对比度的自适应直方图均衡化
	//自适应直方图均衡化将图像分为不重叠区域，然后分别进行直方图均衡化
	//更进一步，为了防止每一个分割的区域直方图均衡化被其中的噪声影响，限制对比度，超过限制对比度的，
	//就将超过的部分均匀分不到其他直方图柱子上
	Ptr<CLAHE> clahe = createCLAHE(40, Size(8, 8));   //默认限制对比度为40,分割的每一区域大小为8*8
	Mat claheDst;
	clahe->apply(src, claheDst);
	imshow("限制对比度的自适应直方图均衡化", claheDst);

	waitKey(0);
	return 0;
}