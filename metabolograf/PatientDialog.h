#pragma once
#include "Database.h"
#include "afxwin.h"


// диалоговое окно PatientDialog

class PatientDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PatientDialog)
	// Данные диалогового окна
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_PATIENT_DIALOG };
	#endif
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

private:
	Database* database;
	bool active;
	CEdit m_FIO_Edit;
	CEdit m_Age_Edit;
	CEdit m_Weight_Edit;
	CEdit m_Height_Edit;
	CComboBox m_Sex_Combo;
	CEdit m_Wrist_Edit;

	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUnlockButton();
	DECLARE_MESSAGE_MAP()

};
