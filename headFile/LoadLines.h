#pragma once
#include "BrowseFolder.h"
#include "TotalHead.h"

//加载直线数据的类
class LineLoader
{
	string lineFolderPath; //存放txt数据的文件夹 最后需要//
	string imgName;  //当前的图片的名字 带后缀
	string txtName;  //txt编号 .txt
	//txt中的数据
public:
	vector<cv::Vec4i> lines; //加载后存放某张图直线的point
	int threshold = 0;	
	float minLineLenth = 0;   
	float maxLineGap = 0;
	int minLen = 0; //显示直线长度 的 最小值

public:
	LineLoader(string _lineFolderPath)
	{
		lineFolderPath = _lineFolderPath;
		imgName = "";
		txtName = "";
	}
	
	bool LoadLines(string fileName);	

};