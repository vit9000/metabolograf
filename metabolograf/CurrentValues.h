#pragma once
#include "afxwin.h"
#include <map>
#include <vector>
#include <string>
#include "ugc.h"
#include "Database.h"
#include "DPIX.h"
#include <sstream>
#include <iomanip>
#include "Metab.h"
class CurrentValues :
	public CWnd
{
public:
	CurrentValues(const Metab &metab);
	~CurrentValues();
	void Init();
	void SetSelected(int new_selected);
	int GetSelected() { return selected; }
	
private:
	int selected;
	const Metab &m_rMetab;
	Database* database;
	int Width;
	int Height;
	vector<string> variable_names;
	vector<string> lables;
	string DoubleToString(double val); 
	string ToString(string var_name, double value);
	double CalculateErr();
	double OldCalculateErr();
	void SetBounds();
	void OnSize(UINT nType, int cx, int cy);
	void OnPaint();
	



	DECLARE_MESSAGE_MAP();
};

