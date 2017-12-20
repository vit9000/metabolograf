// PowerStepDialog.cpp : implementation file
//

#include "stdafx.h"
#include "metabolograf.h"
#include "PowerStepDialog.h"
#include "afxdialogex.h"




// PowerStepDialog dialog

IMPLEMENT_DYNAMIC(PowerStepDialog, CDialogEx)

PowerStepDialog::PowerStepDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POWER_STEP_DIALOG, pParent)
{

}

PowerStepDialog::~PowerStepDialog()
{
}

void PowerStepDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDITBOX, edit);
	DDX_Control(pDX, IDC_EDITBOX, edit);
	DDX_Control(pDX, IDC_SLIDER1, slider);
}

BOOL PowerStepDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//edit.SetWindowTextA("asdasdas");
	slider.SetRange(20, 60);
	slider.SetPos(30);
	edit.SetWindowTextA("30");

	return TRUE;
}


BEGIN_MESSAGE_MAP(PowerStepDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &PowerStepDialog::OnBnClickedOk)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &PowerStepDialog::OnNMReleasedcaptureSlider)
	ON_EN_CHANGE(IDC_EDITBOX, &PowerStepDialog::OnEnChangeEditbox)
END_MESSAGE_MAP()


// PowerStepDialog message handlers


void PowerStepDialog::OnBnClickedOk()
{
	pos = slider.GetPos();
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
	
}


void PowerStepDialog::OnNMReleasedcaptureSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	update();
	*pResult = 0;
}


void PowerStepDialog::OnEnChangeEditbox()
{
	CString str;
	edit.GetWindowTextA(str);
	std::stringstream ss(str.GetBuffer());
	int temp;
	ss >> temp;
	if (!ss)
	{
		//update();
		return;
	}
	if (temp <= 60 && temp >= 20)
		slider.SetPos(temp);
}
