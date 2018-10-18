#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>


using namespace std;
using namespace cv;


Mat dstHist;//得到的直方图信息
int org_size = 256;
void getHistCalwithOpencv(Mat src)
{
	int dims = 1;//得到的直方图的维数 灰度图的维数为1
	float hranges1[2] = { 0, 255 };
	const float *ranges[1] = { hranges1 };  // 这里需要为const类型，二维数组用来指出每个区间的范围  
	int size = org_size;//直方图横坐标的区间数 即横坐标被分成256份
	int channels = 0;
	//calcHist函数计算图像的直方图  
	//参数意义:
	//&src:输入计算灰度直方图的图片(可以是多张图片,即同时统计多个图片的不同灰度值的像素个数做成直方图)
	//1:输入图片的个数(如果计算多个图片共同的灰度直方图，那么这里大于1且src是一个数组)
	//channels:需要计算的图片的通道，比如输入是2副图像，第一副图像有0，1，2共三个channel，
	//第二幅图像只有0一个channel，那么输入就一共有4个channes(先后分别为第一幅图像的三个通道和第二幅图像的第0个通道)，
	//eg.上述如果int channels[3] = {3, 2, 0}，那么就表示是使用第二副图像的第一个通道和第一副图像的第2和第0个通道来计算直方图
	//Mat():掩码，必须是一个8位（CV_8U）的数组，并且它的大小的和arrays[i]的大小相同，且只把mask值为1的点对应的灰度图的点用来计算直方图。
	//dstHist:计算的灰度直方图
	//dims:灰度图的维数
	//size:在每一维上直方图的个数。简单把直方图看作一个一个的竖条的话，就是每一维上竖条的个数:
	//ranges:用来进行统计的范围。即每一维灰度的上下界。
	//比如 float rang1[] = {0, 20};float rang2[] = {30, 40};  
	//const float*rangs[] = {rang1, rang2};那么就是对0，20和30，40范围的值进行统计
	calcHist(&src, 1, &channels, Mat(), dstHist, dims, &size, ranges);

	//cout << "dstHist.size():"<<dstHist.size()<<endl;   //[1*256]
	//cout << dstHist << endl;
}
void myCalHistfromOnePic(Mat src)
{
	//opencv自带的calcHist得到的dstHist数据元素是float,为了后面统一，这里我们也是用32float类型
	dstHist = Mat::zeros(Size(org_size,1), CV_32FC1);//[1*256]大小,写成Size(1,org_size)也行，
													 //但是为了和opencv中自带的行数calcHist返回的dstHist统一，我们也写成单行的

	int rows = src.rows;
	int cols = src.cols;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int ind = int(src.at<uchar>(r, c));
			dstHist.at<int>(0, ind)++;
			//dstHist.at<int>(ind)++;   //单行或者单列的Mat可以直接像这样访问元素
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

	imshow("原灰度图",src);
	     

	//4.1灰度直方图
	getHistCalwithOpencv(src);  //通过opencv自带的统计灰度值信息的函数calcHist获得灰度信息
	//myCalHistfromOnePic(src); //自写的简单统计单通道图像的灰度


	int scale = 2; //每scale个画一个直方柱
	//Mat(行数，列数)
	Mat dstImage(org_size, org_size* scale, CV_8U, Scalar(0)); //用来画出灰度直方图(Scalar(0)表示黑色背景图)
	//获取最大值和最小值  
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0); //找到直方图中灰度的最大值和最小值 

    //绘制出直方图  
	int hpt = saturate_cast<int>(0.9 * org_size);//防止溢出，当运算完之后，结果小于最小值，则转为最小值，结果超出最大值，则返回最大值
	        //eg.saturate_cast<uchar>则是参数为负数，返回0,超出255返回255
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);     //灰度为i的像素数,
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);  //防止溢出
		//根据像素数画出柱子，注意图像的坐标中纵坐标是向下增大的
		//这里一个柱子就是一条直线
		line(dstImage, Point(i*scale, org_size - 1), Point(i*scale, org_size - realValue), Scalar(255));
	}
	imshow("一维直方图", dstImage);


	//4.2线性变换
	Mat I = (Mat_<uchar>(2, 2) << 0, 200, 23, 4);

	//方法1.
	Mat O1;
	I.convertTo(O1, CV_8UC1, 2.0, 5); 
	//执行的变换为:O1(r,c)=saturate_cast<uchar>(2.0*I(r,c)+5)
	//其中系数2.0控制输出图像对比度，5控制输出图像的亮度,
	cout << O1 << endl;
	cout << "--------------" << endl;

	Mat O2=2.0*I+ 5;     //直接将图像进行乘法，这里返回的图像和原图像数据类型，这里是CV_8UC1
	cout << O2 << endl;  //所以如果返回的某个像素值大于255,则返回255，和第一种方法一致(但是第一种方法是可以指定输出图像的数据元素类型)
	cout << "--------------" << endl;

	Mat O3;
	convertScaleAbs(I, O3, 2.5, 5);
	cout << O3 << endl;
	cout << "--------------" << endl;

	//除了对整体的输入图像进行相同参数和偏置的线性变换以外，还可以针对不同的灰度区域选择不同的参数和偏置
	//这样可以更大程度的加大图像的对比度
	waitKey(0);
	return 0;
}