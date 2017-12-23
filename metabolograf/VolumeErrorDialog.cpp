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
}


BEGIN_MESSAGE_MAP(VolumeErrorDialog, CDialogEx)
END_MESSAGE_MAP()

void VolumeErrorDialog::Init(Database* database)
{
	hist.Init(database);
}
BOOL VolumeErrorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CRect rect;
	GetClientRect(&rect);
	DPIX dpix;
	rect.bottom -= 30 * dpix;
	hist.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_BIG_PLOT);
	hist.SetBounds();
	return TRUE;
}