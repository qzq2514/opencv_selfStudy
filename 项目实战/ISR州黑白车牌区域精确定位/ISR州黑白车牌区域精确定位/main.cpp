#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include <io.h>
#include <fstream>

using namespace std;
using namespace cv;
Rect closePlateRect(Mat src)
{
	Mat srcGray;
	cvtColor(src, srcGray, CV_RGB2GRAY);
	Mat srcThresh;
	threshold(srcGray, srcThresh, 100, 255, THRESH_OTSU+ THRESH_BINARY);


	vector<vector<Point>> contours;
	findContours(srcThresh, contours, RETR_TREE,CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		//Ĭ��ֻ��һ�����ƣ�ֻҪ�ҵ�һ�����ϵĳ��ƿ��ֱ�ӷ���
		Rect bound = boundingRect(contours[i]);
		if (bound.area() > src.total()*0.1 && bound.width > bound.height * 2)
			return bound;
	}
	return Rect(0,0,0,0);
}

void generatePos()
{
	string workFloderPath = "plateRegionCascade\\";
	char filespec[255];
	//Mat d=imread("plateRegionCascade\\lena_full.jpg");
	//imshow("d", d);
	//waitKey(0);

	//����ͼƬ�ļ���
	struct _finddata_t fileinfo;
	//�ַ�����ʽ�������ｫ·����"*.*"����ƴ�ӣ��õ���·���������ļ�ƥ�����������ʽ
	sprintf_s(filespec, "%s*.*", workFloderPath.c_str());
	cout << filespec << endl;
	//_findfirst�ҵ�·������filespec��������ʽ�ĵ�һ���ļ�/�ļ���,
	//���ļ���Ϣ����fileinfo��,�����ļ�����
	intptr_t filehandle = _findfirst(filespec, &fileinfo);
	int id = 0;
	if (filehandle != -1)    //��·����ʽ�´����ļ�
	{
		while (_findnext(filehandle, &fileinfo) != -1)  //����ѭ���ļ�
		{

			string filename = fileinfo.name;   //��ǰ�������ļ���
			string filetype = filename.substr(filename.rfind('.') + 1);
			if (filetype != "jpg" && filetype != "png" && filetype != "jpeg" && filetype != "bmp")
				continue;
			if (filename == "")
				continue;
			Mat img = imread(workFloderPath + filename);
			Rect plateRect = closePlateRect(img);
			//�ҵ������򱣴�
			if (plateRect.height > 0 && plateRect.width > 0)
			{
				/*Mat plateShow = img.clone();
				rectangle(plateShow, plateRect, Scalar(0, 0, 255), 2);
				imshow("plateShow", plateShow);
				waitKey(0);*/
				stringstream saveFileName;
				saveFileName << "plate\\pos\\" << id << ".jpg";
				imwrite(saveFileName.str(), img(plateRect));
				cout << filename << "-----" << id << ".jpg" << endl;

				ofstream posTxt;
				posTxt.open("plate\\pos.txt", ios::app);
				posTxt << "pos/" << id << ".jpg" << " 1 0 0 " << plateRect.width << " " << plateRect.height << endl;
				posTxt.close();
				id++;
			}
		}
	}
	cout << "pos�ļ���:" << id << endl;
}
void generateNeg()
{
	string workFloderPath = "plate\\negPre\\";
	char filespec[255];
	//Mat d=imread("plateRegionCascade\\lena_full.jpg");
	//imshow("d", d);
	//waitKey(0);

	//����ͼƬ�ļ���
	struct _finddata_t fileinfo;
	//�ַ�����ʽ�������ｫ·����"*.*"����ƴ�ӣ��õ���·���������ļ�ƥ�����������ʽ
	sprintf_s(filespec, "%s*.*", workFloderPath.c_str());
	cout << filespec << endl;
	//_findfirst�ҵ�·������filespec��������ʽ�ĵ�һ���ļ�/�ļ���,
	//���ļ���Ϣ����fileinfo��,�����ļ�����
	intptr_t filehandle = _findfirst(filespec, &fileinfo);
	int id = 0;
	if (filehandle != -1)    //��·����ʽ�´����ļ�
	{
		while (_findnext(filehandle, &fileinfo) != -1)  //����ѭ���ļ�
		{

			string filename = fileinfo.name;   //��ǰ�������ļ���
			string filetype = filename.substr(filename.rfind('.') + 1);
			if (filetype != "jpg" && filetype != "png" && filetype != "jpeg" && filetype != "bmp")
				continue;
			if (filename == "")
				continue;
			Mat img = imread(workFloderPath + filename);
			stringstream saveFileName;
			saveFileName << "plate\\neg\\" << id << ".jpg";
			imwrite(saveFileName.str(), img);
			cout << filename << "-----" << id << ".jpg" << endl;

			ofstream posTxt;
			posTxt.open("plate\\neg.txt", ios::app);
			posTxt << "neg/" << id << ".jpg"<< endl;
			posTxt.close();
			id++;
		}
	}
	cout << "neg�ļ���:" << id << endl;
}
int main()
{
	//generateNeg();
	generatePos();
	system("pause");
	return 0;
}