#include "Data.h"

bool Data::readData()
{
	ifstream file;
	file.open("beijing-subway.txt", ios::in);
	if (!file)
	{
		return false;
	}
	string temp = "";
	int state = -1;
	string lineName, stationName;
	struct Station *lastPoint = NULL;
	struct Station *nowPoint = NULL;
	while (file>>temp && temp != "")
	{
		if (strcmp(temp.c_str(), "L") == 0)
		{
			state = 0;
		}
		switch (state)
		{
		case(0):
			state = 1;
			if (lastPoint != NULL && nowPoint != NULL)
			{
				lastPoint->transLine[lastPoint->transLineNo - 1]->nextpoint = nowPoint;
			}
			lastPoint = NULL;
			nowPoint = NULL;
			break;
		case(1):
			state = 2;
			lineName = temp;
			firstPoint[lineTop] = (struct LineInfo*)new(struct LineInfo);
			firstPoint[lineTop]->lastpoint = NULL;
			firstPoint[lineTop]->lineName = lineName;
			firstPoint[lineTop]->nextpoint = NULL;
			lineTop++;
			if (lineTop > ALL_LINE_MAX)
			{
				return false;
			}
			break;
		case(2):
			stationName = temp;
			if (nowPoint != NULL)
			{
				if (lastPoint != NULL)
				{
					lastPoint->transLine[lastPoint->transLineNo - 1]->nextpoint = nowPoint;
				}
				lastPoint = nowPoint;
			}
			if ((nowPoint = findStation(stationName)) == NULL)
			{
				nowPoint = (struct Station*)new(struct Station);
				nowPoint->stationName = stationName;
				nowPoint->transLineNo = 0;
				nowPoint->mark = 0;
			}
			nowPoint->transLine[nowPoint->transLineNo] = (struct LineInfo*)new(struct LineInfo);
			nowPoint->transLine[nowPoint->transLineNo]->lineName = lineName;
			nowPoint->transLine[nowPoint->transLineNo]->lastpoint = lastPoint;
			nowPoint->transLine[nowPoint->transLineNo]->nextpoint = NULL;
			nowPoint->transLineNo++;
			if (nowPoint->transLineNo > TRANS_POINT_MAX)
			{
				return false;
			}
			if (lastPoint == NULL)
			{
				firstPoint[lineTop - 1]->nextpoint = nowPoint;
			}
			break;
		default:
			return false;
		}
	}
	return true;
}

Station * Data::findStation(string stationName)
{
	for (int i = 0; i < lineTop; i++)
	{
		struct Station *temp = firstPoint[i]->nextpoint;
		string lineName = firstPoint[i]->lineName;
		while (temp != NULL)
		{
			if (strcmp(stationName.c_str(), temp->stationName.c_str()) == 0)
			{
				return temp;
			}
			for (int j = 0; j < temp->transLineNo; j++)
			{
				if (strcmp(lineName.c_str(), temp->transLine[j]->lineName.c_str()) == 0)
				{
					temp = temp->transLine[j]->nextpoint;
					break;
				}
			}
		}
	}
	return NULL;
}
string Data::putOutData(string stationName)
{
	string name = "";
	for (int i = 0; i < lineTop; i++)
	{
		struct Station *temp = firstPoint[i]->nextpoint;
		string lineName = firstPoint[i]->lineName;
		if (strcmp(lineName.c_str(), stationName.c_str()) != 0)
		{
			continue;
		}
		while (temp != NULL)
		{
			name += temp->stationName;
			for (int j = 0; j < temp->transLineNo; j++)
			{
				if (strcmp(lineName.c_str(), temp->transLine[j]->lineName.c_str()) == 0)
				{
					temp = temp->transLine[j]->nextpoint;
					break;
				}
			}
			if (temp == firstPoint[i]->nextpoint || temp == NULL)
			{
				name += enter;
				break;
			}
			name += "->";
		}
	}
	if (strcmp(name.c_str(), "") == 0)
	{
		return "No Such Line Error!";
	}
	return name;
}

