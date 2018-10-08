#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//2.2.2构造单通道Mat对象
	Mat m1 = Mat(2, 3, CV_32FC1);   //定义2行3列图像矩阵，每个元素是32位float类型,单通道

	Mat m2 = Mat(Size(3, 2), CV_32FC1);    //以Size给出图像矩阵大小，方式:Size(列数，行数)

	Mat m3;
	m3.create(Size(3, 2), CV_32FC1);      //使用Mat类的create函数创建图像

	Mat ones = Mat::ones(2, 3, CV_32FC1); //创建全1矩阵(白色)
	Mat zeros = Mat::zeros(2, 3, CV_32FC1); //创建全0矩阵(黑色)

	Mat m4 = (Mat_<int>(2, 3) << 1, 2, 3, 4, 5, 6);  //快速创建2行3列，元素为int的小型图像矩阵
	cout << "---------------------" << endl;

	//2.2.3获得单通道Mat类基本信息
	cout << m4.rows << endl;
	cout << m4.cols << endl;              //图像矩阵的行数(高)和列数(宽)
	cout << m4.channels() << endl;        //图像通道数-1
	cout << m4.total() << endl;           //图像行数乘列数(与通道数无关)
	cout << m4.dims << endl;              //图像维数-单通道图像是二维的

	Size s = m4.size();   //[3*2]-列在前，行在后
	cout << s << endl;
	cout << "---------------------" << endl;

	//2.2.4访问单通道Mat对象的值
	Mat m5 = (Mat_<int>(3, 2) << 1, 2, 3, 4, 5, 6);
	for (int r = 0; r < m5.rows; r++)
	{
		for (int c = 0; c < m5.cols; c++)
		{
			//cout << m5.at<int>(r, c) << ",";  //1.使用Mat对象的at函数访问，<int>表示单通道且数据类型为CV_32SC1
											  //访问r行c列的图像元素

			//cout << m5.at<int>(Point(c, r)) << ",";     //2.使用坐标形式访问图像矩阵的元素，
													//第一个元素是列坐标，第二个元素是行坐标

			                                           //3.使用step和data访问图像矩阵
			int v = *((int *)(m5.data + m5.step[0] * r + m5.step[1] * c)); //data属性表示整个图像矩阵的首地址
			cout << v << ",";											   //step[0]表示图像每一行的字节数
		}									                               //step[1]表示图像每一个元素所占字节数
		cout << endl;
	}

	cout << "=======" << endl;
	for (int r = 0; r < m5.rows; r++)
	{
		const int * ptr = m5.ptr<int>(r);     //4.ptr函数获得指定行的行首地址
		for (int c = 0; c < m5.cols; c++)
		{
			cout << ptr[c] << ",";           //通过行首地址指针访问该行第c列元素
		}
		cout << endl;
	}

	cout << "=======" << endl;
	//矩阵每一行的值都存储在连续的内存中，但是行与行之间可能会有间隔
	//使用isContinuous()函数判断行与行之间是不是连续的
	if (m5.isContinuous())
	{
		const int * ptr = m5.ptr<int>(0);
		for (int c = 0; c < m5.total(); c++)  //m5.total()就等于m5.rows*m5.cols,单通道下，这样即可访问图像矩阵的所有元素
			cout << ptr[c] << ",";
		cout << endl;
	}
	cout << "----------------" << endl;

	//2.2.6向量类Vec
	//单通道图像的元素是数值，例如1,2...
	//多通道图像的元素是向量(Vec),向量的长度是图像的通道数

	Vec<int, 3> vi(21, 22, 23);
	cout << vi[0] << "," << vi[1] << endl;  //通过下标访问向量的元素
	cout << vi.rows << "," << vi.cols << endl;  //同样可以得到向量的行数和列数

	/*OpenCV为向量类的声明取了别名，如下:
     typedef Vec<uchar,3> Vec3b
	 typedef Vec<int,2> Vec2i
	 typedef Vec<float,4> Vec4f
	 typedef Vec<double,3> Vec3d
	 */
	cout << "-------------------" << endl;

	//2.2.6 构造多通道Mat对象
	Mat mm = (Mat_<Vec3f>(2, 3) << Vec3f(1, 2, 3), Vec3f(4, 5, 6), Vec3f(7, 8, 9),
								   Vec3f(10, 11, 12), Vec3f(13, 14, 15), Vec3f(16, 17, 18));  
	                                 //类似单通道快速构造多通道图像，区别就是多通道图像的元素向量，
									 //所以这里Mat_<int>换成了Mat_<Vec3f>
									 //同样，可以类似构造单通道其他方法一样构造多通道，只是需要把
									 //类似CV_32FC1改成CV_32FC3就行



	cout << "end........" << endl;
	system("pause");
	return 0;
}