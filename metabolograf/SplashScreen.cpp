// SplashScreen.cpp: файл реализации
//

#include "stdafx.h"
#include "metabolograf.h"
#include "SplashScreen.h"
#include "afxdialogex.h"


// диалоговое окно SplashScreen

IMPLEMENT_DYNAMIC(SplashScreen, CDialogEx)

SplashScreen::SplashScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SPLASH, pParent)
{
	dpiX = (double)DPIX();
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(SplashScreen, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

Gdiplus::Bitmap* SplashScreen::ResourceToBitmap(const HINSTANCE hInstance, const int id)
{
	HBITMAP hBmp = ::LoadBitmap(hInstance, MAKEINTRESOURCE(id));
	//if (bBmp == NULL) return NULL;
	return Gdiplus::Bitmap::FromHBITMAP(hBmp, NULL);
}
// обработчики сообщений SplashScreen
BOOL SplashScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	bmp = ResourceToBitmap(AfxGetInstanceHandle(), IDB_LOGO);
	return TRUE;
}

void SplashScreen::OnPaint()
{
	CDialogEx::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	int Width = rect.Width();
	int Height = rect.Height();
	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.FillRectangle(0, 0, Width, Height);
	
	int w = static_cast<int>(bmp->GetWidth()*dpiX/1.5);
	int h = static_cast<int>(bmp->GetHeight()*dpiX/1.5);
	
	int x = Width / 2 - w/2;

	ugc.DrawBitmap(bmp, x, Height / 2 - h / 2, w, h);

	//ugc.DrawString(str, x+w, Height / 2 - ugc.GetTextHeight()/2);

}