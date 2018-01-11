#include "stdafx.h"
#include "TimerWindow.h"


TimerWindow::TimerWindow()
{
	Default();
}


TimerWindow::~TimerWindow()
{
}


BEGIN_MESSAGE_MAP(TimerWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void TimerWindow::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	RedrawWindow();
}

void TimerWindow::SetBounds()
{
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();
	
	FontSize = 14;

}


void TimerWindow::OnPaint()
{
	SetBounds();

	UGC ugc(GetDC(), Width, Height);
	int dpix = ugc.getDPIX();
	

	ugc.g->Clear(Color(225, 225, 225));
	
	ugc.SetDrawColor(50, 50, 50);

	
	int x = Width / 2;
	int y = 0;
	ugc.SetAlign(ugc.CENTER);

	ugc.SetTextSize((FontSize - 1)*dpix);
	ugc.DrawString("Время исследования", x, y);
	y += ugc.TextSize*1.5;
	ugc.SetTextSize((FontSize+2)*dpix);
	ugc.DrawString(rec, x, y);
	y += ugc.TextSize*1.7;

	
	if (!test.empty())
	{
		ugc.SetTextSize((FontSize - 1)*dpix);
		ugc.DrawString("Время теста", x, y);
		y += ugc.TextSize*1.5;
		ugc.SetTextSize((FontSize + 2)*dpix);
		ugc.DrawString(test, x, y);
	}


	ugc.SetAlign(ugc.LEFT);
	CWnd::OnPaint();
}


void TimerWindow::StartRec()
{
	timer = SetTimer((int)3, 1000, NULL);
	start_rec.CurrentTime();
	

}
void TimerWindow::StartTest()
{	
	start_test.CurrentTime();
}

void TimerWindow::StopRec()
{
	KillTimer(timer);
}

void TimerWindow::StopTest()
{
	start_test.Zero();
}

void TimerWindow::Update()
{
	Default();
	Database * database = Database::getInstance();
	if (database->getCount() == 0) return;

	start_rec = database->getDatetime(0).getTime();
	rec = (database->getDatetime(database->getCount()-1).getTime() - start_rec).getStringAlt();
	if (database->getHeader().StartTest > 0 && database->getHeader().EndTest > 0)
	{
		start_test = database->getDatetime(database->getHeader().StartTest).getTime();
		test = (database->getDatetime(database->getHeader().EndTest).getTime() - start_test).getStringAlt();
	}
	RedrawWindow();
}


void TimerWindow::OnTimer(UINT_PTR nIdEvent)
{
	if (nIdEvent != 3)
	{
		CWnd::OnTimer(nIdEvent);
		return;
	}
	MTime curtime;
	curtime.CurrentTime();
	rec = (curtime - start_rec).getStringAlt();
	if(start_test!=MTime(0))
		test = (curtime - start_test).getStringAlt();

	RedrawWindow();

	//.....

	CWnd::OnTimer(nIdEvent);
}