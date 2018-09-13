#include "MouseHandle.h"


//�ӻ���任���ͼ�������ֱ�߻���ɾ��ֱ��
//�����ӣ���������㹹��һ��Vec4i��
//�Ҽ�ɾ�� (1�� ɾ��1��Vec4i)
//���� LineGeter �Ա�֪��ԭʼͼ����ĳ���λ�� = mousePos 
//���� lineGeter->linePos[i]  �� ���λ�ñȽ�
//���޸ĺ��ֵ�ȷֱ�����Ӧ������ ����ҳ���ϰ��� Save֮��ִ���޸Ĳ�����
//����Save��getter�е������޸Ĳ����浽�ļ�



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
			//ȡ����һ����
			isTheFirstPoint = true;
			lineGeter->linedImg = linedImgBeforeMouseLine.clone();
			theFirstPoint = cv::Point(-2, -2);
			startLining = true;  //#####__1 ������˿��� �����滭�ߺ����л�ص�
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
		//ȷ��drag_delAreaΪ������ ������
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
	//����ͼ���ʱ��������ɾ���ȵ�
	if (!lineGeter->linedImg.empty())
	{
		
		//�����ж��ǲ��ǽǵ�	���߽߱��
		if((linedImgBeforeMouseLine.ptr<unsigned char>(mouseRelativePos.y)[lineGeter->linedImg.channels() * mouseRelativePos.x]) > 0
			||(mouseRelativePos.y == linedImgBeforeMouseLine.rows - 1 || mouseRelativePos.y == 0 || mouseRelativePos.x == linedImgBeforeMouseLine.cols - 1 || mouseRelativePos.x == 0) )
		{
			cout << "�ǵ㣡��" << endl;
			//����ǵ�һ����
			if (isTheFirstPoint)
			{
				theFirstPoint = mouseRelativePos;				
				isTheFirstPoint = false;
				startLining = true;
			}
			else
			{
				//�ڶ�����
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
	//�ӱ���linegetor��linePoints�ϼ��� ĳ���㵽����б��
	//��������һ��ؼ�Ƿ�������ֱ����
	startLining = true;
	//�ڱ�Ǻõ�������
	for (int i = 0; i < lineGeter->lines.size(); i++)
	{
		//ֱ�ߵ���ֹ��
		cv::Vec4i l = lineGeter->lines[i];
		if (isMouseByoundLines(l))
		{
			deledtVecs.push_back(l); //��������Ҫɾ������		ѹջ	
			break;
		}
		
	}
	//���Լ���ǵ��������� ֱ�Ӿ�ɾ���� ��������
	for (int i = 0; i < addVecs.size(); i++)
	{
		//ֱ�ߵ���ֹ��
		cv::Vec4i l = addVecs[i];
		if (isMouseByoundLines(l))
		{
			//ͨ��������Ѱ�ҷ�������������
			vector<cv::Vec4i>::iterator iVec = find(addVecs.begin(), addVecs.end(), l);		
			addVecs.erase(iVec);
			cout << "erase!" << endl;
			break;
		}		
		
	}


}

void MouseHandle::onDeleteDragLines(LineGeter * lineGeter)
{
	//ɾ����Χ�ڵ����е㹹�ɵ�ֱ�� ������
	//����
	lineGeter->linedImg = lineGeter->cannyImgRGB.clone();
	
	for (int i = 0; i < lineGeter->lines.size(); i++)
	{
		cv::Vec4i v4i = lineGeter->lines[i];
		//ֻҪ��һ�������������ɾ 
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

//�ж�����Ƿ���ĳ��ֱ���� ����Ϊĳ��ֱ�ߵ���������
bool MouseHandle::isMouseByoundLines(cv::Vec4i line)
{
	cv::Point point1 = cv::Point(line.val[0], line.val[1]);
	cv::Point point2 = cv::Point(line.val[2], line.val[3]);
	int dx1 = point1.x - mouseRelativePos.x;
	int dx2 = mouseRelativePos.x - point2.x;
	int dy1 = point1.y - mouseRelativePos.y;
	int dy2 = mouseRelativePos.y - point2.y;
	//����������������֮�����
	if ((dx1*dx2 >= 0) && (dy1*dy2 >= 0))
	{
		//������ֱ��
		if ((dx1 < 2) && (dx2 <= 2))
		{
			if (dy1 != 0 && dy2 != 0)
			{
				//deledtVecs.push_back(line);
				cout << " DelPonints(noScale): " << point1 << "  " << point2 << endl;

				return true;
			}
		}
		else if ((dy1 < 2) && (dy2 < 2)) //����ƽ��
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
			//��Ϊ��������Ѿ���ǵ����� �ҵ�һ����break
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
	//����ӵ���
	//���theFirstPoint ��ֵ����0 �ʹ����������֮���Ѿ�����һ���� 
	//��֮��û�е㱻��һ��ȷ��
	lineGeter->linedImg = linedImgBeforeMouseLine.clone();
	for (int i = 0; i < addVecs.size(); i++)
	{
		cv::Vec4i l = addVecs[i];
		cv::line(lineGeter->linedImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);	

	}	
	if (theFirstPoint.x > 0)
	{	
		//lineGeter->linedImg = linedImgBeforeMouseLine.clone();
		//�����ɫ��ǰ���ж��ǲ��ǽǵ��ʱ������
		cv::line(lineGeter->linedImg, theFirstPoint, mouseRelativePos, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);	  //BGR��ɫ	 ��һ��ά�ȵ���ɫֵΪ0 ����ǰ���ж��ǽǵ��ʱ���Ӱ��	
	}	
	else
	{
		startLining = false; //#####__1  ���ϻ��ߵĿ��� �������� Ҳ��Ӧ��ɾ���Ļ���

	}

	//��ɾ������
	for (int i = 0; i < deledtVecs.size(); i++)
	{
		cv::Vec4i l = deledtVecs[i];
		cv::line(lineGeter->linedImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
	}


	
}