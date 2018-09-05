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
	vector<Mat>channels;
	Mat imageBlueChannel;

	logoImage = imread("logo.jpg",0);
	srcImage = imread("scene.jpg");
	if (!logoImage.data) { printf("��ȡlogo.jpg����~"); return false; }
	if (!srcImage.data) { printf("��ȡscene.jpg����~"); return false; }

    //��ԭͼ����ͨ����ɫͼƬת����������ͨ��ͼƬ������һ��vector��
	//vector��ÿ��Ԫ����ԭͼ��һ��ͨ��
	split(srcImage, channels);

	//���ԭͼ�ĵ�0��ͨ��-��ɫͨ��
	imageBlueChannel = channels.at(0);    //vector��at�������±���������[]һ�������ã�ֻ��at���Ӱ�ȫ����Ч�ʵ�
	                                       //ע����Ϊ����vector��Ԫ����Mat,�����������at��ֵ�����ã����޸�
										//imageBlueChannel����channels.at(0)�ǵȼ۵�

	Mat blueROI = imageBlueChannel(Rect(20, 30, logoImage.cols, logoImage.rows));
	addWeighted(blueROI, 1.0,logoImage, 0.5,0, blueROI);  //ע��֮ǰ˵��addWeighted������ͼƬ������Ҫ����һ����ҲҪͨ����һ��
													//����logoImage�ڶ�����Ҫ�ǵ�ͨ��ͼ,��imread�ڶ���������0

	//channels�е���ɫͨ����������logoͼ��Ȼ������������ɫ(.at(1)),����(.at(2))ͨ���ϲ�����
	merge(channels, srcImage);

	imshow("��ɫͨ�����", srcImage);
	return true;




}
