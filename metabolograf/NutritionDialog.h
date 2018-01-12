#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Database.h"
#include "Export.h"

// NutritionDialog dialog

class NutritionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NutritionDialog)
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CUSTOM_NUTRITION_DIALOG };
	#endif

public:
	NutritionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NutritionDialog();
private:
	CTabCtrl tab;
	CComboBox comboBox;
	CEdit edit1;
	CEdit edit2;
	CEdit edit3;
	CStatic label1;
	CStatic label2;
	CStatic label3;
	CStatic preview1;
	CStatic preview2;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void Select(int index);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void Calculate(bool save);
	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedInfo();

	DECLARE_MESSAGE_MAP()
};
