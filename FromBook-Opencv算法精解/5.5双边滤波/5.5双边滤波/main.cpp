#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

//����ռ����Ȩ�ؾ�����ʵ���Ǹ�˹��
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


//�ԻҶ�ͼ�����˫���˲�
//image������ͼ���һҶȷ�Χ����һ��Ϊ[0��1],winSizeΪȨ��ģ��Ĵ�С(������)
//sigma_gΪ�ռ�������ƶȾ���ı�׼��(����˹�˵ı�׼��)��sigma_dΪ��ɫ���ƶȾ���ı�׼��

Mat bfltGray(const Mat &image, Size winSize, float sigma_g, float sigma_d)
{
	int winH = winSize.height;
	int winW = winSize.width;

	//��֤Ȩ��ģ�泤��Ϊ����
	CV_Assert(winH>0 && winW >0);
	CV_Assert(winH % 2 == 1 && winW % 2 == 1);

	if (winH == 1 && winW == 1) return image;   //�����(��Ȩ��ģ��)��СΪ1����ô���൱��û�����˲�������ԭͼ

	int half_winW = (winW - 1) / 2;
	int half_winH = (winH - 1) / 2;

	//�õ��ռ�����Ȩ������(��ָ���˿�ߵĸ�˹�����)
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

			//�߽��ж�
			int rTop = (r - half_winH) < 0 ? 0 : r - half_winH;
			int rBottom = (r + half_winH) >rows-1 ? rows-1 : r + half_winH;
			int cLeft = (c - half_winW) < 0 ? 0 : c - half_winW;
			int cRight = (c + half_winW) >cols - 1 ? cols - 1 : c + half_winW;

			//cout << cRight + 1 << "--" << rBottom + 1 << endl;
			//�����õ�����,Rect����������򲻰����ڶ���Point���ڵ����У�����Ҫ��һ
			Mat region = image(Rect(Point(cLeft, rTop), Point(cRight+1 , rBottom + 1))).clone();
		    
			Mat similarityWeight;
			//����˷�Χ������ÿ�������뵱ǰ����(pixel)�ľ���
			pow(region - pixel, 2.0, similarityWeight);   //ÿ�����ȱ�����similarityWeight��
			exp(-0.5*similarityWeight / pow(sigma_d, 2.0), similarityWeight);

			//�ҵ����ƶ�Ȩ��ģ���Ӧ�Ŀռ����Ȩ��ģ��ķ�Χ(��ģ��ӼӼ����е���~�������P167)
			//�ҡ�������΢�����°ɣ���ʵ�����������Ч���ƶȾ���(��ԭͼ��Χ�ڵ�)���˹���غϵ�����,�������ڸ�˹���ڵ�Rect��ʾ

			//��rTopΪ�������r - half_winH>=0˵����Ч���ƶȾ����ϱ߿���ԭͼ�ڣ���ʱ��rTop=r - half_winH
			//����Ч���ƶȾ���͸�˹�˵����ϱ߿���غϾ�����ȫ�غϣ�����Ч���ƶȾ������˹�������ϱ߽���ȫ�غϣ���ô����rTop - r + half_winH=0
			//�������r - half_winH<0(��ʱrTop=0)��˵�����ƶȾ��󳬳�ԭͼ��Χ�����ϱ߽糬������������half_winH -r ,��ô������Ч���ƶȾ����
			//��˹���غϵľ�ֻ���²���(���ϱ߽粻�غ�)�����ⲿ���غϵľ������ǴӸ�˹�˵ĵ�half_winH -r+1�п�ʼ��������Rect�����귶Χ�Ǵ�0��ʼ
			//��half_winH -r+1�о����±�Ϊhalf_winH -r������ͳһ������rTop - r + half_winH(��Ϊ��ʱrTop=0)
			//������£����ұ߽綼����ô˼�����������Ϊʲô�ڶ��������궼��һ����ΪRect����������򲻰����ڶ���Point���ڵ����У�����Ҫ��һ
			Rect regionRect = Rect(Point(cLeft - c + half_winW, rTop - r + half_winH),
								   Point(cRight - c + half_winW + 1, rBottom - r + half_winH + 1));

			//��Ϊ��˹�������ÿ��Ԫ�ص�ֵ������˵Ĵ�С�ͷ����йأ���������ظ������Ѿ��õ��ĸ�˹�����
			Mat closenessWeightTemp = closenessWeight(regionRect).clone();
			Mat weightTemp = closenessWeightTemp.mul(similarityWeight);   //����Ȩ�ؾ����˵õ������Ȩ�ؾ���

			weightTemp = weightTemp / sum(weightTemp)[0];   //sum�����Ƕ�ͼ�����ÿ��ͨ����ͣ�����Ҷ�ͼֻ��һ��ͨ��
			Mat result = weightTemp.mul(region);       //�����Ȩ�ؾ�����ԭͼ��Ķ�Ӧ��Χ�ľ�����е��
			blfImage.at<float>(r, c) = sum(result)[0];
		}
	}
	return blfImage;


	return Mat();
}
int main()
{
	//˫���˲�
	//ԭ�ȵĸ�˹�˲���ֻҪ�˵Ĵ�С�ͷ���ȷ������ô�������ڵ�Ԫ���ǲ���ģ����������ԭͼ�ϻ���ʱ������ԭͼ
	//��Ӧ��Χ������ֵ���Ȩƽ���������Ȩֵ���Ǹ�˹���ڵ�ֵ����ô��ʱȨֵ�ڻ����������Ǳ��ֲ����
	//��˫���˲�����ÿ�λ���ʱ��Ȩֵ���ǲ��ϱ仯�ģ���Ҫ�������Ǹ�˹�˺����ƶȺ˵ĵ��
	//���ƶȺ˼���˿�ס��ԭͼÿ��Ԫ����ê������ƹ��ɵľ���(���ƶ���ŷ�Ͼ���)
	//������������������������ֵ����ϴ�����أ���ʹ����������ê���������Ͻ���Ҳ������С��Ȩֵ��

	//��д��˫���˲�
	//Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat fI;
	//src.convertTo(fI, CV_64FC1, 1.0 / 255, 0);   //�Ҷ�ֵ��һ��
	//Mat blfI=bfltGray(fI, Size(5, 5), 19, 0.5);

	//imshow("Origin", src);
	//imshow("˫���˲�", blfI);    
	//
	//blfI.convertTo(blfI, CV_8U, 255, 0);   //��Ϊ����imwriteд���ǵ�CV_8U��ʽ(����8λ��ͨ��)������
										   //����Ҫ��������CV_32FC1��ʽ(Ԫ��ֵ��0-1֮��)תΪCV_8U��ʽ(0-255֮��)
	//imwrite("blfI.bmp", blfI);    //��Ҫע�⣺��DEBUGģʽ�£�ֻ�ܴ�ΪBMP��ʽ��Ҫ����JPG��PNGʱ������ʹ��Releaseģʽ


	//С����
	//Mat testSrc = (Mat_<uchar>(5, 5) << 11, 12, 13, 14, 15,
	//	21, 22, 23, 24, 25,
	//	31, 32, 33, 34, 35,
	//	41, 42, 43, 44, 45,
	//	51, 52, 53, 54, 55);
	//Mat region = testSrc(Rect(Point(0, 0), Point(6, 6))).clone();
	//cout << region << endl;   //���صľ���ͼ�񲻰����ڶ���������ʾ�ĵ����ڵ��к���
	//system("pause");

	//opencv�Դ���˫���˲�����:bilateralFilter
	Mat srcColor1 = imread("lena_full.jpg", 1);
	Mat dst1;

	//����7Ϊ�˴�С
	//15ΪSigma_color������ɫ���ƶȾ����׼���Ӧ����д��bfltGray������sigma_d
	//20Ϊsigma_sapce�����������ƶȾ����׼���Ӧ����д��bfltGray������sigma_g
	//���ã�Sigma_color�ϴ����������е���ɫ����ֵ���ϴ�����ص�Ҳ������ƽ�������ῼ����ɫ��������ص�
	      //Sigma_space�ϴ�����Ȼ��ý�Զ�����ǣ�ֻҪ��ɫֵ������ͻụ��Ӱ�졣���ῼ�Ǿ����������ص�
	
	bilateralFilter(srcColor1,dst1, 7,150, 200);
	imshow("bilateralFilter", dst1);

	waitKey(0);
	return 0;
}