void Data::modifyMap()
{
	for (int i = 0; i < lineTop; i++)
	{
		struct Station *temp = firstPoint[i]->nextpoint;
		string lineName = firstPoint[i]->lineName;
		if (strcmp(lineName.c_str(), "地铁二号线") == 0
			|| strcmp(lineName.c_str(), "地铁十号线") == 0
			|| strcmp(lineName.c_str(), "机场线") == 0)
		{
			struct LineInfo *headLine = NULL;
			struct Station *headStation = NULL;
			while (temp != NULL)
			{
				for (int j = 0; j < temp->transLineNo; j++)
				{
					if (strcmp(lineName.c_str(), temp->transLine[j]->lineName.c_str()) == 0)
					{
						if (strcmp(temp->stationName.c_str(), "西直门") == 0
							|| strcmp(temp->stationName.c_str(), "巴沟") == 0
							|| (strcmp(temp->stationName.c_str(), "三元桥") == 0
								&& strcmp(lineName.c_str(), "机场线") == 0))
						{
							headLine = temp->transLine[j];
							headStation = temp;
						}
						else if (strcmp(temp->stationName.c_str(), "车公庄") == 0
							|| strcmp(temp->stationName.c_str(), "火器营") == 0)
						{
							temp->transLine[j]->nextpoint = headStation;
							if (headLine != NULL)
							{
								headLine->lastpoint = temp;
							}
							temp = NULL;
							break;
						}
						/*
						由于本程序由终点站向起始站搜索，因此数据地图与实际地图略有不同
						但最终结果符合实际。
						*/
						else if (strcmp(temp->stationName.c_str(), "3号航站楼") == 0)
						{
							/*
							3号的后面是2号，3号前面是三元桥，实际地图中3号可以去2号，本地图中3号可以去三元桥
							*/
							struct Station *t= temp->transLine[j]->nextpoint;
							temp->transLine[j]->nextpoint = NULL;
							temp = t;
							break;
						}
						else if (strcmp(temp->stationName.c_str(), "2号航站楼") == 0)
						{
							/*
							2号后面是三元桥，2号前面是3号，实际地图中2号可以去三元桥，本地图中2号可以去3号
							*/
							if (headLine != NULL)
							{
								headLine->nextpoint = temp;
								temp->transLine[j]->nextpoint = temp->transLine[j]->lastpoint;
								temp->transLine[j]->lastpoint = NULL;
							}
							
							temp = NULL;
							break;
						}
						temp = temp->transLine[j]->nextpoint;
						break;
					}
				}
			}
		}
	}
}

string Data::passAllStation(Station *stationPoint)
{
	return string();
}

