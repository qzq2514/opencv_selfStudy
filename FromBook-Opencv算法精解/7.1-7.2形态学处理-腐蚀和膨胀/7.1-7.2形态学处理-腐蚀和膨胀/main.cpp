#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int r = 1;
int MAXR = 20;
Mat binCopy;
Mat dilateImg;

void callBackFunc(int, void *)
{
	//这里创建一个垂直方向的椭圆状结构元
	Mat sDilate = getStructuringElement(MORPH_ELLIPSE, Size(1, 2 * r + 1));   //获取结构元

	dilate(binCopy, dilateImg, sDilate, Point(-1, -1));    //默认膨胀一次
	imshow("dilateImg", dilateImg);
}
int main()
{
	Mat src = imread("timg.jpg", 0);
	imshow("src", src);

	Mat binImg;
	threshold(src, binImg, 124, 255, THRESH_BINARY);
	imshow("binImg", binImg);

	//腐蚀-白色区域变小
	//原本的中值平滑是将像素的矩形领域内的中值赋值给该像素，
	//而腐蚀操作就是将领域内的最小值赋值给该像素，其次腐蚀操作的领域不仅仅是矩形，
	//还可以是圆形，十字架，形态学处理中称这种核为结构元，通过getStructuringElement获得指定形状和大小的结构元
	Mat sErode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat erodeImg;
	erode(binImg, erodeImg, sErode, Point(-1, -1), 1);//进行腐蚀操作:原图，目标图，结构元，锚点，腐蚀迭代次数
	imshow("erodeImg", erodeImg); //因为取最小值，所以图片中亮度低的区域变大



	//膨胀-白色区域变大
	//腐蚀是取结构元内最小值，膨胀恰好相反，取结构元中的最大值，所以白色区域变大
    //这里使用动态的方式显示膨胀结果-使用createTrackbar函数可以可视化的调节结构元半径大小
	binCopy = binImg.clone();
	namedWindow("dilateImg",WINDOW_AUTOSIZE);   //创建一个可视化界面
	//在同名的可视化界面中创建进度条，用于调节结构元半径,r是不断变化的当前半径，MAXR是进度条最大值
	createTrackbar("半径", "dilateImg", &r, MAXR, callBackFunc);
	callBackFunc(0, 0);  //要先执行一次callBackFunc函数，不然界面都出不来,怎么拉进度条
	
	waitKey(0);
	return 0;
}