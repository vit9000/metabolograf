#pragma once
#include <vector>
#include <utility>
#include <exception>
#include "Ini.h"
using namespace std;

struct Mask
{
	string name;
	int volume;
	Mask() : volume(0) {}
	Mask(string Name, int Volume) : name(Name), volume(Volume)
	{}
};

class Masks
{
private:
	vector<Mask> data;
	string filename;
	int m_iLastUsed;
public:
	Masks()
		: filename("metabolograf_config.ini")
		, m_iLastUsed(0)
	{
		Ini ini(filename);
		int count = ini.Read("Masks", "count", 0);
		if (count > 0)
		{
			data.resize(count);
			int counter = 0;
			for (auto& d : data)
			{
				stringstream ss;
				ss << counter;
				d.name = ini.Read("Masks", ss.str(), ss.str());
				d.volume = ini.Read("Masks", d.name, 0);
				counter++;
			}
		}
		else LoadDefaultValues();
		m_iLastUsed = ini.Read("Masks", "last_used", 0);

	}

	void setLastUsed(int lastUsed)
	{
		Ini ini(filename);
		ini.Write("Masks", "last_used", lastUsed);
	}

	inline int getLastUsed() { return m_iLastUsed; }
	size_t count() { return data.size(); }
	const Mask& getMask(size_t index) const
	{
		if (index >= data.size())
			throw invalid_argument("index>=data.size()");
		return data[index];
	}

private:
	void LoadDefaultValues()
	{	
		data.push_back({ "Без маски",	0 });
		data.push_back({ "F&P small",	120 });
		data.push_back({ "F&P medium",	130 });
		data.push_back({ "HR medium",	200 });
		data.push_back({ "HR large",	220 });

		Ini ini(filename);
		ini.Write("Masks", "count", static_cast<int>(data.size()));
		int counter = 0;
		for (const auto& d : data)
		{
			stringstream ss;
			ss << counter;
			ini.Write("Masks", ss.str(), d.name);
			ini.Write("Masks", d.name, d.volume);
			++counter;
		}
	}

};