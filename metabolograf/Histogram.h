#pragma once
#include "afxwin.h"
#include "Database.h"
#include "UGC.h"

class Histogram :
	public CWnd
{
public:

	Histogram();
	void Init(Database* _database);

	~Histogram();

	
protected:
	//variables
	int Width;
	int Height;
	Database* database;
	

	//functions
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
	void OnLButtonUp(UINT flags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void SetBounds();
	DECLARE_MESSAGE_MAP()
};
