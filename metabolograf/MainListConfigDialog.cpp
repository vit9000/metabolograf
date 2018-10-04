// MainListConfigDialog.cpp: файл реализации
//

#include "stdafx.h"
#include "metabolograf.h"
#include "MainListConfigDialog.h"
#include "afxdialogex.h"


// диалоговое окно MainListConfigDialog

IMPLEMENT_DYNAMIC(MainListConfigDialog, CDialogEx)

MainListConfigDialog::MainListConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINLIST_CONFIG_DIALOG, pParent)
{

}

MainListConfigDialog::~MainListConfigDialog()
{
}

void MainListConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL MainListConfigDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	checklist.Create(LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_BORDER, CRect(5, 5, 254, 290), this, IDC_HEADERS_CHECKLIST);
	//checklist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| WS_BORDER);
	CFont* pFont = new CFont;
	VERIFY(pFont->CreateFont(
		static_cast<int>(14 * (double)DPIX()),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,//ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH,  // nPitchAndFamily
		"Arial"));                 // lpszFacename
		
	checklist.SetFont(pFont);
	checklist.ResetContent();

	if (checked && names)
	{
		size_t j = 0;
		size_t size = (*names).size();
		for (size_t i = 0; i<size; i++)
		{
			checklist.AddString((*names)[i].c_str());
			if ((j < checked->size()) && (*names)[i] == (*checked)[j])
			{
				checklist.SetCheck(i, true);
				j++;	
			}		
		}
	}
	

	

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;
	double dpix = (double) DPIX();
	::SetWindowPos(checklist.m_hWnd,HWND_TOP,
		static_cast<int>(10* dpix),
		static_cast<int>(10* dpix),
		static_cast<int>(rect.right-35*dpix),
		static_cast<int>(rect.bottom - 80* dpix), NULL);
	

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}


BEGIN_MESSAGE_MAP(MainListConfigDialog, CDialogEx)
	ON_BN_CLICKED(IDOK_LISTDLG, &MainListConfigDialog::OnBnClickedListdlg)
	ON_WM_SIZE()
	
END_MESSAGE_MAP()


// обработчики сообщений MainListConfigDialog
void MainListConfigDialog::OnSize(UINT nType, int cx, int cy)
{
	

}

void MainListConfigDialog::OnBnClickedListdlg()
{
	checked->clear();
	char* text = new char[256];
	for (int i = 0; i < checklist.GetCount(); i++)
	{
		if (checklist.GetCheck(i))
		{	
			checklist.GetText(i, text);
			checked->push_back(text);
		}
	}
	delete[] text;
	
	CDialogEx::OnOK();
}

