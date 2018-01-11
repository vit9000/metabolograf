#pragma once
#include "Database.h"
#include "afxwin.h"


// ���������� ���� PatientDialog

class PatientDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PatientDialog)

public:
	PatientDialog(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~PatientDialog();
	void Init(bool _active);
	BOOL OnInitDialog();
	bool IsValid(CString str)
	{
		if (str.IsEmpty())
		{
			MessageBox("��� ���� ����������� ��� ����������", "��������", MB_OK | MB_ICONINFORMATION);
			return false;
		}
		return true;
	}
// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATIENT_DIALOG };
#endif
private:
	Database* database;
	bool active;
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_FIO_Edit;
	CEdit m_Age_Edit;
	CEdit m_Weight_Edit;
	CEdit m_Height_Edit;
	CComboBox m_Sex_Combo;
	CEdit m_Wrist_Edit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUnlockButton();
};
