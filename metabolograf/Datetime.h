#ifndef Datetime_h
#define Datetime_h
#include <string>
#include "MTime.h"

using namespace std;

class Datetime
{
private:
	int Year;
	int Month;
	int Day;
	MTime time;
public:
	Datetime();
	Datetime(int year, int month, int day, int hour, int minute, int second);
	int getHour();
	int getMinute();
	int getSecond();
	MTime getTime();
	string getDateString();
	string getDateStringRU();
	string getDateStringNormal();
	string getTimeString();
	string getDatetimeString();
};

#endif