#pragma once

#include "ugc.h"

#include "Plot.h"
//#include "PlotDialog.h"
#include <math.h>

class CCanvas : public CWnd
{
public:
	CCanvas();
	~CCanvas();
	
	void Init();
	void OnLButtonDblClk(CWnd *dlg, int x, int y);
	void EnlargePlot();
	void Clear();
	string AddPlot(Plot* plot);
	int size() { return plots.size(); }
	void SaveImages(string path);
	void SetPicture(string plotname){active_plot = plotname;}

	int GetScreenDPI();
private:
	map<string,Plot*> plots;
	string active_plot;
	int BitmapWidth;
	int BitmapHeight;
protected:
	int getIndex(int x, int y);
	int getGridWidth();
	int getMinWidth(CRect& rect, int count);
	void OnPaint();
	
	DECLARE_MESSAGE_MAP();
};