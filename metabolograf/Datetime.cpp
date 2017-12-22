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
int Datetime::getHour()
{
	return time.getHour();
}
int Datetime::getMinute()
{
	return time.getMinute();
}
int Datetime::getSecond()
{
	return time.getSecond();
}

MTime Datetime::getTime()
{
	return time;
}
//--------------------------------------------------------------------------
string Datetime::getDateString()
{
	string str = ToString(Year, 4) + "-" + ToString(Month, 2) + "-" + ToString(Day, 2);
	return str;
}
string Datetime::getDateStringRU()
{
	string str = ToString(Day, 2) + "." + ToString(Month, 2) + "." + ToString(Year, 4);
	return str;
}
string Datetime::getDateStringNormal()
{
	string str = ToString(Day, 2) + "." + ToString(Month, 2) + "." + ToString(Year, 4);
	return str;
}
//--------------------------------------------------------------------------
string Datetime::getTimeString()
{

	return time.getString();
}
//--------------------------------------------------------------------------
string Datetime::getDatetimeString()
{
	return  getDateString() + " " + getTimeString();
}

