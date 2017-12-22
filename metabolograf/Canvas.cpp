#include "stdafx.h"
#include "Canvas.h"
#include "string_extentions.h"
using namespace Gdiplus;

BEGIN_MESSAGE_MAP(CCanvas, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CCanvas::OnLButtonDblClk(CWnd *dlg, int x, int y)
{
	if (plots.size() == 0) return;
	CRect rc;
	string str = ToString(x) + " " + ToString(y);
	GetClientRect(&rc);
	if (x >= 0 && x <= rc.Width() && y >= 0 && y <= rc.Height())
	{
		
		EnlargePlot();
		
	}
		
}
void CCanvas::EnlargePlot()
{
	if (plots.count(active_plot) == 0) return;
	//PlotDialog plotdialog;
	//plotdialog.setPlot(plots[active_plot]);
	//plotdialog.DoModal();
	

	
}
int CCanvas::GetScreenDPI()
{
	HDC hdcScreen = ::GetDC(NULL);
	int iDPI = -1; // assume failure
	if (hdcScreen) {
		iDPI = ::GetDeviceCaps(hdcScreen, LOGPIXELSX);
		::ReleaseDC(NULL, hdcScreen);
	}
	return iDPI;
}


CCanvas::CCanvas()
{
	
	active_plot = "";
	
}
//------------------------------------------------------
void CCanvas::Init()
{
	CRect rc;
	BitmapWidth = 400;
	BitmapHeight = 400;

}
//------------------------------------------------------
CCanvas::~CCanvas()
{
	Clear();
	DestroyWindow();
}
//------------------------------------------------------
void CCanvas::Clear()
{
	for (auto& plot : plots)
	{
		delete plot.second;
	}
	plots.clear();
}
//------------------------------------------------------
string CCanvas::AddPlot(Plot* plot)
{
	if (!plot) return "";
	string name="График (" + to_string(static_cast<int>(plots.size())) +")";
	if (!plot->getHeader().empty())
		name += string(" - ") + plot->getHeader();
	if (plots.count(name) > 0)
	{
		delete plots[name];
	}
	plots[name] = plot;
	return name;
}
//------------------------------------------------------
void CCanvas::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());
	
	ugc.g->Clear(Color(255, 255, 255));
	
	
	

	if (plots.count(active_plot) > 0)
	{
		int w = rect.Width();
		if (w > rect.Height()) w = rect.Height();
		ugc.g->DrawImage(plots[active_plot]->getBitmap(), (rect.Width()-w)/2, (rect.Height()-w)/2, w, w);
	}
	else
	{
		ugc.SetAlign(ugc.CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString("Нет изображения", rect.Width() / 2, rect.Height() / 2 - 8);
	}
	
}
//------------------------------------------------------
int CCanvas::getGridWidth()
{
	int count = 1;
	int size = plots.size();
	if (size == 1) count = 1;
	else if (size>0)
	{
		count = sqrt(size);
		if (count*count != size) count++;
	}
	return count;
}

int CCanvas::getMinWidth(CRect& rect, int count)
{
	int w = rect.Width() / count;
	int h = rect.Height() / count;
	if (w >= h) return h;
	else if (h > w) return w;
	
}


int CCanvas::getIndex(int x, int y)
{
	int size = plots.size();
	int count = getGridWidth();
	CRect rcClient;
	GetClientRect(&rcClient);
	int w = getMinWidth(rcClient, count);
	int _x = x / w;
	int _y = y / w;
	if (_x >= count) _x = count-1;
	if (_y >= count) _y = count-1;

	return _y*count+_x;
}

void CCanvas::SaveImages(string path)
{
	int i = 0;
	for (auto& plot : plots)
	{
		CImage image;
		
		image.Attach(plot.second->getHBITMAP());
		string name = path;
		name += plot.first + string(".png");
		
		image.Save(name.c_str(), ImageFormatPNG);
		i++;
	}
}