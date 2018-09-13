#include "SaveLines.h"


void SaveInformations::saveLines(cv::String path, vector<cv::Vec4i> linePositions, float scale)
{
	ofstream in;	
	in.open(path, ios::app); //在文件尾添加
	//添加并缩放
	for (int i = 0; i < linePositions.size(); i++)
	{
		//保存格式<x1,y1><x2,y2>
		in << "<" << (int)(((float)linePositions[i].val[0]) * scale)  << "," << (int)(((float)linePositions[i].val[1]) * scale)
			<< "><" << (int)(((float)linePositions[i].val[2]) * scale) << "," << (int)(((float)linePositions[i].val[3]) * scale)
			<< ">" << endl;

	}
	in.close();
	in.flush();
}

void SaveInformations::saveInt(cv::String path,cv::String information, int value, bool needNewFile)
{
	ofstream in;
	if (needNewFile)
	{
		in.open(path, ios::trunc);
	}
	else
	{
		in.open(path, ios::app);
	}
	in << information << ":" << value << endl;
	in.close();
	in.flush();
}
