#include "HoughLines.h"
#include "BrowseFolder.h"
#include "MouseHandle.h"
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "SaveLines.h"
#include "LoadLines.h"

#define MAX_DISPLAY_WIDTH  520  //�����ʾͼƬ�Ĵ�С ��������
#define MAX_DISPLAY_HEIGHT	520
#define SAVEFOLDERPATH "D:\\Datas\\Buildings\\Lines\\"		//�����ļ���Ŀ¼
#define LOADTXTPATH "D:\\Datas\\Buildings\\Lines\\" 

float caculateImgScale(cv::Mat img);

void main()
{
	LineGeter* lineGeter = new LineGeter();
	
	cv::namedWindow("MAINWINDOW",0);
	cvui::init("MAINWINDOW");
	cv::namedWindow("RawImg"); //��ʾԭʼͼ��Ĵ���
	cvui::watch("RawImg");
	cv::namedWindow("LineLoader"); //���صĴ���
	cvui::watch("LineLoader");
	
	cv::Mat backGroundImg = cv::imread("E:/CCCCCCCC++++++/Dadi.jpg");	//����ͼƬ
	
	FolderBrowser folderBrowser = FolderBrowser();
	cv::Mat choosedMat = cv::Mat();	

	MouseHandle mouseHandle = MouseHandle();
	SaveInformations saveInformations = SaveInformations();
	LineLoader lineLoader = LineLoader(LOADTXTPATH);
	//���ڲ���
	int interval = 10;  //�ؼ����
	int win_width = ((MAX_DISPLAY_WIDTH + 2 * interval) * 5) / 4;
	int win_height = ((MAX_DISPLAY_HEIGHT + 2 * interval) * 5) / 4;
	cv::resize(backGroundImg, backGroundImg, cv::Size(win_width, win_height)); //������ɫ
	cv::Mat backGroundColor = cv:: Mat(win_height, win_width,CV_8UC3);  //������bg
	cv::Mat backGroundColor2 = cv::Mat(win_height, win_width, CV_8UC3);  //ԭʼͼƬbg

	int buttonW = win_width / 5; //��ť����
	int browseListH = 4 * win_height / 5;//����ļ���window�ĳ���
	int check_useHoughX = buttonW + interval; //�Ƿ����û���任��ť�Ŀ�ʼ
	int trackBarY = buttonW / 2; //����������Y
	int trackbarW = ((win_width * 3) / 5 - (3 * 2 * interval)) / 3;
	int trackabar1X = buttonW + interval;
	int trackabar2X = trackabar1X + trackbarW + 2 * interval;
	int trackabar3X = trackabar2X + trackbarW + 2 * interval;
	cv::Rect *showImgRect = new cv::Rect(buttonW + 10, buttonW + 10, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT);
	cv::Size *scaledSize = new cv::Size(1, 1);  //���ź�Ĵ�С
	
	bool* chooseBools = new bool;  //ѡ����ѡ��
	bool useHoughTrans = false;
	bool useLoadLines = false;
	bool hasHoughed = false; //��ǰͼƬ�Ƿ�任
	int preChoosedIndex = -1; //֮ǰѡ���ͼƬ������  
	int fileNums = 0; //���ļ���
	int chooseIndex = -1; //-1��ʾ��û��
	int threshold = 0;	//��ͬһֱ���϶��ٸ���ű���Ϊ�Ǹ�ֱ��
	float minLineLenth = 0;   //��ʾ����С�߳�
	float maxLineGap = 0;	//ͬ�⽫ͬһ�е����֮���������������ľ���
	float scale = 1;  //���ű���
	cv::Point mouseAbsolutePos = cv::Point();
	
	//ˢ����
	while (cvWaitKey(30) != 27)
	{
		//ÿ��ѭ������Ҫ����background
		backGroundColor = cv::Scalar(49, 52, 49); //��ɫ
		cvui::image(backGroundColor, 0, 0, backGroundImg); //������
		backGroundColor2 = cv::Scalar(49, 52, 49);
		//backGroundColor3 = cv::Scalar(49, 52, 49);

		if (cvui::button(backGroundColor, 0, 0, buttonW, buttonW, "chooseFolder"))
		{
			//���·���µ��ļ��������³�ʼ��ѡ����ֵ
			folderBrowser.getFilePathsInFolder("jpg");
			free(chooseBools);
			fileNums = folderBrowser.fileNames.size();

			chooseBools = new bool(fileNums);
			for (int i = 0; i < fileNums; i++)
			{
				chooseBools[i] = false;
			}
			chooseIndex = -1;
			preChoosedIndex = -1;
		}

		cvui::window(backGroundColor, 0, buttonW, buttonW, browseListH, "chooseImg");
		//�ж��Ƿ�ѡ��ĳһ��CheckBox ����ѡ��		
		for (int i = 0; i < fileNums; i++)
		{
			if (cvui::checkbox(backGroundColor, interval, buttonW + 30 + 3 * interval * i, (folderBrowser.fileNames[i]), &(chooseBools[i])))
			{
				for (int j = 0; j < fileNums; j++)
				{
					chooseBools[j] = (j == i) ? true : false;

				}
				
				chooseIndex = i;				
				if (preChoosedIndex != chooseIndex) //�����л���һ��
				{
					preChoosedIndex = chooseIndex;
					hasHoughed = false;
					choosedMat = cv::imread(folderBrowser.folderPath + folderBrowser.fileNames[chooseIndex]);
					//cv::resize(choosedMat, choosedMat, cv::Size(100, 300));
					lineGeter->lines.clear();
					mouseHandle.addVecs.clear();
					mouseHandle.deledtVecs.clear();
					scale = caculateImgScale(choosedMat);
					*scaledSize = cv::Size((int)(choosedMat.cols / scale), (int)(choosedMat.rows / scale)); //���SizeӦ�����Ǹߣ����ǿ� ����Resize������
					cv::resize(choosedMat, lineGeter->rawImg, *scaledSize);
					//���»�ͼ����
					showImgRect->width = scaledSize->width;
					showImgRect->height = scaledSize->height;
					cv::resize(backGroundColor2, backGroundColor2, *scaledSize);	
					//�Ƿ����ֱ��
					if (useLoadLines)
					{
						lineLoader.minLen = (int)((float)min(scaledSize->width, scaledSize->height) / 10.0f);
						if (lineLoader.LoadLines(folderBrowser.fileNames[chooseIndex]))
						{
							cv::Mat backGroundColor3 = cv::Mat(scaledSize->height, scaledSize->width, CV_8UC3); //���ص�ͼƬ��bg
							//cv::resize(backGroundColor3, backGroundColor3, *scaledSize);
							backGroundColor3 = cv::Scalar(0, 0, 0);
							cv::namedWindow("LineLoader"); //���صĴ���
							//cvui::watch("LineLoader");
							cout << "Load OK" << endl;	
							//cvui::context("LineLoader");
							for (int i = 0; i < lineLoader.lines.size(); i++)
							{
								cv::Point p1(lineLoader.lines[i].val[0], lineLoader.lines[i].val[1]);
								cv::Point p2(lineLoader.lines[i].val[2], lineLoader.lines[i].val[3]);
								cv::line(backGroundColor3, p1, p2, cv::Scalar(1, 255, 1));
							}
							//cvui::imshow("LineLoader", backGroundColor3);
							//cvui::context("MAINWINDOW");
							cv::imshow("LineLoader", backGroundColor3);
						}
						else
						{
							cout << "LoadLineError!" << endl;
						}
					}
					else
					{
						cv::destroyWindow("LineLoader");
					}
				}
			}
		}

		cvui::checkbox(backGroundColor, buttonW + interval, 2 * interval, "UseHough", &useHoughTrans);
		cvui::checkbox(backGroundColor, buttonW + 20 * interval, 2 * interval, "UseLoadLines", &useLoadLines);
		
		//�޸�ֵ��Ҫ������һ��hough�任
		if (cvui::trackbar(backGroundColor, trackabar1X, trackBarY, trackbarW, &threshold, 0, 100))
		{
			hasHoughed = false;
			mouseHandle.addVecs.clear();
			mouseHandle.deledtVecs.clear();
		}
		if (cvui::trackbar(backGroundColor, trackabar2X, trackBarY, trackbarW, &minLineLenth, 0.0f, 100.0f))
		{
			hasHoughed = false;
			mouseHandle.addVecs.clear();
			mouseHandle.deledtVecs.clear();
		}
		if (cvui::trackbar(backGroundColor, trackabar3X, trackBarY, trackbarW, &maxLineGap, 0.0f, 60.0f))
		{
			hasHoughed = false;
			mouseHandle.addVecs.clear();
			mouseHandle.deledtVecs.clear();
		}
		//�´�������ʾԭʼͼƬ �����hough�任
		if (chooseIndex != -1)
		{
			if (useHoughTrans)
			{
				if (!hasHoughed)
				{//��һ��hough�任 �õ�ԭʼͼ��û��resize�ĵ��λ�ã�
					lineGeter->lines = lineGeter->getLinesFromImage(choosedMat, CV_PI / 180, threshold, minLineLenth, maxLineGap);
					//�߹������û���任�Ľ���ͼ����Ѿ��е���
					if (lineGeter->lines.size() == 0)
					{
						if (lineLoader.LoadLines(folderBrowser.fileNames[chooseIndex]))
						{
							lineGeter->lines.assign(lineLoader.lines.begin(), lineLoader.lines.end());
						}

					}
					cv::resize(lineGeter->linedImg, lineGeter->linedImg, *scaledSize);
					cv::resize(lineGeter->cannyImgRGB, lineGeter->cannyImgRGB, *scaledSize);
					for (int i = 0; i < lineGeter->lines.size(); i++)
					{
						lineGeter->lines[i] = lineGeter->lines[i] / scale;  //����resize֮���lines

						line(lineGeter->linedImg, cv::Point(lineGeter->lines[i].val[0], lineGeter->lines[i].val[1]), cv::Point(lineGeter->lines[i].val[2], lineGeter->lines[i].val[3]), cv::Scalar(0xff, 0xcc, 0x66), 1.2f, cv::LINE_AA);
					}
					mouseHandle.linedImgBeforeMouseLine = lineGeter->linedImg.clone(); //����û�о�������޸ĵ�ͼƬ				
					hasHoughed = true;
				}
				//image�������while�е���
				cvui::image(backGroundColor, showImgRect->x, showImgRect->y, lineGeter->linedImg);
			}
			else
			{
				//��ִ��hough�任 ��ʾԭͼ��
				cvui::image(backGroundColor, showImgRect->x, showImgRect->y, lineGeter->rawImg);

			}

		}

		//�������¼� ��ͼ�������вſ���ʹ��		
		int mouse_state = cvui::iarea(showImgRect->x, showImgRect->y, showImgRect->width, showImgRect->height);
		if (mouse_state != cvui::OUT)
		{
			mouseAbsolutePos = cvui::mouse("MAINWINDOW");
			mouseHandle.mouseRelativePos = mouseAbsolutePos - cv::Point(showImgRect->x, showImgRect->y); //����������λ��
			mouseHandle.mouseEventHandle(lineGeter);
			cvui::printf(backGroundColor, win_width - buttonW, (int)((float)(buttonW * 2) / 3) + 2 * interval, 0.3f, 0X66CCFF, "AP: (%d, %d)", mouseAbsolutePos.x, mouseAbsolutePos.y);
			cvui::printf(backGroundColor, win_width - buttonW, (int)((float)(buttonW * 2) / 3) + 4 * interval, 0.3f, 0X66CCFF, "RP: (%d, %d)", mouseHandle.mouseRelativePos.x, mouseHandle.mouseRelativePos.y);
		}


		//����LinePos
		//����Save֮��ᱣ���ļ���Ŀ¼
		//��ͨ��������ɾ������Ҫ�ĵ㣬�ϲ���Ҫ�ĵ㣨��ɾ��ӣ�
		if (cvui::button(backGroundColor, win_width - buttonW, 0, buttonW, (int)((float)(buttonW * 2) / 3), "Save"))
		{
			if (chooseIndex != -1)
			{
				lineGeter->updateLinePos(mouseHandle.addVecs, mouseHandle.deledtVecs, scale);
				mouseHandle.addVecs.clear();
				mouseHandle.deledtVecs.clear();
				ofstream in;
				//int t = folderBrowser.fileNames[chooseIndex].find_last_of(".jpg");

				cv::String fineName = folderBrowser.fileNames[chooseIndex].substr(0, folderBrowser.fileNames[chooseIndex].length() - 4);
				cv::String path = SAVEFOLDERPATH + fineName + "_Lines.txt";

				saveInformations.saveInt(path, "threshold", threshold, true);
				saveInformations.saveInt(path, "minLineLenth", minLineLenth, false);
				saveInformations.saveInt(path, "maxLineGap", maxLineGap, false);
				saveInformations.saveLines(path, lineGeter->lines, scale);
				cout << "������ϣ�" << endl;
				mouseHandle.linedImgBeforeMouseLine = lineGeter->cannyImgRGB.clone();
				for (int i = 0; i < lineGeter->lines.size(); i++)
				{
					cv::Vec4i l = lineGeter->lines[i];
					line(mouseHandle.linedImgBeforeMouseLine, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0xff, 0xcc, 0x66), 1.2f, cv::LINE_AA);
				}
				lineGeter->linedImg = mouseHandle.linedImgBeforeMouseLine.clone();
			}

		}

		if (useLoadLines)
		{
			
		}

		cvui::update("MAINWINDOW");
		cvui::imshow("MAINWINDOW", backGroundColor);
		cvui::context("RawImg");
		if (lineGeter->rawImg.size().width > 0)
		{			
			cvui::image(backGroundColor2, 0, 0, lineGeter->rawImg);//ԭʼͼƬ
		}
		cvui::printf(backGroundColor2, mouseHandle.mouseRelativePos.x, mouseHandle.mouseRelativePos.y, 1.0f, 0XFE0302, "\\");
		cvui::imshow("RawImg", backGroundColor2);
		cvui::context("MAINWINDOW");
		
	}
	
}


//����ͼƬ��С ������Ӧ�����ŵ�ʲô��С�����ĸ���ȥ���ţ���ʾ�����ǹ̶��� ����Ҫ����
//����һ�� ���ű���������1��  �ڼ���ԭͼ�������ʱ����ϸ����Ӽ���
float caculateImgScale(cv::Mat img)
{
	if (img.rows <= MAX_DISPLAY_HEIGHT && img.cols <= MAX_DISPLAY_WIDTH)
	{
		//ͼƬ�������� ��������
		return 1.0f;
	}
	else
	{
		if (img.rows >= img.cols)
		{
			//��ͼƬ ����������
			float scale = (float)img.rows / MAX_DISPLAY_HEIGHT;			
			return scale;

		}
		else
		{
			//��ͼƬ
			float scale = (float)img.cols / MAX_DISPLAY_WIDTH;		
			return scale;
		}
	}
	
}





