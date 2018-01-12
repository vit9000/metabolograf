#pragma once
#include "afxwin.h"
#include "Plot.h"
#include "Database.h"
#include "AxisDialog.h"



// ���������� ���� PlotCustomization

class PlotCustomization : private CDialogEx
{
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DIALOG2 };
	#endif

	DECLARE_DYNAMIC(PlotCustomization)

public:
	PlotCustomization(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~PlotCustomization();
	int GetAxisCount(vector<Parameter>& axis);
	int DoModal(Plot* _plot, bool FullCustomization = true);


private:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	
	bool fullCustomization;
	vector<Parameter> axisX;
	vector<Parameter> axisY;
	vector<Parameter>* multiAxis;
	string plot_code_reserv;
	CEdit m_plotHeader;
	CComboBox m_plotMean;
	CListBox m_multivarlist;
	CListBox m_onevarlist;
	CStatic m_text_multi;
	CStatic m_text_one;
	CButton m_type1;
	CButton m_type2;
	bool type1;
	Plot* plot;
	Database* database;

	string BuildCode();
	void UpdateLists();
	void SetView();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedType1Radio();
	afx_msg void OnBnClickedType2Radio();
	afx_msg void OnAddButtonClick();
	afx_msg void OnDeleteButtonClick();
	afx_msg void OnEditListButtonClick();
	afx_msg void OnOneAxisEditButtonClick();
	afx_msg BOOL OnInitDialog();
};
