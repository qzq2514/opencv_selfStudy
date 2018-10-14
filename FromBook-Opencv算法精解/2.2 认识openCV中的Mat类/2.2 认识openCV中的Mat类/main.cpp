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
			int *v = (int *)(m5.data + m5.step[0] * r + m5.step[1] * c); //data属性表示整个图像矩阵的首地址,即第一个数值的地址(不是第一个元素)
			cout << *v << ",";											   //step[0]表示图像每一行的字节数
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
		//const int * ptr = m5.ptr<int>(0);  //通过
		const int * ptr = (int *)m5.data;    //data属性得到图像第一个数值的地址，因为单通道中一个元素就是一个数值，所以这里
											 //(int *)data和ptr<int>(0)的效果是一样的

		for (int c = 0; c < m5.total(); c++)  //m5.total()就等于m5.rows*m5.cols,这样即可访问图像矩阵的所有元素
			cout << ptr[c] << ",";
		cout << endl;
	}
	cout << "----------------" << endl;

	//2.2.5向量类Vec
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
	                                 //类似单通道一样快速构造多通道图像，区别就是多通道图像的元素向量，
									 //所以这里Mat_<int>换成了Mat_<Vec3f>
									 //同样，可以类似构造单通道其他方法一样(Mat构造函数，create函数等)构造多通道，只是需要把
									 //类似CV_32FC1改成CV_32FC3就行
	
	//2.2.7访问多通道Mat中的值
	for (int r = 0; r < mm.rows; r++)
	{
		for (int c = 0; c < mm.cols; c++)
		{
			cout << mm.at<Vec3f>(r, c) << ",";  //可以像单通道一样，通过at访问多通道图像，只不过元素从float,int变成Vec3f之类

			//cout << mm.at<Vec3f>(Point(c, r)) << ",";  //同样可以使用Point点类访问元素

			/*Vec3f *ptr = (Vec3f*)(mm.data + r*mm.step[0] + c*mm.step[1]);
			cout << *ptr << ",";*/               //同样使用首地址配合step属性进行元素值访问
		}                                      //step[0]:每一行所占字节数
		cout << endl;						   //step[1]:每个元素所占字节数(等于mm.elemSize()=mm.elemSize1()*mm.channels())
											   //其中mm.elemSize1():每个元素中每个数值所占字节数
	}
	cout << "========qq" << endl;
	for (int r = 0; r < mm.rows; r++)
	{
		Vec3f *ptr = mm.ptr<Vec3f>(r);   //和单通道的const int * ptr = m5.ptr<int>(r)一样得到指定行的首地址
		for (int c = 0; c < mm.cols; c++)//同样要注意将单通道的int元素变成Vex3f之类的元素
		{
			cout << ptr[c] << "," ;
		}
		cout << endl;
	}
	cout << "========" << endl;

	if (mm.isContinuous())
	{
		Vec3f *ptr = mm.ptr<Vec3f>(0);        //和单通道一样，如果判断图像是连续的(即每一行之间没有间隔)
		//Vec3f *ptr = (Vec3f *)mm.data;         //和单通道一样，将首数值指针类型强转为首元素指针类型即可起到同样的效果

		for (int n = 0; n < mm.total(); n++)  //那么可以通过首元素的地址加上一个循环访问所有的元素
			cout << ptr[n] << endl;
	}

	cout << "========" << endl;

	//分离通道
	//分离图像是将被分离的图像的所有向量元素的第一个值组成第一通道，第二个值组成第二个通道
	//分离后得到n个与原图大小相同的单通道矩阵，n等于原图像的通道数，
	/*vector<Mat> planes_split;
	planes_split.resize(3);*/    //release版本下可以直接split,但是debug版本下，直接split会数组越界,除非手动指定vector的长度
	Mat planes_split[3]; //或者可以使用 Mat planes_split[3];
	split(mm, planes_split);

	//所以综上:
	//1.Release下使用vector<Mat>+resize或者 Mat planes[3]
	//2.Debug下只能使用Mat planes[3]


	for (int r = 0; r < planes_split[0].rows; r++)
	{
		for (int c = 0; c < planes_split[0].cols; c++)
		{
			cout << planes_split[0].at<float>(r, c) << ",";
		}
		cout << endl;
	}
	cout << "=========" << endl;

	//合并通道
	//分离的逆操作，将多个单通道相同位置的值组成一个向量，作为合并后图像的一个向量元素
	Mat planes0 = (Mat_<int>(2,3) << 1,2,3,4,5,6);
	Mat planes1 = (Mat_<int>(2, 3) << 11, 12, 13, 14, 15, 16);
	Mat planes2 = (Mat_<int>(2, 3) << 21, 22, 23, 24, 25, 26);
	Mat planes[] = { planes0 ,planes1,planes2 };
	Mat mat;
	merge(planes, 3, mat);

	for (int r = 0; r < mat.rows; r++)
	{
		Vec3i *ptr = mat.ptr<Vec3i>(r);
		for (int c = 0; c < mat.cols; c++)
		{
			cout << ptr[c] << ",";
		}
		cout << endl;
	}
	cout << "========" << endl;


	//2.2.8获得Mat中某一区域的值
	//mm是三通道Vec3f矩阵
	Mat mc = mm.col(2);     //获得某一列元素->>列数等于1
	Mat mr = mm.row(1);     //获得某一行元素->>行数等于1,注意返回的单列和单行，仍然是一个Mat类型
	
	for (int r = 0; r < mc.rows; r++)
	{
		cout << mc.at<Vec3f>(r,0)<<"-";   //这里遇到一个小Bug,原本mc截取的mm的元素是Vec3f，如果就按照该行的at<Vec3f>进行访问是没问题的
		//cout << mc.at<float>(r, 0) << "-"; //但是如果按照at<float>(r, 0)，则返回每一行的第一个值(不是第一个元素)
	}                                    
	cout << endl << "========" << endl;

	for (int c = 0; c < mr.cols; c++)
	{
		cout << mr.at<Vec3f>(0, c) << "-";  //正确访问
		//cout << mr.at<float>(0, c) << "-";  //同样如果这样访问，则是访问值，表示访问第一行的第c个值
	}

	cout << endl<<"=======" << endl;
	Mat matrix = (Mat_<int>(5, 5) << 1,  2,  3,  4,  5,
									 6,  7,  8,  9, 10,
									11, 12, 13, 14, 15,
									16, 17, 18, 19, 20,
									21, 22, 23, 24, 25);

	//Mat r_range = matrix.rowRange(Range(2, 4));  //Range(2, 4)是左闭右开区间，即返回第2行和第3行的元素
	Mat r_range = matrix.rowRange(2, 4);           //rowRange的另一个重载函数可以直接传入行区间的两个端点
												   //同样，选取多个连续的列，可以使用colRange函数
	for (int r = 0; r < r_range.rows; r++)
	{
		for (int c = 0; c < r_range.cols; c++)
		{
			cout << r_range.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << endl << "=======" << endl;

	r_range.at<int>(0, 0) = 10000;            //需要注意的是，之前的row,col,rowRange,colRange函数返回的矩阵都是指向原矩阵的，所以在返回的矩阵上修改
	cout << matrix.at<int>(2, 0) << endl;     //也会影响到原图像矩阵

	matrix.at<int>(2, 0) = 9999;              //改变原矩阵，也会影响rowRange等返回的矩阵，即两者是指向同一个内存空间，两者互相影响
	cout << r_range.at<int>(0, 0) << endl;

	r_range.at<int>(0, 0) = 11;               //改回原来的元素值

	//r_range = matrix.rowRange(2, 4).clone();  //调用Mat类的clone()函数可以克隆和备份原图像矩阵，这样在备份的矩阵上无论怎么修改
	Mat r_range_copy;						  //都不会影响原图像矩阵
	matrix.rowRange(2, 4).copyTo(r_range_copy);  //这里如果直接copyTo(r_range),则还是会影响到原图像矩阵，不知道为什么？？
	r_range_copy.at<int>(0, 0) = 66666;
	cout << matrix.at<int>(2, 0) << endl;     

	//使用矩阵获取图像矩阵区域的三个方法
	cout << "=======roi1" << endl;
	Mat roi1 = matrix(Rect(Point(0, 1), Point(4, 3)));   //左上顶点坐标到右下顶点坐标
	for (int r = 0; r < roi1.rows; r++)                  //注意：之前提到过，Point两个参数依次表示列号和行号
	{													 //其次，需要注意的是返回的矩阵图像不包括第二个参数表示的点所在的行和列
		for (int c = 0; c < roi1.cols; c++)              //相当于还是左闭右开区间
		{
			cout << roi1.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << roi1.rows << ".." << roi1.cols << endl;     //因为是左闭右开区间，所以返回的矩阵是2行4列

	cout << "=======roi2" << endl;
	Mat roi2 = matrix(Rect(0,1,4,2));         //左上坐标(列号，行号)和宽(列数)，高(行数)
	for (int r = 0; r < roi2.rows; r++)
	{
		for (int c = 0; c < roi2.cols; c++)
		{
			cout << roi2.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	cout << "=======roi3" << endl;
	Mat roi3 = matrix(Rect(Point(0, 1),Size(4,2)));  //左上坐标和Size表示的宽高(之前提到过，Size两个参数依次表示宽高)
	for (int r = 0; r < roi3.rows; r++)
	{
		for (int c = 0; c < roi3.cols; c++)
		{
			cout << roi3.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	//和row,col,rowRange,colRange一样，使用矩阵Rect获取的矩阵同样是指向原矩阵的，改变其中任意一个都会影响另一个矩阵
	//同样的，想单独改变roi的值，需要clone()或者copyTo()
	cout << "end........" << endl;
	system("pause");
	return 0;
}