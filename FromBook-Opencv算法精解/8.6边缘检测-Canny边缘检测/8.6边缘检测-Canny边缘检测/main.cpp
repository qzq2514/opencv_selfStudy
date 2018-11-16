#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


//测试书上Sobel边缘检测的例子，0为填充边界
void testSobel()
{
	Mat testImg = (Mat_<float>(5, 5) << 3, 10, 12, 19, 256, 240, 239, 8, 7, 10, 255, 180, 778, 9, 1,
		170, 200, 197, 168, 50, 2, 10, 180, 140, 140);
	Mat Sobel_X = (Mat_<float>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
	Mat Sobel_Y = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	flip(Sobel_X, Sobel_X, -1);
	flip(Sobel_Y, Sobel_Y, -1);
	Mat srcX, srcY;
	filter2D(testImg, srcX, CV_32FC1, Sobel_X, Point(-1, -1), 0.0, BORDER_CONSTANT);
	filter2D(testImg, srcY, CV_32FC1, Sobel_Y, Point(-1, -1), 0.0, BORDER_CONSTANT);
	cout << srcX << endl;
	cout << srcY << endl;
	cout << "------------" << endl;

	pow(srcX, 2.0, srcX);
	pow(srcY, 2.0, srcY);
	Mat testDst;                    //平方和的开方作为边缘强度
	sqrt(srcX + srcY, testDst);
	cout << testDst << endl;
	
	cout << atan2f(719, 259)/3.1416926*180  << endl;   //求梯度方向
	system("pause");
}
Mat nonMaximum_suppression_default(Mat dx, Mat dy,Mat edgeMat)
{
	int rows = dx.rows;
	int cols = dx.cols;

	Mat nonMaxSupMat = Mat::zeros(dx.size(), dx.type());
	for (int r = 1; r < rows-1; r++)
	{
		for (int c = 1; c < cols-1; c++)
		{
			float x = dx.at<float>(r, c);
			float y = dy.at<float>(r, c);

			//梯度方向，角度表示
			float angle = atan2(y, x) / CV_PI * 180;
			float edge = edgeMat.at<float>(r, c);

			//开始默认的非极大值抑制方法
			if (abs(angle) < 22.5 || abs(angle) > 157.5)
			{
				//计算当前点的邻域点
				float left = edgeMat.at<float>(r, c - 1);
				float right = edgeMat.at<float>(r, c + 1);
				//只有当前点的值大于领域点的值，才将当前点的值赋值给最终的非极大值抑制的结果
				if (edge > left &&edge > right)
					nonMaxSupMat.at<float>(r, c) = edge;

			}
			else if ((angle >= 22.5 && angle < 67.5) || (angle < -112.5 && angle >-157.5))
			{
				float leftTop = edgeMat.at<float>(r - 1, c - 1);
				float rightBottom = edgeMat.at<float>(r + 1, c + 1);
				if (edge > leftTop &&edge > rightBottom)
					nonMaxSupMat.at<float>(r, c) = edge;
			}
			else if ((angle >= 67.5 && angle <= 112.5) || (angle <= -67.5 && angle >=-112.5))
			{
				float top = edgeMat.at<float>(r - 1, c );
				float bottom = edgeMat.at<float>(r + 1, c );
				if (edge > top &&edge > bottom)
					nonMaxSupMat.at<float>(r, c) = edge;
			}
			else if ((angle >= 112.5 && angle <= 157.5) || (angle <= -22.5 && angle > -67.5))
			{
				float rightTop = edgeMat.at<float>(r - 1, c+1);
				float leftBottom = edgeMat.at<float>(r + 1, c-1);
				if (edge > rightTop &&edge > leftBottom)
					nonMaxSupMat.at<float>(r, c) = edge;
			}
		}
	}
	return nonMaxSupMat;
}

//使用插值的方法进行非极大值抑制
//上面默认的非极大值抑制的方法是将当前值与单个邻域比较,例如133度时，就比较右上点和左下点
//而带有插值的方法则在梯度方向经过的领域点都进行加权比较，同样例如133度时，其实是经过右上，正上
//左下，正下的，然后右上和正上加权求和，左下和正下加权求和，两个加权求和与当前的值进行比较，进行非极大值抑制
Mat non_maximum_suppression_Inter(Mat dx, Mat dy)
{
	//使用平方和开方的方式计算边缘强度
	Mat edgeMag;
	cv::magnitude(dx, dy, edgeMag);   //opencv自带的计算二维矢量图的幅值，即求两者的平方和的开方
	//宽高
	int rows = dx.rows;
	int cols = dy.cols;
	//边缘强度的非极大抑制
	Mat edgeMag_nonMaxSup = Mat::zeros(dx.size(), dx.type());
	for (int r = 1; r < rows - 1; r++)
	{
		for (int c = 1; c < cols - 1; c++)
		{
			float x = dx.at<float>(r, c);
			float y = dy.at<float>(r, c);
			if (x == 0 && y == 0)
				continue;
			float angle = atan2f(y, x) / CV_PI * 180;
			//领域内八个方向上的边缘强度
			float leftTop = edgeMag.at<float>(r - 1, c - 1);
			float top = edgeMag.at<float>(r - 1, c);
			float rightBottom = edgeMag.at<float>(r + 1, c + 1);
			float right = edgeMag.at<float>(r, c + 1);
			float rightTop = edgeMag.at<float>(r - 1, c + 1);
			float leftBottom = edgeMag.at<float>(r + 1, c - 1);
			float bottom = edgeMag.at<float>(r + 1, c);
			float left = edgeMag.at<float>(r, c - 1);
			float mag = edgeMag.at<float>(r, c);
			//左上方与上方的插值 右下方和下方的插值
			if ((angle > 45 && angle <= 90) || (angle > -135 && angle <= -90))
			{
				float ratio = x / y;
				float top = edgeMag.at<float>(r - 1, c);
				//插值
				float leftTop_top = ratio*leftTop + (1 - ratio)*top;
				float rightBottom_bottom = ratio*rightBottom + (1 - ratio)*bottom;
				if (mag > leftTop_top && mag > rightBottom_bottom)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//右上方和上方的插值 左下方和下方的插值
			if ((angle > 90 && angle <= 135) || (angle > -90 && angle <= -45))
			{
				float ratio = abs(x / y);
				float rightTop_top = ratio*rightTop + (1 - ratio)*top;
				float leftBottom_bottom = ratio*leftBottom + (1 - ratio)*bottom;
				if (mag > rightTop_top && mag > leftBottom_bottom)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//左上方和左方的插值 右下方和右方的插值
			if ((angle >= 0 && angle <= 45) || (angle > -180 && angle <= -135))
			{
				float ratio = y / x;
				float rightBottom_right = ratio*rightBottom + (1 - ratio)*right;
				float leftTop_left = ratio*leftTop + (1 - ratio)*left;
				if (mag > rightBottom_right && mag > leftTop_left)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//右上方和右方的插值 左下方和左方的插值
			if ((angle > 135 && angle <= 180) || (angle > -45 && angle <= 0))
			{
				float ratio = abs(y / x);
				float rightTop_right = ratio*rightTop + (1 - ratio)*right;
				float leftBottom_left = ratio*leftBottom + (1 - ratio)*left;
				if (mag > rightTop_right && mag > leftBottom_left)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
		}
	}
	return edgeMag_nonMaxSup;
}
bool checkIsInBounder(int r, int c, int rows, int cols)
{
	if (r >= 0 && r <= rows && c >= 0 && c <= cols)
		return true;
	else
		return false;
}

//已经延伸到(r,c)位置了，且(r,c)位置已经在函数外判断过是大于小阈值的
void trace(Mat nonMaxSupMat, Mat &edge, float lowThresh, int r, int c,int rows,int cols)
{
	if (edge.at<float>(r, c) != 0)        //当前点已经bfs过了
		return;

	edge.at<float>(r, c) = 255;           //因为判断过(r,c)位置大于小阈值，所以这里直接将结果图对应的位置确定为边缘点
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (checkIsInBounder(r + i, c + j, rows, cols))
			{
				float mag = nonMaxSupMat.at<float>(r + i, c + j);
				if (mag > lowThresh)   //如果当前点的领域点灰度值大于小阈值，那么继续延伸
					trace(nonMaxSupMat, edge, lowThresh, r + i, c + j, rows, cols);
			}
		}
	}
}
Mat hysteresisThreshold(Mat nonMaxSupMat, float lowThresh, float highThresh)
{
	int rows = nonMaxSupMat.rows;
	int cols = nonMaxSupMat.cols;
	Mat edge = Mat(nonMaxSupMat.size(), nonMaxSupMat.type());

	//双阈值的滞后阈值处理:
	//1.如果当前阈值大于高阈值，那么肯定是边缘点
	//2.如果当前阈值小于低阈值，那么不作为边缘点直接剔除掉
	//3.对边缘点进行延伸，延伸经过的点都是至少大于低阈值的
	for (int r = 1; r < rows - 1; r++)
	{
		for (int c = 1; c < cols - 1; c++)
		{
			float mag = nonMaxSupMat.at<float>(r, c);

			//从大于高阈值点的确定边缘点进行bfs
			if (mag > highThresh)
				trace(nonMaxSupMat, edge, lowThresh, r, c, rows, cols);
			else
				edge.at<float>(r, c) = 0;
				
		}
	}
	return edge;
}
int main()
{
	//canny边缘检测算法
	Mat src=imread("lena_full.jpg",0);
	imshow("src",src);

	//步骤:
	//1.计算出x,y方向的Sobel边缘检测结果和边缘强度
	Mat Sobel_X = (Mat_<float>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
	Mat Sobel_Y = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	flip(Sobel_X, Sobel_X, -1);
	flip(Sobel_Y, Sobel_Y, -1);
	Mat srcX, srcY;               //CV_32FC1类型的两个方向边缘检测结果
	filter2D(src, srcX, CV_32FC1, Sobel_X, Point(-1, -1), 0.0, BORDER_CONSTANT);
	filter2D(src, srcY, CV_32FC1, Sobel_Y, Point(-1, -1), 0.0, BORDER_CONSTANT);
	pow(srcX, 2.0, srcX);
	pow(srcY, 2.0, srcY);

	Mat sobelDst;                    //平方和的开方作为边缘强度
	sqrt(srcX + srcY, sobelDst);      //至此水平和垂直方向检测的Sobel都是CV_32FC1类型
	Mat sobelDstShow = sobelDst.clone();
	convertScaleAbs(sobelDstShow, sobelDstShow, 1.0);
	imshow("sobelDst", sobelDstShow);

	//2.非极大值抑制
	Mat nonMaxSupMat = nonMaximum_suppression_default(srcX, srcY, sobelDst);
	//Mat nonMaxSupMat = non_maximum_suppression_Inter(srcX, srcY);

	Mat nonMaxSupMatShow = nonMaxSupMat.clone();
	convertScaleAbs(nonMaxSupMatShow, nonMaxSupMatShow, 1.0);
	imshow("nonMaxSupMatDefault", nonMaxSupMatShow);
	cout << nonMaxSupMat.type() << endl;
	
	//3.双阈值的滞后阈值处理
	Mat edge = hysteresisThreshold(nonMaxSupMat, 20, 100);

	Mat edgeShow = edge.clone();
	convertScaleAbs(edgeShow, edgeShow, 1.0);
	imshow("Canny滞后阈值后的结果", edgeShow);


	//opencv自带的canny边缘检测函数
	Mat cannyOpencv;
	//Canny(原图，结果图，低阈值，高阈值，Sobel核大小，是否使用平方和开方的边缘强度计算方法)
	//最后一个参数为false则使用绝对值求和的方法计算边缘强度
	Canny(src,cannyOpencv, 20,100,3,true);
	imshow("cannyOpencv", cannyOpencv);

	waitKey(0);
	return 0;
}