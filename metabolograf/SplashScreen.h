#pragma once
#include "afxwin.h"
#include "ugc.h"
#include "DPIX.h"

// ���������� ���� SplashScreen

class SplashScreen : public CDialogEx
{
	DECLARE_DYNAMIC(SplashScreen)

public:
	SplashScreen(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~SplashScreen();
	
// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPLASH };
#endif

protected:
	double dpiX;
	Gdiplus::Bitmap *bmp;
	Gdiplus::Bitmap *ResourceToBitmap(const HINSTANCE hInstance, const int id);
	afx_msg void OnPaint();
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	
};
