#pragma once
//专门处理鼠标事件的类
#include "TotalHead.h"
#include "cvui.h"
#include "HoughLines.h"

class MouseHandle
{
public:
	cv::Point mouseRelativePos; //相对位置 在图片上的位置
	bool isTheFirstPoint = true;
	bool startLining = false; //只有在状态变化的时候才画图 画完图state就回来了
	//这些点都是缩放之后的点 需要在保存的时候重新缩放在保存
	cv::Point theFirstPoint;
	cv::Vec4i drag_delArea;
	vector<cv::Vec4i> addVecs;   //新添加的点到向量中去 在点击主页面上的Save之后保存向量到文件和到linegeter中去
	vector<cv::Vec4i> deledtVecs;   //删除的直线的vec 在点击主页面上的Save之后保存向量到文件和到linegeter中去
	
	cv::Mat linedImgBeforeMouseLine;  //动态划线需要用到的Mat 只在动态添加点的时候用到

	void lineMouseTrace(LineGeter *lineGeter); //画上鼠标的轨迹
	void mouseEventHandle(LineGeter *lineGeter); //鼠标的点击事件
	void onAddLinesClick(LineGeter * lineGeter); //左键添加线
	void onDeleteLinesClick(LineGeter * lineGeter); //右键删除线
	void onDeleteDragLines(LineGeter* lineGeter); //中键  按下抬起 删除框选的元素
	bool isMouseByoundLines(cv::Vec4i l);

	MouseHandle()
	{
		theFirstPoint = cv::Point(-2, -2);  //如果为- 就代表没有点		
	}

};