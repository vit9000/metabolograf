// ���� �������� ��� MFC Samples ������������� ���������������� ����������������� ���������� Fluent �� ������ MFC � Microsoft Office
// ("Fluent UI") � ��������������� ������������� ��� ���������� �������� � �������� ���������� �
// ����������� �� ������ Microsoft Foundation Classes � ��������� ����������� ������������,
// ���������� � ����������� ����������� ���������� MFC C++.  
// ������� ������������� ���������� �� �����������, ������������� ��� ��������������� Fluent UI �������� ��������.  
// ��� ��������� �������������� �������� � ����� ������������ ��������� Fluent UI �������� ���-����
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// (C) ���������� ���������� (Microsoft Corp.)
// ��� ����� ��������.

// ChildView.h : ��������� ������ CChildView
//


#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "Metab.h"
#include "MyListCtrl.h"
#include "YuraDatabase.h"
#include "MainPlot.h"
#include "PlaygroundView.h"
#include "Export.h"
#include "PatientDialog.h"
#include "CurrentValues.h"
#include "TimerWindow.h"
#include "BigPlot.h"
#include "NutritionDialog.h"
#include "PowerStepDialog.h"
#include "DPIX.h"
#include "VolumeErrorDialog.h"

// ���� CChildView

class CChildView : public CWnd
{
// ��������
public:
	CChildView();

	UINT_PTR timer;
	int Circle;
	Database *database;
	YuraDatabase ydata;
	CMFCRibbonBar* ribbonbar;
	CMFCRibbonStatusBar* statusbar;

	void OnTimer(UINT_PTR nIdEvent);
	void UpdateStatusBar();
	void OnStatusBarTimeUpdate(CCmdUI *pCmdUI);
	
	void Init(CMFCRibbonBar* _ribbonbar, CMFCRibbonStatusBar* _statusbar);
	virtual ~CChildView();
	bool GetNewData();
	void OnClose();
	
	bool ProtolsAvailable();
	// ��������� ������� ����� ���������
protected:
	MainPlot main_plot;
	PlaygroundView playground;
	MyListCtrl main_list;
	CTabCtrl m_Tab;
	Metab metab;
	CurrentValues curValues;
	TimerWindow timerWindow;
	CString filename;
	CString filename_from_commandline;
	BigPlot listplot;
	Plot plot;
	bool RecordMode;
	bool Recording;
	bool CurValuesVisible;

	bool UnavailableMessage()
	{
		if (Recording)
		{
			MessageBoxA("���������� �� ����� ������������", "��������", MB_OK | MB_ICONINFORMATION);
			return true;
		}
		return false;
	}
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void LoadToList();
	void OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateUseExpiratoryVolume(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUseInspiratoryVolume(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUseMeanVolume(CCmdUI* pCmdUI);

	afx_msg void OnUpdateFilterTVolume(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAllRibbonElements(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterBrKoef(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurValuesVisible(CCmdUI* pCmdUI);
	afx_msg void OnCurValuesClick();
	afx_msg void OnUpdateRecordButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartTestButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEndTestButton(CCmdUI* pCmdUI);
	
	void ChangeFilterStatus();


	DECLARE_MESSAGE_MAP()
public:
	void OnBigPlotClick(int index);

	afx_msg void OnOpenFile();
	void OpenFile(CString fname);
	afx_msg void OnSaveFile();
	afx_msg void OnSaveAsFile();
	void SaveFile(CString fname);
	afx_msg void OnFilterCheckboxClick();
	afx_msg void OnRecord();
	afx_msg void OnShowListConfigDlg();
	afx_msg void OnShowPlotdialog();
	afx_msg void OnNewScript();
	afx_msg void OnRunScript();
	afx_msg void OnLoadScript();
	afx_msg void OnSaveScript();
	afx_msg void OnScriptProtocol();
	afx_msg void OnProtocol1();
	afx_msg void OnProtocol3();
	afx_msg void OnProtocol4();
	afx_msg void OnProtocolExcel();
	afx_msg void OnProtocolData();
	afx_msg void OnUseExpiratoryVolumeClick();
	afx_msg void OnUseInspiratoryVolumeClick();
	afx_msg void OnUseMeanVolumeClick();
	afx_msg void OnFilterTVolumeClick();
	afx_msg void OnFilterBrkoefClick();
	afx_msg void OnPatientButtonClick();
	afx_msg void OnPlotConfigButton();
	afx_msg void OnEnlargePlotButton();
	afx_msg void OnClearPlot();
	afx_msg void OnDefaultPlotButton();
	afx_msg void OnEnlargePlotShowDialogButton();
	afx_msg void OnStartTestButton();
	afx_msg void OnEndTestButton();
	afx_msg void OnSetExperienceMode();
	afx_msg void OnListPlotConfigButtonClick();
	
	afx_msg void OnUpdateExperienceButton(CCmdUI *pCmdUI);
	afx_msg void OnEnlargeButton();
	afx_msg void OnUpdateEnlargeButton(CCmdUI *pCmdUI);
	afx_msg void OnCustomNutritionButtonClick();
	afx_msg void OnProtocolMpk();
	afx_msg void OnStepChanged();
	afx_msg void OnUpdatePowerStep(CCmdUI *pCmdUI);
	afx_msg void OnVolumeFilterButton();
};

