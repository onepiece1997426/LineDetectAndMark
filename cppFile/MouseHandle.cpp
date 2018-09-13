#include "MouseHandle.h"


//从霍夫变换后的图像中添加直线或者删除直线
//左键添加（添加两个点构成一个Vec4i）
//右键删除 (1次 删除1个Vec4i)
//输入 LineGeter 以便知道原始图像上某点的位置 = mousePos 
//或者 lineGeter->linePos[i]  和 鼠标位置比较
//将修改后的值先分别存入对应向量中 在主页面上按下 Save之后执行修改操作，
//按下Save将getter中的向量修改并保存到文件



void MouseHandle::mouseEventHandle(LineGeter* lineGeter)
{
	
	if (cvui::mouse(cvui::LEFT_BUTTON, cvui::DOWN))
	{
		//cout << "Down left button " << endl;
		onAddLinesClick(lineGeter);
	}

	if (cvui::mouse(cvui::RIGHT_BUTTON, cvui::DOWN))
	{
		//cout << "Down right button " << endl;
		if (theFirstPoint.x > 0)
		{
			//取消第一个点
			isTheFirstPoint = true;
			lineGeter->linedImg = linedImgBeforeMouseLine.clone();
			theFirstPoint = cv::Point(-2, -2);
			startLining = true;  //#####__1 这里打开了开关 在下面画线函数中会关掉
		}
		else
			onDeleteLinesClick(lineGeter);

		
	}

	if (cvui::mouse(cvui::MIDDLE_BUTTON, cvui::DOWN))
	{
		drag_delArea.val[0] = mouseRelativePos.x;
		drag_delArea.val[1] = mouseRelativePos.y;
		cout << "begin drag..." << endl;
	}
	if (cvui::mouse(cvui::MIDDLE_BUTTON, cvui::UP))
	{
		//确保drag_delArea为先左上 后右下
		if (mouseRelativePos.x < drag_delArea.val[0])
		{
			drag_delArea.val[2] = drag_delArea.val[0];
			drag_delArea.val[0] = mouseRelativePos.x;
		}
		else
		{
			drag_delArea.val[2] = mouseRelativePos.x;
		}
		if (mouseRelativePos.y < drag_delArea.val[1])
		{
			drag_delArea.val[3] = drag_delArea.val[1];
			drag_delArea.val[1] = mouseRelativePos.y;
		}
		else
		{
			drag_delArea.val[3] = mouseRelativePos.y;
		}	
		
		onDeleteDragLines(lineGeter);
		cout << "end drag!" << endl;
	}


	if (startLining)
	{
		lineMouseTrace(lineGeter);	
		
	}


}

void MouseHandle::onAddLinesClick(LineGeter * lineGeter)
{
	//再有图像的时候才能添加删除等等
	if (!lineGeter->linedImg.empty())
	{
		
		//首先判断是不是角点	或者边界点
		if((linedImgBeforeMouseLine.ptr<unsigned char>(mouseRelativePos.y)[lineGeter->linedImg.channels() * mouseRelativePos.x]) > 0
			||(mouseRelativePos.y == linedImgBeforeMouseLine.rows - 1 || mouseRelativePos.y == 0 || mouseRelativePos.x == linedImgBeforeMouseLine.cols - 1 || mouseRelativePos.x == 0) )
		{
			cout << "角点！！" << endl;
			//如果是第一个点
			if (isTheFirstPoint)
			{
				theFirstPoint = mouseRelativePos;				
				isTheFirstPoint = false;
				startLining = true;
			}
			else
			{
				//第二个点
				cv::Vec4i vec4i = cv::Vec4i(theFirstPoint.x, theFirstPoint.y, mouseRelativePos.x, mouseRelativePos.y);
				addVecs.push_back(vec4i);

				theFirstPoint = cv::Point(-2, -2);
				isTheFirstPoint = true;
				startLining = false;
			}

		}

	}
	
}

void MouseHandle::onDeleteLinesClick(LineGeter * lineGeter)
{
	//从遍历linegetor的linePoints上计算 某个点到鼠标的斜率
	//并计算另一个丶是否在这条直线上
	startLining = true;
	//在标记好的线上找
	for (int i = 0; i < lineGeter->lines.size(); i++)
	{
		//直线的起止点
		cv::Vec4i l = lineGeter->lines[i];
		if (isMouseByoundLines(l))
		{
			deledtVecs.push_back(l); //这条线是要删除的线		压栈	
			break;
		}
		
	}
	//在自己标记的线里面找 直接就删除了 不画蓝线
	for (int i = 0; i < addVecs.size(); i++)
	{
		//直线的起止点
		cv::Vec4i l = addVecs[i];
		if (isMouseByoundLines(l))
		{
			//通过迭代器寻找符合条件的向量
			vector<cv::Vec4i>::iterator iVec = find(addVecs.begin(), addVecs.end(), l);		
			addVecs.erase(iVec);
			cout << "erase!" << endl;
			break;
		}		
		
	}


}

