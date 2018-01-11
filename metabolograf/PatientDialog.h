#pragma once
#include "Database.h"
#include "afxwin.h"


// диалоговое окно PatientDialog

class PatientDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PatientDialog)

public:
	PatientDialog(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~PatientDialog();
	void Init(bool _active);
	BOOL OnInitDialog();
	bool IsValid(CString str)
	{
		if (str.IsEmpty())
		{
			MessageBox("Все поля обязательны для заполнения", "Внимание", MB_OK | MB_ICONINFORMATION);
			return false;
		}
		return true;
	}
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATIENT_DIALOG };
#endif
private:
	Database* database;
	bool active;
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

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
