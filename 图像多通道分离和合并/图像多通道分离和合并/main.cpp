#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

bool MultiChannelBlending();
int main()
{
	//system("color5E");
	MultiChannelBlending();
	waitKey(0);
	return 0;
}

bool MultiChannelBlending()
{
	Mat srcImage;
	Mat logoImage;
	Mat imageBlueChannel;

	logoImage = imread("logo.jpg",0);
	srcImage = imread("scene.jpg");
	if (!logoImage.data) { printf("��ȡlogo.jpg����~"); return false; }
	if (!srcImage.data) { printf("��ȡscene.jpg����~"); return false; }

    //��ԭͼ����ͨ����ɫͼƬת����������ͨ��ͼƬ������һ��vector��
	//vector��ÿ��Ԫ����ԭͼ��һ��ͨ��
	/*vector<Mat>channels;
	channels.resize(3);*/          //release�汾�¿���ֱ��split,����debug�汾�£�ֱ��split������Խ��,�����ֶ�ָ��vector�ĳ���
	Mat channels[3]; //���߿���ʹ�� Mat planes[3];
	split(srcImage, channels);

	//��������:
	//1.Release��ʹ��vector<Mat>+resize���� Mat planes[3]
	//2.Debug��ֻ��ʹ��Mat planes[3]
	    

	//���ԭͼ�ĵ�0��ͨ��-��ɫͨ��
	imageBlueChannel = channels[0];
	//imageBlueChannel = channels.at(0);    //vector��at�������±���������[]һ�������ã�ֻ��at���Ӱ�ȫ����Ч�ʵ�
	                                       //ע����Ϊ����vector��Ԫ����Mat,�����������at��ֵ�����ã����޸�
										//imageBlueChannel����channels.at(0)�ǵȼ۵�
	
	Mat blueROI = imageBlueChannel(Rect(20, 30, logoImage.cols, logoImage.rows));
	addWeighted(blueROI, 1.0,logoImage, 0.5,0, blueROI);  //ע��֮ǰ˵��addWeighted������ͼƬ������Ҫ����һ����ҲҪͨ����һ��
													//����logoImage�ڶ�����Ҫ�ǵ�ͨ��ͼ,��imread�ڶ���������0

	//channels�е���ɫͨ����������logoͼ��Ȼ������������ɫ(.at(1)),����(.at(2))ͨ���ϲ�����
	//merge(channels, srcImage);    //srcImage��vector<Mat>�ĺϲ���ʽ
	merge(channels, 3, srcImage);   //srcImage��Mat[] �ĺϲ���ʽ

	imshow("��ɫͨ�����", srcImage);
	return true;




}
