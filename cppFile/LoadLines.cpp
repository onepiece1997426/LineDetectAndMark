#include "LoadLines.h"

float calDist(int x1, int x2, int y1, int y2);

bool LineLoader::LoadLines(string fileName)
{
	//设置txt名字
	imgName = fileName;
	int cnt = imgName.find(".jpg");
	if (cnt == string::npos)
	{
		cout << "File Name Error!" << endl;
		return false;
	}
	txtName = imgName.substr(0, cnt) + "_Lines.txt";
	lines.clear();
	ifstream ifs;
	ifs.open(lineFolderPath + txtName);
	if (!ifs.is_open())
	{
		cout << txtName <<  "  txt error!" << endl;
		return false;
	}

	string fileLine;
	int num;
	stringstream ss;
	//首先读取前三行
	getline(ifs, fileLine);
	num = fileLine.find(":", 0);
	fileLine.substr(num); //从Num开始到结尾
	ss << fileLine;
	ss >> threshold;
	//ss.clear();

	getline(ifs, fileLine);
	num = fileLine.find(":", 0);
	ss << fileLine.substr(num); //从Num开始到结尾;
	ss >> minLineLenth;
	ss.clear();
	ss.str("");

	getline(ifs, fileLine);
	num = fileLine.find(":", 0);
	fileLine.substr(num); //从Num开始到结尾
	ss << fileLine;
	ss >> maxLineGap;
	ss.clear();
	ss.str("");

	//最后读取所有的点
	while (getline(ifs, fileLine))
	{
		int l1Pos, l2Pos, r1Pos, r2Pos; //<><>开始和结束的位置
		int comma1Pos, comma2Pos; //逗号
		//匹配格式<,><,>找到位置
		l1Pos = fileLine.find_first_of("<");
		l2Pos = fileLine.find_last_of("<");
		r1Pos = fileLine.find_first_of(">");
		r2Pos = fileLine.find_last_of(">");
		comma1Pos = fileLine.find_first_of(",");
		comma2Pos = fileLine.find_last_of(",");
		//设置数据
		cv::Vec4i v4i;
		string str;
		int num;
		str = fileLine.substr(l1Pos + 1, comma1Pos - l1Pos - 1);
		ss << str;
		ss >> num;
		v4i.val[0] = num; 
		ss.str("");
		ss.clear();

		str = fileLine.substr(comma1Pos + 1, r1Pos - comma1Pos - 1);
		ss << str;
		ss >> num;
		v4i.val[1] = num;
		ss.str("");
		ss.clear();

		str = fileLine.substr(l2Pos + 1, comma2Pos - l2Pos - 1);
		ss << str;
		ss >> num;
		v4i.val[2] = num;
		ss.str("");
		ss.clear();

		str = fileLine.substr(comma2Pos + 1, r2Pos - comma2Pos - 1);
		ss << str;
		ss >> num;
		v4i.val[3] = num;
		ss.str("");
		ss.clear();

		
		if(calDist(v4i.val[0], v4i.val[2], v4i.val[1], v4i.val[3]) > minLen)
			lines.push_back(v4i);
	}
	cout << txtName << endl;
	ifs.close();

	return true;
}

float calDist(int x1, int x2, int y1, int y2)
{
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));

}