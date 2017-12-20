#pragma once
#include "afxwin.h"
#include "Database.h"


struct Parameter
{
	string varname;
	string legend;
	double start;
	double end;
	int axis_number;


};


// диалоговое окно AxisDialog

class AxisDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AxisDialog)

public:
	AxisDialog(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~AxisDialog();
	void SetParameter(const Parameter& p)
	{
		param = p;
	}
	Parameter& GetParameter() 
	{
		return param;
	}
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AXIS_DIALOG };
#endif

protected:
	
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

	Parameter param;
public:
	CComboBox m_VarCombo;
	CEdit m_LegendEdit;
	CEdit m_MinEdit;
	CEdit m_MaxEdit;
	CComboBox m_ScaleCombo;

	int CountScale;
	Database* database;
	
	

	afx_msg void OnBnClickedOk();

	double ToDouble(CString str);
	afx_msg void OnEnChangeLegendedit();
};
