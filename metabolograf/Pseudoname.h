#pragma once

#include <map>
#include <string>
using namespace std;
#include "Ini.h"

class Pseudoname
{
private:
	map<string, string> pseudo_real;
	map<string, string> real_pseudo;
public:
	Pseudoname() 
	{
		Default();
		load(); 
	}
	~Pseudoname()
	{
		save();
	}


	void Add(const string& real, const string& pseudo)
	{
		pseudo_real[pseudo] = real;
		real_pseudo[real] = pseudo;
	}

	void clear()
	{
		pseudo_real.clear();
		real_pseudo.clear();
	}

	bool getPseudoName(const string& realname, string& pseudoname) const
	{
		if (real_pseudo.count(realname) == 0)
			return false;
		else
		{
			try {
				pseudoname = real_pseudo.at(realname);
			}
			catch (exception& ex)
			{
				MessageBox(0, ex.what(), "Err", MB_OK);
			}
		}
		return true;
	}

	bool getRealName(const string& pseudoname, string& realname) const
	{
		if (pseudo_real.count(pseudoname) == 0)
			return false;
		else
		{
			try {
				realname = pseudo_real.at(pseudoname);
			}
			catch (exception& ex)
			{
				MessageBox(0, ex.what(), "Err", MB_OK);
			}
		}
		return true;
	}

	void save()
	{
		Ini ini("metabolograf_config.ini");
		for (auto& var : real_pseudo)
		{
			ini.Write("Variables", var.first, var.second);
		}
	}

	void load()
	{
		Ini ini("metabolograf_config.ini");

		for (auto& var : real_pseudo)
		{
			string temp = ini.Read("Variables", var.first, var.second);
			Add(var.first, temp);
		}
		
		
	}

	void Default()
	{
		Add("Vвдоха", "Vвдоха (л)");
		Add("Vвыдоха", "Vвыдоха (л)");
		Add("Vвдоха_без_МП", "Vвдоха без МП (л)");
		Add("Vвыдоха_без_МП", "Vвыдоха без МП (л)");
		Add("Потребление_O2", "VO2 (л)");
		Add("Выделение_CO2", "VCO2 (л)");
		Add("Потребление_O2_мл_кг_мин", "VO2 (мл/кг/мин.)");
		Add("Выделение_CO2_мл_кг_мин", "VCO2 (мл/кг/мин.)");
		Add("Минутное_потребление_O2", "VO2 (л/мин.)");
		Add("Минутное_выделение_CO2", "VCO2 (л/мин.)");
		Add("Вентиляционный_эквивалент_O2", "ВЭКВ O2");
		Add("Вентиляционный_эквивалент_CO2", "ВЭКВ CO2");
		Add("Дыхательный_коэффициент", "ДК");
		Add("Метаболизм_O2", "Метаболизм по O2 (ккал/сут.)");
		Add("Метаболизм_CO2", "Метаболизм по CO2 (ккал/сут.)");
		Add("ДМП", "ДМП");
		Add("Минутная_вентиляция", "Мин.вент.(л)");
	}
};