#pragma once

#include <string>
#include <vector>
#include <algorithm>
using namespace std;


#include "ugc.h"


class CFastListCtrl : public CWnd
{
private:
	vector<vector<string>> data;
	vector<string> column_headers;
	vector<int> column_width;
	vector<int> selected_items;
	vector<bool> checked;

	int Width;
	int Height;
	int HeaderHeight;

	inline bool IsColumnIndexValid(int index) const
	{
		size_t i = static_cast<size_t>(index);
		if (i >= column_headers.size())
			return false;
		return true;
	}
public:
	CFastListCtrl(): Width(100), Height(100), HeaderHeight(20) {}
	virtual ~CFastListCtrl(){}

protected:
	


	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetBounds();
	DECLARE_MESSAGE_MAP()
public:
	void DeleteAllItems();
	void DeleteColumn(int index);
	void SetSelectedItem(int index);
	void UnselectItem(int index);
	//void UnSelectCurrentItem();

	const vector<int>& GetSelectedItems() const;

	int GetSelectedItem() const;
	void InsertColumn(int index, const string& name, int width);//добавляем колонки
	int InsertItem(int column, int index, const string& value);
	void SetItemText(int column, int index, const string& value);
	int GetItemCount() const;
	
	int GetColumnWidth(int index) const;

	void SetCheck(int index, bool status);
	bool GetCheck(int index) const;
};