#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat imgColor = imread("lena_full.jpg", IMREAD_COLOR);
	//cout << IMREAD_GRAYSCALE << endl;    //0 将原图作为灰度图读取
	//cout << IMREAD_COLOR << endl;        //1 作为彩图读取
	

	//RGB彩色图片的每个元素是一个长度为3的向量，例如Vec3b
	if (imgColor.empty())
	{
		cout << "Cant't fing the pic..." << endl;
		system("pause");
		return -1;
	}
	//cout << "imgColor.channels():" <<  imgColor.channels() << endl;   //三通道RGB彩图
	//cout << imgColor.rows << ":" << imgColor.cols << endl;
	//cout << "type:"<<imgColor.type() << endl;    //返回整数值，每一值对应一个Mat元素类型,这里即可根据对应值得到Mat元素的类型
	//cout << "CV_8UC3:" << CV_8UC3 << endl;     //CV_8UC3=8
	//cout << "imgColor.at<Vec3b>(r, c):" << imgColor.at<Vec3b>(2, 3) << endl;
	imshow("BGR", imgColor);


	/*vector<Mat> planes;
	planes.resize(3);*/   //release版本下可以直接split,但是debug版本下，直接split会数组越界,除非手动指定vector的长度
	Mat planes[3];    //或者可以使用 Mat planes[3];
	split(imgColor, planes);  
	
	//所以综上:
	//1.Release下使用vector<Mat>+resize或者 Mat planes[3]
	//2.Debug下只能使用Mat planes[3]

	//要注意每一个像素的向量中的元素值并不是按照R,G,B的顺序排列的
	//而是按照B,G,R的顺序，所以下面依次得到蓝，绿，红色通道的单通道图像
	imshow("channal0", planes[0]);  
	imshow("channal1", planes[1]);
	imshow("channal2", planes[2]);
	cout << planes[2].channels() << endl;

	//自己的小练习，将彩色图变为灰度图
	Mat myGray;
	myGray.create(imgColor.rows, imgColor.cols, CV_8UC1);   //灰度图肯定是单通道的
    //这里要选择合适的图像类型，对于最后的灰度图，可以选择CV_8UC1
	//而要获得原来图像的像素值，要通过type()函数得到彩色图片的类型，例如这里imgColor.type()=8
	//通过查表得到是CV_8UC3类型，即Vec3b
	for(int r=0;r<myGray.rows;r++)
		for (int c = 0; c < myGray.cols; c++)
		{
			//[0],[1],[2]依次得到指定行列上的蓝，绿，红通道的值
			 uchar pixel = 0.2*imgColor.at<Vec3b>(r, c)[0] +
							0.2*imgColor.at<Vec3b>(r, c)[1] +
							0.5*imgColor.at<Vec3b>(r, c)[2];
			myGray.at<uchar>(r, c) = pixel;
		}
	imshow("myGray", myGray);
	waitKey();      //不要用system(""pause);
	return 0;
}