#include "stdafx.h"
#include "MyListCtrl.h"


BEGIN_MESSAGE_MAP(MyListCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


MyListCtrl::MyListCtrl()
{
	dpiX = DPIX();
	busy = false;
	experienceStatusTracker = nullptr;
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::Init(IExperienceStatusTracker* ExperienceStatusTracker)
{
	database = Database::getInstance();
	experienceStatusTracker = ExperienceStatusTracker;
	LoadConfig();
	SetHeadersInList();
}
MyListCtrl::~MyListCtrl()
{
	//WriteConfig();
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::Clear()
{
	marks.clear();
	errmarks.clear();
	DeleteAllItems();
	for (size_t i = 0; i<database->getVariableNames().size() + 2; i++)
	{
		DeleteColumn(0);
	}
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::InsertParameter(string param, int pos)
{
	show_parameters.insert(show_parameters.begin() + pos, param);
	SetHeadersInList();
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::InsertParameterAfter(string param, string prevparam)
{
	int pos = 0;
	for (int i = 0; i < show_parameters.size(); i++)
	{
		if (show_parameters[i] == prevparam)
		{
			pos = i + 1;
			break;
		}
	}

	show_parameters.insert(show_parameters.begin() + pos, param);
	SetHeadersInList();
}

void MyListCtrl::SetSelectedItem(int index)
{
	busy = true;



	SetSelectionMark(index);
	//SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
	SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//		RedrawItems(index, index);


	busy = false;

}
void MyListCtrl::UnSelectCurrentItem()
{
	int old = GetSelectedItem();
	if (old >= 0)
	{
		SetSelectionMark(old);
		SetItemState(old, 0, LVIS_SELECTED | LVIS_FOCUSED);
		//			RedrawItems(old, old);
	}
}

vector<int> MyListCtrl::GetSelectedItems() const
{
	vector<int> v;
	POSITION pos = this->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = this->GetNextSelectedItem(pos);
			
			v.push_back(nItem);
		}
	}
	

	return v;
}

int MyListCtrl::GetSelectedItem() const
{
	return GetNextItem(-1, LVNI_FOCUSED);;
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::SetHeadersInList()
{
	Clear();

	InsertColumn(0, "N", LVCFMT_CENTER, 50 * dpiX, 0);//добавляем колонки
	InsertColumn(1, "Время", LVCFMT_CENTER, 60 * dpiX, 0);//добавляем колонки

														  //vector<string>& headers = database->variable_names;
	int index = 2;
	Ini ini("listview.ini");
	
	for (int i = 0; i<show_parameters.size(); i++)
	{
		MyInsertColumn(index, show_parameters[i], ini);
		index++;

	}
	WriteConfig();
	
}

void MyListCtrl::MyInsertColumn(int index, const string& param, Ini& ini)
{
	//for (char& t : param)
	//	if (t == '_') t = ' ';

	string var_name;
	database->getRealName(param, var_name);

	int width = 5 * 10 * dpiX;
	if (var_name == "ЧД" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2" ||
		var_name == "ДМП" || var_name == "ЧСС" || var_name == "RR" || var_name == "SD" ||
		var_name == "Минутная_вентиляция" || var_name == "Вентиляционный_эквивалент_O2" || var_name == "Вентиляционный_эквивалент_CO2")
		width = 4 * 10 * dpiX;
	if (ini.IsExists())
	{
		//int wt = 0;
		//Write("ColumnWidth", varname.c_str(), GetColumnWidth(col));
		width = ini.Read("ColumnWidth", param.c_str(), width);
	}

	InsertColumn(index, param.c_str(), LVCFMT_CENTER, width, index);//добавляем колонки
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::AddToList(int i)
{
	busy = true;
	int Count = GetItemCount();
	int index = InsertItem(LVIF_TEXT, Count, to_string(i).c_str(), 0, 0, 0, NULL);

	
	
	int start = 0;
	if (experienceStatusTracker && experienceStatusTracker->GetExperienceStatus())
	{
		start = database->getHeader().StartTest;
	}

	string time;
	if (start > i)
	{
		time = string("-") + (database->getDatetime(start).getTime() - database->getDatetime(i).getTime()).getString();
	}
	else
		time = (database->getDatetime(i).getTime() - database->getDatetime(start).getTime()).getString();
	

	SetItemText(index, 1, time.c_str());//time
	int column = 2;
	int col = 0;
	for (auto& varname : show_parameters)
	{
		int size = static_cast<int>(database->getVariable(varname).size());
		if (i < size &&  database->isVariableExists(varname))
		{
			double value = database->getVariable(varname)[i];
			string str = ToString(varname, value);//to_string(database->variables[varname][i]);
			if (str == "НД")
				errmarks.insert({ index, column });
			else if (varname == "ДМП" && (value > 55 || value < 33))
				marks.insert({ index, column });

			SetItemText(index, column, str.c_str());
		}
		column++;

	}

	SetCheck(i, database->getChecked(i));
	busy = false;
}
//-------------------------------------------------------------------------------------------
string MyListCtrl::ToString(string var_name, double value)
{
	if (value < 0) return string("НД");


	database->getRealName(var_name, var_name);

	int precision = 3;
	if (var_name == "Метаболизм_O2" ||
		var_name == "Метаболизм_CO2" ||
		var_name == "ДМП" || var_name == "ЧСС" || var_name == "RR" || var_name == "SD")
		return to_string(static_cast<int>(value));

	else if (var_name == "ЧД" || var_name == "ЧД_old" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2" 
		|| var_name == "Минутная_вентиляция" || var_name == "Вентиляционный_эквивалент_O2" || var_name == "Вентиляционный_эквивалент_CO2"
		|| var_name == "Возраст" || var_name == "Пол" || var_name == "Рост" || var_name == "Вес")
		precision = 1;
	else if (var_name == "Дыхательный_коэффициент")
	{
		if (value < 0.01) return string("НД");
		precision = 2;
	}

	stringstream ss;
	ss << setprecision(precision) << fixed << value;

	return ss.str();
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::WriteConfig() const
{
	Ini ini("listview.ini");
	ini.Delete();

	int col = 2;
	for (auto& varname : show_parameters)
	{
		
		ini.Write("Variable_names", varname.c_str(), "1");
		ini.Write("ColumnWidth", varname.c_str(), GetColumnWidth(col));
		++col;
	}
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::LoadConfig()
{
	show_parameters.clear();

	Ini ini("listview.ini");
	if (!ini.IsExists())
	{
		for (const auto& variable_name : database->getVariableNames())
		{
			if (database->isVariableExists(variable_name))
			{
				if (database->getVariable(variable_name).GetType() == Vector)
				{
				}
			}
			else
				show_parameters.push_back(variable_name);
		}
		return;
	}

	vector<string> var_names = database->getVariableNames();
	for (size_t i = 0; i<var_names.size(); i++)
	{
		int defaultVal = 0;
		const string& varname = var_names[i];

		int temp = (bool)ini.Read<int>("Variable_names", varname.c_str(), defaultVal);
		if (temp == 1)
			show_parameters.push_back(varname);
	}
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::ShowConfigDialog()
{
	MainListConfigDialog cfDlg;
	vector<string> varnames;
	for (const auto& varname : database->getVariableNames())
	{
		//if (database->getVariable(varname).GetType() == Vector)
			varnames.push_back(varname);
	}
	//&database->variable_names

	vector<string> old_parameters(show_parameters);
	cfDlg.Init(&show_parameters, &varnames);
	int result = cfDlg.DoModal();
	if (result == 1)
	{
		Reload();
		//WriteConfig();
		//int old_count = static_cast<int>(old_parameters.size());
		//int new_count = static_cast<int>(show_parameters.size());
		
	}
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::Reload()
{

	//LockWindowUpdate();

	SetHeadersInList();
	int size = database->getCount();
	for (int i = 0; i < size; i++)
		AddToList(i);



	//UnlockWindowUpdate();

}
//-------------------------------------------------------------------------------------------
void MyListCtrl::OnDrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
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

		if (marks.count({ line_index , pLVCD->iSubItem }) != 0)
			crText = RGB(255, 0, 0);
		else if ((database->getHeader().StartTest != 0 && database->getHeader().EndTest != 0 && line_index >= database->getHeader().StartTest && line_index <= database->getHeader().EndTest) ||
			(database->getHeader().StartTest != 0 && database->getHeader().EndTest == 0 && line_index >= database->getHeader().StartTest))
			crText = RGB(15, 135, 0);
		else
			crText = RGB(0, 0, 0);

		if (errmarks.count({ static_cast<int>(pLVCD->nmcd.dwItemSpec) , pLVCD->iSubItem }) != 0)
			pLVCD->clrTextBk = RGB(255, 200, 200);
		/*else if (static_cast<int>(pLVCD->nmcd.dwItemSpec) % 2)
			pLVCD->clrTextBk = RGB(240, 240, 240);*/
		else
			pLVCD->clrTextBk = RGB(255, 255, 255);
			
		pLVCD->clrText = crText;

		*pResult = CDRF_DODEFAULT;
	}
}


BOOL MyListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CBrush  br;
	CRect   rcCli;
	CRect   rcItemsRect(0, 0, 0, 0);
	int     nHeadHeight = 0;
	int     nItems = GetItemCount();

	GetClientRect(&rcCli);

	CHeaderCtrl* pHeadCtrl = GetHeaderCtrl();
	if (pHeadCtrl)
	{
		CRect  rcHead;
		pHeadCtrl->GetWindowRect(&rcHead);
		nHeadHeight = rcHead.Height();
	}
	rcCli.top += nHeadHeight;

	if (nItems > 0)
	{
		CPoint  ptItem;
		CRect   rcItem;

		GetItemRect(nItems - 1, &rcItem, LVIR_BOUNDS);
		GetItemPosition(nItems - 1, &ptItem);

		rcItemsRect.top = rcCli.top;
		rcItemsRect.left = ptItem.x;
		rcItemsRect.right = rcItem.right;
		rcItemsRect.bottom = rcItem.bottom;

		if (GetExtendedStyle() & LVS_EX_CHECKBOXES)
			rcItemsRect.left -= GetSystemMetrics(SM_CXEDGE) + 16;
	}

	br.CreateSolidBrush(GetBkColor());

	if (rcItemsRect.IsRectEmpty())
		pDC->FillRect(rcCli, &br);
	else
	{
		if (rcItemsRect.left > rcCli.left)     // fill left rectangle
			pDC->FillRect(
				CRect(0, rcCli.top, rcItemsRect.left, rcCli.bottom), &br);
		if (rcItemsRect.bottom < rcCli.bottom) // fill bottom rectangle
			pDC->FillRect(
				CRect(0, rcItemsRect.bottom, rcCli.right, rcCli.bottom), &br);
		if (rcItemsRect.right < rcCli.right) // fill right rectangle
			pDC->FillRect(
				CRect(rcItemsRect.right, rcCli.top, rcCli.right, rcCli.bottom), &br);
	}

	return TRUE;
}


void MyListCtrl::OnPaint()
{
	
	CListCtrl::OnPaint();
	
}
