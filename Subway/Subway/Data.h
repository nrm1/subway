#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<iostream>

#define TRANS_POINT_MAX 3
#define ALL_LINE_MAX 20
#define MAX 999
#define TRANSFER_MODE 1

using namespace std;

struct LineInfo
{
	string lineName;
	struct Station *lastpoint, *nextpoint;
};

struct Station
{
	string stationName;
	int transLineNo;
	struct LineInfo *transLine[TRANS_POINT_MAX];
	int mark;
};

struct Queue
{
	struct Station *stationPoint;
	struct Queue *nextPoint;
	struct Queue *fromPoint;
	string lineName;
	int no = 0;//换乘车站数
};

struct Path
{
	string path;
	int no;//换乘车站数
	int num;//经过站点数
	struct Path *nextPoint;
};

class Data
{
private:
	int lineTop = 0;
	struct LineInfo *firstPoint[ALL_LINE_MAX];

	int pathTop = 0;
	struct Path *headPath = NULL;

	string enter = "\n";
	string getPath(struct Queue *, struct Queue *, Station *,int);
	bool isPass(struct Queue *);
public:
	void modifyMap();
	bool readData();
	struct Station* findStation(string);
	string putOutData(string);
	string passAllStation(struct Station*);
	string passStationAtLeast(struct Station*, struct Station*,int=0);
	string transferStationAtLeast(struct Station*, struct Station*);
};