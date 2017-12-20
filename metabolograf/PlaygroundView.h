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
public:
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

	void Init(Database* _database) { database = _database; }
	

	void UpdateVariablesList();
	string BuildCode(string& code, Analizator& analizator);
	void SaveHTML();
	void SaveHTML(string& text);
	afx_msg void OnLbnDblclkVariableslist();
	afx_msg void OnRunScript(bool repeat);
	afx_msg void OnEnChangeEditor();
	afx_msg void OnNewScript();
	void SaveWorkScript() { SaveScript("work_script.txt"); };
	void LoadWorkScript() { LoadScript("work_script.txt"); };
	void SaveScript(string filename);
	afx_msg void OnSaveScript();
	void LoadScript(string filename);
	afx_msg void OnLoadScript();
	
	afx_msg void OnLbnSelchangeOutputlist();
	afx_msg void OnLbnDblclkOutputlist();
	afx_msg void OnTimer(UINT_PTR nIdEvent);

protected:
	
	
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT flags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

