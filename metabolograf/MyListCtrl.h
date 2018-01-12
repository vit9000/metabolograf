#pragma once
#include "afxcmn.h"

#include "Database.h"
#include "MainListConfigDialog.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>
#include "DPIX.h"
#include "Ini.h"
using namespace std;


class MyListCtrl : public CListCtrl
{
protected:
	Database *database;
	vector<string> show_parameters;
	set<vector<int>> marks;
	set<vector<int>> errmarks;
	bool busy;
	double dpiX;


	string ToString(string var_name, double value);
	void WriteConfig() const;
	
	
public:
	MyListCtrl();
	virtual ~MyListCtrl(){}
	void Init();
	bool IsBusy() const { return busy; }
	void InsertParameter(string param, int pos);
	void InsertParameterAfter(string param, string prevparam);
	void SetSelectedItem(int index);
	void UnSelectCurrentItem();
	int GetSelectedItem() const;
	void SetHeadersInList();
	void AddToList(int i);
	void LoadConfig();
	void OnDrawMyList(NMHDR* pNMHDR, LRESULT* pResult);
	void ShowConfigDialog();
	void Reload();
	void Clear();
	
	
};
