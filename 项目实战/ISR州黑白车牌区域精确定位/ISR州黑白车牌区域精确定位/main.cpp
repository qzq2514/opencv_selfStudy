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
		//默认只有一个车牌，只要找到一个符合的车牌框就直接返回
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

	//便利图片文件夹
	struct _finddata_t fileinfo;
	//字符串格式化，这里将路径和"*.*"进行拼接，得到该路径下所有文件匹配的正则表达形式
	sprintf_s(filespec, "%s*.*", workFloderPath.c_str());
	cout << filespec << endl;
	//_findfirst找到路径符合filespec正则表达形式的第一个文件/文件夹,
	//将文件信息放在fileinfo中,例如文件名等
	intptr_t filehandle = _findfirst(filespec, &fileinfo);
	int id = 0;
	if (filehandle != -1)    //该路径形式下存在文件
	{
		while (_findnext(filehandle, &fileinfo) != -1)  //不断循环文件
		{

			string filename = fileinfo.name;   //当前遍历的文件名
			string filetype = filename.substr(filename.rfind('.') + 1);
			if (filetype != "jpg" && filetype != "png" && filetype != "jpeg" && filetype != "bmp")
				continue;
			if (filename == "")
				continue;
			Mat img = imread(workFloderPath + filename);
			Rect plateRect = closePlateRect(img);
			//找到车牌则保存
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
	cout << "pos文件数:" << id << endl;
}
void generateNeg()
{
	string workFloderPath = "plate\\negPre\\";
	char filespec[255];
	//Mat d=imread("plateRegionCascade\\lena_full.jpg");
	//imshow("d", d);
	//waitKey(0);

	//便利图片文件夹
	struct _finddata_t fileinfo;
	//字符串格式化，这里将路径和"*.*"进行拼接，得到该路径下所有文件匹配的正则表达形式
	sprintf_s(filespec, "%s*.*", workFloderPath.c_str());
	cout << filespec << endl;
	//_findfirst找到路径符合filespec正则表达形式的第一个文件/文件夹,
	//将文件信息放在fileinfo中,例如文件名等
	intptr_t filehandle = _findfirst(filespec, &fileinfo);
	int id = 0;
	if (filehandle != -1)    //该路径形式下存在文件
	{
		while (_findnext(filehandle, &fileinfo) != -1)  //不断循环文件
		{

			string filename = fileinfo.name;   //当前遍历的文件名
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
	cout << "neg文件数:" << id << endl;
}
int main()
{
	//generateNeg();
	generatePos();
	system("pause");
	return 0;
}