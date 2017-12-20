#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Database.h"
#include "Export.h"

// NutritionDialog dialog

class NutritionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NutritionDialog)

public:
	NutritionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NutritionDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOM_NUTRITION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	void Select(int index);
	
public:
	CTabCtrl tab;
	Database *database;
	CComboBox comboBox;
	afx_msg void OnCbnSelchangeCombo1();
	CEdit edit1;
	CEdit edit2;
	CEdit edit3;
	CStatic label1;
	CStatic label2;
	CStatic label3;
	afx_msg void OnBnClickedOk();
	
	
	afx_msg void Calculate(bool save);
	afx_msg void OnEnChangeEdit();
	CStatic preview1;
	CStatic preview2;
	afx_msg void OnBnClickedInfo();
};
