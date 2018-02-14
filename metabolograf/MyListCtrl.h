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
#include <algorithm>
#include <iterator>

#include "DPIX.h"
#include "Ini.h"
#include "ugc.h"
#include "IExperienceStatusTracker.h"
using namespace std;


class MyListCtrl : public CListCtrl
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
	
	void DeleteUnselectedColumns(const vector<string>& sorted_new_parameters, const vector<string>& sorted_old_parameters, vector<string>& old_parameters);
	void AddSelectedColumns(const vector<string>& sorted_new_parameters, const vector<string>& sorted_old_parameters);
	
public:
	void WriteConfig() const;
	MyListCtrl();
	virtual ~MyListCtrl();
	void Init(IExperienceStatusTracker* ExperienceStatusTracker);
	bool IsBusy() const { return busy; }
	void InsertParameter(string param, int pos);
	void InsertParameterAfter(string param, string prevparam);
	void SetSelectedItem(int index);
	void UnSelectCurrentItem();
	int GetSelectedItem() const;
	vector<int> GetSelectedItems() const;
	void SetHeadersInList();
	void AddToList(int i);
	void AddParameter(const string& param, int column, int index);
	void LoadConfig();
	void OnDrawMyList(NMHDR* pNMHDR, LRESULT* pResult);
	void ShowConfigDialog();
	void Reload();
	void Clear();

	

	inline const vector<string>& getShowParameters() const 
	{
		return show_parameters;
	}
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
};
