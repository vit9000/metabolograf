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
	void Init(Database* _database);
	void SetBounds();
	~Histogram();

	
protected:
	//variables
	int Width;
	int Height;
	double min;
	double max;
	Database* database;
	vector<double> err_vector;// ������ ������
	vector<vector<size_t>> table;// ������� ������ ������� (1) � ������ (2) � ���������� ��������� �������� err_vector

	//functions
	void BuildErrVector();
	void BuildTable(int count);
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
	void OnLButtonUp(UINT flags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()
};
