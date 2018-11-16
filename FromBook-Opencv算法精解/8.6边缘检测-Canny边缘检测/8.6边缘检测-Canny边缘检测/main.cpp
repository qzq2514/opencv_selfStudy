#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


//��������Sobel��Ե�������ӣ�0Ϊ���߽�
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
	Mat testDst;                    //ƽ���͵Ŀ�����Ϊ��Եǿ��
	sqrt(srcX + srcY, testDst);
	cout << testDst << endl;
	
	cout << atan2f(719, 259)/3.1416926*180  << endl;   //���ݶȷ���
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

			//�ݶȷ��򣬽Ƕȱ�ʾ
			float angle = atan2(y, x) / CV_PI * 180;
			float edge = edgeMat.at<float>(r, c);

			//��ʼĬ�ϵķǼ���ֵ���Ʒ���
			if (abs(angle) < 22.5 || abs(angle) > 157.5)
			{
				//���㵱ǰ��������
				float left = edgeMat.at<float>(r, c - 1);
				float right = edgeMat.at<float>(r, c + 1);
				//ֻ�е�ǰ���ֵ����������ֵ���Ž���ǰ���ֵ��ֵ�����յķǼ���ֵ���ƵĽ��
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

//ʹ�ò�ֵ�ķ������зǼ���ֵ����
//����Ĭ�ϵķǼ���ֵ���Ƶķ����ǽ���ǰֵ�뵥������Ƚ�,����133��ʱ���ͱȽ����ϵ�����µ�
//�����в�ֵ�ķ��������ݶȷ��򾭹�������㶼���м�Ȩ�Ƚϣ�ͬ������133��ʱ����ʵ�Ǿ������ϣ�����
//���£����µģ�Ȼ�����Ϻ����ϼ�Ȩ��ͣ����º����¼�Ȩ��ͣ�������Ȩ����뵱ǰ��ֵ���бȽϣ����зǼ���ֵ����
Mat non_maximum_suppression_Inter(Mat dx, Mat dy)
{
	//ʹ��ƽ���Ϳ����ķ�ʽ�����Եǿ��
	Mat edgeMag;
	cv::magnitude(dx, dy, edgeMag);   //opencv�Դ��ļ����άʸ��ͼ�ķ�ֵ���������ߵ�ƽ���͵Ŀ���
	//���
	int rows = dx.rows;
	int cols = dy.cols;
	//��Եǿ�ȵķǼ�������
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
			//�����ڰ˸������ϵı�Եǿ��
			float leftTop = edgeMag.at<float>(r - 1, c - 1);
			float top = edgeMag.at<float>(r - 1, c);
			float rightBottom = edgeMag.at<float>(r + 1, c + 1);
			float right = edgeMag.at<float>(r, c + 1);
			float rightTop = edgeMag.at<float>(r - 1, c + 1);
			float leftBottom = edgeMag.at<float>(r + 1, c - 1);
			float bottom = edgeMag.at<float>(r + 1, c);
			float left = edgeMag.at<float>(r, c - 1);
			float mag = edgeMag.at<float>(r, c);
			//���Ϸ����Ϸ��Ĳ�ֵ ���·����·��Ĳ�ֵ
			if ((angle > 45 && angle <= 90) || (angle > -135 && angle <= -90))
			{
				float ratio = x / y;
				float top = edgeMag.at<float>(r - 1, c);
				//��ֵ
				float leftTop_top = ratio*leftTop + (1 - ratio)*top;
				float rightBottom_bottom = ratio*rightBottom + (1 - ratio)*bottom;
				if (mag > leftTop_top && mag > rightBottom_bottom)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//���Ϸ����Ϸ��Ĳ�ֵ ���·����·��Ĳ�ֵ
			if ((angle > 90 && angle <= 135) || (angle > -90 && angle <= -45))
			{
				float ratio = abs(x / y);
				float rightTop_top = ratio*rightTop + (1 - ratio)*top;
				float leftBottom_bottom = ratio*leftBottom + (1 - ratio)*bottom;
				if (mag > rightTop_top && mag > leftBottom_bottom)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//���Ϸ����󷽵Ĳ�ֵ ���·����ҷ��Ĳ�ֵ
			if ((angle >= 0 && angle <= 45) || (angle > -180 && angle <= -135))
			{
				float ratio = y / x;
				float rightBottom_right = ratio*rightBottom + (1 - ratio)*right;
				float leftTop_left = ratio*leftTop + (1 - ratio)*left;
				if (mag > rightBottom_right && mag > leftTop_left)
					edgeMag_nonMaxSup.at<float>(r, c) = mag;
			}
			//���Ϸ����ҷ��Ĳ�ֵ ���·����󷽵Ĳ�ֵ
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

//�Ѿ����쵽(r,c)λ���ˣ���(r,c)λ���Ѿ��ں������жϹ��Ǵ���С��ֵ��
void trace(Mat nonMaxSupMat, Mat &edge, float lowThresh, int r, int c,int rows,int cols)
{
	if (edge.at<float>(r, c) != 0)        //��ǰ���Ѿ�bfs����
		return;

	edge.at<float>(r, c) = 255;           //��Ϊ�жϹ�(r,c)λ�ô���С��ֵ����������ֱ�ӽ����ͼ��Ӧ��λ��ȷ��Ϊ��Ե��
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (checkIsInBounder(r + i, c + j, rows, cols))
			{
				float mag = nonMaxSupMat.at<float>(r + i, c + j);
				if (mag > lowThresh)   //�����ǰ��������Ҷ�ֵ����С��ֵ����ô��������
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

	//˫��ֵ���ͺ���ֵ����:
	//1.�����ǰ��ֵ���ڸ���ֵ����ô�϶��Ǳ�Ե��
	//2.�����ǰ��ֵС�ڵ���ֵ����ô����Ϊ��Ե��ֱ���޳���
	//3.�Ա�Ե��������죬���쾭���ĵ㶼�����ٴ��ڵ���ֵ��
	for (int r = 1; r < rows - 1; r++)
	{
		for (int c = 1; c < cols - 1; c++)
		{
			float mag = nonMaxSupMat.at<float>(r, c);

			//�Ӵ��ڸ���ֵ���ȷ����Ե�����bfs
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
	//canny��Ե����㷨
	Mat src=imread("lena_full.jpg",0);
	imshow("src",src);

	//����:
	//1.�����x,y�����Sobel��Ե������ͱ�Եǿ��
	Mat Sobel_X = (Mat_<float>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
	Mat Sobel_Y = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	flip(Sobel_X, Sobel_X, -1);
	flip(Sobel_Y, Sobel_Y, -1);
	Mat srcX, srcY;               //CV_32FC1���͵����������Ե�����
	filter2D(src, srcX, CV_32FC1, Sobel_X, Point(-1, -1), 0.0, BORDER_CONSTANT);
	filter2D(src, srcY, CV_32FC1, Sobel_Y, Point(-1, -1), 0.0, BORDER_CONSTANT);
	pow(srcX, 2.0, srcX);
	pow(srcY, 2.0, srcY);

	Mat sobelDst;                    //ƽ���͵Ŀ�����Ϊ��Եǿ��
	sqrt(srcX + srcY, sobelDst);      //����ˮƽ�ʹ�ֱ�������Sobel����CV_32FC1����
	Mat sobelDstShow = sobelDst.clone();
	convertScaleAbs(sobelDstShow, sobelDstShow, 1.0);
	imshow("sobelDst", sobelDstShow);

	//2.�Ǽ���ֵ����
	Mat nonMaxSupMat = nonMaximum_suppression_default(srcX, srcY, sobelDst);
	//Mat nonMaxSupMat = non_maximum_suppression_Inter(srcX, srcY);

	Mat nonMaxSupMatShow = nonMaxSupMat.clone();
	convertScaleAbs(nonMaxSupMatShow, nonMaxSupMatShow, 1.0);
	imshow("nonMaxSupMatDefault", nonMaxSupMatShow);
	cout << nonMaxSupMat.type() << endl;
	
	//3.˫��ֵ���ͺ���ֵ����
	Mat edge = hysteresisThreshold(nonMaxSupMat, 20, 100);

	Mat edgeShow = edge.clone();
	convertScaleAbs(edgeShow, edgeShow, 1.0);
	imshow("Canny�ͺ���ֵ��Ľ��", edgeShow);


	//opencv�Դ���canny��Ե��⺯��
	Mat cannyOpencv;
	//Canny(ԭͼ�����ͼ������ֵ������ֵ��Sobel�˴�С���Ƿ�ʹ��ƽ���Ϳ����ı�Եǿ�ȼ��㷽��)
	//���һ������Ϊfalse��ʹ�þ���ֵ��͵ķ��������Եǿ��
	Canny(src,cannyOpencv, 20,100,3,true);
	imshow("cannyOpencv", cannyOpencv);

	waitKey(0);
	return 0;
}