
// ChildView.cpp : ���������� ������ CChildView
//

#include "stdafx.h"
#include "metabolograf.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


CChildView::CChildView()
{
	Circle = 0;
	timer = 0;
	statusbar = nullptr;
	ribbonbar = nullptr;
	CurValuesVisible = (bool)GetPrivateProfileInt("View", "CurValuesVisible", 1, "metabolograf_config.ini");
	
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_NOTIFY(NM_CLICK, IDC_MAINLIST, &CChildView::OnItemClicked)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAINLIST, &CChildView::OnLvnItemchangedList)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, &CChildView::OnTabChange)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_OPEN_FILE, &CChildView::OnOpenFile)
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnOpenFile)
	ON_COMMAND(ID_SAVE_FILE, &CChildView::OnSaveFile)
	ON_COMMAND(ID_SAVE_FILE_AS, &CChildView::OnSaveAsFile)
	ON_COMMAND(ID_FILE_SAVE, &CChildView::OnSaveFile)
	ON_COMMAND(ID_FILE_SAVE_AS, &CChildView::OnSaveAsFile)

	ON_COMMAND(ID_RECORD, &CChildView::OnRecord)
	ON_COMMAND(ID_SHOW_LIST_CONFIG, &CChildView::OnShowListConfigDlg)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MAINLIST, OnCustomdrawMyList)
	ON_COMMAND(ID_SHOW_PLOTDIALOG, &CChildView::OnShowPlotdialog)
	ON_COMMAND(ID_NEW_SCRIPT, &CChildView::OnNewScript)
	ON_COMMAND(ID_RUN_SCRIPT, &CChildView::OnRunScript)
	ON_COMMAND(ID_LOAD_SCRIPT, &CChildView::OnLoadScript)
	ON_COMMAND(ID_SAVE_SCRIPT, &CChildView::OnSaveScript)
	ON_COMMAND(ID_SCRIPT_PROTOCOL, &CChildView::OnScriptProtocol)
	ON_COMMAND(ID_PROTOCOL1, &CChildView::OnProtocol1)
	ON_COMMAND(ID_PROTOCOL3, &CChildView::OnProtocol3)
	ON_COMMAND(ID_PROTOCOL4, &CChildView::OnProtocol4)
	ON_COMMAND(ID_PROTOCOL_EXCEL, &CChildView::OnProtocolExcel)
	ON_COMMAND(ID_PROTOCOL_DATA, &CChildView::OnProtocolData)

	ON_COMMAND(ID_PLOT_CONFIG_BUTTON, OnPlotConfigButton)
	ON_COMMAND(ID_ENLARGE_PLOT_BUTTON, OnEnlargePlotButton)
	ON_COMMAND(ID_CLEAR_PLOT, OnClearPlot)

	//���������� ������� �������
	ON_UPDATE_COMMAND_UI(ID_UseExpiratoryVolume, OnUpdateUseExpiratoryVolume)
	ON_UPDATE_COMMAND_UI(ID_UseInspiratoryVolume, OnUpdateUseInspiratoryVolume)
	ON_UPDATE_COMMAND_UI(ID_UseMeanVolume, OnUpdateUseMeanVolume)

	ON_UPDATE_COMMAND_UI(ID_Filter_TVolume, OnUpdateFilterTVolume)
	ON_UPDATE_COMMAND_UI(ID_Filter_BrKoef, OnUpdateFilterBrKoef)
	
	

	//���������� ������ ������ - ������� ��� ���������
	ON_UPDATE_COMMAND_UI(ID_RECORD, OnUpdateRecordButton)
	ON_UPDATE_COMMAND_UI(ID_START_TEST_BUTTON, OnUpdateStartTestButton)
	ON_UPDATE_COMMAND_UI(ID_END_TEST_BUTTON, OnUpdateEndTestButton)

	//������, ������� ����������� ��� ������
	ON_UPDATE_COMMAND_UI(ID_OPEN_FILE, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_SAVE_FILE_AS, OnUpdateAllRibbonElements)
	//ON_UPDATE_COMMAND_UI(ID_PATIENT_BUTTON, OnUpdateAllRibbonElements)
	//ON_UPDATE_COMMAND_UI(ID_SHOW_LIST_CONFIG, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_RUN_SCRIPT, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_NEW_SCRIPT, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_LOAD_SCRIPT, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_SAVE_SCRIPT, OnUpdateAllRibbonElements)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PROTOCOL, OnUpdateAllRibbonElements)
	//-----------
	
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_PANE2, OnStatusBarTimeUpdate)

	ON_COMMAND(ID_UseExpiratoryVolume, &CChildView::OnUseExpiratoryVolumeClick)
	ON_COMMAND(ID_UseInspiratoryVolume, &CChildView::OnUseInspiratoryVolumeClick)
	ON_COMMAND(ID_UseMeanVolume, &CChildView::OnUseMeanVolumeClick)

	ON_COMMAND(ID_Filter_TVolume, &CChildView::OnFilterTVolumeClick)
	ON_COMMAND(ID_Filter_BrKoef, &CChildView::OnFilterBrkoefClick)
	ON_COMMAND(ID_PATIENT_BUTTON, &CChildView::OnPatientButtonClick)

	ON_UPDATE_COMMAND_UI(ID_CURVALUES_VISIBLE, OnUpdateCurValuesVisible)
	ON_COMMAND(ID_CURVALUES_VISIBLE, &CChildView::OnCurValuesClick)
	ON_COMMAND(ID_DEFAULT_PLOT_BUTTON, &CChildView::OnDefaultPlotButton)
	ON_COMMAND(ID_ENLARGE_PLOT_SHOW_DIALOG_BUTTON, &CChildView::OnEnlargePlotShowDialogButton)
	ON_COMMAND(ID_START_TEST_BUTTON, &CChildView::OnStartTestButton)
	ON_COMMAND(ID_END_TEST_BUTTON, &CChildView::OnEndTestButton)
	ON_COMMAND(ID_MPKMODE_BUTTON, &CChildView::OnSetExperienceMode)
	ON_COMMAND(ID_LISTPLOTCONFIG_BUTTON, &CChildView::OnListPlotConfigButtonClick)
	
	ON_UPDATE_COMMAND_UI(ID_MPKMODE_BUTTON, &CChildView::OnUpdateExperienceButton)
	ON_COMMAND(ID_ENLARGEBUTTON, &CChildView::OnEnlargeButton)
	ON_UPDATE_COMMAND_UI(ID_ENLARGEBUTTON, &CChildView::OnUpdateEnlargeButton)
	ON_COMMAND(ID_CUSTOM_NUTRITION, &CChildView::OnCustomNutritionButtonClick)
	ON_COMMAND(ID_PROTOCOL_MPK, &CChildView::OnProtocolMpk)
	
	ON_COMMAND(ID_VOLUME_FILTER_BUTTON, &CChildView::OnVolumeFilterButton)
