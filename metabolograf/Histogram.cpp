#include "stdafx.h"
#include "Histogram.h"


Histogram::Histogram()
{
	border = DPIX()(20);
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


void Histogram::Init()
{
	database = Database::getInstance();
	if (database->getCount() == 0) return;
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

void Histogram::OnPaint()
{
	CWnd::OnPaint();
	
	int table_size = static_cast<int>(table.size());
	if (table_size == 0) return;

	int bitW = (Width - border*2) / table_size;
	int maxHBits = 0;// максимальное число значений в самом большом столбце
	for (const auto& column : table)
	{
		int size = static_cast<int>(column.size());
		if (size > maxHBits) maxHBits = size;
	}
	int bitH = (Height - border*2) / maxHBits;
	if (bitH <= 0) bitH = 1;
	UGC ugc(GetDC(), Width, Height);
	
	ugc.SetAlign(Align::CENTER);
	ugc.SetTextSize(DPIX()(8));
	for (int i = 0; i < table_size; ++i)
	{
		int length = static_cast<int>(table[i].size());
		if (!column_status[i])// если выключено
		{
			ugc.SetDrawColor(170, 170, 170);
			ugc.FillRectangle(border + i*bitW, border, bitW, Height - border);
		}

		ugc.SetDrawColor(255, 0, 0);
		ugc.FillRectangle(border + i*bitW, Height-length*bitH-border, bitW, length*bitH);
		ugc.DrawNumber(length, border + i*bitW + bitW / 2, Height - length*bitH - border-ugc.GetTextHeight());

		ugc.SetDrawColor(0, 0, 0);
		ugc.DrawNumber(min+step*i, border + i*bitW, Height - border);
	}
	ugc.DrawNumber(min + step*table_size, border + table_size*bitW, Height - border);

	// рисуем ошибку
	ugc.SetTextSize(DPIX()(14));
	ugc.DrawNumber(CalculateErr(), Width/2, DPIX()(2));
}

double Histogram::CalculateErr()
{
	double result = 0;
	int count = 0;
	for (size_t i = 0; i < table.size(); ++i)
	{
		if (column_status[i])
		{
			for (size_t j=0; j<table[i].size(); ++j)
			{
				result += err_vector[table[i][j]];
				++count;
			}
		}
	}
	result = result / count;
	return result;
}

void Histogram::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x - border;
	//int y = point.y - border;
	int count = static_cast<int>(table.size());
	int bitW = (Width - border * 2) / count;//ширина прорисовки
	
	if (x<0 || x>bitW*count) return;
	
	x /= bitW;
	if (x >= count) x = count - 1;
	column_status[x] = !column_status[x];
	RedrawWindow();
}

void Histogram::OnMouseMove(UINT nFlags, CPoint point)
{

}

void Histogram::BuildErrVector()
{
	max = 0;
	min = 0;

	// создаем массив ошибок и сохраняем max и min
	err_vector.clear();
	err_vector.resize(database->getCount());
	size_t i = 0;
	for (auto& result : err_vector)
	{
		const double& Vinsp = database->getVariable("Vвдоха")[i];
		const double& Vexp = database->getVariable("Vвыдоха")[i];

		if (Vinsp > 0 && Vexp > 0)
			result = (Vinsp - Vexp) / Vinsp * 100.;
		if (result > max) max = result;
		if (result < min) min = result;

		++i;
	}
}


void Histogram::BuildTable(int count)
{
	table.clear();
	column_status.clear();

	table.resize(count);
	column_status.resize(count, true);
	
	step = (max - min) / static_cast<double>(count);
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

void Histogram::Apply()
{
	for (size_t i = 0; i < table.size(); ++i)
	{
		for (size_t j = 0; j < table[i].size(); ++j)
		{
			database->setChecked(table[i][j], column_status[i]);
		}
	}
}