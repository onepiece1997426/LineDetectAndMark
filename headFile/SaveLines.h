#pragma once
#include "fstream"
#include "TotalHead.h"

using namespace std;

//保存信息
class SaveInformations
{
public:
	//从向量保存到文件 输入为     /.../(有反斜杠)   文件名（带后缀）   需要保存的Vec4i向量
	void saveLines(cv::String path, vector<cv::Vec4i>linePositions,float scale);
	void saveInt(cv::String path,cv::String information, int value, bool needNewFile);

};




