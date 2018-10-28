#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat fastMeanBlur(Mat src, Size size, int BorderTpye, Scalar value = Scalar())
{
	int khei = size.height;
	int kwid = size.width;

	//保证核的宽高都是奇数
	CV_Assert(khei % 2 == 1 && kwid % 2 == 1);

	//计算核窗口半径
	int hh = (khei - 1) / 2;
	int ww = (kwid - 1) / 2;

	//计算核窗口面积
	float area = float(khei*kwid);

	//为了省去在计算时进行边界判断，这里进行边界扩充
	Mat padding;
	copyMakeBorder(src, padding, hh, hh, ww, ww, BorderTpye, value);
	//imshow("padding", padding);   
	//cout << "padding_size" << padding.size() << endl;
	//cout << padding << endl;

	cout << padding.type() << endl;
	//进行图像积分,得到的inte图像的第(r,c)个位置的元素值是该位置左上角的矩形区域的元素和
	//得到的inte是在最左列和最右列外侧补零的图像，所以最后得到的图像大小是(padding.height+1)*(padding.width+1)
	Mat inte;
	integral(padding, inte, CV_64FC1);  //这里元素不断求和，用double保存，不然会产生溢出
	//imshow("inte", inte);
	//cout << "inte_size" << inte.size() << endl;
	//cout << inte << endl;

	/*cout << inte.at<double>(50,100) << endl;
	cout << inte.at<double>(230, 240) << endl;*/
	//cout << inte.type() << endl;    //5(CV_32FC1)

	int rows = src.rows;
	int cols = src.cols;
	int r = 0, c = 0;
	Mat fastMeanImg = Mat::zeros(src.size(), CV_8UC1);   //为了最后图片的显示效果，这里用uchar类型作为图像元素类型
													     //不然使用double,算出来的结果并不是0代表黑色,255代表白色
	//cout << hh + rows << "-" << hh << "-" << ww + cols << "-" << ww << endl;
	//cout << "-----" << endl;
	for (int h = hh; h < hh + rows; h++)
	{
		for (int w = ww; w < ww + cols; w++)
		{
			//cout << h << "-" << h + hh + 1 << "-" << h - hh << "-" << w << "-" << w + ww + 1 << "-" << w - ww << endl;
			double bottomRight = inte.at<double>(h + hh + 1, w + ww + 1);
			double topLeft = inte.at<double>(h - hh , w - ww );
			double bottomLeft = inte.at<double>(h + hh + 1, w - ww);
			double topRight = inte.at<double>(h - hh , w + ww + 1);
			double value= (bottomRight + topLeft - bottomLeft - topRight) / area;
			//cout << bottomRight<<"-"<< topLeft<<"-"<< bottomLeft<<"-"<< topRight<<"-"<<value<<endl;
			fastMeanImg.at<uchar>(r, c) = saturate_cast<uchar>(value);
			c++;
		}
		r++;
		c = 0;
	}
	//cout << fastMeanImg << endl; //这里图像明显也不是像素值都是255,但是就是显示纯白，搞不懂，玄学--不玄学，因为原来图像是CV_64FC1，那么每个元素类型是double,这时并不是0代表黑色,255代表白色
	return fastMeanImg;
}
int main()
{
	//了解快速均值平滑的小demo
	/*Mat testSrc = (Mat_<uchar>(5, 5) << 11, 12, 13, 14, 15,
										 21, 22, 23, 24, 25,
										 31, 32, 33, 34, 35,
										 41, 42, 43, 44, 45,
										 51, 52, 53, 54, 55);
	imshow("testSrc", testSrc);
	Mat testDst = fastMeanBlur(testSrc, Size(5, 5), BORDER_DEFAULT, Scalar());
	cout << testDst << endl;
	imshow("testDst", testDst);*/

	Mat src = imread("lena_small.jpg",0);   //不要用太大的图像，不然快速均值求和时容易数据溢出
	Mat dst = fastMeanBlur(src,Size(5,5),BORDER_DEFAULT,Scalar());
	imshow("Origin", src);
	imshow("fastMeanBlur", dst);

	//opencv自带的两个均值平滑的函数boxFilter和blur
	//且对于多通道的图像，分别在每个通道上进行平滑
	Mat src1= imread("lena_full.jpg", 1);
	Mat dst1;
	boxFilter(src1, dst1, -1, Size(5, 5), Point(-1, -1));
	imshow("src1", src1);
	imshow("boxFilter", dst1);

	Mat src2 = imread("lena_full.jpg", 1);
	Mat dst2;
	blur(src2, dst2, Size(5, 5), Point(-1, -1));
	imshow("src2", src2);
	imshow("blur", dst2);

	waitKey(0);
	return 0;
}