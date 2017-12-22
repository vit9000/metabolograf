#ifndef Database_h
#define Database_h

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include "Variable.h"
#include "Structures.h"
#include "Datetime.h"
#include "Ini.h"


using namespace std;

class Database
{
public:
	struct HeaderDatabase hdata;
	Variable_<Datetime> datetime;
	VariableBOOL checked;
	map<string, Variable> variables;
	vector<string> variable_names;

	double Benedict;
	double Muffin_Jeor;
	double Katch_MacArdle;
	double ADS;//дополнительное мертвое пространство

	Database();
	virtual ~Database();
	size_t getCount();
	void Default();
	Variable_<MTime> GetTimeFromZero(int zerotime = 0);
	void DefaultVarNames();
	void CalculateMetab();
	int getHour(int index);
	int getMinute(int index);
	bool SaveFile(const char *fname);
	bool OpenFile(const char *fname);
	void SetVariable(string name, Variable& var, bool constant = false);
	void AddPatientParametersToVariables();
	void Filter(int index);
	void Clear();
	double ChildTV(int x/*month*/);
	void CalculateParameters();
	void FillSD();
	void CalculateSD(int i, bool checktime = true);
	void CalculateParameters(int i);
	void DeleteUncheckedValues();
	string  GetHTMLHeader();
	



};




#endif