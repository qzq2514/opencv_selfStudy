#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
Mat ConvFull(Mat I, Mat k)
{
	Mat kCopy = k.clone();
	int kRow = k.rows;
	int kCol = k.cols;

	Mat Img_padded;
	copyMakeBorder(I, Img_padded, (kRow - 1) / 2, kRow / 2, (kCol - 1) / 2, kCol / 2, BORDER_CONSTANT, Scalar::all(0));
	cout <<"全填充后:"<<endl<< Img_padded << endl;
	cout << "------------------" << endl;
	
	flip(k, kCopy, -1);    //卷积核旋转180度

	Mat convFull;
	filter2D(Img_padded, convFull, -1, kCopy, Point(-1, -1), 0.0, BORDER_CONSTANT);

	//cout << convFull << endl;
	return convFull;
}
int main()
{
	//先亮出公式，便于时候查询
	//I*K   <<====>> FT_Ip.*FT_Kp  
	Mat Img = (Mat_<double>(8, 8) << 
		34, 56, 1, 0, 255, 230, 45, 12,
		0, 201, 101, 125, 52, 12, 124, 12,
		3, 41, 42, 40, 12, 90, 123, 45,
		5, 245, 98, 32, 34, 234, 90, 123,
		12, 12, 10, 41, 56, 89, 189, 5,
		112, 87, 12, 45, 78, 45, 10, 1,
		42, 123, 234, 12, 12, 21, 56, 43,
		1, 2, 45, 123, 10, 44, 123, 90);

	Mat kernel = (Mat_<double>(3, 3) << 1, 0, -1, 1, 0, 1, 1, 0, -1);

	//本程序没用到，但是可以测试全卷积和same卷积的关系(相同填充方式下，same卷积是全卷积的部分)
	/*Mat convFull = ConvFull(Img, kernel);
	cout <<"全卷积:"<<endl<< convFull << endl;   */

	//原图宽高
	int imgR = Img.rows;
	int imgC = Img.cols;

	//卷积核宽高
	int kerR = kernel.rows;
	int kerC = kernel.cols;

	//卷积核半径
	int radiusR = (kerR - 1) / 2;
	int radiusC = (kerC - 1) / 2;


	//原图边界填充-这里使用以边界为轴的镜像填充
	//此时填充图大小是(imgR+kerR-1)*(imgC+kerC-1)
	Mat Img_padded;
	copyMakeBorder(Img, Img_padded, radiusR, radiusR, radiusC, radiusC, BORDER_DEFAULT);

	//获得满足快速傅立叶变换的行数列数
	//注意这里是根据Img_padded.rows + kerR - 1，而不是根据Img_padded.rows得到快速傅立叶变换的长宽
	int rowPadd = getOptimalDFTSize(Img_padded.rows + kerR - 1);
	int colPadd = getOptimalDFTSize(Img_padded.cols + kerC - 1);

	//快速傅立叶变换填充在右侧和下侧填充0即可
	Mat Img_padded_zeros, ker_zeros;
	copyMakeBorder(Img_padded, Img_padded_zeros, 0, rowPadd - Img_padded.rows, 0, colPadd - Img_padded.cols, BORDER_CONSTANT, Scalar::all(0));
	copyMakeBorder(kernel, ker_zeros, 0, rowPadd - kernel.rows, 0, colPadd - kernel.cols, BORDER_CONSTANT, Scalar::all(0));
	//Img_padded_zeros和ker_zeros尺寸是一样的，都是rowPadd*colPadd

	//快速傅立叶变换
	Mat FFT_ImgPaddZero, FFT_KerZero;
	dft(Img_padded_zeros, FFT_ImgPaddZero, DFT_COMPLEX_OUTPUT);
	dft(ker_zeros, FFT_KerZero, DFT_COMPLEX_OUTPUT);

	//两个快速傅立叶变换的点乘
	Mat Img_ker_FFT;
	mulSpectrums(FFT_ImgPaddZero, FFT_KerZero, Img_ker_FFT, DFT_ROWS);

	//傅立叶变换，只取实部
	Mat invFFT;
	dft(Img_ker_FFT, invFFT, DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);

	Mat sameConv_FFT = invFFT(Rect(kerC - 1, kerR - 1, imgC + kerC - 1, imgR + kerR - 1));
	cout <<"傅立叶变换得到卷积结果:"<<endl<< sameConv_FFT << endl;


	Mat kerFlip;
	flip(kernel, kerFlip, -1);
	Mat sameConv;
	//注意这里使用filter2D进行same卷积的填充方式要和Img->Img_padded的填充方式相同
	filter2D(Img, sameConv, -1, kerFlip, Point(-1, -1),0.0, BORDER_DEFAULT);
	cout << "same卷积:" << endl << sameConv <<endl;

    //至此，可以看到傅立叶变换出来和直接same卷积出来的结果是一样，所以可以使用快速傅立叶变换得到same卷积
	//复习:在之前"10.4卷积与傅立叶变换的关系"工程中，可以基于傅立叶变换得到图像与核的full卷积结果


	//总结:至此，傅立叶变换的章节结束
	//之前的图像平滑和边缘检测都被统称为空间域滤波(像素空间内的操作)，而通过卷积定理，可以将空间域的卷积操作
	//转换到频率域完成。
	system("pause");
	return 0;
}