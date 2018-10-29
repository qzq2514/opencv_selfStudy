#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

//计算空间距离权重矩阵，其实就是高斯核
Mat getClosenessWeight(double sigma_g, Size size)
{
	
	int W = size.width;
	int H = size.height;

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


//对灰度图像进行双边滤波
//image是输入图像，且灰度范围被归一化为[0，1],winSize为权重模版的大小(宽高相等)
//sigma_g为空间距离相似度矩阵的标准差(即高斯核的标准差)，sigma_d为颜色相似度矩阵的标准差

Mat bfltGray(const Mat &image, Size winSize, float sigma_g, float sigma_d)
{
	int winH = winSize.height;
	int winW = winSize.width;

	//保证权重模版长宽为奇数
	CV_Assert(winH>0 && winW >0);
	CV_Assert(winH % 2 == 1 && winW % 2 == 1);

	if (winH == 1 && winW == 1) return image;   //卷积核(即权重模版)大小为1，那么就相当于没有做滤波，返回原图

	int half_winW = (winW - 1) / 2;
	int half_winH = (winH - 1) / 2;

	//得到空间距离的权重因子(即指定核宽高的高斯卷积核)
	Mat closenessWeight = getClosenessWeight(sigma_g, winSize);
	//cout << closenessWeight << endl;

	Mat blfImage = Mat::zeros(image.size(), CV_32FC1);

	int rows = image.rows;
	int cols = image.cols;
	cout << rows << "===" << cols << endl;
	cout << "-------------" << endl;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double pixel= image.at <double>(r, c);

			//边界判断
			int rTop = (r - half_winH) < 0 ? 0 : r - half_winH;
			int rBottom = (r + half_winH) >rows-1 ? rows-1 : r + half_winH;
			int cLeft = (c - half_winW) < 0 ? 0 : c - half_winW;
			int cRight = (c + half_winW) >cols - 1 ? cols - 1 : c + half_winW;

			//cout << cRight + 1 << "--" << rBottom + 1 << endl;
			//核作用的区域,Rect框出来的区域不包括第二个Point所在的行列，所以要加一
			Mat region = image(Rect(Point(cLeft, rTop), Point(cRight+1 , rBottom + 1))).clone();
		    
			Mat similarityWeight;
			//计算核范围区域内每个像素与当前像素(pixel)的距离
			pow(region - pixel, 2.0, similarityWeight);   //每步都先保存在similarityWeight中
			exp(-0.5*similarityWeight / pow(sigma_d, 2.0), similarityWeight);

			//找到相似度权重模版对应的空间距离权重模版的范围(妈的，加加减减有点绕~，详见书P167)
			//我。。。稍微解释下吧，其实这里就是求有效相似度矩阵(在原图范围内的)与高斯核重合的区域,该区域在高斯核内的Rect表示

			//以rTop为例，如果r - half_winH>=0说明有效相似度矩阵上边框在原图内，这时，rTop=r - half_winH
			//则有效相似度矩阵和高斯核的在上边框的重合就是完全重合，即有效相似度矩阵与高斯核内在上边界完全重合，那么就是rTop - r + half_winH=0
			//但是如果r - half_winH<0(此时rTop=0)，说明相似度矩阵超出原图范围，在上边界超出的行数就是half_winH -r ,那么这是有效相似度矩阵和
			//高斯核重合的就只是下部分(即上边界不重合)，而这部分重合的就正好是从高斯核的第half_winH -r+1行开始，而由于Rect的坐标范围是从0开始
			//第half_winH -r+1行就是下标为half_winH -r，保持统一，就有rTop - r + half_winH(因为此时rTop=0)
			//其余的下，左，右边界都是这么思考，至于最后为什么第二个点坐标都加一，因为Rect框出来的区域不包括第二个Point所在的行列，所以要加一
			Rect regionRect = Rect(Point(cLeft - c + half_winW, rTop - r + half_winH),
								   Point(cRight - c + half_winW + 1, rBottom - r + half_winH + 1));

			//因为高斯卷积核中每个元素的值仅仅与核的大小和方差有关，这里可以重复利用已经得到的高斯卷积核
			Mat closenessWeightTemp = closenessWeight(regionRect).clone();
			Mat weightTemp = closenessWeightTemp.mul(similarityWeight);   //两个权重矩阵点乘得到总体的权重矩阵

			weightTemp = weightTemp / sum(weightTemp)[0];   //sum好像是对图像矩阵每个通道求和，这里灰度图只有一个通道
			Mat result = weightTemp.mul(region);       //总体的权重矩阵与原图像的对应范围的矩阵进行点乘
			blfImage.at<float>(r, c) = sum(result)[0];
		}
	}
	return blfImage;


	return Mat();
}
int main()
{
	//双边滤波
	//原先的高斯滤波，只要核的大小和方差确定，那么其卷积核内的元素是不变的，即卷积核在原图上滑动时，其与原图
	//对应范围的像素值求加权平均，这里的权值就是高斯核内的值，那么这时权值在滑动过程中是保持不变的
	//而双边滤波则是每次滑动时，权值都是不断变化的，主要就是其是高斯核和相似度核的点乘
	//相似度核即求核框住的原图每个元素与锚点的相似构成的矩阵(相似度用欧氏距离)
	//这样，就能抑制与中心像素值差异较大的像素（即使邻域像素与锚点像素相距较近，也给它较小的权值）

	//自写的双边滤波
	//Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat fI;
	//src.convertTo(fI, CV_64FC1, 1.0 / 255, 0);   //灰度值归一化
	//Mat blfI=bfltGray(fI, Size(5, 5), 19, 0.5);

	//imshow("Origin", src);
	//imshow("双边滤波", blfI);    
	//
	//blfI.convertTo(blfI, CV_8U, 255, 0);   //因为保存imwrite写入是的CV_8U格式(或者8位多通道)，所以
										   //这里要将本来是CV_32FC1格式(元素值在0-1之间)转为CV_8U格式(0-255之间)
	//imwrite("blfI.bmp", blfI);    //且要注意：在DEBUG模式下，只能存为BMP格式，要保存JPG或PNG时，必须使用Release模式


	//小测试
	//Mat testSrc = (Mat_<uchar>(5, 5) << 11, 12, 13, 14, 15,
	//	21, 22, 23, 24, 25,
	//	31, 32, 33, 34, 35,
	//	41, 42, 43, 44, 45,
	//	51, 52, 53, 54, 55);
	//Mat region = testSrc(Rect(Point(0, 0), Point(6, 6))).clone();
	//cout << region << endl;   //返回的矩阵图像不包括第二个参数表示的点所在的行和列
	//system("pause");

	//opencv自带的双边滤波函数:bilateralFilter
	Mat srcColor1 = imread("lena_full.jpg", 1);
	Mat dst1;

	//其中7为核大小
	//15为Sigma_color，即颜色相似度矩阵标准差，对应于自写的bfltGray函数中sigma_d
	//20为sigma_sapce，即距离相似度矩阵标准差，对应于自写的bfltGray函数中sigma_g
	//作用：Sigma_color较大，则在邻域中的颜色像素值相差较大的像素点也会用来平均。即会考虑颜色差别大的像素点
	      //Sigma_space较大，则虽然离得较远，但是，只要颜色值相近，就会互相影响。即会考虑距离差别大的像素点
	
	bilateralFilter(srcColor1,dst1, 7,150, 200);
	imshow("bilateralFilter", dst1);

	waitKey(0);
	return 0;
}