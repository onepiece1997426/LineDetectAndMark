#pragma once
//����ļ���  �����ļ����µ�·��
#include "afxwin.h"
#include <fstream>
#include <sstream>
#include "io.h"
#include <shlobj.h>
#include "vector"
#include "string"

using namespace std;


class FolderBrowser
{
	
public:
	FolderBrowser()
	{
		folderPath = "";
		fileNames = vector<string>();
	}
	string folderPath;
	vector<string> fileNames;  //�ļ�����������·���� �к�׺
	void getFilePathsInFolder(string format);
	void getFilePathsInFolder(string fp, string format);
	void test();
};