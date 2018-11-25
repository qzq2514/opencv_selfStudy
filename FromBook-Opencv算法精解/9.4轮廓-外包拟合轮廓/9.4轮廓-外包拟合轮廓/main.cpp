#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


//检测车牌中的字符
void detectPlate()
{
	Mat src = imread("testPlate.jpg", 0);
	Mat colorSrc = imread("testPlate.jpg");
	imshow("原图", src);

	Mat gBlur;
	GaussianBlur(src, gBlur, Size(3, 3), 0.5);
	imshow("高斯平滑", gBlur);
	cout << gBlur.type() << endl;


	//像车牌这种字符与背景区域颜色差异明显并且字符内颜色几乎没变化的，
	//针对每个字符，其边缘检测的结果是有规则的(整个图片的边缘检测可能会比较混乱)
	//这时候可以不进行二值化，直接进行边缘检测,当然，二值化后，效果肯定也更好
	/*Mat thresh;
	threshold(gBlur, thresh, 140, 255, THRESH_BINARY);
	imshow("二值化", thresh);*/

	Mat canny;
	Canny(gBlur, canny, 180, 200, 5, true);
	imshow("边缘检测", canny);


	vector<vector<Point> > contours;
	//RETR_TREE参数完整画出带有树状层次结构的包围框，先不剔除异常框
	findContours(canny, contours, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat imgShow = colorSrc.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		//根据最小包围框的高宽比和其与车牌图片的面积比进行筛选
		//以尽力选出真正的字符区域
		float areaRatio = 1.0*rect.area() / canny.total();
		float high_width_ratio = rect.height*1.0 / rect.width;
		if (areaRatio > 0.025 && areaRatio < 0.05 && high_width_ratio >1.2 && high_width_ratio <2.5)
		{
			rectangle(imgShow, rect, Scalar(0, 0, 255), 2);
		}
	}
	imshow("检测结果", imgShow);
}

void detectDog()
{
	Mat src = imread("dog.jpg", 0);
	Mat colorSrc = imread("dog.jpg");
	imshow("原图", src);

	Mat gBlur;
	GaussianBlur(src, gBlur, Size(3, 3), 0.5);
	imshow("高斯平滑", gBlur);
	cout << gBlur.type() << endl;

	Mat thresh;
	//OSTSU自动阈值计算出是168，但是狗腿被分开，这里手动设置阈值比168小一点，保证目标的完整连续性
	threshold(gBlur, thresh, 155, 255,THRESH_BINARY);
	imshow("二值化", thresh); 

	//进行开运算(先腐蚀后膨胀)消除目标区域外的细小孤立的白点
	Mat open;
	Mat kelOpen=getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresh, open, MORPH_OPEN, kelOpen);
	imshow("开运算", open);

	//闭运算(先膨胀后腐蚀)，填充物体间的细小黑洞
	Mat close;
	Mat kelClose = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(open, close, MORPH_CLOSE, kelClose);
	imshow("闭运算", close);

	Mat canny;
	Canny(close, canny, 180, 200, 5, true);
	imshow("边缘检测", canny);


	vector<vector<Point> > contours;
	//RETR_EXTERNAL只找到最为层的轮廓
	findContours(canny, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat imgShowRect = colorSrc.clone();
	Mat imgShowPloy = colorSrc.clone();
	Mat imgShowContour = colorSrc.clone();
	for (int i = 0; i < contours.size(); i++)
	{

		//根据一条轮廓是点的集合，然后依次连接相邻点画出轮廓
		/*int lenC = contours[i].size();
		for (int j = 0; j < lenC; j++)
		{
			Point curPoint = contours[i][j];
			Point nextPoint = contours[i][(j+1)%lenC];

			line(imgShowContour, curPoint, nextPoint, Scalar(0,0,0), 2,7);
		}*/

		//OpenCV自带的画轮廓的函数-上次程序中用过
		drawContours(imgShowContour, contours, i, Scalar(0, 0, 0), 2);

		//根据第i个轮廓画出该轮廓的最小直立矩形
		Rect rect = boundingRect(contours[i]);
		rectangle(imgShowRect, rect, Scalar(0, 255, 0), 1);

		//多边形拟合
		vector<Point> ploy;
		//approxPolyDP(原轮廓点集，存放拟合后的轮廓点集，拟合精度，输出的多边形是否封闭)
		//精度表示拟合的点集中相邻点的最大距离，精度越小，相邻点距离越小，那么拟合越准确，可以手动改变该值进行比较
		//approxPolyDP其实就是对轮廓原点集的进一步近似表示
		approxPolyDP(contours[i], ploy, 10,true);
		int len = ploy.size();
		for (int j = 0; j < len; j++)
		{
			Point curPoint = ploy[j];
			Point nextPoint = ploy[(j + 1) %len];

			line(imgShowPloy, curPoint, nextPoint, Scalar(0, 0, 255), 2);
		}
	}
	imshow("检测结果-完整轮廓", imgShowContour);
	imshow("检测结果-多边形拟合", imgShowPloy);
	imshow("检测结果-最小外包直立矩形", imgShowRect);
}
int main()
{


	detectPlate();
	//detectDog();

	waitKey(0);
	return 0;
}