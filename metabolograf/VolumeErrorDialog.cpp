// VolumeErrorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "metabolograf.h"
#include "VolumeErrorDialog.h"
#include "afxdialogex.h"


// VolumeErrorDialog dialog

IMPLEMENT_DYNAMIC(VolumeErrorDialog, CDialogEx)

VolumeErrorDialog::VolumeErrorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VOLUME_ERROR_DIALOG, pParent)
{

}

VolumeErrorDialog::~VolumeErrorDialog()
{
}

void VolumeErrorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, combo);
}


BEGIN_MESSAGE_MAP(VolumeErrorDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO, &VolumeErrorDialog::OnCbnSelchangeCombo)
END_MESSAGE_MAP()

void VolumeErrorDialog::Init()
{
	hist.Init();
}
BOOL VolumeErrorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CRect rect;
	GetClientRect(&rect);
	DPIX dpix;
	rect.bottom -= 30 * (double)dpix;
	hist.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_BIG_PLOT);
	hist.SetBounds();

	
	for (int i = 5; i <= 25; ++i)
	{
		std::stringstream ss;
		ss << i;
		combo.AddString(ss.str().c_str());
		
	}
	combo.SetCurSel(15);
	

	return TRUE;
}

void VolumeErrorDialog::OnCbnSelchangeCombo()
{
	int res = combo.GetCurSel() + 5;
	hist.BuildTable(res);
	hist.RedrawWindow();
}
