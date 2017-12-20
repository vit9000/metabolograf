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


public:
	MyListCtrl()
	{
		dpiX = DPIX();
		busy = false;
	}
	~MyListCtrl()
	{}

public:
	void Init(Database* _database)
	{
		database = _database;
		
		LoadConfig();
		SetHeadersInList();
	}
	bool IsBusy() { return busy; }
	//-------------------------------------------------------------------------------------------
	void Clear()
	{
		marks.clear();
		errmarks.clear();
		DeleteAllItems();
		for (int i = 0; i<database->variable_names.size()+2; i++)
		{
			DeleteColumn(0);
		}
	}
	void InsertParameter(string param, int pos)
	{
		show_parameters.insert(show_parameters.begin() + pos, param);
		SetHeadersInList();
	}

	void InsertParameterAfter(string param, string prevparam)
	{
		int pos=0;
		for (int i=0; i < show_parameters.size(); i++)
		{
			if (show_parameters[i] == prevparam)
			{
				pos = i+1;
				break;
			}
		}
		
		show_parameters.insert(show_parameters.begin() + pos, param);
		SetHeadersInList();
	}

	void SetSelectedItem(int index)
	{
		busy = true;
		
		

		SetSelectionMark(index);
		//SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
//		RedrawItems(index, index);

		
		busy = false;

	}
	void UnSelectCurrentItem()
	{
		int old = GetSelectedItem();
		if (old >= 0)
		{
			SetSelectionMark(old);
			SetItemState(old, 0, LVIS_SELECTED | LVIS_FOCUSED);
			//			RedrawItems(old, old);
		}
	}
	int GetSelectedItem()
	{
		return GetNextItem(-1, LVNI_FOCUSED);;
	}
	//-------------------------------------------------------------------------------------------
	void SetHeadersInList()
	{
		Clear();

		InsertColumn(0, "N", LVCFMT_CENTER, 50*dpiX, 0);//добавляем колонки
		InsertColumn(1, "Время", LVCFMT_CENTER, 60*dpiX, 0);//добавляем колонки

		//vector<string>& headers = database->variable_names;
		int index=2;
		for(int i=0; i<show_parameters.size(); i++)
		{
			string temp = show_parameters[i];
			for (char& t : temp)
				if (t == '_') t = ' ';
				InsertColumn(index, temp.c_str(), LVCFMT_CENTER, (temp.length() * 6 + 40)*dpiX, index);//добавляем колонки
				index++;
			
		}
	}
	//-------------------------------------------------------------------------------------------
	void AddToList(int i)
	{
		busy = true;
		int Count = GetItemCount();
		int index = InsertItem(LVIF_TEXT, Count, to_string(i).c_str(), 0, 0, 0, NULL);

		SetItemText(index, 1, database->datetime[i].getTimeString().c_str());//time
		int column = 2;
		int col=0;
		for (auto& varname : show_parameters)
		{
			int size = database->variables[varname].size();
			if (i < size)
			{
				double value = database->variables[varname][i];
				string str = ToString(varname, value);//to_string(database->variables[varname][i]);
				if (str == "НД")
					errmarks.insert({ index, column });
				else if (varname == "ДМП" && (value > 55 || value < 33))
					marks.insert({ index, column });

				SetItemText(index, column, str.c_str());
			}
			column++;
			
		}

		SetCheck(i, database->checked[i]);
		busy = false;
	}
	//-------------------------------------------------------------------------------------------
	string ToString(string var_name, double value)
	{
		if (value < 0) return string("НД");

		int precision = 3;
		if (var_name == "Метаболизм_O2" ||
			var_name == "Метаболизм_CO2" ||
			var_name == "ДМП" || var_name == "ЧСС" || var_name == "RR" || var_name == "SD")
			return to_string(static_cast<int>(value));

		else if (var_name == "ЧД" || var_name == "ЧД_old" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2")
			precision = 1;
		else if (var_name == "Дыхательный_коэффициент")
		{
			if (value < 0.01) return string ("НД");
			precision = 2;
		}

		stringstream ss;
		ss << setprecision(precision) <<fixed << value;

		return ss.str();
	}
	//-------------------------------------------------------------------------------------------
	void WriteConfig()
	{
		CString fname;
		char buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		fname.Format("%s\\metabolograf.ini", buffer);

		DeleteFile(fname);

		for (auto& varname: show_parameters)
		{
			//if(database->variables[database->variable_names[i]].isConst())//сохраняем только константные переменные
			WritePrivateProfileString("Variable_names", varname.c_str(), "1",  fname);
		}	
	}
	//-------------------------------------------------------------------------------------------
	void LoadConfig()
	{
		show_parameters.clear();
		//show_parameters.resize(database->variable_names.size(), 1);
		

		CString fname;
		char buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		fname.Format("%s\\metabolograf.ini", buffer);

		if (!PathFileExists(fname))
		{
			
			show_parameters.push_back("Vвдоха");
			show_parameters.push_back("Vвыдоха");
			show_parameters.push_back("ЧД");
			show_parameters.push_back("FiO2");
			show_parameters.push_back("FetO2");
			//show_parameters.push_back("FiCO2");
			//show_parameters.push_back("FetCO2");
			//show_parameters.push_back("Vвдоха_без_МП");
			//show_parameters.push_back("Vвыдоха_без_МП");
			//show_parameters.push_back("Потребление_O2");
			//show_parameters.push_back("Выделение_CO2");
			show_parameters.push_back("Минутное_потребление_O2");
			show_parameters.push_back("Минутное_выделение_CO2");
			show_parameters.push_back("Дыхательный_коэффициент");
			//show_parameters.push_back("Метаболизм_O2");
			show_parameters.push_back("Метаболизм_CO2");
			show_parameters.push_back("ДМП");
			return;
		}

		for (int i = 0; i<database->variable_names.size(); i++)
		{
			int defaultVal = 0;
			string& varname = database->variable_names[i];

			//if (i < 4) defaultVal = 0;//убираем скаляры константные
			int temp = (bool)GetPrivateProfileInt("Variable_names", varname.c_str(), defaultVal, fname);
			if (temp == 1)
				show_parameters.push_back(varname);
		}
	}
	//-------------------------------------------------------------------------------------------
	void ShowConfigDialog()
	{
		MainListConfigDialog cfDlg;
		vector<string> varnames;
		for (auto& varname : database->variable_names)
		{
			if (database->variables[varname].GetType() == Vector)
				varnames.push_back(varname);
		}
		//&database->variable_names


		cfDlg.Init(&show_parameters, &varnames);
		int result = cfDlg.DoModal();
		if (result == 1)
		{
			Reload();
			WriteConfig();
		}
	}
	//-------------------------------------------------------------------------------------------
	void Reload()
	{
		SetHeadersInList();
		int size = database->getCount();
		for (int i = 0; i < size; i++)
			AddToList(i);

	}
	//-------------------------------------------------------------------------------------------
	void OnDrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

		// Take the default processing unless we 
		// set this to something else below.
		*pResult = CDRF_DODEFAULT;

		// First thing - check the draw stage. If it's the control's prepaint
		// stage, then tell Windows we want messages for every item.

		if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		{
			// This is the notification message for an item.  We'll request
			// notifications before each subitem's prepaint stage.

			*pResult = CDRF_NOTIFYSUBITEMDRAW;
		}
		else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
		{
			// This is the prepaint stage for an item. Here's where we set the
			// item's text color. Our return value will tell Windows to draw the
			// item itself, but it will use the new color we set here.
			// We'll cycle the colors through red, green, and light blue.

			COLORREF crText;
			int line_index = static_cast<int>(pLVCD->nmcd.dwItemSpec);

			if(marks.count({ line_index , pLVCD->iSubItem })!=0)
				crText = RGB(255, 0, 0);
			else if ((database->hdata.StartTest!=0 && database->hdata.EndTest!=0 && line_index >= database->hdata.StartTest && line_index <= database->hdata.EndTest) ||
				(database->hdata.StartTest != 0 && database->hdata.EndTest == 0 && line_index >= database->hdata.StartTest))
				crText = RGB(15, 135, 0);
			else
				crText = RGB(0, 0, 0);

			if (errmarks.count({ static_cast<int>(pLVCD->nmcd.dwItemSpec) , pLVCD->iSubItem }) != 0)
				pLVCD->clrTextBk = RGB(255, 200, 200);
			else if(static_cast<int>(pLVCD->nmcd.dwItemSpec)%2)
				pLVCD->clrTextBk = RGB(240, 240, 240);
			else 
				pLVCD->clrTextBk = RGB(255, 255, 255);

			pLVCD->clrText = crText;

			*pResult = CDRF_DODEFAULT;
		}
	}

};
