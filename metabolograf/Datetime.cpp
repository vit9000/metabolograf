#include "stdafx.h"
#include "Datetime.h"

Datetime::Datetime()
{
	Year = 0;
	Month = 0;
	Day = 0;

}
Datetime::Datetime(int year, int month, int day, int hour, int minute, int second)
{
	Year = year;
	Month = month;
	Day = day;
	time = MTime(hour, minute, second);

}
int Datetime::getHour() const
{
	return time.getHour();
}
int Datetime::getMinute() const
{
	return time.getMinute();
}
int Datetime::getSecond() const
{
	return time.getSecond();
}

MTime Datetime::getTime() const
{
	return time;
}
//--------------------------------------------------------------------------
string Datetime::getDateString() const
{
	string str = ToString(Year, 4) + "-" + ToString(Month, 2) + "-" + ToString(Day, 2);
	return str;
}
string Datetime::getDateStringRU() const
{
	string str = ToString(Day, 2) + "." + ToString(Month, 2) + "." + ToString(Year, 4);
	return str;
}
string Datetime::getDateStringNormal() const
{
	string str = ToString(Day, 2) + "." + ToString(Month, 2) + "." + ToString(Year, 4);
	return str;
}
//--------------------------------------------------------------------------
string Datetime::getTimeString() const
{

	return time.getString();
}
//--------------------------------------------------------------------------
string Datetime::getDatetimeString() const
{
	return  getDateString() + " " + getTimeString();
}

string Datetime::getDatetimeStringNormal() const
{
	return  getDateStringNormal() + " " + getTimeString();
}

