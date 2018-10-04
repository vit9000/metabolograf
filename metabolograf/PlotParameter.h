#ifndef PlotParameter_h
#define PlotParameter_h

#include "Variable.h"
#include "Algorithm.h"
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class PlotParameter
{
private:
	double start;
	double end;
	vector<string> legend;
	vector<string> varnames;
	
	vector<Variable> varY;
	int legendcount;

	bool autoStart;
	bool autoEnd;

public:
	

	PlotParameter()
	{
		start = 0;
		end = 0;
		legendcount = 0;
	}
	
	PlotParameter(string varname, vector<int>& intervals, Variable& _varY)
	{
		PlotParameter();
		legendcount = 0;
		

		// формируем оси Х и Y
		AddVariable(varname, intervals, _varY);
	}

	bool getAutoStart() const { return autoStart; }
	bool getAutoEnd() const { return autoEnd; }

	void AddVariable(string varname, vector<int>& intervals, Variable& _varY)
	{
		legend.push_back(getNameWithoutUnderline(varname));
		varnames.push_back(varname);

		if (intervals.size()==0)// копируем значения показателя
			varY.push_back(_varY);
		else//  осредняем
			varY.push_back(_varY.meanByIntervals(intervals));

		Variable& var = varY[varY.size() - 1];

		size_t size = var.size();
		if (size == 0)
		{
			start = 0;
			end = 0;
		}
		else
		{
			auto min_max = minmax_element(var.begin(), var.end());

			double min = *min_max.first;
			if (varY.size() == 1 || min < start)
				start = min;
			double max = *min_max.second;
			if (varY.size() == 1 || max > end)
				end = max;

			double temp = (end - start)*0.05;
			start -= temp;
			end += temp;
		}
		

		autoStart = true;
		autoEnd = true;
	}

	void setParameter(string parameter, string value)
	{
		int size = varY.size();
		// формируем границы прорисовки оси Y
		if (parameter == "startY" || parameter == "start" || parameter == "начало")
		{
			start = atof(value.c_str());
			autoStart = false;
		}
		
		else if (parameter == "endY" || parameter == "end" || parameter == "конец")
		{
			end = atof(value.c_str());
			autoEnd = false;
		}
		//сохраняем название для легеды 
		else if (parameter == "legend" || parameter == "легенда")
		{
			if (legendcount < static_cast<int>(legend.size()))
			{
				legend[legendcount] = value;
				legendcount++;
			}
		}
			
	}

	Variable& getVar(size_t index)
	{
		if (index >= varY.size())
		{
			throw runtime_error("Ошибка обращения к несуществующей переменной");
		}
		return varY[index];
	}
	

	const string& getLegend(int index) const
	{
		if (index >= static_cast<int>(varY.size()))
		{
			throw runtime_error("Ошибка обращения к несуществующей переменной");
		}
		return legend[index];
	}
	const string& getVarname(int index) const
	{
		if (index >= static_cast<int>(varnames.size()))
		{
			throw runtime_error("Ошибка обращения к несуществующей переменной");
		}
		return varnames[index];
	}

	size_t count() const
	{
		return varY.size();
	}

	size_t size(size_t index)
	{
		if (index >= varY.size())
		{
			throw runtime_error("Ошибка обращения к несуществующей переменной");
		}
		return varY[index].size();
	}

	double getStart() const
	{
		return start;
	}
	double getEnd() const
	{
		return end;
	}
	double getRange()
	{
		return end - start;
	}
	double getZoom(int height)
	{
		return static_cast<double>(height) / getRange();
	}


private:
	string getNameWithoutUnderline(string temp)
	{
		for(size_t i=0; i<temp.length(); i++)
			if (temp[i] == '_') temp[i] = ' ';
		return temp;
	}


};


#endif