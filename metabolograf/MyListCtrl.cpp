#include "stdafx.h"
#include "MyListCtrl.h"


BEGIN_MESSAGE_MAP(MyListCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


MyListCtrl::MyListCtrl()
	: m_iWidth(0)
	, m_iHeight(0)
{
	busy = false;
	experienceStatusTracker = nullptr;
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::Init(IExperienceStatusTracker* ExperienceStatusTracker)
{
	if (experienceStatusTracker)
		return;
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
	for (size_t i = 0; i < show_parameters.size(); i++)
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

	InsertColumn(0, "N", LVCFMT_CENTER, DPIX()(50), 0);//��������� �������
	InsertColumn(1, "�����", LVCFMT_CENTER, DPIX()(60), 0);//��������� �������

														  //vector<string>& headers = m_pDatabase->variable_names;
	int index = 2;
	Ini ini("listview.ini");
	
	for (size_t i = 0; i<show_parameters.size(); i++)
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

	int width = m_iWidth - DPIX()(110);
	width /= (int)show_parameters.size();
	//int width = DPIX()(5 * 10);
	/*if (var_name == "��" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2" ||
		var_name == "���" || var_name == "���" || var_name == "RR" || var_name == "SD" ||
		var_name == "��������_����������" || var_name == "��������������_����������_O2" || var_name == "��������������_����������_CO2")
		width = DPIX()(4 * 10);*/
	if (ini.IsExists())
	{
		//int wt = 0;
		//Write("ColumnWidth", varname.c_str(), GetColumnWidth(col));
		width = ini.Read("ColumnWidth", param.c_str(), width);
	}

	InsertColumn(index, param.c_str(), LVCFMT_CENTER, width, index);//��������� �������
}
//-------------------------------------------------------------------------------------------
string MyListCtrl::getTime(int i)
{
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
	return time;
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::OverwriteTime()
{
	thread t([this]() {
		for (size_t i = 0; i < database->getCount(); ++i)
		{
			SetItemText((int)i, 1, getTime(i).c_str());//time
		}});
	t.detach();
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::AddToList(int i)
{
	busy = true;
	int Count = GetItemCount();
	int index = InsertItem(LVIF_TEXT, Count, to_string(i).c_str(), 0, 0, 0, NULL);

	

	SetItemText(index, 1, getTime(i).c_str());//time
	int column = 2;
	int col = 0;
	for (auto& varname : show_parameters)
	{
		AddParameter(varname, column, index);
		column++;

	}

	SetCheck(i, database->getChecked(i));
	busy = false;
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::AddParameter(const string& param, int column, int index)
{
	int size = static_cast<int>(database->getVariable(param).size());
	if (index < size &&  database->isVariableExists(param))
	{
		double value = database->getVariable(param)[index];
		string str = ToString(param, value);//to_string(m_pDatabase->variables[varname][i]);
		if (str == "��")
			errmarks.insert({ index, column });
		else if (param == "���" && (value > 55 || value < 33))
			marks.insert({ index, column });

		SetItemText(index, column, str.c_str());
	}
}
//-------------------------------------------------------------------------------------------
string MyListCtrl::ToString(string var_name, double value)
{
	if (value < 0) return string("��");


	database->getRealName(var_name, var_name);

	int precision = 3;
	if (var_name == "����������_O2" ||
		var_name == "����������_CO2" ||
		var_name == "���" || var_name == "���" || var_name == "RR" || var_name == "SD")
		return to_string(static_cast<int>(value));

	else if (var_name == "��" || var_name == "��_old" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2" 
		|| var_name == "��������_����������" || var_name == "��������������_����������_O2" || var_name == "��������������_����������_CO2"
		|| var_name == "�������" || var_name == "���" || var_name == "����" || var_name == "���")
		precision = 1;
	else if (var_name == "�����������_�����������")
	{
		if (value < 0.01) return string("��");
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
	//ini.Delete();

	int col = 2;
	int i = 0;
	for (auto& varname : database->getVariableNames())
	{
		if (i<static_cast<int>(show_parameters.size()) && varname == show_parameters[i])
		{
			ini.Write("Variable_names", varname.c_str(), "1");
			int w = GetColumnWidth(col);
			ini.Write("ColumnWidth", varname.c_str(), w);
			++i;
			++col;
		}
		else
			ini.Write("Variable_names", varname.c_str(), "0");
		
		
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
	WriteConfig();

	MainListConfigDialog cfDlg;
	vector<string> varnames;
	for (const auto& varname : database->getVariableNames())
	{
		//if (m_pDatabase->getVariable(varname).GetType() == Vector)
			varnames.push_back(varname);
	}
	
	vector<string> old_parameters(show_parameters);
	cfDlg.Init(&show_parameters, &varnames);
	int result = cfDlg.DoModal();
	if (result == 1)
	{
		//Reload();
		
		if (old_parameters != show_parameters)
		{
			vector<string> sorted_new_parameters(show_parameters);
			vector<string> sorted_old_parameters(old_parameters);
			std::sort(sorted_new_parameters.begin(), sorted_new_parameters.end());
			std::sort(sorted_old_parameters.begin(), sorted_old_parameters.end());

			DeleteUnselectedColumns(sorted_new_parameters, sorted_old_parameters, old_parameters);
			AddSelectedColumns(sorted_new_parameters, sorted_old_parameters);
			
		}
		
	}
}
void MyListCtrl::DeleteUnselectedColumns(const vector<string>& sorted_new_parameters, const vector<string>& sorted_old_parameters, vector<string>& old_parameters)
{
	vector<string> to_delete;
	std::set_difference(
		sorted_old_parameters.begin(), sorted_old_parameters.end(),
		sorted_new_parameters.begin(), sorted_new_parameters.end(),
		std::inserter(to_delete, to_delete.begin()));
	for (const string& d : to_delete)
	{
		auto it = find(old_parameters.begin(), old_parameters.end(), d);
		if (it == old_parameters.end()) continue;

		int column = it - old_parameters.begin();
		old_parameters.erase(it);
		DeleteColumn(column + 2);
	}
}

void  MyListCtrl::AddSelectedColumns(const vector<string>& sorted_new_parameters, const vector<string>& sorted_old_parameters)
{
	vector<string> to_add;
	std::set_difference(
		sorted_new_parameters.begin(), sorted_new_parameters.end(),
		sorted_old_parameters.begin(), sorted_old_parameters.end(),
		std::inserter(to_add, to_add.begin()));

	Ini ini("listview.ini");
	for (const string& add : to_add)
	{
		auto it = std::find(show_parameters.begin(), show_parameters.end(), add);
		if (it == show_parameters.end()) continue;
		int column = it - show_parameters.begin() + 2;
		MyInsertColumn(column, add, ini);
		for (size_t i = 0; i < database->getCount(); ++i)
			AddParameter(add, column, (int)i);
	}
}
//-------------------------------------------------------------------------------------------
void MyListCtrl::Reload()
{
	SetHeadersInList();

	std::thread t([this]()
	{
		int size = database->getCount();
		SetItemCount(size);
		SetRedraw(FALSE);
		int count_on_page = GetCountPerPage()*2;
		for (int i = 0; i < size; i++)
		{
			if (i == count_on_page || i%20==0)
				SetRedraw(TRUE);
			AddToList(i);
			if(i%20==0) 
				SetRedraw(FALSE);
		}
		SetRedraw(TRUE);
	});
	t.detach();

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
	{
		pDC->FillRect(rcCli, &br);
		
	}
	else
	{
		if (rcItemsRect.left > rcCli.left)     // fill left rectangle
			pDC->FillRect(CRect(0, rcCli.top, rcItemsRect.left, rcCli.bottom), &br);
		if (rcItemsRect.bottom < rcCli.bottom) // fill bottom rectangle
			pDC->FillRect(CRect(0, rcItemsRect.bottom, rcCli.right, rcCli.bottom), &br);
		if (rcItemsRect.right < rcCli.right) // fill right rectangle
			pDC->FillRect(CRect(rcItemsRect.right, rcCli.top, rcCli.right, rcCli.bottom), &br);

		pDC->FillRect(CRect(0, rcCli.top, 3, rcCli.bottom), &br);

		
	}

	return TRUE;
}


void MyListCtrl::OnPaint()
{
	if(!busy)
		CListCtrl::OnPaint();
	
}

void MyListCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd())
	{
		int oldWidth = m_iWidth;
		m_iWidth = cx;
		m_iHeight = cy;

		if (oldWidth != 0)
		{
			busy = true;
			int countColumnes = 2 + (int)show_parameters.size();
			for (int i = 2; i < countColumnes; i++)
			{
				double colWidth = GetColumnWidth(i);
				colWidth = colWidth / oldWidth * m_iWidth;
				SetColumnWidth(i, (int)colWidth);
			}
			busy = false;
		}
	}
	CWnd::OnSize(nType, cx, cy);
}

