#include "HoughLines.h"
#include "BrowseFolder.h"
#include "MouseHandle.h"
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "SaveLines.h"
#include "LoadLines.h"

#define MAX_DISPLAY_WIDTH  520  //最大显示图片的大小 否则缩放
#define MAX_DISPLAY_HEIGHT	520
#define SAVEFOLDERPATH "D:\\Datas\\Buildings\\Lines\\"		//保存文件的目录
#define LOADTXTPATH "D:\\Datas\\Buildings\\Lines\\" 

float caculateImgScale(cv::Mat img);

void main()
{
	LineGeter* lineGeter = new LineGeter();
	
	cv::namedWindow("MAINWINDOW",0);
	cvui::init("MAINWINDOW");
	cv::namedWindow("RawImg"); //显示原始图像的窗口
	cvui::watch("RawImg");
	cv::namedWindow("LineLoader"); //加载的窗口
	cvui::watch("LineLoader");
	
	cv::Mat backGroundImg = cv::imread("E:/CCCCCCCC++++++/Dadi.jpg");	//背景图片
	
	FolderBrowser folderBrowser = FolderBrowser();
	cv::Mat choosedMat = cv::Mat();	

	MouseHandle mouseHandle = MouseHandle();
	SaveInformations saveInformations = SaveInformations();
	LineLoader lineLoader = LineLoader(LOADTXTPATH);
	//窗口参数
	int interval = 10;  //控件间隔
	int win_width = ((MAX_DISPLAY_WIDTH + 2 * interval) * 5) / 4;
	int win_height = ((MAX_DISPLAY_HEIGHT + 2 * interval) * 5) / 4;
	cv::resize(backGroundImg, backGroundImg, cv::Size(win_width, win_height)); //背景颜色
	cv::Mat backGroundColor = cv:: Mat(win_height, win_width,CV_8UC3);  //主界面bg
	cv::Mat backGroundColor2 = cv::Mat(win_height, win_width, CV_8UC3);  //原始图片bg

	int buttonW = win_width / 5; //按钮长宽
	int browseListH = 4 * win_height / 5;//浏览文件的window的长度
	int check_useHoughX = buttonW + interval; //是否启用霍夫变换按钮的开始
	int trackBarY = buttonW / 2; //三个滑条的Y
	int trackbarW = ((win_width * 3) / 5 - (3 * 2 * interval)) / 3;
	int trackabar1X = buttonW + interval;
	int trackabar2X = trackabar1X + trackbarW + 2 * interval;
	int trackabar3X = trackabar2X + trackbarW + 2 * interval;
	cv::Rect *showImgRect = new cv::Rect(buttonW + 10, buttonW + 10, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT);
	cv::Size *scaledSize = new cv::Size(1, 1);  //缩放后的大小
	
	bool* chooseBools = new bool;  //选择框的选中
	bool useHoughTrans = false;
	bool useLoadLines = false;
	bool hasHoughed = false; //当前图片是否变换
	int preChoosedIndex = -1; //之前选择的图片的索引  
	int fileNums = 0; //总文件数
	int chooseIndex = -1; //-1表示都没有
	int threshold = 0;	//在同一直线上多少个点才被认为是个直线
	float minLineLenth = 0;   //显示的最小线长
	float maxLineGap = 0;	//同意将同一行点与点之间连接起来的最大的距离
	float scale = 1;  //缩放倍数
	cv::Point mouseAbsolutePos = cv::Point();
	
	//刷新率
	while (cvWaitKey(30) != 27)
	{
		//每次循环都需要更新background
		backGroundColor = cv::Scalar(49, 52, 49); //灰色
		cvui::image(backGroundColor, 0, 0, backGroundImg); //画背景
		backGroundColor2 = cv::Scalar(49, 52, 49);
		//backGroundColor3 = cv::Scalar(49, 52, 49);

		if (cvui::button(backGroundColor, 0, 0, buttonW, buttonW, "chooseFolder"))
		{
			//获得路径下的文件名并重新初始化选择框的值
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
		//判断是否选中某一个CheckBox （单选）		
		for (int i = 0; i < fileNums; i++)
		{
			if (cvui::checkbox(backGroundColor, interval, buttonW + 30 + 3 * interval * i, (folderBrowser.fileNames[i]), &(chooseBools[i])))
			{
				for (int j = 0; j < fileNums; j++)
				{
					chooseBools[j] = (j == i) ? true : false;

				}
				
				chooseIndex = i;				
				if (preChoosedIndex != chooseIndex) //表明切换了一次
				{
					preChoosedIndex = chooseIndex;
					hasHoughed = false;
					choosedMat = cv::imread(folderBrowser.folderPath + folderBrowser.fileNames[chooseIndex]);
					//cv::resize(choosedMat, choosedMat, cv::Size(100, 300));
					lineGeter->lines.clear();
					mouseHandle.addVecs.clear();
					mouseHandle.deledtVecs.clear();
					scale = caculateImgScale(choosedMat);
					*scaledSize = cv::Size((int)(choosedMat.cols / scale), (int)(choosedMat.rows / scale)); //这个Size应该先是高，再是宽 后面Resize才正常
					cv::resize(choosedMat, lineGeter->rawImg, *scaledSize);
					//更新画图区域
					showImgRect->width = scaledSize->width;
					showImgRect->height = scaledSize->height;
					cv::resize(backGroundColor2, backGroundColor2, *scaledSize);	
					//是否加载直线
					if (useLoadLines)
					{
						lineLoader.minLen = (int)((float)min(scaledSize->width, scaledSize->height) / 10.0f);
						if (lineLoader.LoadLines(folderBrowser.fileNames[chooseIndex]))
						{
							cv::Mat backGroundColor3 = cv::Mat(scaledSize->height, scaledSize->width, CV_8UC3); //加载的图片的bg
							//cv::resize(backGroundColor3, backGroundColor3, *scaledSize);
							backGroundColor3 = cv::Scalar(0, 0, 0);
							cv::namedWindow("LineLoader"); //加载的窗口
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
		
		//修改值就要重新来一次hough变换
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
		//新窗口上显示原始图片 并检测hough变换
		if (chooseIndex != -1)
		{
			if (useHoughTrans)
			{
				if (!hasHoughed)
				{//做一次hough变换 得到原始图像（没有resize的点的位置）
					lineGeter->lines = lineGeter->getLinesFromImage(choosedMat, CV_PI / 180, threshold, minLineLenth, maxLineGap);
					//蔬果不适用霍夫变换的结果就加载已经有的线
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
						lineGeter->lines[i] = lineGeter->lines[i] / scale;  //保存resize之后的lines

						line(lineGeter->linedImg, cv::Point(lineGeter->lines[i].val[0], lineGeter->lines[i].val[1]), cv::Point(lineGeter->lines[i].val[2], lineGeter->lines[i].val[3]), cv::Scalar(0xff, 0xcc, 0x66), 1.2f, cv::LINE_AA);
					}
					mouseHandle.linedImgBeforeMouseLine = lineGeter->linedImg.clone(); //保存没有经过鼠标修改的图片				
					hasHoughed = true;
				}
				//image必须得在while中调用
				cvui::image(backGroundColor, showImgRect->x, showImgRect->y, lineGeter->linedImg);
			}
			else
			{
				//不执行hough变换 显示原图像
				cvui::image(backGroundColor, showImgRect->x, showImgRect->y, lineGeter->rawImg);

			}

		}

		//获得鼠标事件 在图像区域中才可以使用		
		int mouse_state = cvui::iarea(showImgRect->x, showImgRect->y, showImgRect->width, showImgRect->height);
		if (mouse_state != cvui::OUT)
		{
			mouseAbsolutePos = cvui::mouse("MAINWINDOW");
			mouseHandle.mouseRelativePos = mouseAbsolutePos - cv::Point(showImgRect->x, showImgRect->y); //设置鼠标相对位置
			mouseHandle.mouseEventHandle(lineGeter);
			cvui::printf(backGroundColor, win_width - buttonW, (int)((float)(buttonW * 2) / 3) + 2 * interval, 0.3f, 0X66CCFF, "AP: (%d, %d)", mouseAbsolutePos.x, mouseAbsolutePos.y);
			cvui::printf(backGroundColor, win_width - buttonW, (int)((float)(buttonW * 2) / 3) + 4 * interval, 0.3f, 0X66CCFF, "RP: (%d, %d)", mouseHandle.mouseRelativePos.x, mouseHandle.mouseRelativePos.y);
		}


		//保存LinePos
		//按下Save之后会保存文件到目录
		//将通过迭代器删除不需要的点，合并需要的点（先删后加）
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
				cout << "保存完毕！" << endl;
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
			cvui::image(backGroundColor2, 0, 0, lineGeter->rawImg);//原始图片
		}
		cvui::printf(backGroundColor2, mouseHandle.mouseRelativePos.x, mouseHandle.mouseRelativePos.y, 1.0f, 0XFE0302, "\\");
		cvui::imshow("RawImg", backGroundColor2);
		cvui::context("MAINWINDOW");
		
	}
	
}


//计算图片大小 按比例应该缩放到什么大小，按哪个轴去缩放：显示区域是固定的 所以要缩放
//返回一个 缩放比例（大于1）  在计算原图上坐标的时候乘上该因子即可
float caculateImgScale(cv::Mat img)
{
	if (img.rows <= MAX_DISPLAY_HEIGHT && img.cols <= MAX_DISPLAY_WIDTH)
	{
		//图片本身不够大 不用缩放
		return 1.0f;
	}
	else
	{
		if (img.rows >= img.cols)
		{
			//长图片 按纵轴缩放
			float scale = (float)img.rows / MAX_DISPLAY_HEIGHT;			
			return scale;

		}
		else
		{
			//宽图片
			float scale = (float)img.cols / MAX_DISPLAY_WIDTH;		
			return scale;
		}
	}
	
}





