#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat scene = imread("secne.jpg");
	namedWindow("风景图");
	imshow("风景图",scene);

	Mat logo = imread("logo.jpg");
	namedWindow("logo图");
	imshow("logo图", logo);
	
	cout << scene.cols << "-" << scene.rows << endl;
	cout << "qzq2514" << endl;
	Mat sceneROI;
	//截取图片的ROI时，要保证ROI范围不会超出图片的原范围
	//sceneROI = scene(Rect(80, 35, logo.cols, logo.rows));           //第一种方法:给出ROI矩形区域的左上角横纵坐标和矩形的长高

	sceneROI = scene(Range(35, 35 + logo.rows), Range(80, 80 + logo.cols)); //第二种方法:依次给出ROI区域的纵坐标和横坐标范围
	addWeighted(sceneROI,0.5,logo,0.05,0.,sceneROI);   //两张图片的简单混合，其中后面的参数是各自的混合权重，第五个参数是gamma,表示偏重
	    //其中要注意的是sceneROI表示scene的ROI，并不是根据矩形框对scene进行截图生成新的mat图片，所以最后混合的结果赋值给sceneROI,
		//就会自动引用到原图片的矩形框范围内

	namedWindow("混合图");
	imshow("混合图", scene);

	imwrite("第一个输出图片.jpg",scene);

	waitKey();
	return 0;
}