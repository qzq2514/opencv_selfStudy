#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

bool MultiChannelBlending();
int main()
{
	//system("color5E");
	MultiChannelBlending();
	waitKey(0);
	return 0;
}

bool MultiChannelBlending()
{
	Mat srcImage;
	Mat logoImage;
	vector<Mat>channels;
	Mat imageBlueChannel;

	logoImage = imread("logo.jpg",0);
	srcImage = imread("scene.jpg");
	if (!logoImage.data) { printf("读取logo.jpg错误~"); return false; }
	if (!srcImage.data) { printf("读取scene.jpg错误~"); return false; }

    //将原图的三通道彩色图片转换成三个单通道图片，放在一个vector中
	//vector中每个元素是原图的一个通道
	split(srcImage, channels);

	//获得原图的第0个通道-蓝色通道
	imageBlueChannel = channels.at(0);    //vector的at方法和下标索引符号[]一样的作用，只是at更加安全，但效率低
	                                       //注意因为这里vector中元素是Mat,所以是这里的at赋值是引用，即修改
										//imageBlueChannel或者channels.at(0)是等价的

	Mat blueROI = imageBlueChannel(Rect(20, 30, logoImage.cols, logoImage.rows));
	addWeighted(blueROI, 1.0,logoImage, 0.5,0, blueROI);  //注意之前说的addWeighted中两张图片，不仅要长宽一样，也要通道数一样
													//所以logoImage在度入市要是单通道图,即imread第二个参数是0

	//channels中的蓝色通道被加上了logo图，然后与其他的绿色(.at(1)),红条(.at(2))通道合并起来
	merge(channels, srcImage);

	imshow("蓝色通道混合", srcImage);
	return true;




}