END_MESSAGE_MAP()


void CChildView::OnShowWindow(BOOL bShow, UINT nStatus)
{}
//----------------------------------------------------------------------------------------------
void CChildView::Init(CMFCRibbonBar* _ribbonbar, CMFCRibbonStatusBar* _statusbar)
{
	ribbonbar = _ribbonbar;
	statusbar = _statusbar;
	main_plot.ribbonbar = ribbonbar;
	ydata.Connect();
	RecordMode = true;
	Recording = false;
	string commline = GetCommandLine();
	//MessageBox(commline.c_str(), "");
	int i = commline.length() - 2;
	for (i; i >= 0; i--)
	{
		if (commline[i] == '\"')
			break;
	}

	string fname;
	if (i > 0)
	{
		fname = commline.substr(i + 1);
	}
	if(fname.length()>3)
	{
		//fname.erase(0, 1);
		fname.erase(fname.size() - 1, 1);
		OpenFile(fname.c_str());
		//MessageBox(fname.c_str(), "");
		filename_from_commandline = filename;
	}
	else fname.clear();


	if (ydata.Initialized && filename_from_commandline.IsEmpty())
	{
		database.hdata.PatientAge = ydata.PatientAge;
		database.hdata.PatientSex = ydata.PatientSex;
		database.hdata.PatientWeight = ydata.PatientWeight;
		database.hdata.PatientHeight = ydata.PatientHeight;
		sprintf(database.hdata.PatientName, "%s", ydata.FIO);
		for (int i = strlen(database.hdata.PatientName) - 1; i >= 0; i--)
		{
			if (database.hdata.PatientName[i] == ' ') database.hdata.PatientName[i] = '\0';
			else break;
		}
		sprintf(database.hdata.AdditionalInformation, "%s", ydata.Info);
	}

	playground.UpdateVariablesList();

}
//----------------------------------------------------------------------------------------------
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CRect rect(0, 0, 300, 300);

	//m_Tab.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | LBS_NOTIFY, rect, this, IDC_TABCTRL);
	m_Tab.Create(WS_CHILD | WS_VISIBLE | TCS_BOTTOM, rect, this, IDC_TABCTRL);//TCS_BOTTOM

	CFont* pFont = new CFont;
	VERIFY(pFont->CreateFont(
		14 * (double)DPIX(),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH,				// nPitchAndFamily
		"Arial"));                 // lpszFacename

	m_Tab.SetFont(pFont);

	m_Tab.InsertItem(0, " ������� ");
	m_Tab.InsertItem(1, " ������� ");
	//m_Tab.InsertItem(2, " ������ ");//����� ������ ����

	main_list.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | LBS_NOTIFY, rect, this, IDC_MAINLIST);
	main_list.SetFocus();
	main_list.ModifyStyle(LVS_LIST, LVS_REPORT, 0); //- ������ ����� �������
	main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	main_list.Init(&database);

	curValues.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, IDC_CURRVALUES);
	curValues.Init(&database);
	curValues.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);

	playground.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_PLAYGROUNDWINDOW);
	playground.Init(&database);
	playground.ShowWindow(SW_HIDE);

	main_plot.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_MAINPLOTWINDOW);
	main_plot.Init(&database);
	main_plot.ribbonbar = ribbonbar;
	main_plot.ShowWindow(SW_HIDE);

	timerWindow.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_TIMERWINDOW);
	timerWindow.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);

	listplot.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, IDC_LISTPLOT);
	main_plot.LoadPlotFromConfig(plot);
	plot.EnlargeUI();
	//plot.SetBackgroundColor(225, 225, 225);
	plot.DrawPlot();
	listplot.Init(&plot, &database, &main_list, &curValues);
	return 0;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnClose()
{
	playground.SaveWorkScript();
	if (ydata.Initialized && database.getCount()>0)
	{
		//if (filename_from_commandline.IsEmpty())
		//filename_from_commandline = "rec.mbf";
		int result = MessageBox("��������� ��������� � ���� ������?", "�������������", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && database.getCount()>0)
		{				
			SaveFile("rec.mbf");
			if (ydata.Initialized)
				ydata.g_pdata->action = 1;
		}
		else
		{
			if (ydata.Initialized)
				ydata.g_pdata->action = 0;
		}

	}
	else if(database.getCount()>0)
	{
		int result = MessageBox("��������� ��������� � ����� \"" + filename + "\"?", "�������������", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && database.getCount()>0)
		{
			SaveFile(filename);
		}
	}
	if (metab.Connect())
		metab.Disconnect();
	
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void CChildView::OnSize(UINT nType, int cx, int cy)
{

	CWnd::OnSize(nType, cx, cy);
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;
	DPIX dpiX;

	//Tab
	::SetWindowPos(GetDlgItem(IDC_TABCTRL)->m_hWnd, HWND_TOP,
		0, 0, rect.right,
		rect.bottom, NULL);

	int curWidth = (CurValuesVisible) ? 240 * (double)dpiX : 0;
	int timerHeight = 90 * dpiX;
	//
	::SetWindowPos(GetDlgItem(IDC_CURRVALUES)->m_hWnd, HWND_TOP,
		0,
		0,
		240 * dpiX,
		rect.bottom - 20 * dpiX - timerHeight,
		NULL);



	::SetWindowPos(GetDlgItem(ID_TIMERWINDOW)->m_hWnd, HWND_TOP,
		0,
		rect.bottom - timerHeight - 20 * dpiX,
		240 * dpiX,
		timerHeight,
		NULL);

	// �������
	::SetWindowPos(GetDlgItem(IDC_MAINLIST)->m_hWnd, HWND_TOP,
		curWidth, 0,
		rect.right - curWidth,
		rect.bottom - 220 * dpiX,
		NULL);

	//������ ��� ��������
	::SetWindowPos(GetDlgItem(IDC_LISTPLOT)->m_hWnd, HWND_TOP,
		curWidth, rect.bottom - 220 * dpiX,
		rect.right - curWidth,
		200 * dpiX,
		NULL);

	//�������
	//...
	::SetWindowPos(GetDlgItem(ID_MAINPLOTWINDOW)->m_hWnd, HWND_TOP,
		curWidth, 0,
		rect.right - curWidth,
		rect.bottom - 20 * dpiX,
		NULL);

	::SetWindowPos(GetDlgItem(ID_PLAYGROUNDWINDOW)->m_hWnd, HWND_TOP,
		0, 0,
		rect.right,
		rect.bottom - 20 * dpiX,
		NULL);

	curValues.RedrawWindow();
	main_plot.RedrawWindow();
	main_list.RedrawWindow();
	timerWindow.RedrawWindow();

}
//----------------------------------------------------------------------------------------------
// ��������� ������������ Tab
void CChildView::OnTabChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	main_list.ShowWindow(SW_HIDE);
	main_plot.ShowWindow(SW_HIDE);
	listplot.ShowWindow(SW_HIDE);
	playground.ShowWindow(SW_HIDE);

	ribbonbar->HideAllContextCategories();
	//...
	int tab_selected = m_Tab.GetCurSel();
	switch (tab_selected)
	{
	case 0://�������
		main_list.ShowWindow(SW_SHOW);
		listplot.ShowWindow(SW_SHOW);
		if (CurValuesVisible)
		{
			curValues.ShowWindow(SW_SHOW);
			timerWindow.ShowWindow(SW_SHOW);
		}
		break;
	case 1://�������

		main_plot.UpdatePlots();
		main_plot.ShowWindow(SW_SHOW);
		if (CurValuesVisible)
		{
			curValues.ShowWindow(SW_SHOW);
			timerWindow.ShowWindow(SW_SHOW);
		}
		main_plot.ShowContextMenu();
		break;
	case 2://�����������
		playground.ShowWindow(SW_SHOW);

		ribbonbar->ShowContextCategories(ID_SCRIPT_CONTEXT);
		ribbonbar->ActivateContextCategory(ID_SCRIPT_CONTEXT);

		curValues.ShowWindow(SW_HIDE);
		timerWindow.ShowWindow(SW_HIDE);
		break;
	}
}
//----------------------------------------------------------------------------------------------

