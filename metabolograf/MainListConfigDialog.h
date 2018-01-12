#pragma once

#include <vector>
#include <string>
#include "DPIX.h"
using namespace std;
// ���������� ���� MainListConfigDialog

class MainListConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainListConfigDialog)
// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINLIST_CONFIG_DIALOG };
#endif

public:
	MainListConfigDialog(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~MainListConfigDialog();
	
	void Init(vector<string>* _checked, vector<string>* _names)
	{
		checked = _checked;
		names = _names;
	}
	


private:
	CCheckListBox checklist;
	vector<string>* checked;
	vector<string>* names;

	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedListdlg();
	
};