bool Data::isPass(struct Queue *headPoint)
{
	struct Queue *temp = headPoint;
	string stationName = headPoint->stationPoint->stationName;
	while (temp->fromPoint != NULL)
	{
		temp = temp->fromPoint;
		if(strcmp(temp->stationPoint->stationName.c_str(), stationName.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}

string Data::passStationAtLeast(Station *stationPoint1, Station *stationPoint2,int mode)
{
	int no = MAX;

	stationPoint2->mark = 1;
	struct Queue *headPoint = (struct Queue*)new(struct Queue);
	struct Queue *tailPoint = headPoint;
	struct Queue *firstPoint = headPoint;
	headPoint->stationPoint = stationPoint2;
	headPoint->nextPoint = NULL;
	headPoint->fromPoint = NULL;
	headPoint->lineName = "";

	string lineName = "";
	

	while (headPoint != NULL)
	{
		lineName = headPoint->lineName;
		
		if (mode == 1 && isPass(headPoint))
		{
			headPoint = headPoint->nextPoint;
			continue;
		}
		
		if (mode == 1 && no < headPoint->no)
		{
			break;
		}
 		if (strcmp(headPoint->stationPoint->stationName.c_str(), stationPoint1->stationName.c_str()) == 0)
		{
			if (mode == 0)
			{
				break;
			}
			else
			{
				if (no == MAX)
				{
					no = headPoint->no;
				}
				headPoint = headPoint->nextPoint;
				continue;
			}
		}
		int transLineNo = headPoint->stationPoint->transLineNo;
		headPoint->stationPoint->mark = 2;
		
		struct Station *temp;
		for (int i = 0; i < transLineNo; i++)
		{
			if (mode == 1 && (strcmp(lineName.c_str(), "") == 0
				|| strcmp(lineName.c_str(), headPoint->stationPoint->transLine[i]->lineName.c_str()) == 0))
			{
				struct Queue *tempQueue;
				temp = headPoint->stationPoint->transLine[i]->lastpoint;
				if (temp != NULL && (headPoint ->fromPoint == NULL
					||(headPoint->fromPoint != NULL 
						&& strcmp(temp->stationName.c_str(),headPoint->fromPoint->stationPoint->stationName.c_str())!=0)))
				{
					temp->mark = 1;
					tempQueue = (struct Queue*)new(struct Queue);
					tempQueue->fromPoint = headPoint;
					tempQueue->stationPoint = temp;
					tempQueue->nextPoint = headPoint->nextPoint;
					headPoint->nextPoint = tempQueue;
					tempQueue->lineName = headPoint->stationPoint->transLine[i]->lineName;
					tempQueue->no = headPoint->no;
				}
				temp = headPoint->stationPoint->transLine[i]->nextpoint;
				if (temp != NULL && (headPoint->fromPoint == NULL
					|| (headPoint->fromPoint != NULL
						&& strcmp(temp->stationName.c_str(), headPoint->fromPoint->stationPoint->stationName.c_str()) != 0)))
				{
					temp->mark = 1;
					tempQueue = (struct Queue*)new(struct Queue);
					tempQueue->fromPoint = headPoint;
					tempQueue->stationPoint = temp;
					tempQueue->nextPoint = headPoint->nextPoint;
					headPoint->nextPoint = tempQueue;
					tempQueue->lineName = headPoint->stationPoint->transLine[i]->lineName;
					tempQueue->no = headPoint->no;
				}
				while (tailPoint->nextPoint != NULL)
				{
					tailPoint = tailPoint->nextPoint;
				}
			}
			else
			{
				temp = headPoint->stationPoint->transLine[i]->lastpoint;
				if (temp != NULL && temp->mark != 2)
				{
					temp->mark = 1;
					tailPoint->nextPoint = (struct Queue*)new(struct Queue);
					tailPoint = tailPoint->nextPoint;
					tailPoint->fromPoint = headPoint;
					tailPoint->stationPoint = temp;
					tailPoint->nextPoint = NULL;
					tailPoint->lineName = headPoint->stationPoint->transLine[i]->lineName;
					tailPoint->no++;
				}
				temp = headPoint->stationPoint->transLine[i]->nextpoint;
				if (temp != NULL && temp->mark != 2)
				{
					temp->mark = 1;
					tailPoint->nextPoint = (struct Queue*)new(struct Queue);
					tailPoint = tailPoint->nextPoint;
					tailPoint->fromPoint = headPoint;
					tailPoint->stationPoint = temp;
					tailPoint->nextPoint = NULL;
					tailPoint->lineName = headPoint->stationPoint->transLine[i]->lineName;
					tailPoint->no++;
				}
			}
		}
		headPoint = headPoint->nextPoint;
	}
	return getPath(firstPoint, tailPoint, stationPoint1,mode);
}

string Data::getPath(struct Queue *headPoint,struct Queue *tailPoint, Station *stationPoint1,int mode)
{
	while (true)
	{
		if (strcmp(headPoint->stationPoint->stationName.c_str(), stationPoint1->stationName.c_str()) == 0)
		{
			int no = 1;
			int num = 1;
			string path = "";
			struct Queue *temp = headPoint;
			string lineName = temp->lineName;
			while(true)
			{
				path += temp->stationPoint->stationName;
				if(strcmp(lineName.c_str(),temp->lineName.c_str())!=0
					&& strcmp("", temp->lineName.c_str())!=0)
				{
					string v = "换乘";
					path += v;
					lineName = temp->lineName;
					path += lineName;
					no++;
				}
				path += enter;
				if (temp->fromPoint == NULL)
				{
					break;
				}
				temp = temp->fromPoint;
				num++;
			}
			char ch[3];
			sprintf_s(ch, "%d", num);
			path = ch + enter + path;
			//cout << path;
			if (headPath == NULL)
			{
				headPath = (struct Path*)new(struct Path);
				headPath->nextPoint = NULL;
				headPath->no = no;
				headPath->num = num;
				headPath->path = path;
			}
			else
			{
				if ((mode == 1 && (no < headPath->no 
					|| (no == headPath->no && num <= headPath->num)))
					||(mode == 0 && num <= headPath->num))
				{
					struct Path *tempPathPoint = (struct Path*)new(struct Path);
					tempPathPoint->nextPoint = headPath;
					headPath = tempPathPoint;
					headPath->no = no;
					headPath->num = num;
					headPath->path = path;
				}
				else
				{
					struct Path *tempPathPoint = (struct Path*)new(struct Path);
					tempPathPoint->nextPoint = headPath->nextPoint;
					headPath->nextPoint = tempPathPoint;
					tempPathPoint->no = no;
					tempPathPoint->num = num;
					tempPathPoint->path = path;
				}
			}
		}

		if (headPoint == tailPoint)
		{
			break;
		}
		headPoint = headPoint->nextPoint;
	}
	if (headPath == NULL)
	{
		return "No Found Error!";
	}
	return headPath->path;
	/*
	struct Path *tempPoint = headPath;
	string allPath = "";
	int sno = MAX;
	int tno = (mode == 1) ? tempPoint->no : tempPoint->num;
	while (sno >= tno) 
	{
		allPath += tempPoint->path;
		allPath += enter;
		tempPoint = tempPoint->nextPoint;
		if (tempPoint == NULL)
		{
			break;
		}
		sno = tno;
		tno = (mode == 1) ? tempPoint->no : tempPoint->num;
	}
	return allPath;
	*/
}

string Data::transferStationAtLeast(Station *stationPoint1, Station *stationPoint2)
{
	return passStationAtLeast(stationPoint1, stationPoint2,TRANSFER_MODE);
}