#include "HoughLines.h"



//RGB->GRAY
vector<cv::Vec4i> LineGeter::getLinesFromImage(cv::Mat img_RGB, double theta, int threshold,double minLineLength,double maxLineGap)
{
	
	//首先；利用Canny算子检测边缘 并灰度化
	cv::Mat dstImg;
	Canny(img_RGB, dstImg, 120, 200); 
	
	//转化为彩色就可以在上面标RGB线了
	cvtColor(dstImg, this->linedImg, CV_GRAY2BGR);
	cvtColor(dstImg, this->cannyImgRGB, CV_GRAY2BGR);
	vector<cv::Vec4i> lines;  //存储直线的起止点
	if(threshold > 0)
		HoughLinesP(dstImg, lines, 1, theta, threshold, minLineLength,maxLineGap);
	//得到的直线仅仅是直线的参数
	/*
	rho表示离坐标原点（就是图片左上角的点）的距离，
	theta是直线的旋转角度（rad）。
	第四个参数：最低线段的长度
	第五个参数：同意将同一行点与点之间连接起来的最大的距离
	*/

	//获得直线（起止点）vec
	return lines;
}

vector<cv::Vec4i> LineGeter:: getLinesFromImage(cv::String imgPath, double theta, int threshold, double minLineLength, double maxLineGap)
{
	cv::Mat src = cv::imread(imgPath);
	return getLinesFromImage(src, theta, threshold,minLineLength,maxLineGap);
}


void LineGeter::test(vector<cv::Vec4i> lines)
{
	
	for (int i = 0; i < lines.size(); i++)
	{
		cv::Vec4i l = lines[i];
		line(this->linedImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	}
	cv::namedWindow("DST", 0);
	cv::imshow("DST",this->linedImg);
	cvWaitKey();
}

void LineGeter::updateLinePos(vector<cv::Vec4i> addVecs, vector<cv::Vec4i> delVecs, float scale)
{
	cout << "scale:" << scale << endl;
	//使用迭代器查找del 并清除

	vector<cv::Vec4i>::iterator Vdel;
	for (int i = 0; i < delVecs.size(); i++)
	{	
		Vdel = find(lines.begin(), lines.end(), delVecs[i]);
		if (Vdel != lines.end())
		{
			lines.erase(Vdel);
			cout << "erase ok! " << endl;
		}
		else
			cout << "not find" << endl;
	}
			
	
	//拼接
	cout << "addVecsSize:" << addVecs.size() << endl;
	lines.insert(lines.end(), addVecs.begin(), addVecs.end());

}


