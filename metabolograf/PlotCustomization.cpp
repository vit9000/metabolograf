// PlotCustomization.cpp: файл реализации
//

#include "stdafx.h"
#include "metabolograf.h"
#include "PlotCustomization.h"
#include "afxdialogex.h"


// диалоговое окно PlotCustomization

IMPLEMENT_DYNAMIC(PlotCustomization, CDialogEx)

PlotCustomization::PlotCustomization(CWnd* pParent /*=NULL*/)
	: CDialogEx(ID_PLOT_CUSTOMIZATION, pParent)
{

}

PlotCustomization::~PlotCustomization()
{
}

int PlotCustomization::DoModal(Plot* _plot, bool FullCustomization)
{
	database = Database::getInstance(); 
	plot = _plot; 
	plot_code_reserv = plot->getPlotCode();
	fullCustomization = FullCustomization;
	return CDialogEx::DoModal();
};

void PlotCustomization::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_HEADER_EDIT, m_plotHeader);
	DDX_Control(pDX, IDC_MEAN_EDIT, m_plotMean);
	DDX_Control(pDX, IDC_MULTIVARLIST, m_multivarlist);
	DDX_Control(pDX, IDC_ONEVARLIST, m_onevarlist);
	DDX_Control(pDX, IDC_TYPE1_RADIO, m_type1);
	DDX_Control(pDX, IDC_TYPE2_RADIO, m_type2);

	DDX_Control(pDX, IDC_TEXT_MULTI, m_text_multi);
	DDX_Control(pDX, IDC_TEXT_ONE, m_text_one);
}


BEGIN_MESSAGE_MAP(PlotCustomization, CDialogEx)
	//ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &PlotCustomization::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &PlotCustomization::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_TYPE1_RADIO, &PlotCustomization::OnBnClickedType1Radio)
	ON_BN_CLICKED(IDC_TYPE2_RADIO, &PlotCustomization::OnBnClickedType2Radio)
	ON_BN_CLICKED(IDC_ADDBUTTON, &PlotCustomization::OnAddButtonClick)
	ON_BN_CLICKED(IDC_DELETEBUTTON, &PlotCustomization::OnDeleteButtonClick)
	ON_BN_CLICKED(IDC_EDITLISTBUTTON, &PlotCustomization::OnEditListButtonClick)
	ON_BN_CLICKED(IDC_ONEAXISEDITBUTTON, &PlotCustomization::OnOneAxisEditButtonClick)
END_MESSAGE_MAP()

// обработчики сообщений PlotCustomization
BOOL PlotCustomization::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_plotHeader.SetWindowTextA(plot->getHeader().c_str());

	vector<string> m = { "1 сек", "15 сек", "20 сек", "30 сек", "1 мин", "2 мин", "5 мин", "10 мин", "15 мин" };
	int index = 0;
	for (auto& _m : m)
	{
		m_plotMean.AddString(_m.c_str());
		if (plot->getMeanTime() == _m)
			m_plotMean.SetCurSel(index);
		index++;
	}
	//m_plotMean.SetWindowTextA(m_pPlot->meanTime.c_str());

	type1 = false;
	if (plot->get_var_X().size() == 0)
		type1 = true;
	SetView();

	if (plot->getPlotCode().empty()) return TRUE;
	

	const vector<PlotParameter>* var_Y;

	var_Y = &plot->get_var_Y();
	if (var_Y->size() == 0) return TRUE;

	if (type1)// x - one-time, y - multi
	{
		
	}
	else // x - one, y - multi
	{
		const PlotParameter& varX = plot->get_var_X()[0];

		//m_onevarlist.SetWindowTextA(database->GetPseudoname(varX.getVarname(0)).c_str());

		/*for (const auto& varname : database->getVariableNames())
		{
			if (varX.getVarname(0) == varname)
				m_onevarlist.SetWindowTextA(varname.c_str());
		}*/
		double start = -1;
		if (!varX.getAutoStart())
			start = varX.getStart();

		double end = -1;
		if (!varX.getAutoEnd())
			end = varX.getEnd();

		Parameter p = { varX.getVarname(0), varX.getLegend(0), start, end, 0 };
		axisX.push_back(p);
	}
	//загрузка данных
	int axis_number = 0;
	for (const auto& var : (*var_Y))
	{
		//Parameter p;
		int count = var.count();
		for (int i = 0; i < count; i++)
		{
			double start = -1;
			if (!var.getAutoStart())
				start = var.getStart();

			double end = -1;
			if (!var.getAutoEnd())
				end = var.getEnd();

			multiAxis->push_back({ var.getVarname(i), var.getLegend(i), start, end,  axis_number });
		}
		axis_number++;

	}

	UpdateLists();


	
	return TRUE;
}

