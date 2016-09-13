#include "Console.h"



bool Console::isParameter(Data data,int argc, char **argv, char *mark, Station **stationPoint1, Station **stationPoint2)
{
	if (argc == 4)
	{
		if (strlen(argv[1]) != 2)
		{
			return false;
		}
		if (argv[1][0] != '-')
		{
			return false;
		}
		switch (argv[1][1])
		{
		case('b'):*mark = 'b'; break;
		case('c'):*mark = 'c'; break;
		default:return false;
		}

		stationName1 = string(argv[2]);
		stationName2 = string(argv[3]);

		*stationPoint1 = data.findStation(stationName1);
		*stationPoint2 = data.findStation(stationName2);
		if (*stationPoint1 != NULL && *stationPoint2 != NULL) {
			return true;
		}
		return false;
	}
	else if (argc == 3)
	{
		if (strlen(argv[1]) != 2)
		{
			return false;
		}
		if (argv[1][0] != '-')
		{
			return false;
		}
		if (argv[1][1] != 'a')
		{
			return false;
		}

		stationName1 = string(argv[2]);

		*stationPoint1 = data.findStation(stationName1);
		if (*stationPoint1 != NULL) {
			return true;
		}
		return false;
	}
	return false;
}

string Console::findPath(Data data,char mark, struct Station *stationPoint1, struct Station *stationPoint2)
{
	data.modifyMap();
	switch (mark)
	{
	case('a'):
		return data.passAllStation(stationPoint1);
	case('b'):
		return data.passStationAtLeast(stationPoint1, stationPoint2);
	case('c'):
		return data.transferStationAtLeast(stationPoint1, stationPoint2);
	default:
		return "Find Path Error!";
	}
}
