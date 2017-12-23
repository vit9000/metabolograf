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
	BuildErrVector();
	BuildTable(20);
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



	int table_size = static_cast<int>(table.size());
	int bitW = Width / table_size;
	int maxHBits = 0;// максимальное число значений в самом большом столбце
	for (const auto& column : table)
	{
		int size = static_cast<int>(column.size());
		if (size > maxHBits) maxHBits = size;
	}
	int bitH = Height / maxHBits;

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 0, 0);
	
	for (int i = 0; i < table_size; ++i)
	{
		int length = static_cast<int>(table[i].size());
		ugc.FillRectangle(i*bitW, Height- length*bitH, bitW, length*bitH);
	}

}

void Histogram::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	

}

void Histogram::BuildErrVector()
{
	max = 0;
	min = 0;

	// создаем массив ошибок и сохраняем max и min
	err_vector.resize(database->getCount());
	size_t i = 0;
	for (auto& result : err_vector)
	{
		double& Vinsp = database->variables.at("Vвдоха")[i];
		double& Vexp = database->variables.at("Vвыдоха")[i];

		if (Vinsp > 0 && Vexp > 0)
			result = (Vinsp - Vexp) / Vinsp * 100.;
		if (result > max) max = result;
		if (result < min) min = result;

		++i;
	}
}


void Histogram::BuildTable(int count)
{
	table.resize(count);
	
	double step = (max - min) / static_cast<double>(count);
	if (step < 0) step = -step;
	// размещаем данные в таблице
	size_t size = err_vector.size();
	for (size_t i = 0; i < size; ++i)
	{
		size_t j = static_cast<size_t>((err_vector[i]-min) / step);// рассчитываем столбец
		if (j == table.size()) --j;
		table[j].push_back(i);//в стоблец добавляем строку с порядковым номером значения из err_vector
	}

}