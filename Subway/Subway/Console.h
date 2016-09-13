#pragma once
#include<string>
#include"Data.h"

class Console
{
private:
	string stationName1 = "", stationName2 = "";
public:
	bool isParameter(Data,int,char**,char*,struct Station**,struct Station**);
	string findPath(Data,char, struct Station*, struct Station*);
};