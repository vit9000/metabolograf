// PatientDialog.cpp: файл реализации
//

#include "stdafx.h"
#include "metabolograf.h"
#include "PatientDialog.h"
#include "afxdialogex.h"

using namespace std;
// диалоговое окно PatientDialog

IMPLEMENT_DYNAMIC(PatientDialog, CDialogEx)

PatientDialog::PatientDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATIENT_DIALOG, pParent)
{

}

PatientDialog::~PatientDialog()
{
}

void PatientDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIO_EDIT, m_FIO_Edit);
	DDX_Control(pDX, IDC_AGE_EDIT, m_Age_Edit);
	DDX_Control(pDX, IDC_WEIGHT_EDIT, m_Weight_Edit);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_Height_Edit);
	DDX_Control(pDX, IDC_SEX_COMBO, m_Sex_Combo);
	DDX_Control(pDX, IDC_WRIST_EDIT, m_Wrist_Edit);
}


BEGIN_MESSAGE_MAP(PatientDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &PatientDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UNLOCK_BUTTON, &PatientDialog::OnBnClickedUnlockButton)
END_MESSAGE_MAP()

void PatientDialog::Init(Database* _database, bool _active)
{
	database = _database;
	active = _active;

}

BOOL PatientDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_FIO_Edit.SetWindowTextA(database->hdata.PatientName);
	m_Age_Edit.SetWindowTextA(to_string((int)database->hdata.PatientAge).c_str());
	m_Weight_Edit.SetWindowTextA(to_string((int)database->hdata.PatientWeight).c_str());
	m_Height_Edit.SetWindowTextA(to_string((int)database->hdata.PatientHeight).c_str());
	m_Wrist_Edit.SetWindowTextA(to_string((int)database->hdata.PatientWrist).c_str());

	m_Sex_Combo.AddString("Женский");
	m_Sex_Combo.AddString("Мужской");
	m_Sex_Combo.SetCurSel(database->hdata.PatientSex);

	m_FIO_Edit.EnableWindow(active);
	m_Age_Edit.EnableWindow(active);
	m_Weight_Edit.EnableWindow(active);
	m_Height_Edit.EnableWindow(active);
	m_Sex_Combo.EnableWindow(active);


	return TRUE;
}

// обработчики сообщений PatientDialog


void PatientDialog::OnBnClickedOk()
{
	CString temp;
	
	m_Age_Edit.GetWindowText(temp);
	if (!IsValid(temp)) return;
	double Age = (double)stoi(temp.GetBuffer());

	m_Weight_Edit.GetWindowText(temp);
	if (!IsValid(temp)) return;
	int Weight = stoi(temp.GetBuffer());

	m_Height_Edit.GetWindowText(temp);
	if (!IsValid(temp)) return;
	int Height = stoi(temp.GetBuffer());

	m_Wrist_Edit.GetWindowText(temp);
	if (!IsValid(temp)) return;
	double Wrist = (double)stoi(temp.GetBuffer());

	int Sex = m_Sex_Combo.GetCurSel();

	m_FIO_Edit.GetWindowText(temp);
	if (!IsValid(temp)) return;

	sprintf(database->hdata.PatientName, "%s", temp.GetBuffer());
	database->hdata.PatientAge = Age;
	database->hdata.PatientWeight = Weight;
	database->hdata.PatientHeight = Height;
	database->hdata.PatientWrist = Wrist;
	database->hdata.PatientSex = Sex;

	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}


void PatientDialog::OnBnClickedUnlockButton()
{
	bool active = true;
	m_FIO_Edit.EnableWindow(active);
	m_Age_Edit.EnableWindow(active);
	m_Weight_Edit.EnableWindow(active);
	m_Height_Edit.EnableWindow(active);
	m_Sex_Combo.EnableWindow(active);

}
