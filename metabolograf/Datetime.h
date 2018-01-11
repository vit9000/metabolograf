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
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	MTime getTime() const;
	string getDateString() const;
	string getDateStringRU() const;
	string getDateStringNormal() const;
	string getTimeString() const;
	string getDatetimeString() const;
};

#endif