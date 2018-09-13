#pragma once
#include "BrowseFolder.h"
#include "TotalHead.h"

//����ֱ�����ݵ���
class LineLoader
{
	string lineFolderPath; //���txt���ݵ��ļ��� �����Ҫ//
	string imgName;  //��ǰ��ͼƬ������ ����׺
	string txtName;  //txt��� .txt
	//txt�е�����
public:
	vector<cv::Vec4i> lines; //���غ���ĳ��ͼֱ�ߵ�point
	int threshold = 0;	
	float minLineLenth = 0;   
	float maxLineGap = 0;
	int minLen = 0; //��ʾֱ�߳��� �� ��Сֵ

public:
	LineLoader(string _lineFolderPath)
	{
		lineFolderPath = _lineFolderPath;
		imgName = "";
		txtName = "";
	}
	
	bool LoadLines(string fileName);	

};