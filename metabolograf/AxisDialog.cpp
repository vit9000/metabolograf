// AxisDialog.cpp: файл реализации
//

#include "stdafx.h"
#include "metabolograf.h"
#include "AxisDialog.h"
#include "afxdialogex.h"


// диалоговое окно AxisDialog

IMPLEMENT_DYNAMIC(AxisDialog, CDialogEx)

AxisDialog::AxisDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AXIS_DIALOG, pParent)
{
	database = nullptr;
	CountScale = 0;

	
}

AxisDialog::~AxisDialog()
{
}

void AxisDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VARCOMBO, m_VarCombo);
	DDX_Control(pDX, IDC_LEGENDEDIT, m_LegendEdit);
	DDX_Control(pDX, IDC_MINEDIT, m_MinEdit);
	DDX_Control(pDX, IDC_MAXEDIT, m_MaxEdit);
	DDX_Control(pDX, IDC_SCALECOMBO, m_ScaleCombo);
}


BEGIN_MESSAGE_MAP(AxisDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &AxisDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_LEGENDEDIT, &AxisDialog::OnEnChangeLegendedit)
END_MESSAGE_MAP()


// обработчики сообщений AxisDialog

// обработчики сообщений PlotCustomization
BOOL AxisDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (!database) return FALSE;

	int index = -1;
	int counter = 0;
	for (const auto& varname : database->getVariableNames())
	{
		if (database->getVariable(varname).GetType() == Vector)
		{
			
			m_VarCombo.AddString(varname.c_str());
			if (database->GetPseudoname(param.varname) == varname)
				index = counter;
			counter++;
		}
	}
	

	for (int i = 1; i <= CountScale + 1; i++)
		m_ScaleCombo.AddString(to_string(i).c_str());


	if (param.varname.length() != 0)//редактирование
	{
		SetWindowTextA("Редактирование параметра");
		if(index>0)
			m_VarCombo.SetCurSel(index);
		/*for (int i=0; i< database->variable_names.size(); i++)
			if (param.varname == database->variable_names[i])
			{
				m_VarCombo.SetCurSel(i);
				break;
			}
		*/
		


		m_LegendEdit.SetWindowTextA(param.legend.c_str());

		if (param.start != -1)
			m_MinEdit.SetWindowTextA(to_string(param.start).c_str());

		if (param.end != -1)
			m_MaxEdit.SetWindowTextA(to_string(param.end).c_str());

		m_ScaleCombo.SetCurSel(param.axis_number);

	}
	else
	{
		SetWindowTextA("Добавление параметра");
		m_ScaleCombo.SetCurSel(m_ScaleCombo.GetCount() - 1);

	}

	


	return TRUE;
}

void AxisDialog::OnBnClickedOk()
{
	if (m_VarCombo.GetCurSel()<0)
	{
		MessageBox("Необходимо выбрать переменную", "Внимание", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CString varname;
	m_VarCombo.GetLBText(m_VarCombo.GetCurSel(), varname);
	

	CString legend;
	m_LegendEdit.GetWindowTextA(legend);

	
	CString temp;
	double min = -1;
	double max = -1;
	try
	{
		m_MinEdit.GetWindowTextA(temp);
		min = ToDouble(temp);
		m_MaxEdit.GetWindowTextA(temp);
		max = ToDouble(temp);
	}
	catch (exception& ex)
	{
		MessageBox(ex.what(), "Ошибка", MB_OK | MB_ICONERROR);
		return;
	}
	
	m_ScaleCombo.GetLBText(m_ScaleCombo.GetCurSel(), temp);
	int ScaleN = atoi(temp.GetBuffer())-1;

	string real_var_name = database->GetRealname(varname.GetBuffer());
	string legend_ = varname.GetBuffer();
	if (!legend.IsEmpty())
		legend_ = legend.GetBuffer();
	param = { real_var_name, legend_, min, max , ScaleN };

	CDialogEx::OnOK();
}

#include <stdio.h>
#include <regex>
double AxisDialog::ToDouble(CString str)
{
	if (str.IsEmpty()) return -1;
	string temp = str.GetBuffer();
	//
	regex txt_regex("[0-9]*[\.]?[0-9]*");
	if (!regex_match(temp, txt_regex))
	{
		throw runtime_error("Проверьте правильность введения минимального и максимального значения.");
	}
	double d;
	sscanf(str.GetBuffer(), "%lf", &d);
	return d;
}


void AxisDialog::OnEnChangeLegendedit()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}
