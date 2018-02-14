#pragma once
#include "afxcmn.h"

#include "Database.h"
#include "MainListConfigDialog.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>
#include <utility>
#include "DPIX.h"
#include "Ini.h"
#include "ugc.h"
#include "IExperienceStatusTracker.h"
#include "CFastListCtrl.h"

using namespace std;


class MyListCtrl : public CFastListCtrl
{
protected:
	const string ini_filename;
	Database *database;
	vector<string> show_parameters;
	set<vector<int>> marks;
	set<vector<int>> errmarks;
	bool busy;
	double dpiX;
	IExperienceStatusTracker* experienceStatusTracker;

	string ToString(string var_name, double value);
	
	void MyInsertColumn(int index, const string& param, Ini& ini);
	
	
public:
	void WriteConfig() const;
	MyListCtrl();
	virtual ~MyListCtrl();
	void Init(IExperienceStatusTracker* ExperienceStatusTracker);
	bool IsBusy() const { return busy; }
	void InsertParameter(string param, int pos);
	void InsertParameterAfter(string param, string prevparam);
	void AddToList(int i);
	void LoadConfig();
	void SetHeadersInList();
	
	void ShowConfigDialog();
	void Reload();
	void Clear();

	inline const vector<string>& getShowParameters() const 
	{
		return show_parameters;
	}
	
	
	//void OnDrawMyList(NMHDR* pNMHDR, LRESULT* pResult);
	
};
