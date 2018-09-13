#include "BrowseFolder.h"



string createBroseWindow()
{

	CString str;
	BROWSEINFO bi; //BROWSEINFO结构中包含有用户选中目录的重要信息。
	TCHAR name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));	
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = name;

	//设置对话框 并读取信息
	BIF_BROWSEINCLUDEFILES;
	bi.lpszTitle = _T("选择文件夹");
	bi.ulFlags = 0x80;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	string folderPath = "";
	folderPath = CT2A(str.GetBuffer());
	if (str.GetLength() > 1)
	{
		if (str.GetAt(str.GetLength() - 1) != '\\')
			folderPath += "\\";
		//AfxGetMainWnd()->UpdateData(FALSE);

		IMalloc *imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(idl);
			imalloc->Release();
		}
	}
	return folderPath;
}


void FolderBrowser::getFilePathsInFolder(string format)
{
	string fpath = createBroseWindow();
	if (fpath != "")
	{
		this->folderPath = fpath;

		this->fileNames.clear();//先初始化
		string fileFolder = this->folderPath + "*." + format; //带格式的文件名

		char fileName[2000]; //文件名
		struct _finddata_t fileInfo;
		// struct _finddata_t ，这个结构体是用来存储文件各种信息的

		/*
		先用_findfirst查找第一个文件，
		若成功则用返回的句柄调用_findnext函数查找其他的文件，
		当查找完毕后用，用_findclose函数结束查找。
		*/
		intptr_t findResult = _findfirst(fileFolder.c_str(), &fileInfo);
		if (findResult == -1)
		{
			_findclose(findResult); //结束查找
			return;
		}

		do
		{
			//以指定格式写入buffer
			//sprintf(fileName, "%s%s", folderPath.c_str(), fileInfo.name);
			if (fileInfo.attrib == _A_ARCH)
			{
				//如果该文件的属性为一个存档
				this->fileNames.push_back(fileInfo.name);
				//outf << fileInfo.name << endl;
			}
		} while (_findnext(findResult, &fileInfo) == 0);

		_findclose(findResult);
	}
}


void FolderBrowser::getFilePathsInFolder(string fp, string format)
{
	string fpath = fp;
	if (fpath != "")
	{
		this->folderPath = fpath;

		this->fileNames.clear();//先初始化
		string file = this->folderPath + "*." + format; //带格式的文件名

		char fileName[2000]; //文件名
		struct _finddata_t fileInfo;
		// struct _finddata_t ，这个结构体是用来存储文件各种信息的

		/*
		先用_findfirst查找第一个文件，
		若成功则用返回的句柄调用_findnext函数查找其他的文件，
		当查找完毕后用，用_findclose函数结束查找。
		*/
		intptr_t findResult = _findfirst(file.c_str(), &fileInfo);
		if (findResult == -1)
		{
			_findclose(findResult); //结束查找
			return;
		}

		do
		{
			//以指定格式写入buffer
			//sprintf(fileName, "%s%s", folderPath.c_str(), fileInfo.name);
			if (format == "txt")
			{
				if (fileInfo.attrib == _A_NORMAL) //txt文件
				{
					//如果该文件的属性为一个存档
					this->fileNames.push_back(fileInfo.name);
					//outf << fileInfo.name << endl;
				}
			}
			else
			{
				if (fileInfo.attrib == _A_ARCH) //非TXT？
				{
					//如果该文件的属性为一个存档
					this->fileNames.push_back(fileInfo.name);
					//outf << fileInfo.name << endl;
				}
			}
		} while (_findnext(findResult, &fileInfo) == 0);

		_findclose(findResult);
	}
}

void FolderBrowser::test()
{
	getFilePathsInFolder("jpg");
	
}



