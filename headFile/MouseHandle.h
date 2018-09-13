#pragma once
//ר�Ŵ�������¼�����
#include "TotalHead.h"
#include "cvui.h"
#include "HoughLines.h"

class MouseHandle
{
public:
	cv::Point mouseRelativePos; //���λ�� ��ͼƬ�ϵ�λ��
	bool isTheFirstPoint = true;
	bool startLining = false; //ֻ����״̬�仯��ʱ��Ż�ͼ ����ͼstate�ͻ�����
	//��Щ�㶼������֮��ĵ� ��Ҫ�ڱ����ʱ�����������ڱ���
	cv::Point theFirstPoint;
	cv::Vec4i drag_delArea;
	vector<cv::Vec4i> addVecs;   //����ӵĵ㵽������ȥ �ڵ����ҳ���ϵ�Save֮�󱣴��������ļ��͵�linegeter��ȥ
	vector<cv::Vec4i> deledtVecs;   //ɾ����ֱ�ߵ�vec �ڵ����ҳ���ϵ�Save֮�󱣴��������ļ��͵�linegeter��ȥ
	
	cv::Mat linedImgBeforeMouseLine;  //��̬������Ҫ�õ���Mat ֻ�ڶ�̬��ӵ��ʱ���õ�

	void lineMouseTrace(LineGeter *lineGeter); //�������Ĺ켣
	void mouseEventHandle(LineGeter *lineGeter); //���ĵ���¼�
	void onAddLinesClick(LineGeter * lineGeter); //��������
	void onDeleteLinesClick(LineGeter * lineGeter); //�Ҽ�ɾ����
	void onDeleteDragLines(LineGeter* lineGeter); //�м�  ����̧�� ɾ����ѡ��Ԫ��
	bool isMouseByoundLines(cv::Vec4i l);

	MouseHandle()
	{
		theFirstPoint = cv::Point(-2, -2);  //���Ϊ- �ʹ���û�е�		
	}

};