
// ChildView.cpp : реализация класса CChildView
//
#pragma warning disable 4996

#include "stdafx.h"
#include "metabolograf.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


CChildView::CChildView()
	: curValues(metab)
{
	database = Database::getInstance();
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

	//обновления галочек риббона
	ON_UPDATE_COMMAND_UI(ID_UseExpiratoryVolume, OnUpdateUseExpiratoryVolume)
	ON_UPDATE_COMMAND_UI(ID_UseInspiratoryVolume, OnUpdateUseInspiratoryVolume)
	ON_UPDATE_COMMAND_UI(ID_UseMeanVolume, OnUpdateUseMeanVolume)

	ON_UPDATE_COMMAND_UI(ID_Filter_TVolume, OnUpdateFilterTVolume)
	ON_UPDATE_COMMAND_UI(ID_Filter_BrKoef, OnUpdateFilterBrKoef)
	
	

	//обновление кнопка запись - активна или неактивна
	ON_UPDATE_COMMAND_UI(ID_RECORD, OnUpdateRecordButton)
	ON_UPDATE_COMMAND_UI(ID_START_TEST_BUTTON, OnUpdateStartTestButton)
	ON_UPDATE_COMMAND_UI(ID_END_TEST_BUTTON, OnUpdateEndTestButton)

	//кнопки, которые выключаются при записи
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
		database->getHeader().PatientAge = ydata.PatientAge;
		database->getHeader().PatientSex = ydata.PatientSex;
		database->getHeader().PatientWeight = ydata.PatientWeight;
		database->getHeader().PatientHeight = ydata.PatientHeight;
		sprintf_s(database->getHeader().PatientName, "%s", ydata.FIO);
		for (int i = strlen(database->getHeader().PatientName) - 1; i >= 0; i--)
		{
			if (database->getHeader().PatientName[i] == ' ') database->getHeader().PatientName[i] = '\0';
			else break;
		}
		sprintf_s(database->getHeader().AdditionalInformation, "%s", ydata.Info);
	}

	playground.UpdateVariablesList();

}
//----------------------------------------------------------------------------------------------
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0, 0, 300, 300);

	//m_Tab.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | LBS_NOTIFY, rect, this, IDC_TABCTRL);
	m_Tab.Create(WS_CHILD | WS_VISIBLE | TCS_BOTTOM, rect, this, IDC_TABCTRL);//TCS_BOTTOM

	CFont* pFont = new CFont;
	VERIFY(pFont->CreateFont(
		DPIX()(14),                // nHeight
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

	m_Tab.InsertItem(0, " Таблица ");
	m_Tab.InsertItem(1, " Графики ");
	//m_Tab.InsertItem(2, " Скрипт ");//убрал скрипт пока

	main_list.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | LBS_NOTIFY, rect, this, IDC_MAINLIST);
	main_list.SetFocus();
	main_list.ModifyStyle(LVS_LIST, LVS_REPORT, 0); //- ставим режим таблицы
	main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	main_list.Init(&main_plot);

	curValues.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, IDC_CURRVALUES);
	curValues.Init();
	curValues.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);

	playground.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_PLAYGROUNDWINDOW);
	playground.Init();
	playground.ShowWindow(SW_HIDE);

	main_plot.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_MAINPLOTWINDOW);
	main_plot.Init();
	main_plot.ribbonbar = ribbonbar;
	main_plot.ShowWindow(SW_HIDE);

	timerWindow.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, ID_TIMERWINDOW);
	timerWindow.ShowWindow((CurValuesVisible) ? SW_SHOW : SW_HIDE);

	listplot.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), this, IDC_LISTPLOT);
	main_plot.LoadPlotFromConfig(plot);
	plot.EnlargeUI();
	//m_pPlot.SetBackgroundColor(225, 225, 225);
	plot.DrawPlot();
	listplot.Init(&plot, &main_list, &curValues);
	return 0;
}
//----------------------------------------------------------------------------------------------
bool CChildView::OnClose()
{
	main_list.WriteConfig();
	playground.SaveWorkScript();

	if (Recording)
	{
		int result = MessageBox("Проводится исследование. Выход из программы приведет к потере данных. Хотите закрыть программу?", "Внимание", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES)
		{

			if (ydata.Initialized)
				ydata.g_pdata->action = 0;
			if (metab.Connect())
				metab.Disconnect();
			return true;
		}
		else return false;
	}

	
	if (ydata.Initialized && database->getCount()>0)
	{
		//if (filename_from_commandline.IsEmpty())
		//filename_from_commandline = "rec.mbf";
		int result = MessageBox("Сохранить изменения в базе данных?", "Подтверждение", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && database->getCount()>0)
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
	else if(database->getCount()>0)
	{
		int result = MessageBox("Сохранить изменения в файле \"" + filename + "\"?", "Подтверждение", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES && database->getCount()>0)
		{
			SaveFile(filename);
		}
	}
	if (metab.Connect())
		metab.Disconnect();
	return true;
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
	
	//Tab
	::SetWindowPos(GetDlgItem(IDC_TABCTRL)->m_hWnd, HWND_TOP,
		0, 0, rect.right,
		rect.bottom, NULL);

	int curWidth = static_cast<int>((CurValuesVisible) ? DPIX()(240) : 0);
	int timerHeight = DPIX()(100);
	//
	::SetWindowPos(GetDlgItem(IDC_CURRVALUES)->m_hWnd, HWND_TOP,
		0,
		0,
		DPIX()(240),
		rect.bottom - DPIX()(20) - timerHeight,
		NULL);



	::SetWindowPos(GetDlgItem(ID_TIMERWINDOW)->m_hWnd, HWND_TOP,
		0,
		rect.bottom - timerHeight - DPIX()(20),
		DPIX()(240),
		timerHeight,
		NULL);

	// Таблица
	::SetWindowPos(GetDlgItem(IDC_MAINLIST)->m_hWnd, HWND_TOP,
		curWidth, 0,
		rect.right - curWidth,
		rect.bottom - DPIX()(220),
		NULL);

	//график под таблицей
	::SetWindowPos(GetDlgItem(IDC_LISTPLOT)->m_hWnd, HWND_TOP,
		curWidth, 
		rect.bottom - DPIX()(220),
		rect.right - curWidth,
		DPIX()(200),
		NULL);

	//графики
	//...
	::SetWindowPos(GetDlgItem(ID_MAINPLOTWINDOW)->m_hWnd, HWND_TOP,
		curWidth, 0,
		rect.right - curWidth,
		rect.bottom - DPIX()(20),
		NULL);

	::SetWindowPos(GetDlgItem(ID_PLAYGROUNDWINDOW)->m_hWnd, HWND_TOP,
		0, 0,
		rect.right,
		rect.bottom - DPIX()(20),
		NULL);

	curValues.RedrawWindow();
	main_plot.RedrawWindow();
	main_list.RedrawWindow();
	timerWindow.RedrawWindow();

}
//----------------------------------------------------------------------------------------------
// обработка переключения Tab
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
	case 0://таблица
		main_list.ShowWindow(SW_SHOW);
		listplot.ShowWindow(SW_SHOW);
		if (CurValuesVisible)
		{
			curValues.ShowWindow(SW_SHOW);
			timerWindow.ShowWindow(SW_SHOW);
		}
		break;
	case 1://графики

		main_plot.UpdatePlots();
		main_plot.ShowWindow(SW_SHOW);
		if (CurValuesVisible)
		{
			curValues.ShowWindow(SW_SHOW);
			timerWindow.ShowWindow(SW_SHOW);
		}
		main_plot.ShowContextMenu();
		break;
	case 2://лаборатория
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
		if (i >= 0 && i < static_cast<int>(database->getCount()))
		{
			bool temp = main_list.GetCheck(i);
			auto vec = main_list.GetSelectedItems();
			// проверяем, нажата галка в выделенном диапазоне или нет.
			auto it = find(vec.begin(), vec.end(), i);
			if (it == vec.end())
			{
				vec.clear();
				vec.push_back(i);
			}
			//если внутри выделенного диапазона, тогда изменения коснутя всех, что внутри диапазона

			if (database->getChecked(i) != temp)
			{
				//getSelectedItems
				
				for (int j : vec)
				{
					database->setChecked(j, temp);
					main_list.SetCheck(j, temp);
				}
				listplot.UpdateWithoutDraw();
				listplot.SetTimeCursor((vec.size()>0)?vec.at(0):0);
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
	CPaintDC dc(this); // контекст устройства для рисования
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OpenFile(CString fname)
{
	if (UnavailableMessage()) return;

	filename = fname;
	if (!database->OpenFile(fname.GetBuffer()))
	{
		MessageBox("Ошибка открытия файла \""+fname + "\"", "Ошибка", MB_OK | MB_ICONERROR);
		return;
	}
		
	playground.UpdateVariablesList();
	OnRunScript();//LoadToList();

	UpdateStatusBar();

	RecordMode = false;
	
	if (m_Tab.GetCurSel() == 1)
		main_plot.UpdatePlots();	
	listplot.Update();
	if (CurValuesVisible)// если открыто панель Выбранных значений
	{
		curValues.SetSelected(-1);//обновляем последнее полученной

	}

	timerWindow.Update();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateAllRibbonElements(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!Recording);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseExpiratoryVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database->getHeader().UseExpiratoryVolume);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseInspiratoryVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database->getHeader().UseInspiratoryVolume);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateUseMeanVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database->getHeader().UseMeanVolume);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateFilterTVolume(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database->getHeader().Filter_TVolume);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateFilterBrKoef(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(database->getHeader().Filter_BrKoef);

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUpdateCurValuesVisible(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CurValuesVisible);
}
//----------------------------------------------------------------------------------------------
void CChildView::OnOpenFile()
{
	CFileDialog fileDialog(TRUE, "Файл метаболографа", "*.mbf");	//объект класса выбора файла
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	if (result == IDOK)	//если файл выбран
	{
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();
		OpenFile(str);
	}
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnSaveFile()
{
	if (database->getCount() == 0)
	{
		MessageBox("Нет данных для сохранения", "Внимание", MB_OK | MB_ICONINFORMATION);
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
	if (database->getCount() == 0)
	{
		MessageBox("Нет данных для сохранения", "Внимание", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CFileDialog fileDialog(FALSE, "Файл метаболографа", "*.mbf");	//объект класса выбора файла
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	if (result == IDOK)	//если файл выбран
	{
		CString str = fileDialog.GetFolderPath() + "\\" + fileDialog.GetFileName();
		SaveFile(str);
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::SaveFile(CString fname)
{
	if (UnavailableMessage()) return;

	for (int i = 0; i < static_cast<int>(database->getCount()); i++)
	{
		database->setChecked(i, main_list.GetCheck(i));
	}
	if (database->SaveFile(fname.GetBuffer(fname.GetLength())))
		filename = fname;
	else
		MessageBox("Ошибка сохранения файла", "Ошибка", MB_OK|MB_ICONERROR);	
}
//----------------------------------------------------------------------------------------------
void CChildView::LoadToList()
{
	for (int i = 0; i < static_cast<int>(database->getCount()); i++)
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

	if (!metab.IsNewDataRecieved() || metab.RespRate == 0 || metab.RespRate > 100)
	{
		curValues.RedrawWindow();
		return false;//если новые данные не получили, то пропускаем
	}
	
	metab.NewDataReaded();//если данные получили

	time_t t = time(NULL);
	tm *tmp = localtime(&t);
	if (Circle == 0)
	{
		database->getHeader().Year = tmp->tm_year + 1900;
		database->getHeader().Month = tmp->tm_mon + 1;
		database->getHeader().Day = tmp->tm_mday;
	}

	database->Append(Datetime(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec));
	database->Append("Vвдоха", metab.Vol_insp);
	database->Append("Vвыдоха", metab.Vol_exsp);
	database->Append("FiO2", metab.FiO2);
	database->Append("FetO2", metab.FetO2);
	database->Append("FiCO2", metab.FiCO2 / 760. * 100.);
	database->Append("FetCO2", metab.FetCO2 / 760. * 100.);
	database->Append("ЧД", metab.RespRate);

	
	database->Append("ЧД_old", metab.RespRateOld);
	//в релизе это нужно удалить


	if (metab.HR >= 0)
	{
		if (!database->getHeader().HR)
		{
			database->getHeader().HR = true;
			database->AddVariable("ЧСС");
			database->AddVariable("RR");
			database->AddVariable("SD");
		}
		database->Append("ЧСС", metab.HR);

	}
	database->Append(true);

	//Filter(Circle);//Filter of the data
	database->CalculateParameters(Circle);//пересчитываем все расчетные параметры
	Circle++;
	database->getHeader().count = Circle;//обновляем размер массива
	
	playground.OnRunScript(true);//выполняем скрипт, там же обновляется лист переменных
	main_plot.UpdatePlots();//обновляем графики
	listplot.Update();
	if(CurValuesVisible)// если открыто панель Выбранных значений
		curValues.SetSelected(Circle - 1);//обновляем последнее полученной
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
		main_list.WriteConfig();
		PatientDialog pDlg;
		pDlg.Init(true);
		int result = pDlg.DoModal();
		if (result == IDCANCEL)
			return;
		database->AddPatientParametersToVariables();
		
		if (!metab.Connect()) return;
		timer = SetTimer((int)1, 1000, NULL);
		Recording = true;

		//эта переменная "ЧД_old" появляетя только при записи нового файла
		/*if (Circle == 0)
		{
			m_pDatabase->insertVariableName("ЧД_old");
			//m_pDatabase.variable_names.push_back("ЧД_old");
			main_list.InsertParameterAfter("ЧД_old", "ЧД");
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

	int index = database->getCount() - 1;
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
	// TODO: добавьте свой код обработчика команд
}
//----------------------------------------------------------------------------------------------
void CChildView::OnNewScript()
{
	playground.OnNewScript();
	// TODO: добавьте свой код обработчика команд
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
	playground.SaveHTML();// TODO: добавьте свой код обработчика команд
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocol1()
{
	if (!ProtolsAvailable()) return;
	Export *fitnes_export = new Export(database);
	fitnes_export->DietaExport();
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
bool CChildView::ProtolsAvailable()
{
	bool checked = false;
	for (int i = 0; i<database->getHeader().count; i++)
	{
		if (database->getChecked(i) == true)
		{
			checked = true;
			break;
		}
	}

	if (database->getCount() <= 0 || !checked)
	{
		MessageBox("Недостаточно данных для формирования протокола", "Внимание", MB_OK);
		return false;
	}
	if (UnavailableMessage())
		return false;
	/*if (m_pDatabase.hdata.PatientWrist <= 0)
	{
		MessageBox(L"Для формирования протокола необходимо ввести окружность запястия", L"Внимание", MB_OK);
		PatientForm->ShowModal();
		if (calculator->m_pDatabase.hdata.PatientWrist <= 0)
		{
			Application->MessageBox(L"Вы не ввели окружность запястия. Протокол формироваться не будет", L"Внимание", MB_OK);
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

	/*if (m_pDatabase.hdata.PatientWrist<2)
	{
		MessageBox("Для получения протокола необходимо указать окружность запястья", L"Внимание", MB_OK);
		PatientForm->SetEditable(true, true);
		PatientForm->ShowModal();

		if (calculator->m_pDatabase.hdata.PatientWrist<2) return;
	}*/

	Export *fitnes_export = new Export(database);
	fitnes_export->StandartExport(false);
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocol4()
{
	if (!ProtolsAvailable()) return;
	Export *fitnes_export = new Export(database);
	fitnes_export->StandartExport(true);
	delete  fitnes_export;
	fitnes_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocolExcel()
{
	if (!ProtolsAvailable()) return;
	Export *xls_export = new Export(database);
	xls_export->ExportExcel(main_list.getShowParameters());
	delete  xls_export;
	xls_export = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::OnProtocolData()
{
	if (!ProtolsAvailable()) return;
	Export *fexport = new Export(database);
	fexport->ExportDataString();
	delete  fexport;
	fexport = NULL;
}
//----------------------------------------------------------------------------------------------
void CChildView::ChangeFilterStatus()
{
	for (size_t i = 0; i<database->getCount(); i++)
	{
		database->Filter(i);
		main_list.SetCheck(i, database->getChecked(i));
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseExpiratoryVolumeClick()
{
	database->getHeader().UseExpiratoryVolume = !database->getHeader().UseExpiratoryVolume;
	database->getHeader().UseInspiratoryVolume = false;
	database->getHeader().UseMeanVolume = false;
	if (database->getCount() == 0) return;

	database->CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseInspiratoryVolumeClick()
{
	database->getHeader().UseExpiratoryVolume = false;
	database->getHeader().UseInspiratoryVolume = !database->getHeader().UseInspiratoryVolume;
	database->getHeader().UseMeanVolume = false;

	if (database->getCount() == 0) return;

	database->CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();

}
//----------------------------------------------------------------------------------------------
void CChildView::OnUseMeanVolumeClick()
{
	database->getHeader().UseExpiratoryVolume = false;
	database->getHeader().UseInspiratoryVolume = false;
	database->getHeader().UseMeanVolume = !database->getHeader().UseMeanVolume;

	if (database->getCount() == 0) return;

	database->CalculateParameters();
	main_list.Reload();
	listplot.Update();
	main_plot.UpdatePlots();

}
//----------------------------------------------------------------------------------------------


void CChildView::OnFilterTVolumeClick()
{
	database->getHeader().Filter_TVolume = !database->getHeader().Filter_TVolume;
	if (database->getCount() == 0) return;
	ChangeFilterStatus();
	listplot.Update();
	main_plot.UpdatePlots();
}
//----------------------------------------------------------------------------------------------
void CChildView::OnFilterBrkoefClick()
{
	database->getHeader().Filter_BrKoef = !database->getHeader().Filter_BrKoef;
	if (database->getCount() == 0) return;
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
	pDlg.Init(active);
	int result = pDlg.DoModal();
	if (result == IDOK)
	{
		database->CalculateParameters();
		main_list.Reload();
	}
}
//----------------------------------------------------------------------------------------------
void CChildView::UpdateStatusBar()
{
	

	if (!statusbar || !ribbonbar) return;

	CString age_lbl = "лет";
	int age = static_cast<int>(database->getHeader().PatientAge);
	int ostatok = age - (age / 10) * 10;

	if(age>10 && age<20) age_lbl = "лет";
	else if(ostatok==1) age_lbl = "год";
	else if(ostatok>=2 && ostatok <= 4) age_lbl = "года";


	
	CString st = "Пациент: " + CString(database->getHeader().PatientName);
	st += " (" + CString(to_string(age).c_str()) +" "+ age_lbl + ")";
	st += " | Дата исследования: " + CString(database->getDatetime(0).getDateStringNormal().c_str());
	st += " | Время исследования: " + CString(database->getDatetime(0).getTime().getString().c_str()) + "-" + CString(database->getDatetime(database->getCount() - 1).getTime().getString().c_str());



	statusbar->GetElement(0)->SetText(st);
	CRect sbrect = statusbar->GetElement(0)->GetRect();
	sbrect.right = st.GetLength() * DPIX()(12);
	statusbar->GetElement(0)->SetRect(sbrect);

	statusbar->RedrawWindow();
	ribbonbar->UpdateWindow();
	
}
//----------------------------------------------------------------------------------------------
void CChildView::OnStatusBarTimeUpdate(CCmdUI *pCmdUI)
{
	/*CString st;
	if (m_pDatabase.getCount() == 0)
	{
		st = "Исследование не проводилось";
		return;
	}

	MTime temp = m_pDatabase.datetime[m_pDatabase.getCount() - 1].getTime() - m_pDatabase.datetime[0].getTime();
	st = "Время исследования: ";
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
	if (database->getHeader().StartTest <= 0)
	{
		
		PowerStepDialog dlg;
		dlg.DoModal();
		database->getHeader().PowerStep = (int8_t)dlg.getValue();

		database->getHeader().StartTest = database->getCount() - 1;
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
		database->getHeader().EndTest = database->getCount() - 1;
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
		pCmdUI->SetText("Завершить исследование");
		if(database->getHeader().StartTest>0)//если проводится опыт
		{
			MTime curtime;
			curtime.CurrentTime();
			if(database->getHeader().EndTest <= 0)// если вторая метка не выставлена, кнопка недоступна
				pCmdUI->Enable(false);
			else if(curtime < (database->getDatetime(database->getHeader().EndTest).getTime() + 120))//если вторую метку поставили, но нужно подождать 2 минуты
				pCmdUI->Enable(false);
			else//если прошло 120 сек после постановки второй метки
				pCmdUI->Enable(true);
			
		}
		else // если опыт не проводится
			pCmdUI->Enable(true);
			
	}
	else if (database->getCount() > 0)//опыт уже проведет (recording false и количество записей больше 0)
	{
		pCmdUI->SetText("Завершено");
		pCmdUI->Enable(false);
	}
	else //исследование не проводилось
	{
		
		pCmdUI->SetText("Начать    исследование");
		pCmdUI->Enable(!database->getCount());
	}
	

}

void CChildView::OnUpdateStartTestButton(CCmdUI* pCmdUI)
{
	bool status = false;

	if (Recording && database->getCount() > 0)
	{
		if ((MTime(true) - database->getDatetime(0).getTime()) > MTime(60))
			status = true;
	}
	
	
	if (database->getHeader().StartTest <= 0)
		pCmdUI->SetText("Запустить тест");
	else if (database->getHeader().EndTest <= 0)
		pCmdUI->SetText("Завершить тест");
	else
	{
		pCmdUI->SetText("Завершен");
		status = false;
	}
	pCmdUI->Enable(status);
}

void CChildView::OnUpdateEndTestButton(CCmdUI* pCmdUI)
{
	/*bool result = false;
	if (Recording && m_pDatabase.hdata.StartTest != 0 && m_pDatabase.hdata.EndTest == 0) 
		result = true;

	pCmdUI->Enable(result);*/
	
}

void CChildView::OnSetExperienceMode()
{
	main_plot.SetExperienceMode();
	main_list.OverwriteTime();
}


void CChildView::OnListPlotConfigButtonClick()
{
	
	PlotCustomization pltDlg;
	
	int result = pltDlg.DoModal(&plot, false);
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
		pCmdUI->SetText("Выкл. режим нагрузочного теста");
	else 
		pCmdUI->SetText("Вкл. режим нагрузочного  теста");
	if (database->getHeader().StartTest > 0 && database->getHeader().EndTest > 0)
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
		pCmdUI->SetText("Уменьшить график");
	else
		pCmdUI->SetText("Увеличить график");
}


void CChildView::OnCustomNutritionButtonClick()
{
	if (!ProtolsAvailable()) return;
	NutritionDialog nDlg;
	nDlg.DoModal();
}


void CChildView::OnProtocolMpk()
{

	if (database->getCount() == 0) return;

	if (database->getHeader().StartTest <= 0 || database->getHeader().EndTest <= 0)
	{
		MessageBoxA("Данный протокол доступен только для исследования с нагрузочным тестом", "Внимание", MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	if (database->getHeader().PowerStep > (int8_t)60 || database->getHeader().PowerStep < (int8_t)20)
	{

		PowerStepDialog dlg;
		dlg.DoModal();
		database->getHeader().PowerStep = (int8_t)dlg.getValue();
	}

	BigPlotDialog bpDlg;
	Plot plot;
	plot.SetExperience(true);

	string code = "график (осреднение = \"20 сек\", y = Вентиляционный_эквивалент_O2, легенда=\"Вент.эквивалент по O2\",начало=0.000000,конец=60.000000,y = ЧСС,начало=0.000000,конец=200.000000,y = Потребление_O2_мл_кг_мин, легенда=\"Потребление O2 (мл/кг/мин)\",начало=0.000000,конец=60.000000)";
	plot.Run(database, code);

	if (bpDlg.DoModal(&plot, true) == IDOK && plot.IsExperience())
	{
		database->getHeader().AeT = bpDlg.getBigPlot().getValue(0);
		database->getHeader().AT = bpDlg.getBigPlot().getValue(1);
		database->getHeader().MCO = bpDlg.getBigPlot().getValue(2);
		main_plot.UpdatePlots();	
	}
}


void CChildView::OnStepChanged()
{
	// TODO: добавьте свой код обработчика команд

	//m_pDatabase.hdata.PowerStep;
}


void CChildView::OnUpdatePowerStep(CCmdUI *pCmdUI)
{
	
	// TODO: добавьте свой код обработчика ИП обновления команд
	//m_pDatabase.hdata.PowerStep;
}

void CChildView::OnBigPlotClick(int index)
{
	
	
}

void CChildView::OnVolumeFilterButton()
{
	if (database->getCount() == 0) return;

	
	VolumeErrorDialog dlg;
	dlg.Init();
	if (dlg.DoModal() == IDOK)
	{
		//снимаем галки, обновляем все
		dlg.Apply();
		main_list.Reload();
		listplot.Update();
		curValues.RedrawWindow();
		main_plot.UpdatePlots();
		
	}

}//таблица без галок - ошибка
