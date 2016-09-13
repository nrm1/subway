#include<stdio.h>
#include"Data.h"
#include"Console.h"

int main(int argc,char* argv[])
{
	char mark;
	struct Station *stationPoint1 = NULL,*stationPoint2 = NULL;

	string path;

	Data data;
	Console con;
	if (!data.readData())
	{
		cout << "Read Data Error!" << endl;
	}
	if (!con.isParameter(data,argc, argv, &mark, &stationPoint1, &stationPoint2))
	{
		cout << "Input Parameter Error!" << endl;
	}
	path = con.findPath(data,mark, stationPoint1, stationPoint2);
	cout << path << endl;
	while (true)
	{
		string stationName;
		cin >> stationName;
		cout << data.putOutData(stationName);
	}
	return 0;
}