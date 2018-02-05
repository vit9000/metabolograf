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
#include "Masks.h"


using namespace std;

class Database
{

private:
	Database();
	Database(const Database&) = delete;
	Database& operator=(Database&) = delete;
	
	struct CalculatedMetab
	{
		double Benedict;
		double Muffin_Jeor;
		double Katch_MacArdle;
	} calculatedMetab;

	Masks masks;
	double ADS;//дополнительное мертвое пространство

	HeaderDatabase hdata;// заголовок файла
	VariableBOOL checked; // галки каждого измерения: вкл - обрабатывается, выкл - нет
	Variable_<Datetime> datetime; // дата и время каждого измерения
	vector<string> variable_names; // список переменных
	map<string, Variable> variables; // все переменные
public:

	static Database* getInstance()//ключевая часть паттерна Одиночка
	{
		static Database p_instance;	
		return &p_instance;
	}

	void insertVariableName(const string& name) { variable_names.push_back(name); }
	const vector<string>& getVariableNames() const { return variable_names; }
	const Variable& getVariable(string var_name) const { return variables.at(var_name); };
	const map<string, Variable>& getVariables() const { return variables; };
	bool isVariableExists(string var_name) { return variables.count(var_name) > 0; }

	const CalculatedMetab& getCalculatedMetab() const { return calculatedMetab;}
	const VariableBOOL& getChecked() { return checked; }
	
	bool getChecked(size_t i) { return checked[i]; }
	void setChecked(size_t i, bool status) { checked[i] = status; }
	HeaderDatabase& getHeader() { return hdata; }

	const Variable_<Datetime>& getDatetime() const { return datetime; }
	const Datetime& getDatetime(size_t i) const { return datetime.at(i); }

	// добавление значений
	void Append(bool status) { checked.append(status); }
	void Append(Datetime dt) { datetime.append(dt); }
	void Append(string var_name, double value) { variables[var_name].append(value); }
	// добавление переменных
	void AddVariable(string var_name) { variable_names.push_back(var_name);  }

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