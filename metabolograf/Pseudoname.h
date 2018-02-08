#pragma once

#include <map>
#include <string>
using namespace std;

class Pseudoname
{
private:
	map<string, string> pseudo_real;
	map<string, string> real_pseudo;
public:
	Pseudoname() {}

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
};