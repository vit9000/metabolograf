#ifndef MTime_h
#define MTime_h

#include "string_extentions.h"
#include <time.h>

class MTime
{
	int Hour;
	int Minute;
	int Second;
public:
	MTime(bool CurrTime=false)
	{
		if (CurrTime)
			CurrentTime();
		else
		{
			Hour = 0;
			Minute = 0;
			Second = 0;
		}
	}


	MTime(int seconds)
	{
		Hour = seconds / 3600;
		seconds -= Hour * 3600;
		Minute = seconds / 60;
		seconds -= Minute * 60;
		Second = seconds;
	}

	

	MTime(int hour, int minute, int second)
	{
		Hour = hour;
		Minute = minute;
		Second = second;

		

		if (Second >= 60)
		{
			int temp = Second / 60;
			Minute += temp;
			Second = Second - temp * 60;
		}
		else if (Second < 0)
		{
			Second = -Second;
			int temp = Second / 60 + 1;
			Minute -= temp;
			Second = temp * 60 - Second;
		}
		if (Minute >= 60)
		{
			int temp = Minute / 60;
			Hour += temp;
			Minute = Minute - temp * 60;
		}
		else if (Minute < 0)
		{
			Minute = -Minute;
			int temp = Minute / 60 + 1;
			Hour -= temp;
			Minute = temp * 60 - Minute;
		}
		//if (Hour >= 24)
		//	Hour = 0;
		Hour;

	}
	void Zero()
	{
		Hour = 0;
		Minute = 0;
		Second = 0;
	}
	void CurrentTime()
	{
		time_t t = time(NULL);
		tm *tmp = localtime(&t);
		Hour = tmp->tm_hour;
		Minute = tmp->tm_min;
		Second = tmp->tm_sec;
	}

	string getString() const
	{
		string str = ToString(Hour, 2) + ":" + ToString(Minute, 2) + ":" + ToString(Second, 2);
		return str;
	}

	string getStringAlt() const
	{
		string str;
		str = ToString(Minute) + ":";
		str+=ToString(Second, 2);
		return str;
	}

	int getHour() const
	{
		return Hour;
	}

	int getMinute() const
	{
		return Minute;
	}

	int getSecond() const
	{
        return Second;
	}
	unsigned int toSec() const
	{
		return Hour * 3600 + Minute * 60 + Second;
	}

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
		//unsigned int temp = lhs.toSec() + (unsigned int)sec;
		//return MTime(temp);
		return MTime(lhs.Hour, lhs.Minute, lhs.Second + sec);
	}

	friend MTime operator+(const MTime& lhs, const MTime& rhs)
	{
		//unsigned int temp = lhs.toSec() + rhs.toSec();
		//return MTime(temp);

		return MTime(lhs.Hour + rhs.Hour, lhs.Minute + rhs.Minute, lhs.Second + rhs.Second);
	}

	friend MTime operator-(const MTime& lhs, int sec)
	{
		//unsigned int temp = lhs.toSec() - (unsigned int)sec;
		//return MTime(temp);
		return MTime(lhs.Hour, lhs.Minute, lhs.Second - sec);
	}

	friend MTime operator-(const MTime& lhs, const MTime& rhs)
	{
		//unsigned int temp = lhs.toSec() - rhs.toSec();
		//return MTime(temp);
		MTime time = MTime(lhs.Hour - rhs.Hour, lhs.Minute - rhs.Minute, lhs.Second - rhs.Second);
		return time;
	}

	MTime getEndTimeOfInterval(string _interval)
	{
		stringstream ss(_interval);
		unsigned int interval;
		string type;
		ss >> interval >> type;

		int H = Hour;
		int M = Minute;
		int S = Second;
		//переводим интервал в секуднды
		if (type == "sec" || type == "с" || type == "c" || type == "сек")
		{
			S += interval;
			S /= interval;
			S *= interval;
			
		}
		else if (type == "min" || type == "м" || type == "мин")
		{
			S = 0;
			M += interval;
		}
		else if (type == "hour" || type == "ч")
		{
			S = 0;
			M = 0;
			H += interval;
		}


		MTime t(H,M,S);
		return t;
	}
};

#endif
