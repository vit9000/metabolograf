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
		Add("V�����", "V����� (�)");
		Add("V������", "V������ (�)");
		Add("V�����_���_��", "V����� ��� �� (�)");
		Add("V������_���_��", "V������ ��� �� (�)");
		Add("�����������_O2", "VO2 (�)");
		Add("���������_CO2", "VCO2 (�)");
		Add("�����������_O2_��_��_���", "VO2 (��/��/���.)");
		Add("���������_CO2_��_��_���", "VCO2 (��/��/���.)");
		Add("��������_�����������_O2", "VO2 (�/���.)");
		Add("��������_���������_CO2", "VCO2 (�/���.)");
		Add("��������������_����������_O2", "���� O2");
		Add("��������������_����������_CO2", "���� CO2");
		Add("�����������_�����������", "��");
		Add("����������_O2", "���������� �� O2 (����/���.)");
		Add("����������_CO2", "���������� �� CO2 (����/���.)");
		Add("���", "���");
		Add("��������_����������", "���.����.(�)");
	}
};