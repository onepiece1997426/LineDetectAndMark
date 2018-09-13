#pragma once

#include "TotalHead.h"
using namespace std;

/*
	��ͼƬ�еõ�ֱ��
	HoughLinesP����HoughLines�Ļ�������ĩβ����һ������Probabilistic�����ʣ���P������ʹ�õ����ۼƸ��ʱ任��
	���룺img RGBͼƬ
		  theta ÿ�ε�����ʱ���ֱ����ת�ĽǶ�
		  threshold ���ٸ��㹲�ߵ�ʱ���ۼ��������ޣ������ڸ�ֵ����Ϊ�ⶫ���Ǹ�ֱ��
*/

class LineGeter
{
public:
	cv::Mat linedImg;   //������ֱ�߼����ͼ�� ������main�б�resize
	cv::Mat rawImg;  //resize֮���ԭʼͼ��  ���ͼ������ͼ�񶼻���main��resize�ɺ���ʾ�������ͼ��
	cv::Mat cannyImgRGB; //����canny����֮���RGB�ڰ�ͼ�� ��main�б�resize
	vector<cv::Vec4i> lines; //������� ����֮�� ���λ�� Ҫ������������ �����ʱ��ǻ�ȥ
public:
	LineGeter()
	{
		linedImg = cv::Mat();
		rawImg = cv::Mat();
		cannyImgRGB = cv::Mat();
	}
	vector<cv::Vec4i> getLinesFromImage(cv::Mat img_RGB, double theta, int threshold, double minLineLength, double maxLineGap);
	vector<cv::Vec4i> getLinesFromImage(cv::String imgPath, double theta, int threshold, double minLineLength, double maxLineGap);
	void test(vector<cv::Vec4i> lines);
	void updateLinePos(vector<cv::Vec4i> addVecs, vector<cv::Vec4i> delVecs,float scale);
};




