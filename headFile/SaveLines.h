#pragma once
#include "fstream"
#include "TotalHead.h"

using namespace std;

//������Ϣ
class SaveInformations
{
public:
	//���������浽�ļ� ����Ϊ     /.../(�з�б��)   �ļ���������׺��   ��Ҫ�����Vec4i����
	void saveLines(cv::String path, vector<cv::Vec4i>linePositions,float scale);
	void saveInt(cv::String path,cv::String information, int value, bool needNewFile);

};




