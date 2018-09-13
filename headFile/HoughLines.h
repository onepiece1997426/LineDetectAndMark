#pragma once

#include "TotalHead.h"
using namespace std;

/*
	从图片中得到直线
	HoughLinesP：在HoughLines的基础上在末尾加了一个代表Probabilistic（概率）的P，表明使用的是累计概率变换。
	输入：img RGB图片
		  theta 每次迭代的时候的直线旋转的角度
		  threshold 多少个点共线的时候（累加器的门限），大于该值则认为这东西是个直线
*/

class LineGeter
{
public:
	cv::Mat linedImg;   //保存在直线检测后的图像 他会在main中被resize
	cv::Mat rawImg;  //resize之后的原始图像  如果图大，这俩图像都会在main中resize成和显示区域差不多的图像
	cv::Mat cannyImgRGB; //保存canny算子之后的RGB黑白图像 在main中被resize
	vector<cv::Vec4i> lines; //保存的是 缩放之后 点的位置 要不会出许多问题 保存的时候城回去
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




