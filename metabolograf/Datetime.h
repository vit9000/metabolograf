#ifndef Datetime_h
#define Datetime_h
#include <string>

#include "MTime.h"

class Datetime
{
private:
	int Year;
	int Month;
	int Day;
	MTime time;
public:
	Datetime()
	{
		Year=0;
		Month=0;
		Day=0;
		
	}
	Datetime(int year, int month, int day, int hour, int minute, int second)
	{
		Year = year;
		Month = month;
		Day = day;
		time = MTime(hour, minute, second);

	}
	int getHour()
	{
		return time.getHour();
	}
	int getMinute()
	{
		return time.getMinute();
	}
	int getSecond()
	{
		return time.getSecond();
	}

	MTime getTime()
	{
		return time;
	}
	//--------------------------------------------------------------------------
	string getDateString()
	{
		string str = ToString(Year,4)+"-"+ToString(Month,2)+"-"+ToString(Day,2);
		return str;
	}
	string getDateStringRU()
	{
		string str = ToString(Day, 2) +"."+ ToString(Month, 2) + "." + ToString(Year, 4);
		return str;
	}
	string getDateStringNormal()
	{
		string str = ToString(Day, 2) + "." + ToString(Month, 2) + "." + ToString(Year, 4);
		return str;
	}
    //--------------------------------------------------------------------------
	string getTimeString()
	{
		
		return time.getString();
	}
	//--------------------------------------------------------------------------
	string getDatetimeString()
	{
		return  getDateString() + " " + getTimeString();
	}
};

#endif