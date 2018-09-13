#pragma once
//浏览文件夹  返回文件夹下的路径
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
	vector<string> fileNames;  //文件名（不包含路径） 有后缀
	void getFilePathsInFolder(string format);
	void getFilePathsInFolder(string fp, string format);
	void test();
};