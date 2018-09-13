#include "BrowseFolder.h"



string createBroseWindow()
{

	CString str;
	BROWSEINFO bi; //BROWSEINFO�ṹ�а������û�ѡ��Ŀ¼����Ҫ��Ϣ��
	TCHAR name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));	
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = name;

	//���öԻ��� ����ȡ��Ϣ
	BIF_BROWSEINCLUDEFILES;
	bi.lpszTitle = _T("ѡ���ļ���");
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

		this->fileNames.clear();//�ȳ�ʼ��
		string fileFolder = this->folderPath + "*." + format; //����ʽ���ļ���

		char fileName[2000]; //�ļ���
		struct _finddata_t fileInfo;
		// struct _finddata_t ������ṹ���������洢�ļ�������Ϣ��

		/*
		����_findfirst���ҵ�һ���ļ���
		���ɹ����÷��صľ������_findnext���������������ļ���
		��������Ϻ��ã���_findclose�����������ҡ�
		*/
		intptr_t findResult = _findfirst(fileFolder.c_str(), &fileInfo);
		if (findResult == -1)
		{
			_findclose(findResult); //��������
			return;
		}

		do
		{
			//��ָ����ʽд��buffer
			//sprintf(fileName, "%s%s", folderPath.c_str(), fileInfo.name);
			if (fileInfo.attrib == _A_ARCH)
			{
				//������ļ�������Ϊһ���浵
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

		this->fileNames.clear();//�ȳ�ʼ��
		string file = this->folderPath + "*." + format; //����ʽ���ļ���

		char fileName[2000]; //�ļ���
		struct _finddata_t fileInfo;
		// struct _finddata_t ������ṹ���������洢�ļ�������Ϣ��

		/*
		����_findfirst���ҵ�һ���ļ���
		���ɹ����÷��صľ������_findnext���������������ļ���
		��������Ϻ��ã���_findclose�����������ҡ�
		*/
		intptr_t findResult = _findfirst(file.c_str(), &fileInfo);
		if (findResult == -1)
		{
			_findclose(findResult); //��������
			return;
		}

		do
		{
			//��ָ����ʽд��buffer
			//sprintf(fileName, "%s%s", folderPath.c_str(), fileInfo.name);
			if (format == "txt")
			{
				if (fileInfo.attrib == _A_NORMAL) //txt�ļ�
				{
					//������ļ�������Ϊһ���浵
					this->fileNames.push_back(fileInfo.name);
					//outf << fileInfo.name << endl;
				}
			}
			else
			{
				if (fileInfo.attrib == _A_ARCH) //��TXT��
				{
					//������ļ�������Ϊһ���浵
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



