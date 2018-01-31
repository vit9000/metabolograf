#pragma once
#include "afxwin.h"
#include "ugc.h"

#include "Plot.h"
//#include "PlotDialog.h"
#include <math.h>
#include "string_extentions.h"
#include "PlotCustomization.h"
#include "BigPlotDialog.h"
#include "Ini.h"
#include "IExperienceStatusTracker.h"


class MainPlot :
	public CWnd, 
	public IExperienceStatusTracker
{
public:
	MainPlot();
	~MainPlot();
public:
	CMFCRibbonBar* ribbonbar;
	
	void SetBounds();
	void Init();
	void OnLButtonUp(UINT flags, CPoint point);
	void OnLButtonDblClk(UINT flags, CPoint point);
	void ShowCustomizationDialog();
	void ShowPlotWindow();
	void EnlargePlot();
	void Clear();
	bool AddPlot(string plot_string, int width, int height);
	int size() { return plots.size(); }
	void UpdatePlots(bool reload=true);
	bool IsLarge() { return LargeMode; }
	void WritePlotToConfig(const Plot& plot);
	void LoadPlotFromConfig(Plot& plot);
	void WriteConfig();
	void LoadConfig();
	void ShowContextMenu();
	virtual bool GetExperienceStatus();
	virtual void SetExperienceMode();
	afx_msg void OnPlotConfigButton();
	afx_msg void OnEnlargePlotButton();
	afx_msg void OnClearPlot();
	afx_msg void OnDefaultPlotButton();
private:
	vector<string> defaultVal;
	int selectedPlot;
	vector<Plot*> plots;
	Database* database;
	int Width;
	int Height;
	bool LargeMode;

	int getIndex(int x, int y);
	int getGridWidth();
	int getMinWidth(CRect& rect, int count);
	void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP();

	

};