void MouseHandle::onDeleteDragLines(LineGeter * lineGeter)
{
	//删除范围内的所有点构成的直线 并画线
	//画线
	lineGeter->linedImg = lineGeter->cannyImgRGB.clone();
	
	for (int i = 0; i < lineGeter->lines.size(); i++)
	{
		cv::Vec4i v4i = lineGeter->lines[i];
		//只要有一个点在这里面就删 
		if ((v4i.val[0] > drag_delArea.val[0] && v4i.val[0] < drag_delArea.val[2]
			&& v4i.val[1] > drag_delArea.val[1] && v4i.val[1] < drag_delArea.val[3])
			|| (v4i.val[2] > drag_delArea.val[0] && v4i.val[2] < drag_delArea.val[2]
				&& v4i.val[3] > drag_delArea.val[1] && v4i.val[3] < drag_delArea.val[3])
			)
		{
			vector<cv::Vec4i>::iterator i_vec = find(lineGeter->lines.begin(), lineGeter->lines.end(), v4i);
			lineGeter->lines.erase(i_vec);
		}
		else
		{
			cv::line(lineGeter->linedImg, cv::Point(v4i[0], v4i[1]), cv::Point(v4i[2], v4i[3]), cv::Scalar(0xff, 0xcc, 0x66), 1, cv::LINE_AA);
		}
	}

	linedImgBeforeMouseLine = lineGeter->linedImg.clone();


}

//判断鼠标是否在某条直线上 参数为某条直线的两个坐标
bool MouseHandle::isMouseByoundLines(cv::Vec4i line)
{
	cv::Point point1 = cv::Point(line.val[0], line.val[1]);
	cv::Point point2 = cv::Point(line.val[2], line.val[3]);
	int dx1 = point1.x - mouseRelativePos.x;
	int dx2 = mouseRelativePos.x - point2.x;
	int dy1 = point1.y - mouseRelativePos.y;
	int dy2 = mouseRelativePos.y - point2.y;
	//鼠标必须在这两个点之间才行
	if ((dx1*dx2 >= 0) && (dy1*dy2 >= 0))
	{
		//几乎垂直？
		if ((dx1 < 2) && (dx2 <= 2))
		{
			if (dy1 != 0 && dy2 != 0)
			{
				//deledtVecs.push_back(line);
				cout << " DelPonints(noScale): " << point1 << "  " << point2 << endl;

				return true;
			}
		}
		else if ((dy1 < 2) && (dy2 < 2)) //几乎平行
		{
			if (dx1 != 0 && dx2 != 0)
			{
				//deledtVecs.push_back(line);
				cout << " DelPonints(noScale): " << point1 << "  " << point2 << endl;

				return true;
			}
		}
		else
		{
			float k1 = (float)(dy1) / (float)(dx1);
			float k2 = (float)(point1.y - point2.y) / (float)(point1.x - point2.x);
			//认为这个线在已经标记的线内 找到一条就break
			if (abs(k1 - k2) < 0.1f)
			{			
				//cout << k1 << "  " << k2 << endl;
				cout << " DelPonints(noScale): " << point1 <<  "  " << point2 << endl;
				return true;
			}
		}
	}
	return false;
}

void MouseHandle::lineMouseTrace(LineGeter * lineGeter)
{
	//画添加的线
	//如果theFirstPoint 的值大于0 就代表左键按下之后已经定了一个点 
	//反之则没有点被第一个确定
	lineGeter->linedImg = linedImgBeforeMouseLine.clone();
	for (int i = 0; i < addVecs.size(); i++)
	{
		cv::Vec4i l = addVecs[i];
		cv::line(lineGeter->linedImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);	

	}	
	if (theFirstPoint.x > 0)
	{	
		//lineGeter->linedImg = linedImgBeforeMouseLine.clone();
		//这个颜色在前面判断是不是角点的时候误判
		cv::line(lineGeter->linedImg, theFirstPoint, mouseRelativePos, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);	  //BGR颜色	 第一个维度的颜色值为0 否则前面判断是角点的时候会影响	
	}	
	else
	{
		startLining = false; //#####__1  关上划线的开关 代表划过了 也对应了删除的划线

	}

	//画删除的线
	for (int i = 0; i < deledtVecs.size(); i++)
	{
		cv::Vec4i l = deledtVecs[i];
		cv::line(lineGeter->linedImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
	}


	
}