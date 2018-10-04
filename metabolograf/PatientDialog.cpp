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
	DDX_Control(pDX, IDC_MASKCOMBO, m_MaskCombo);
}


BEGIN_MESSAGE_MAP(PatientDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &PatientDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UNLOCK_BUTTON, &PatientDialog::OnBnClickedUnlockButton)
END_MESSAGE_MAP()

void PatientDialog::Init(bool _active)
{
	database = Database::getInstance();
	active = _active;

}

BOOL PatientDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_FIO_Edit.SetWindowTextA(database->getHeader().PatientName);
	m_Age_Edit.SetWindowTextA(to_string((int)database->getHeader().PatientAge).c_str());
	m_Weight_Edit.SetWindowTextA(to_string((int)database->getHeader().PatientWeight).c_str());
	m_Height_Edit.SetWindowTextA(to_string((int)database->getHeader().PatientHeight).c_str());
	m_Wrist_Edit.SetWindowTextA(to_string((int)database->getHeader().PatientWrist).c_str());

	m_Sex_Combo.AddString("Женский");
	m_Sex_Combo.AddString("Мужской");
	m_Sex_Combo.SetCurSel(database->getHeader().PatientSex);

	m_FIO_Edit.EnableWindow(active);
	m_Age_Edit.EnableWindow(active);
	m_Weight_Edit.EnableWindow(active);
	m_Height_Edit.EnableWindow(active);
	m_Sex_Combo.EnableWindow(active);
	m_MaskCombo.EnableWindow(active);

	Masks masks;
	for (size_t i = 0; i < masks.count(); ++i)
	{
		try 
		{
			m_MaskCombo.AddString(masks.getMask(i).name.c_str());
		}
		catch (...)
		{
			break;
		}
	}
	auto& hdata = database->getHeader();
	int index = static_cast<int>(database->getHeader().MaskNumber);
	m_MaskCombo.SetCurSel(index);

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
	
	//убираем пробелы в конце имени пациента
	int i;
	for (i = temp.GetLength() - 1; i >= 0; --i)
	{
		if (temp[i] != ' ')
			break;
	}
	sprintf_s(database->getHeader().PatientName, "%s", temp.GetBufferSetLength(i + 1));
	database->getHeader().PatientAge = Age;
	database->getHeader().PatientWeight = Weight;
	database->getHeader().PatientHeight = Height;
	database->getHeader().PatientWrist = Wrist;
	database->getHeader().PatientSex = Sex;
	database->getHeader().MaskNumber = m_MaskCombo.GetCurSel();
	
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
	m_MaskCombo.EnableWindow(active);

}
