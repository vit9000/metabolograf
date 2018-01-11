#pragma once
#include <vector>
#include <string>

#include "afxwin.h"
#include "Database.h"
#include "UGC.h"

using std::vector;
using std::string;

class Histogram :
	public CWnd
{
public:

	Histogram();
	void Init();
	void SetBounds();
	~Histogram();
	void BuildTable(int count);
	void Apply();
	
protected:
	//variables
	int Width;
	int Height;
	int border;
	double min;
	double max;
	double step;
	Database* database;
	vector<double> err_vector;// вектор ошибок
	vector<vector<size_t>> table;// таблица хранит столбцы (1) и строки (2) с порядковым значением элемента err_vector
	vector<bool> column_status;
	//functions
	void BuildErrVector();
	
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
	void OnLButtonUp(UINT flags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	double CalculateErr();
	
	DECLARE_MESSAGE_MAP()
};