string PlotCustomization::BuildCode()
{
	string result = "график (";
	CString temp;
	m_plotHeader.GetWindowTextA(temp);
	if (!temp.IsEmpty())
		result += string("заголовок=\"") + string(temp.GetBuffer()) + string("\",");
	m_plotMean.GetWindowTextA(temp);

	if (!temp.IsEmpty())
		result += string("осреднение=\"") + string(temp.GetBuffer()) + string("\",");
	else 
		result += string("осреднение=\"1 c\",");


	string multivar;
	if (type1)
	{
		multivar = "y=";
	}
	else
	{

		multivar = "y=";
		result +=
			string("x=") +
			axisX[0].varname + string(",") +
			string("легенда=\"") +
			axisX[0].legend + string("\",");
		if (axisX[0].start != -1)
			result += string("начало=") + to_string(axisX[0].start) + ",";
		if (axisX[0].end != -1)
			result += string("конец=") + to_string(axisX[0].end) + ",";

	}

	//---
	int axisCount = GetAxisCount((*multiAxis))+1;
	
	for (int i = 0; i < axisCount; i++)
	{
		string var = multivar;
		string legend = "легенда=";
		double min = -1;
		double max = -1;

		for (auto& par : (*multiAxis))
		{
			if (par.axis_number == i)
			{
				var += par.varname + string("&");
				if (par.legend.empty()) par.legend = par.varname;
				legend += "\"" + par.legend + "\"&";

				if (par.start != -1 && (min == -1 || par.start < min))
					min = par.start;
				if (par.end != -1 && par.end > max)
					max = par.end;
			}	
			
		}
		//if()

		var[var.length() - 1] = ',';
		legend[legend.length() - 1] = ',';

		result.append(var);
		result.append(legend);

		if (min != -1)
			result.append(string("начало=")+to_string(min) + string(","));

		if (max != -1)
			result.append(string("конец=") + to_string(max) + string(","));

		

	}
	result[result.length() - 1] = ')';

	return result;
}


void PlotCustomization::OnBnClickedOk()
{
	string str = BuildCode().c_str();
	//MessageBox(str.c_str(), "", MB_OK);
	
	
	try
	{
		plot->Run(database, str);
		//m_pPlot->Run(m_pDatabase, "график(y=ЧД)");
		plot->DrawPlot();
	}
	catch (exception &ex)
	{
		MessageBox(ex.what(), "Ошибка", MB_OK|MB_ICONERROR);
	}
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}


void PlotCustomization::OnBnClickedCancel()
{
	try
	{
		plot->Run(database, plot_code_reserv);
		plot->DrawPlot();
	}
	catch (exception &ex)
	{
		MessageBox(ex.what(), "Ошибка", MB_OK | MB_ICONERROR);
		
	}
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void PlotCustomization::OnBnClickedType1Radio()
{
	if (type1) return;
	type1 = true;
	SetView();
	

}


void PlotCustomization::OnBnClickedType2Radio()
{
	if (!type1) return;
	type1 = false;
	SetView();
	
}

void PlotCustomization::SetView()
{
	m_type1.SetCheck(type1);
	m_type2.SetCheck(!type1);
	
	//if (type1)
	{
		multiAxis = &axisY;
		m_text_multi.SetWindowTextA("Ось Y");
		m_text_one.ShowWindow(type1 ? SW_HIDE : SW_SHOW);
		m_onevarlist.ShowWindow(type1 ? SW_HIDE : SW_SHOW);
		GetDlgItem(IDC_ONEAXISEDITBUTTON)->ShowWindow(type1 ? SW_HIDE : SW_SHOW);
	}
	/*else
	{
		multiAxis = &axisX;
		m_onevarlist.ShowWindow(SW_SHOW);
		m_text_multi.SetWindowTextA("Ось X");
		m_text_one.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ONEAXISEDITBUTTON)->ShowWindow(SW_SHOW);
	}*/
	if (!fullCustomization)
	{
		m_onevarlist.ShowWindow(SW_HIDE);
		m_text_one.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ONEAXISEDITBUTTON)->ShowWindow(SW_HIDE);
		m_type1.EnableWindow(0);
		m_type2.EnableWindow(0);
		

	}
	m_multivarlist.ResetContent();
	
}

void PlotCustomization::UpdateLists()
{
	m_multivarlist.ResetContent();
	vector<Parameter>* multiAxis = &axisY;
	//загружаем лист переменных
	int scaleN = 1;
	for (auto& var : (*multiAxis))
	{
		string str = "шкала " + to_string(var.axis_number+1) + ": ";
		str += database->GetPseudoname(var.varname);
		m_multivarlist.AddString(str.c_str());
	}
	m_onevarlist.ResetContent();
	if(axisX.size()!=0)
		m_onevarlist.AddString(database->GetPseudoname(axisX[0].varname).c_str());
}

void PlotCustomization::OnAddButtonClick()
{
	AxisDialog dlg;
	dlg.Init(GetAxisCount((*multiAxis)) + 1);

	if (dlg.DoModal() == IDOK)
	{
		multiAxis->push_back(dlg.GetParameter());
		UpdateLists();
	}
}


void PlotCustomization::OnDeleteButtonClick()
{
	int selected = m_multivarlist.GetCurSel();
	if (selected < 0) return;

	//m_multivarlist.DeleteString(selected);
	
		multiAxis->erase(multiAxis->begin() + selected, multiAxis->begin() + selected+1);
	UpdateLists();
}


void PlotCustomization::OnEditListButtonClick()
{
	int selected = m_multivarlist.GetCurSel();
	if (selected < 0) return;

	AxisDialog dlg;
	int CountScale = 0;
	for (const auto& p : (*multiAxis))
		CountScale = p.axis_number + 1;
	dlg.Init(CountScale);


	dlg.SetParameter((*multiAxis)[selected]);

	if (dlg.DoModal() == IDOK)
	{
		(*multiAxis)[selected] = dlg.GetParameter();
		UpdateLists();
	}
}


void PlotCustomization::OnOneAxisEditButtonClick()
{
	int selected = 0;//m_onevarlist.GetCurSel();
	
	AxisDialog dlg;
	dlg.Init(GetAxisCount(axisX) + 1);
	dlg.SetParameter(axisX[selected]);

	if (dlg.DoModal() == IDOK)
	{
		axisX[selected] = dlg.GetParameter();
		UpdateLists();
	}
}


int PlotCustomization::GetAxisCount(vector<Parameter>& axis)
{
	int axisCount = 0;
	for (const auto& p : axis)
		if(axisCount < p.axis_number)
			axisCount = p.axis_number;

	return axisCount;
}