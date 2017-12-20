#include "stdafx.h"
#include "BigPlot.h"


BigPlot::BigPlot()
{
	plot = nullptr;
	cursor = -1;
	type = 0;
	values.resize(3, -1);
}


BigPlot::~BigPlot()
{
}



BEGIN_MESSAGE_MAP(BigPlot, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void BigPlot::Init(Plot* _plot, Database* _database, MyListCtrl* _main_list, CurrentValues* _curValues)
{

	database = _database;
	plot = _plot;
	main_list = _main_list;
	curValues = _curValues;
	SetBounds();
}




void BigPlot::SetType(int _type) 
{
	type = _type; 
};

void BigPlot::LoadValues(const vector<int>& v)//�������� ����� �� �����
{
	for (int i = 0; i < v.size(), i < values.size(); i++)
		values[i] = (v[i] > 0) ? v[i] : -1;

}

int BigPlot::getValue(int i) const
{
	if (i<0 || i >= values.size()) return -1;
	return values[i];
}

void BigPlot::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}

void BigPlot::SetBounds()
{
	
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();

	if (plot)
	{
		plot->Resize(Width, Height);
		plot->DrawPlot();


	}
}


void BigPlot::OnPaint()
{
	CWnd::OnPaint();

	if (!plot) return;

	UGC ugc(GetDC(), Width, Height);
	ugc.DrawBitmap(plot->getBitmap(), 0, 0, Width, Height);

	if (cursor < 0) return;
	if (type == 0)
		ugc.SetDrawColor(255, 0, 0);
	else if(type==1)
		ugc.SetDrawColor(0, 255, 0);
	else 
		ugc.SetDrawColor(0, 0, 255);

	double dpix = ugc.getDPIX();

	ugc.FillRectangle(cursor, plot->getPlotRect().y - plot->getPlotRect().height, 2 * dpix, plot->getPlotRect().height);

	
}

void BigPlot::OnLButtonUp(UINT flags, CPoint point)
{
	//if (!plot->experience) return;
	//if (database->hdata.StartTest <= 0) return;
	int x = point.x;
	int y = point.y;
	cursor = -1;
	if (x >= plot->getPlotRect().x && x <= plot->getPlotRect().x + plot->getPlotRect().width
		&& y >= plot->getPlotRect().y - plot->getPlotRect().height)
	{
		plot->SetMarkPosByMouseCoordinate(type, x - plot->getPlotRect().x);// ���������� ����� �� ������� �� ���������� �����
		
		int index = plot->FromPlotIndexToTableIndex(type, database);//�������� ������ ������� � �������
		if (index > 0 && index < database->getCount())
			values[type] = index;//��������� �� ���� �������
		
		

		if (main_list && curValues)//���� �� �������� ��������� �� ������� �����������
		{//����� ���������� ��������� �� ����������� �������
			main_list->UnSelectCurrentItem();//������ ������� ��������� ������� ������
			main_list->SetSelectedItem(index);//�������� ����������� �������
			main_list->EnsureVisible(index, true);//���������� �����, ����� ������ ���������
			main_list->SetFocus();//�������� ����� �������
			curValues->SetSelected(index);
		}
		
		plot->DrawPlot();//������ ������ � bitmap
		RedrawWindow();//�������������� ��� �������
		
	}

}

void BigPlot::SetCursor(int index)//���������� ����� �� �������
{
	plot->SetMarkPosByPlotIndex(type, index);
	cursor = -1;
	plot->DrawPlot();
	RedrawWindow();
}

void BigPlot::SetTimeCursor(int selectedItem)//���������� ����� �� �������
{
	if (selectedItem < 0 || selectedItem >= database->getCount())
	{
		SetCursor(-1);
		return;
	}

	MTime time = database->datetime[selectedItem].getTime();
	MTime starttime = database->datetime[0].getTime();

	time = time - starttime;
	
	plot->SetMarkPosByTime(type, time);// x - plot->plotRect.x);
	cursor = -1;
	plot->DrawPlot();
	RedrawWindow();
}


void BigPlot::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!plot->IsExperience()) return;
	if (database->hdata.StartTest <= 0) return;
	int x = point.x;
	int y = point.y;

	if (x >= plot->getPlotRect().x && x <= plot->getPlotRect().x + plot->getPlotRect().width
		&& y >= plot->getPlotRect().y - plot->getPlotRect().height)
	{
		cursor = x;
		RedrawWindow();

	}


}

void BigPlot::Update()
{
	if (!plot) return;
	plot->Reload(database);
	plot->DrawPlot();
	RedrawWindow();
}

void BigPlot::UpdateWithoutDraw()
{
	if (!plot) return;
	plot->Reload(database);
	
}