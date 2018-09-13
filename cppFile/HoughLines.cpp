#include "HoughLines.h"



//RGB->GRAY
vector<cv::Vec4i> LineGeter::getLinesFromImage(cv::Mat img_RGB, double theta, int threshold,double minLineLength,double maxLineGap)
{
	
	//���ȣ�����Canny���Ӽ���Ե ���ҶȻ�
	cv::Mat dstImg;
	Canny(img_RGB, dstImg, 120, 200); 
	
	//ת��Ϊ��ɫ�Ϳ����������RGB����
	cvtColor(dstImg, this->linedImg, CV_GRAY2BGR);
	cvtColor(dstImg, this->cannyImgRGB, CV_GRAY2BGR);
	vector<cv::Vec4i> lines;  //�洢ֱ�ߵ���ֹ��
	if(threshold > 0)
		HoughLinesP(dstImg, lines, 1, theta, threshold, minLineLength,maxLineGap);
	//�õ���ֱ�߽�����ֱ�ߵĲ���
	/*
	rho��ʾ������ԭ�㣨����ͼƬ���Ͻǵĵ㣩�ľ��룬
	theta��ֱ�ߵ���ת�Ƕȣ�rad����
	���ĸ�����������߶εĳ���
	�����������ͬ�⽫ͬһ�е����֮���������������ľ���
	*/

	//���ֱ�ߣ���ֹ�㣩vec
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
	//ʹ�õ���������del �����

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
			
	
	//ƴ��
	cout << "addVecsSize:" << addVecs.size() << endl;
	lines.insert(lines.end(), addVecs.begin(), addVecs.end());

}


