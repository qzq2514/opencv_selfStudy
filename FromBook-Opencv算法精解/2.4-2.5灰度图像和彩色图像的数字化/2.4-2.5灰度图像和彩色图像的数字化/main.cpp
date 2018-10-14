#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat imgColor = imread("lena_full.jpg", IMREAD_COLOR);
	//cout << IMREAD_GRAYSCALE << endl;    //0 ��ԭͼ��Ϊ�Ҷ�ͼ��ȡ
	//cout << IMREAD_COLOR << endl;        //1 ��Ϊ��ͼ��ȡ
	

	//RGB��ɫͼƬ��ÿ��Ԫ����һ������Ϊ3������������Vec3b
	if (imgColor.empty())
	{
		cout << "Cant't fing the pic..." << endl;
		system("pause");
		return -1;
	}
	//cout << "imgColor.channels():" <<  imgColor.channels() << endl;   //��ͨ��RGB��ͼ
	//cout << imgColor.rows << ":" << imgColor.cols << endl;
	//cout << "type:"<<imgColor.type() << endl;    //��������ֵ��ÿһֵ��Ӧһ��MatԪ������,���Ｔ�ɸ��ݶ�Ӧֵ�õ�MatԪ�ص�����
	//cout << "CV_8UC3:" << CV_8UC3 << endl;     //CV_8UC3=8
	//cout << "imgColor.at<Vec3b>(r, c):" << imgColor.at<Vec3b>(2, 3) << endl;
	imshow("BGR", imgColor);


	/*vector<Mat> planes;
	planes.resize(3);*/   //release�汾�¿���ֱ��split,����debug�汾�£�ֱ��split������Խ��,�����ֶ�ָ��vector�ĳ���
	Mat planes[3];    //���߿���ʹ�� Mat planes[3];
	split(imgColor, planes);  
	
	//��������:
	//1.Release��ʹ��vector<Mat>+resize���� Mat planes[3]
	//2.Debug��ֻ��ʹ��Mat planes[3]

	//Ҫע��ÿһ�����ص������е�Ԫ��ֵ�����ǰ���R,G,B��˳�����е�
	//���ǰ���B,G,R��˳�������������εõ������̣���ɫͨ���ĵ�ͨ��ͼ��
	imshow("channal0", planes[0]);  
	imshow("channal1", planes[1]);
	imshow("channal2", planes[2]);
	cout << planes[2].channels() << endl;

	//�Լ���С��ϰ������ɫͼ��Ϊ�Ҷ�ͼ
	Mat myGray;
	myGray.create(imgColor.rows, imgColor.cols, CV_8UC1);   //�Ҷ�ͼ�϶��ǵ�ͨ����
    //����Ҫѡ����ʵ�ͼ�����ͣ��������ĻҶ�ͼ������ѡ��CV_8UC1
	//��Ҫ���ԭ��ͼ�������ֵ��Ҫͨ��type()�����õ���ɫͼƬ�����ͣ���������imgColor.type()=8
	//ͨ�����õ���CV_8UC3���ͣ���Vec3b
	for(int r=0;r<myGray.rows;r++)
		for (int c = 0; c < myGray.cols; c++)
		{
			//[0],[1],[2]���εõ�ָ�������ϵ������̣���ͨ����ֵ
			 uchar pixel = 0.2*imgColor.at<Vec3b>(r, c)[0] +
							0.2*imgColor.at<Vec3b>(r, c)[1] +
							0.5*imgColor.at<Vec3b>(r, c)[2];
			myGray.at<uchar>(r, c) = pixel;
		}
	imshow("myGray", myGray);
	waitKey();      //��Ҫ��system(""pause);
	return 0;
}