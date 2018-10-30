#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

Mat getClosenessWeight(float sigma_g, Size size)
{
	int H = size.height;
	int W = size.width;

	int cH = (H - 1) / 2;
	int cW = (W - 1) / 2;
	Mat closenessWeight = Mat::zeros(size, CV_64FC1);

	for (int r = 0; r < H; r++)
	{
		for (int c = 0; c < W; c++)
		{
			double norm2 = pow(double(r - cH), 2.0) + pow(double(c - cW), 2.0);
			double sigma_g2 = 2.0*pow(sigma_g, 2.0);
			closenessWeight.at<double>(r, c) = exp(-norm2 / sigma_g2);
		}
	}
	return closenessWeight;
}
Mat JoinBlur(Mat I, Size size, float sigma_g, float sigma_d, int BorderType = BORDER_DEFAULT)
{
	Mat closeWeight = getClosenessWeight(sigma_g, size);

	//对原图进行高斯平滑
	Mat Ig;
	GaussianBlur(I, Ig, size, sigma_g);

	int cH = (size.height - 1) / 2;
	int cW = (size.width - 1) / 2;

	Mat Ip, Igp;
	copyMakeBorder(I, Ip, cH, cH, cW, cW, BorderType);
	copyMakeBorder(Ig, Igp, cH, cH, cW, cW, BorderType);

	int rows = I.rows;
	int cols = I.cols;
	int i = 0, j = 0;
	Mat jblf = Mat::zeros(I.size(), CV_64FC1);
	for (int r = cH; r < cH + rows; r++)
	{
		for (int c = cW; c < cW + cols; c++)
		{
			//联合双边滤波在高斯平滑的结果上计算颜色相似度矩阵
			//所以这里是 Igp.at,而不是Ig.at
			double pixel = Igp.at<double>(r, c);   //这里卷积的锚点在卷积核中间，pixel是当前卷积所在位置下,锚点对应的原图像素值
												   //这里采用稍微和双边滤波不同的方式，
												   //之前双边滤波的程序没有padding,所以在进行权重值和图片矩阵进行卷积时，要考虑边界的情况，
												   //所以当时就是加加减减的进行边界计算，而这里有了padding,那么直接从图片内部进行卷积核的滑动，不会出界
			Mat region = Igp(Rect(c - cW, r - cH, size.width, size.height));

			//根据当前的region计算颜色相似度权重矩阵,这里和之前双边滤波计算一样
			Mat similarityWeight;
			pow(region - pixel, 2.0, similarityWeight);
			cv::exp(-0.5*similarityWeight / pow(sigma_d, 2.0), similarityWeight);

			//根据距离和颜色相似度矩阵的点乘得到整体权重矩阵
			Mat weight = closeWeight.mul(similarityWeight);
			weight = weight / sum(weight)[0];  //权重归一化(0-1)，因为这里结果图jblf也是CV_64FC1,值在 0-1,
											   //这样归一化后保证权重和原图Ip进行卷积后，得到的值也是在CV_64FC1的0-1内(Ip也是CV_64FC1)
											   //在原图上与总权重矩阵进行卷积，所以这里是Ip,不是Ipg
			Mat regionK = Ip(Rect(c - cW, r - cH, size.width, size.height));
			jblf.at<double>(i, j) = sum(regionK.mul(weight))[0];
			j++;
		}
		i++;
		j = 0;
	}
	return jblf;
}
int main()
{
	//联合双边滤波
	//能够对纹理的平滑效果更彻底，同时又比均值平滑和双边滤波更好的保存边缘信息

	//双边滤波是在原图基础上计算高斯核-即距离相似权重矩阵和颜色相似矩阵
	//而联合双边滤波是在原图上计算高斯核-即距离相似权重矩(但其实高斯核与图无关，它只与核大小有关)，
	//但是在其高斯平滑的结果上计算颜色相似度矩阵,然后根据距离相似和颜色相似矩阵得到总的权重矩阵
	//之后在原图上(非高斯平滑的结果上)与总的权重矩阵进行卷积
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat fI;
	src.convertTo(fI, CV_64FC1, 1.0, 0);   //和双边滤波一样，进行灰度值归一化
										   //cout << fI.at<double>(10, 200) << endl;
	Mat jblf = JoinBlur(fI, Size(3, 3), 7, 2);

	//imshow("联合双边滤波jblf", jblf);   //纯白色
	Mat jblf8U;
	jblf.convertTo(jblf8U, CV_8U, 1.0, 0);
	imshow("Origin", src);
	imshow("联合双边滤波jblf8U", jblf8U);

	/*
	这里要注意的一点就是，这里将src变为CV_64FC1时，并没有想双边滤波中一样除以255,而只是除以1.0(相当于整数变为小时)
	但是这里并不影响结果，因为虽然说CV_64FC1的图像中的元素在0-1中，但是并不代表其不能保存大于1的数，只是如果值大于1，
	那么在imshow时会显示白色(因为超出了CV_64FC1的范围0-1),就像这里JoinBlur函数后得到的jblf是CV_64FC1,但是其值是大于1的，
	仍然在0-255之间，没有归一化，那么imshow("联合双边滤波jblf", jblf);就会显示纯白色图
	但是这里在jblf变为CV_8U类型的jblf8U时，就不用乘以255,因为jblf本来就是0-255范围的，只是将其转为CV_8U类型，
	保证0-255范围是"合法的",即保证imshow时不会出现纯白色的现象

	当然这里如果一开始除以255，然后再乘以255，也是可以，只要保证类型和数值范围匹配即可
	*/
	waitKey(0);
	return 0;
}