#pragma once
#include "afxwin.h"
#include <map>
#include <vector>
#include <string>
#include "ugc.h"
#include "Database.h"
#include <sstream>
#include <iomanip>
class CurrentValues :
	public CWnd
{
public:
	CurrentValues();
	~CurrentValues();
	void Init(Database* _database);
	void SetSelected(int new_selected);
	int GetSelected() { return selected; }
	double getDPIX();
private:
	int selected;
	Database* database;
	int Width;
	int Height;
	vector<string> variable_names;
	vector<string> lables;
	string DoubleToString(double val); 
	string ToString(string var_name, double value);
protected:
	void SetBounds();
	void OnSize(UINT nType, int cx, int cy);
	void OnPaint();
	



	DECLARE_MESSAGE_MAP();
};

