#include "stdafx.h"
#include "CFastListCtrl.h"


BEGIN_MESSAGE_MAP(CFastListCtrl, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------
void CFastListCtrl::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());

	ugc.getGraphics()->Clear(Color(255, 255, 255));

	ugc.SetTextSize(10);
	DPIX dpix;
	// DRAW HEADERS
	int headerH = static_cast<int>(dpix.getValue() * HeaderHeight);
	int x = 0;
	ugc.SetAlign(Align::CENTER);
	for (int i = 0; i < static_cast<int>(column_headers.size()); ++i)
	{
		ugc.SetDrawColor(185, 185, 185);
		int width = column_width.at(i) - 1;
		ugc.FillRectangle(x, 0, width, headerH);

		ugc.SetDrawColor(0, 0, 0);
		string header(column_headers.at(i));

		int text_width = ugc.GetTextWidth(header);
		if(text_width > width)
			header += "...";
		while (text_width > width)
		{
			
			header.resize(header.size()-4);
			header += "...";
			text_width = ugc.GetTextWidth(header);

		}
		ugc.DrawString(header, x+width/2, 0);

		x += column_width.at(i);
	}



	ugc.SetAlign(Align::LEFT);

}
//-------------------------------------------------------------------------------------
void CFastListCtrl::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::SetBounds()
{

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();
	
	
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::OnLButtonDown(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int x = point.x;
	int y = point.y;

}
//-------------------------------------------------------------------------------------
void CFastListCtrl::DeleteAllItems()
{
	data.clear();
	selected_items.clear();
	RedrawWindow();
}
//-------------------------------------------------------------------------------------

void CFastListCtrl::SetSelectedItem(int index)
{
	if (!IsColumnIndexValid(index)) return;

	selected_items.push_back(index);
	sort(selected_items.begin(), selected_items.end());
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::UnselectItem(int index)
{
	auto it = find(selected_items.begin(), selected_items.end(), index);
	if (it == selected_items.end()) return;
	selected_items.erase(it);
}
//-------------------------------------------------------------------------------------
const vector<int>& CFastListCtrl::GetSelectedItems() const
{
	return selected_items;
}
//-------------------------------------------------------------------------------------
int CFastListCtrl::GetSelectedItem() const
{
	if (selected_items.size() == 0)
		return -1;
	return selected_items.at(0);
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::InsertColumn(int index, const string& name, int width)//добавляем колонки
{
	column_headers.insert(column_headers.begin() + index, name);
	vector<string> v;
	data.insert(data.begin() + index,v);
	column_width.insert(column_width.begin() + index, width);
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::DeleteColumn(int index)
{
	if (!IsColumnIndexValid(index)) return;

	column_headers.erase(column_headers.begin() + index);
	data.erase(data.begin() + index);
	column_width.erase(column_width.begin() + index);
}
//-------------------------------------------------------------------------------------
int CFastListCtrl::InsertItem(int column, int index, const string& value)
{
	if (!IsColumnIndexValid(column))
		return -1;
	
	data.at(column).insert(data.at(column).begin()+index, value);
	checked.insert(checked.begin() + index, true);
	return static_cast<int>(data.at(column).size() - 1);
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::SetItemText(int column, int index, const string& value)
{
	if (!IsColumnIndexValid(column))
		return;
	if (static_cast<size_t>(index) >= data.at(column).size())
		return;
	data[column][index] = value;
}
//-------------------------------------------------------------------------------------
int CFastListCtrl::GetItemCount() const
{
	if (data.size() == 0) return 0;
	return static_cast<int>(data.at(0).size());
}
//-------------------------------------------------------------------------------------
int CFastListCtrl::GetColumnWidth(int index) const
{
	if (!IsColumnIndexValid(index))
		return -1;
	return column_width.at(index);
}
//-------------------------------------------------------------------------------------
void CFastListCtrl::SetCheck(int index, bool status)
{
	if (static_cast<size_t>(index) >= checked.size()) 
		return;
	checked[index] = status;
	RedrawWindow();
}
//-------------------------------------------------------------------------------------
bool CFastListCtrl::GetCheck(int index) const
{
	if (static_cast<size_t>(index) >= checked.size()) 
		return false;
	return checked.at(index);
}
//-------------------------------------------------------------------------------------