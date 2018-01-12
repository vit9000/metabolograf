#pragma once
#include "afxwin.h"

#include "ugc.h"
#include "Analizator.h"
#include "Database.h"
#include "Dictionary.h"
#include "Highlighter.h"
#include "Canvas.h"
#include "Algorithm.h"

class PlaygroundView :
	public CWnd
{
	//Variables
private:
	CRichEditCtrl Editor;
	CListBox OutputList;
	CListBox VariablesList;
	CCanvas canvas;
	Database* database;
	Highlighter highlighter;
	Dictionary dictionary;
	string protocol;
public:
	PlaygroundView();
	~PlaygroundView();
	void Init() { database = Database::getInstance(); }
	void UpdateVariablesList();
	string BuildCode(string& code, Analizator& analizator);
	void SaveHTML();
	void SaveHTML(string& text);
	void SaveWorkScript() { SaveScript("work_script.txt"); };
	void LoadWorkScript() { LoadScript("work_script.txt"); };
	void SaveScript(string filename);
	void LoadScript(string filename);

	afx_msg void OnSaveScript();
	afx_msg void OnLoadScript();
	afx_msg void OnNewScript();
	afx_msg void OnRunScript(bool repeat);
private:
	afx_msg void OnLbnDblclkVariableslist();
	afx_msg void OnEnChangeEditor();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT flags, CPoint point);
	afx_msg void OnLbnSelchangeOutputlist();
	afx_msg void OnLbnDblclkOutputlist();
	afx_msg void OnTimer(UINT_PTR nIdEvent);
	

	DECLARE_MESSAGE_MAP()
};

