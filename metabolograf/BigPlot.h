#pragma once
#include "afxwin.h"
#include "Plot.h"
#include "MyListCtrl.h"
#include "CurrentValues.h"


class BigPlot :
	public CWnd
{
public:
	
	BigPlot();
	void Init(Plot* _plot, Database* _database, MyListCtrl* _main_list=nullptr, CurrentValues* _curValues=nullptr);
	

	~BigPlot();
	void SetType(int _type);
	int getValue(int i) const;
	void Update();
	void UpdateWithoutDraw();
	void SetCursor(int _cursor);
	void SetTimeCursor(int selectedItem);
	void LoadValues(const vector<int>& v);
	void ClearMarks()
	{
		plot->ClearMarks();
		for (auto& v : values) v = 0;
		RedrawWindow();
	}

	vector<int> values;
protected:
	//variables
	int Width;
	int Height;
	int type;
	int cursor;
	Plot* plot;
	Database* database;
	MyListCtrl* main_list;
	CurrentValues* curValues;
	
	
	//functions
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
	void OnLButtonUp(UINT flags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void SetBounds();
	DECLARE_MESSAGE_MAP()
};

