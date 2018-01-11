#pragma once
#include "afxwin.h"
#include "Plot.h"
#include "Database.h"
#include "AxisDialog.h"



// диалоговое окно PlotCustomization

class PlotCustomization : private CDialogEx
{
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DIALOG2 };
	#endif

	DECLARE_DYNAMIC(PlotCustomization)

public:
	PlotCustomization(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~PlotCustomization();
	int GetAxisCount(vector<Parameter>& axis);
	int DoModal(Plot* _plot, bool FullCustomization = true);


protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	
	bool fullCustomization;
	vector<Parameter> axisX;
	vector<Parameter> axisY;
	vector<Parameter>* multiAxis;

	string plot_code_reserv;
	
	CEdit m_plotHeader;
	CComboBox m_plotMean;
	//CEdit m_plotMean;

	CListBox m_multivarlist;
	CListBox m_onevarlist;
	CStatic m_text_multi;
	CStatic m_text_one;
	bool type1;
	CButton m_type1;
	CButton m_type2;

	Plot* plot;
	Database* database;
	void SetView();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedType1Radio();
	afx_msg void OnBnClickedType2Radio();

	string BuildCode();

	void UpdateLists();
	afx_msg void OnAddButtonClick();
	afx_msg void OnDeleteButtonClick();
	afx_msg void OnEditListButtonClick();
	afx_msg void OnOneAxisEditButtonClick();
	afx_msg BOOL OnInitDialog();
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