void CChildView::OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (main_list.IsBusy()) return;
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->uChanged & LVIF_STATE) // item state has been changed
	{
		int i = pNMLV->iItem;
		if (i >= 0 && i < database.getCount())
		{
			bool temp = main_list.GetCheck(i);
		
			if (database.checked[i] != temp)
			{
				database.checked[i] = temp;
				listplot.UpdateWithoutDraw();
				listplot.SetTimeCursor(main_list.GetSelectedItem());
			}
			


			bool bSelectedNow = (pNMLV->uNewState  & LVIS_SELECTED);
			bool bSelectedBefore = (pNMLV->uOldState  & LVIS_SELECTED);
			if (bSelectedNow && !bSelectedBefore)
			{
				int selectedItem = main_list.GetSelectedItem();
				if (selectedItem > -1)
				{
					//MessageBox(to_string(selectedItem).c_str(), "", MB_OK);

					curValues.SetSelected(selectedItem);
					listplot.SetTimeCursor(selectedItem);

				}
			}
		}
		
	}
	
	

	*pResult = 0;
}

//----------------------------------------------------------------------------------------------
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	CRect rect(85, 110, 180, 210);


	
	return TRUE;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // �������� ���������� ��� ���������
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OpenFile(CString fname)
{
	if (UnavailableMessage()) return;

	filename = fname;
	if (!database.OpenFile(fname.GetBuffer()))
	{
		MessageBox("������ �������� ����� \""+fname + "\"", "������", MB_OK | MB_ICONERROR);
		return;
	}
		
	playground.UpdateVariablesList();
	OnRunScript();//LoadToList();

	UpdateStatusBar();

	RecordMode = false;
	
	if (m_Tab.GetCurSel() == 1)
		main_plot.UpdatePlots();	
	listplot.Update();
	if (CurValuesVisible)// ���� ������� ������ ��������� ��������
	{
		curValues.SetSelected(-1);//��������� ��������� ����������

	}

	timerWindow.Update(&database);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateAllRibbonElements(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!Recording);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseExpiratoryVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database.hdata.UseExpiratoryVolume);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseInspiratoryVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database.hdata.UseInspiratoryVolume);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseMeanVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database.hdata.UseMeanVolume);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateFilterTVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database.hdata.Filter_TVolume);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateFilterBrKoef(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database.hdata.Filter_BrKoef);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateCurValuesVisible(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CurValuesVisible);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnOpenFile()
{
	CFileDialog fileDialog(TRUE, "���� �������������", "*.mbf");	//������ ������ ������ �����
	int result = fileDialog.DoModal();	//��������� ���������� ����
	if (result == IDOK)	//���� ���� ������
	{
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();
		OpenFile(str);
	}
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnSaveFile()
{
	if (database.getCount() == 0)
	{
		MessageBox("��� ������ ��� ����������)", "��������", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (filename.IsEmpty())
		OnSaveAsFile();
	else
	{
		SaveFile(filename);
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::OnSaveAsFile()
{
	if (database.getCount() == 0)
	{
		MessageBox("��� ������ ��� ����������)", "��������", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CFileDialog fileDialog(FALSE, "���� �������������", "*.mbf");	//������ ������ ������ �����
	int result = fileDialog.DoModal();	//��������� ���������� ����
	if (result == IDOK)	//���� ���� ������
	{
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();
		SaveFile(str);
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::SaveFile(CString fname)
{
	if (UnavailableMessage()) return;

	for (int i = 0; i < database.getCount(); i++)
	{
		database.checked[i] = main_list.GetCheck(i);
	}
	if (database.SaveFile(fname.GetBuffer(fname.GetLength())))
		filename = fname;
	else
		MessageBox("������ ���������� �����", "������", MB_OK|MB_ICONERROR);	
}
//----------------------------------------------------------------------------------------------
void CChildView::LoadToList()
{
	for (int i = 0; i < database.getCount(); i++)
	{
		main_list.AddToList(i);
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::OnFilterCheckboxClick()
{


	int t = 0;
	return; 
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnCurValuesClick()
{
	CurValuesVisible = !CurValuesVisible;

	curValues.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);
	timerWindow.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);
	
	if (m_Tab.GetCurSel() == 0)
	{
		main_list.ShowWindow(SW_HIDE);
		main_list.ShowWindow(SW_SHOW);
	}
	if (!CurValuesVisible) curValues.SetSelected(-1);
	OnSize(0, 0, 0);

	WritePrivateProfileString("View", "CurValuesVisible", to_string((int)CurValuesVisible).c_str(), "metabolograf_config.ini");
	
}
//----------------------------------------------------------------------------------------------
bool CChildView::GetNewData()
{
	metab.ReadData();

	if (!metab.NewDataRecieved) return false;//���� ����� ������ �� ��������, �� ����������
	if (metab.RespRate == 0 || metab.RespRate>100) return false;
	metab.NewDataRecieved = false;//���� ������ ��������, ����� ���������� �����

	time_t t = time(NULL);
	tm *tmp = localtime(&t);
	if (Circle == 0)
	{
		database.hdata.Year = tmp->tm_year + 1900;
		database.hdata.Month = tmp->tm_mon + 1;
		database.hdata.Day = tmp->tm_mday;
	}

	database.datetime.append(Datetime(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec));
	database.variables["V�����"].append(metab.Vol_insp);
	database.variables["V������"].append(metab.Vol_exsp);
	database.variables["FiO2"].append(metab.FiO2);
	database.variables["FetO2"].append(metab.FetO2);
	database.variables["FiCO2"].append(metab.FiCO2 / 760. * 100.);
	database.variables["FetCO2"].append(metab.FetCO2 / 760. * 100.);
	database.variables["��"].append(metab.RespRate);

	
	//database.variables["��_old"].append(metab.RespRateOld);
	//� ������ ��� ����� �������


	if (metab.HR >= 0)
	{
		if (!database.hdata.HR)
		{
			database.hdata.HR = true;
			database.variable_names.push_back("���");
			database.variable_names.push_back("RR");
			database.variable_names.push_back("SD");
		}
		database.variables["���"].append(metab.HR);

	}
	database.checked.append(true);	

	//Filter(Circle);//Filter of the data
	database.CalculateParameters(Circle);//������������� ��� ��������� ���������
	Circle++;
	database.hdata.count = Circle;//��������� ������ �������
	
	playground.OnRunScript(true);//��������� ������, ��� �� ����������� ���� ����������
	main_plot.UpdatePlots();//��������� �������
	listplot.Update();
	if(CurValuesVisible)// ���� ������� ������ ��������� ��������
		curValues.SetSelected(Circle - 1);//��������� ��������� ����������
	main_list.UnSelectCurrentItem();
	UpdateStatusBar();
	//main_list.SetSelectedItem(Circle - 1);
	
	return true;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnRecord()
{
	CMFCRibbonButton *button = (CMFCRibbonButton*)ribbonbar->FindByID(ID_RECORD);
	if (timer != 0)
	{
		KillTimer(timer);
		metab.RestoreMode();
		Recording = false;
		timerWindow.StopRec();
	}
	else
	{
		PatientDialog pDlg;
		pDlg.Init(&database, true);
		int result = pDlg.DoModal();
		if (result == IDCANCEL)
			return;
		database.AddPatientParametersToVariables();
		
		if (!metab.Connect()) return;
		timer = SetTimer((int)1, 1000, NULL);
		Recording = true;

		//��� ���������� "��_old" ��������� ������ ��� ������ ������ �����
		/*if (Circle == 0)
		{
			database.variable_names.push_back("��_old");
			main_list.InsertParameterAfter("��_old", "��");
		}*/
		timerWindow.StartRec();
	}
	

	//CMFCRibbonButton *p = DYNAMIC_DOWNCAST(CMFCRibbonButton, ribbonbar.FindByID(ID_RECORD));
	
	
	ribbonbar->RecalcLayout();
	ribbonbar->RedrawWindow();
	
	


	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnTimer(UINT_PTR nIdEvent)
{
	if (nIdEvent != 1)
	{
		CWnd::OnTimer(nIdEvent);
		return;
	}
	if (!GetNewData()) return;

	int index = database.getCount() - 1;
	main_list.AddToList(index);
	main_list.SetCheck(index, true);
	CWnd::OnTimer(nIdEvent);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnShowListConfigDlg()
{
	//if (UnavailableMessage()) return;
	main_list.ShowConfigDialog();

}
//----------------------------------------------------------------------------------------------
void CChildView::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	main_list.OnDrawMyList(pNMHDR, pResult);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnShowPlotdialog()
{
	// TODO: �������� ���� ��� ����������� ������
}
//----------------------------------------------------------------------------------------------
void CChildView::OnNewScript()
{
	playground.OnNewScript();
	// TODO: �������� ���� ��� ����������� ������
}
//----------------------------------------------------------------------------------------------
void CChildView::OnRunScript()
{
	playground.OnRunScript(false);
	main_list.LoadConfig();
	main_list.Reload();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnLoadScript()
{
	playground.OnLoadScript();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnSaveScript()
{
	playground.OnSaveScript();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnScriptProtocol()
{
	playground.SaveHTML();// TODO: �������� ���� ��� ����������� ������
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocol1()
{
	if (!ProtolsAvailable()) return;
	Export *fitnes_export = new Export(&database);
	fitnes_export->DietaExport();
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
bool CChildView::ProtolsAvailable()
{
	bool checked = false;
	for (int i = 0; i<database.hdata.count; i++)
	{
		if (database.checked[i] == true)
		{
			checked = true;
			break;
		}
	}

	if (database.hdata.count <= 0 || !checked)
	{
		MessageBox("������������ ������ ��� ������������ ���������", "��������", MB_OK);
		return false;
	}
	/*if (database.hdata.PatientWrist <= 0)
	{
		MessageBox(L"��� ������������ ��������� ���������� ������ ���������� ��������", L"��������", MB_OK);
		PatientForm->ShowModal();
		if (calculator->database.hdata.PatientWrist <= 0)
		{
			Application->MessageBox(L"�� �� ����� ���������� ��������. �������� ������������� �� �����", L"��������", MB_OK);
			return false;
		}
		else
			return true;
	}*/

	return true;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocol3()
{
	if (!ProtolsAvailable()) return;

	/*if (database.hdata.PatientWrist<2)
	{
		MessageBox("��� ��������� ��������� ���������� ������� ���������� ��������", L"��������", MB_OK);
		PatientForm->SetEditable(true, true);
		PatientForm->ShowModal();

		if (calculator->database.hdata.PatientWrist<2) return;
	}*/

	Export *fitnes_export = new Export(&database);
	fitnes_export->StandartExport(false);
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocol4()
{
	if (!ProtolsAvailable()) return;
	Export *fitnes_export = new Export(&database);
	fitnes_export->StandartExport(true);
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocolExcel()
{
	if (!ProtolsAvailable()) return;
	Export *xls_export = new Export(&database);
	xls_export->ExportExcel();
	delete  xls_export;
	xls_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocolData()
{
	if (!ProtolsAvailable()) return;
	Export *fexport = new Export(&database);
	fexport->ExportDataString();
	delete  fexport;
	fexport = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::ChangeFilterStatus()
{
	for (int i = 0; i<database.getCount(); i++)
	{
		database.Filter(i);
		main_list.SetCheck(i, database.checked[i]);
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseExpiratoryVolumeClick()
{
	database.hdata.UseExpiratoryVolume = !database.hdata.UseExpiratoryVolume;
	database.hdata.UseInspiratoryVolume = false;
	database.hdata.UseMeanVolume = false;
	if (database.getCount() == 0) return;

	database.CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseInspiratoryVolumeClick()
{
	database.hdata.UseExpiratoryVolume = false;
	database.hdata.UseInspiratoryVolume = !database.hdata.UseInspiratoryVolume;
	database.hdata.UseMeanVolume = false;

	if (database.getCount() == 0) return;

	database.CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseMeanVolumeClick()
{
	database.hdata.UseExpiratoryVolume = false;
	database.hdata.UseInspiratoryVolume = false;
	database.hdata.UseMeanVolume = !database.hdata.UseMeanVolume;

	if (database.getCount() == 0) return;

	database.CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();

}
//----------------------------------------------------------------------------------------------


void CChildView::OnFilterTVolumeClick()
{
	database.hdata.Filter_TVolume = !database.hdata.Filter_TVolume;
	if (database.getCount() == 0) return;
	ChangeFilterStatus();
	listplot.Update();
	main_plot.UpdatePlots();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnFilterBrkoefClick()
{
	database.hdata.Filter_BrKoef = !database.hdata.Filter_BrKoef;
	if (database.getCount() == 0) return;
	ChangeFilterStatus();
	listplot.Update();
	main_plot.UpdatePlots();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnPatientButtonClick()
{
	if (UnavailableMessage())return;
	PatientDialog pDlg;
	bool active = true;
	if (ydata.Initialized || !filename.IsEmpty()) active = false;
	pDlg.Init(&database, active);
	int result = pDlg.DoModal();
	if (result == IDOK)
	{
		database.CalculateParameters();
		main_list.Reload();
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::UpdateStatusBar()
{
	

	if (!statusbar || !ribbonbar) return;

	CString age_lbl = "���";
	int age = static_cast<int>(database.hdata.PatientAge);
	int ostatok = age - (age / 10) * 10;

	if(age>10 && age<20) age_lbl = "���";
	else if(ostatok==1) age_lbl = "���";
	else if(ostatok>=2 && ostatok <= 4) age_lbl = "����";


	
	CString st = "�������: " + CString(database.hdata.PatientName);
	st += " (" + CString(to_string(age).c_str()) +" "+ age_lbl + ")";
	st += " | ���� ������������: " + CString(database.datetime[0].getDateStringNormal().c_str()); 

	statusbar->GetElement(0)->SetText(st);
	CRect sbrect = statusbar->GetElement(0)->GetRect();
	sbrect.right = st.GetLength() * 12 * (double)DPIX();
	statusbar->GetElement(0)->SetRect(sbrect);

	statusbar->RedrawWindow();
	ribbonbar->UpdateWindow();
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnStatusBarTimeUpdate(CCmdUI *pCmdUI)
{
	/*CString st;
	if (database.getCount() == 0)
	{
		st = "������������ �� �����������";
		return;
	}

	MTime temp = database.datetime[database.getCount() - 1].getTime() - database.datetime[0].getTime();
	st = "����� ������������: ";
	st.Append(temp.getString().c_str());

	pCmdUI->SetText(st);
	*/
}
//----------------------------------------------------------------------------------------------

void CChildView::OnPlotConfigButton()
{
	main_plot.OnPlotConfigButton();
}


void CChildView::OnEnlargePlotButton()
{
	
}


void CChildView::OnClearPlot()
{
	main_plot.OnClearPlot();
}

void CChildView::OnDefaultPlotButton()
{
	main_plot.OnDefaultPlotButton();
}


void CChildView::OnEnlargePlotShowDialogButton()
{
	main_plot.OnEnlargePlotButton();
}


void CChildView::OnStartTestButton()
{
	if (database.hdata.StartTest <= 0)
	{
		
		PowerStepDialog dlg;
		dlg.DoModal();
		database.hdata.PowerStep = (int8_t)dlg.getValue();

		database.hdata.StartTest = database.getCount() - 1;
		OnSetExperienceMode();
		CurValuesVisible = true;
		curValues.ShowWindow(SW_SHOW);
		timerWindow.ShowWindow(SW_SHOW);
		timerWindow.StartTest();
		main_list.RedrawWindow();
		OnSize(0, 0, 0);
	}
	else
	{
		database.hdata.EndTest = database.getCount() - 1;
		timerWindow.StopTest();
	}
	ribbonbar->RecalcLayout();
	ribbonbar->RedrawWindow();

}


void CChildView::OnEndTestButton()
{
	
}


void CChildView::OnUpdateRecordButton(CCmdUI* pCmdUI)
{
	 
	if (Recording)
	{
		
		pCmdUI->SetText("��������� ������������");
		if(database.hdata.StartTest>0)//���� ���������� ����
		{
			MTime curtime;
			curtime.CurrentTime();
			if(database.hdata.EndTest <= 0)// ���� ������ ����� �� ����������, ������ ����������
				pCmdUI->Enable(false);
			else if(curtime < (database.datetime[database.hdata.EndTest].getTime() + 120))//���� ������ ����� ���������, �� ����� ��������� 2 ������
				pCmdUI->Enable(false);
			else//���� ������ 120 ��� ����� ���������� ������ �����
				pCmdUI->Enable(true);
			
		}
		else // ���� ���� �� ����������
			pCmdUI->Enable(true);
			
	}
	else if (database.getCount() > 0)//���� ��� �������� (recording false � ���������� ������� ������ 0)
	{
		pCmdUI->SetText("���������");
		pCmdUI->Enable(false);
	}
	else //������������ �� �����������
	{
		
		pCmdUI->SetText("������    ������������");
		pCmdUI->Enable(!database.getCount());
	}
	

}

void CChildView::OnUpdateStartTestButton(CCmdUI* pCmdUI)
{
	bool status = false;

	if (Recording && database.getCount() > 0)
	{
		if ((MTime(true) - database.datetime[0].getTime()) > MTime(60))
			status = true;
	}
	
	
	if (database.hdata.StartTest <= 0) 
		pCmdUI->SetText("��������� ����");
	else if (database.hdata.EndTest <= 0)
		pCmdUI->SetText("��������� ����");
	else
	{
		pCmdUI->SetText("��������");
		status = false;
	}
	pCmdUI->Enable(status);
}

void CChildView::OnUpdateEndTestButton(CCmdUI* pCmdUI)
{
	/*bool result = false;
	if (Recording && database.hdata.StartTest != 0 && database.hdata.EndTest == 0) 
		result = true;

	pCmdUI->Enable(result);*/
	
}

void CChildView::OnSetExperienceMode()
{
	main_plot.SetExperienceMode();
}


void CChildView::OnListPlotConfigButtonClick()
{
	
	PlotCustomization pltDlg;
	
	int result = pltDlg.DoModal(&database, &plot, false);
	if (result == IDOK)
	{
		main_plot.WritePlotToConfig(plot);
	}
	listplot.RedrawWindow();
	//listplot.RedrawWindow();
}


void CChildView::OnUpdateExperienceButton(CCmdUI *pCmdUI)
{
	bool status = main_plot.GetExperienceStatus();
	if(status)
		pCmdUI->SetText("����. ����� ������������ �����");
	else 
		pCmdUI->SetText("���. ����� ������������  �����");
	if (database.hdata.StartTest > 0 && database.hdata.EndTest > 0)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CChildView::OnEnlargeButton()
{
	main_plot.EnlargePlot();
}


void CChildView::OnUpdateEnlargeButton(CCmdUI *pCmdUI)
{
	if (main_plot.IsLarge())
		pCmdUI->SetText("��������� ������");
	else
		pCmdUI->SetText("��������� ������");
}


void CChildView::OnCustomNutritionButtonClick()
{
	if (!ProtolsAvailable()) return;
	NutritionDialog nDlg;
	nDlg.database = &database;
	nDlg.DoModal();
}


void CChildView::OnProtocolMpk()
{

	if (database.getCount() == 0) return;

	if (database.hdata.StartTest <= 0 || database.hdata.EndTest <= 0)
	{
		MessageBoxA("������ �������� �������� ������ ��� ������������ � ����������� ������", "��������", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	if (database.hdata.PowerStep > (int8_t)60 || database.hdata.PowerStep < (int8_t)20)
	{

		PowerStepDialog dlg;
		dlg.DoModal();
		database.hdata.PowerStep = (int8_t)dlg.getValue();
	}

	BigPlotDialog bpDlg;
	Plot plot;
	plot.SetExperience(true);

	string code = "������ (���������� = \"20 ���\", y = ��������������_����������_O2, �������=\"����.���������� �� O2\", y = ���,  y = �����������_O2_��_��_���, �������=\"����������� O2 (��/��/���)\",)";
	plot.Run(&database, code);

	if (bpDlg.DoModal(&plot, &database, true) == IDOK && plot.IsExperience())
	{
		database.hdata.AeT = bpDlg.getBigPlot().getValue(0);
		database.hdata.AT = bpDlg.getBigPlot().getValue(1);
		database.hdata.MCO = bpDlg.getBigPlot().getValue(2);
		main_plot.UpdatePlots();	
	}
}


void CChildView::OnStepChanged()
{
	// TODO: �������� ���� ��� ����������� ������

	//database.hdata.PowerStep;
}


void CChildView::OnUpdatePowerStep(CCmdUI *pCmdUI)
{
	
	// TODO: �������� ���� ��� ����������� �� ���������� ������
	//database.hdata.PowerStep;
}

void CChildView::OnBigPlotClick(int index)
{
	
	
}

void CChildView::OnVolumeFilterButton()
{
	for (int i = 0; i < database.getCount(); ++i)
	{
		double& Vinsp = database.variables.at("V�����")[i];
		double& Vexp = database.variables.at("V������")[i];
		double result=0;
		if (Vinsp != 0)
			result = (Vinsp - Vexp) / Vinsp * 100.;
		if (result < 0) result = -result;
		if (result > 5 && database.checked[i]) database.checked[i] = false;
	}
	main_list.Reload();
	listplot.Update();
	curValues.RedrawWindow();
	main_plot.UpdatePlots();
}//������� ��� ����� - ������
