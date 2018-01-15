#ifndef MTime_h
#define MTime_h

#include "string_extentions.h"
#include <time.h>

class MTime
{
private:
	int Hour;
	int Minute;
	int Second;
public:
	MTime(bool CurrTime = false);
	MTime(int seconds);
	MTime(int hour, int minute, int second);
	void Zero();
	void CurrentTime();
	string getString() const;
	string getStringAlt() const;
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	unsigned int toSec() const;
	MTime getEndTimeOfInterval(string _interval);

	friend bool operator<(const MTime& time1, MTime time2)
	{
		if (time1.getString() < time2.getString())
			return true;
		return false;
	}
	friend bool operator<=(const MTime& time1, const MTime& time2)
	{
		if (time1.getString() <= time2.getString())
			return true;
		return false;
	}
	friend bool operator>(const MTime& time1, const MTime& time2)
	{
		if (time1.getString() > time2.getString())
			return true;
		return false;
	}
	friend bool operator>=(const MTime& time1, const MTime& time2)
	{
		if (time1.getString() > time2.getString())
			return true;
		return false;
	}

	friend bool operator==(const MTime& time1, const MTime& time2)
	{
		if (time1.getString() == time2.getString())
			return true;
		return false;
	}

	friend bool operator!=(const MTime& time1, const MTime& time2)
	{
		if (time1.getString() != time2.getString())
			return true;
		return false;
	}

	friend MTime operator+(const MTime& lhs, int sec)
	{
		return MTime(lhs.Hour, lhs.Minute, lhs.Second + sec);
	}

	friend MTime operator+(const MTime& lhs, const MTime& rhs)
	{
		return MTime(lhs.Hour + rhs.Hour, lhs.Minute + rhs.Minute, lhs.Second + rhs.Second);
	}

	friend MTime operator-(const MTime& lhs, int sec)
	{
		return MTime(lhs.Hour, lhs.Minute, lhs.Second - sec);
	}

	friend MTime operator-(const MTime& lhs, const MTime& rhs)
	{
		MTime time = MTime(lhs.Hour - rhs.Hour, lhs.Minute - rhs.Minute, lhs.Second - rhs.Second);
		return time;
	}

	
};

#endif
