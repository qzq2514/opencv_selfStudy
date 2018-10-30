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

	//��ԭͼ���и�˹ƽ��
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
			//����˫���˲��ڸ�˹ƽ���Ľ���ϼ�����ɫ���ƶȾ���
			//���������� Igp.at,������Ig.at
			double pixel = Igp.at<double>(r, c);   //��������ê���ھ�����м䣬pixel�ǵ�ǰ�������λ����,ê���Ӧ��ԭͼ����ֵ
												   //���������΢��˫���˲���ͬ�ķ�ʽ��
												   //֮ǰ˫���˲��ĳ���û��padding,�����ڽ���Ȩ��ֵ��ͼƬ������о��ʱ��Ҫ���Ǳ߽�������
												   //���Ե�ʱ���ǼӼӼ����Ľ��б߽���㣬����������padding,��ôֱ�Ӵ�ͼƬ�ڲ����о���˵Ļ������������
			Mat region = Igp(Rect(c - cW, r - cH, size.width, size.height));

			//���ݵ�ǰ��region������ɫ���ƶ�Ȩ�ؾ���,�����֮ǰ˫���˲�����һ��
			Mat similarityWeight;
			pow(region - pixel, 2.0, similarityWeight);
			cv::exp(-0.5*similarityWeight / pow(sigma_d, 2.0), similarityWeight);

			//���ݾ������ɫ���ƶȾ���ĵ�˵õ�����Ȩ�ؾ���
			Mat weight = closeWeight.mul(similarityWeight);
			weight = weight / sum(weight)[0];  //Ȩ�ع�һ��(0-1)����Ϊ������ͼjblfҲ��CV_64FC1,ֵ�� 0-1,
											   //������һ����֤Ȩ�غ�ԭͼIp���о���󣬵õ���ֵҲ����CV_64FC1��0-1��(IpҲ��CV_64FC1)
											   //��ԭͼ������Ȩ�ؾ�����о��������������Ip,����Ipg
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
	//����˫���˲�
	//�ܹ��������ƽ��Ч�������ף�ͬʱ�ֱȾ�ֵƽ����˫���˲����õı����Ե��Ϣ

	//˫���˲�����ԭͼ�����ϼ����˹��-����������Ȩ�ؾ������ɫ���ƾ���
	//������˫���˲�����ԭͼ�ϼ����˹��-����������Ȩ�ؾ�(����ʵ��˹����ͼ�޹أ���ֻ��˴�С�й�)��
	//���������˹ƽ���Ľ���ϼ�����ɫ���ƶȾ���,Ȼ����ݾ������ƺ���ɫ���ƾ���õ��ܵ�Ȩ�ؾ���
	//֮����ԭͼ��(�Ǹ�˹ƽ���Ľ����)���ܵ�Ȩ�ؾ�����о��
	Mat src = imread("lena_full.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat fI;
	src.convertTo(fI, CV_64FC1, 1.0, 0);   //��˫���˲�һ�������лҶ�ֵ��һ��
										   //cout << fI.at<double>(10, 200) << endl;
	Mat jblf = JoinBlur(fI, Size(3, 3), 7, 2);

	//imshow("����˫���˲�jblf", jblf);   //����ɫ
	Mat jblf8U;
	jblf.convertTo(jblf8U, CV_8U, 1.0, 0);
	imshow("Origin", src);
	imshow("����˫���˲�jblf8U", jblf8U);

	/*
	����Ҫע���һ����ǣ����ｫsrc��ΪCV_64FC1ʱ����û����˫���˲���һ������255,��ֻ�ǳ���1.0(�൱��������ΪСʱ)
	�������ﲢ��Ӱ��������Ϊ��Ȼ˵CV_64FC1��ͼ���е�Ԫ����0-1�У����ǲ��������䲻�ܱ������1������ֻ�����ֵ����1��
	��ô��imshowʱ����ʾ��ɫ(��Ϊ������CV_64FC1�ķ�Χ0-1),��������JoinBlur������õ���jblf��CV_64FC1,������ֵ�Ǵ���1�ģ�
	��Ȼ��0-255֮�䣬û�й�һ������ôimshow("����˫���˲�jblf", jblf);�ͻ���ʾ����ɫͼ
	����������jblf��ΪCV_8U���͵�jblf8Uʱ���Ͳ��ó���255,��Ϊjblf��������0-255��Χ�ģ�ֻ�ǽ���תΪCV_8U���ͣ�
	��֤0-255��Χ��"�Ϸ���",����֤imshowʱ������ִ���ɫ������

	��Ȼ�������һ��ʼ����255��Ȼ���ٳ���255��Ҳ�ǿ��ԣ�ֻҪ��֤���ͺ���ֵ��Χƥ�伴��
	*/
	waitKey(0);
	return 0;
}