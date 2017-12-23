#include "stdafx.h"
#include "Histogram.h"


Histogram::Histogram()
{
	
}


Histogram::~Histogram()
{
}



BEGIN_MESSAGE_MAP(Histogram, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void Histogram::Init(Database* _database)
{

	database = _database;
	SetBounds();
}

void Histogram::SetBounds()
{

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();

	
}


void Histogram::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

}
void Histogram::OnMouseMove(UINT nFlags, CPoint point)
{

}

void Histogram::OnPaint()
{
	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 0, 0);
	ugc.FillRectangle(0, 0, Width, Height);

}

void Histogram::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	

}