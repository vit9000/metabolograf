#ifndef Algorithm_h
#define Algorithm_h
#include <vector>
#include <map>
#include <string>
#include "Datetime.h"
#include "Variable.h"

using namespace std;



class Algorithm
{
public:

	void Temp()
	{}

	vector<int> getIntervalsByTime(Variable_<MTime>& time, string interval)
	{
		int size = time.size();
	
		Variable_<MTime> timevector;
		
		vector<int> intervals;

		for (int i = 0; i < size; i++)
		{
			MTime temptime = time[i].getEndTimeOfInterval(interval);
			intervals.push_back(i);
			timevector.append(temptime);

			while (temptime > time[i])
			{
				i++;
				if (i >= size) break;
			}
			i--;
		}
		intervals.push_back(size-1);
		swap(timevector, time);
		return intervals;
	}
	//------------------------------------------------------
	vector<MTime> getTimeByIntervals(vector<Datetime>& datetime, const vector<int>& intervals)
	{
		vector<MTime> result;
		for (size_t i = 1; i < intervals.size(); i++)
		{
			int temp = intervals[i] - 1;
			if (temp < 0) temp = 0;
			if (temp >= static_cast<int>(datetime.size())) temp = static_cast<int>(datetime.size()) - 1;
			result.push_back(datetime[temp].getTime());
		}
		return result;
	}

};
#endif

