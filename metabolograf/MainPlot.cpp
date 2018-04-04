#include "stdafx.h"
#include "MainPlot.h"
#include "resource.h"


MainPlot::MainPlot()
{
	selectedPlot = -1;
	LargeMode = false;
	//backbuffer = nullptr;
	
	/*defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"����������� O2 � ��������� CO2\", y = �����������_O2_��_��_��� & ���������_CO2_��_��_���, �������=\"����������� O2\" & \"��������� CO2 (��/��/���)\")");
	defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"�������������� �����������\", y = ��������������_����������_O2 & ��������������_����������_CO2, �������=\"����.���������� �� O2\" & \"����.���������� �� CO2\")");
	defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"����������� �����������\", y = �����������_�����������)");
	defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"�������� ���������� � ����������� O2\", x = �����������_O2_��_��_��� & ���������_CO2_��_��_���, ������� = \"����������� O2\" & \"��������� CO2\", y = ��������_����������, ������� = \"�������� ����������\"); ");
	defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"����������� O2 � ��������� CO2\", x = ���������_CO2_��_��_���, ������� = \"����������� O2 vs ��������� CO2\", y = �����������_O2_��_��_���, ������� = \"����������� O2\"); ");
	defaultVal.push_back("������ (���������� = \"30 �\", ��������� = \"�������� ����������\", y = ��������_����������, ������� = \"�������� ����������\");");
	*/
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"�������� ����������\", y = ��������_����������, ������� = \"�������� ����������\", � = ���)");
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"����������� O2 � ��������� CO2\", y = �����������_O2_��_��_��� & ���������_CO2_��_��_���, �������=\"����������� O2\" & \"��������� CO2 (��/��/���)\")");
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"�������� ���������� �� ��2 � �CO2\", x = �����������_O2_��_��_��� & ���������_CO2_��_��_���, ������� = \"����������� O2\" & \"��������� CO2\", y = ��������_����������, ������� = \"�������� ����������\"); ");
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"�������������� �����������\", y = ��������������_����������_O2 & ��������������_����������_CO2, �������=\"����.���������� �� O2\" & \"����.���������� �� CO2\")");
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"����������� �����������\", y = �����������_�����������)");
	defaultVal.push_back("������ (���������� = \"15 ���\", ��������� = \"FetO2 � FetCO2\", y = FetO2, y = FetCO2); ");
	
}


MainPlot::~MainPlot()
{
	Clear();
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(MainPlot, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	
END_MESSAGE_MAP()

void MainPlot::Init()
{
	database = Database::getInstance();
	SetBounds();
	
	LoadConfig();
}
//------------------------------------------------------


void MainPlot::SetBounds()
{
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();
}
void MainPlot::OnSize(UINT nType, int cx, int cy)
{
	UpdatePlots(false);
}
void MainPlot::OnLButtonUp(UINT flags, CPoint point)
{
	if (plots.size() == 0) return;

	
	if (!LargeMode)
	{
		int x = point.x;
		int y = point.y;


		CRect rc;
		string str = ToString(x) + " " + ToString(y);
		GetClientRect(&rc);
		if (x >= 0 && x <= rc.Width() && y >= 0 && y <= rc.Height())
		{
			selectedPlot = getIndex(x, y);
			RedrawWindow();

		}
	}

	ShowContextMenu();
}

void MainPlot::OnLButtonDblClk(UINT flags, CPoint point)
{
	EnlargePlot();
}

void MainPlot::ShowCustomizationDialog()
{
	
	if (selectedPlot < 0 && selectedPlot >= plots.size()) return;
	PlotCustomization pltDlg;
	int result = pltDlg.DoModal(plots[selectedPlot]);
	if (result == IDOK)
	{
		WriteConfig();
	}
	RedrawWindow();
}


void MainPlot::ShowPlotWindow()
{
	if (database->getCount() == 0) return;

	BigPlotDialog bpDlg;
	Plot plot;
	plot.SetExperience(plots[selectedPlot]->IsExperience());
	plot.Run(database, plots[selectedPlot]->getPlotCode());

	if (bpDlg.DoModal(&plot, false) == IDOK && plot.IsExperience())
	{
		database->getHeader().AeT = bpDlg.getBigPlot().getValue(0);
		database->getHeader().AT = bpDlg.getBigPlot().getValue(1);
		database->getHeader().MCO = bpDlg.getBigPlot().getValue(2);
		for (auto& plot : plots)
		{
			plot->SetMarkPosByTableIndex(database, 0, database->getHeader().AeT);
			plot->SetMarkPosByTableIndex(database, 1, database->getHeader().AT);
			plot->SetMarkPosByTableIndex(database, 2, database->getHeader().MCO);
			plot->DrawPlot();
		}
		RedrawWindow();
	}

}

//------------------------------------------------------
void MainPlot::Clear()
{
	selectedPlot = -1;
	for (auto plot : plots)
		delete plot;
	plots.clear();
}
//------------------------------------------------------
bool MainPlot::AddPlot(string plot_string, int width, int height)
{
	
	Plot* plot = new Plot(width, height);
	try
	{
		plot->Run(database, plot_string);
		plot->DrawPlot();
	}
	catch (...)
	{

	}
	plots.push_back(plot);
	return true;
}
//------------------------------------------------------
void MainPlot::OnPaint()
{

	CWnd::OnPaint();

	UGC ugc(GetDC(), Width, Height);
	
	int count = plots.size();
	if (count == 0) return;
	int cy = 2;
	int cx = count / cy;
	
	if (LargeMode)
	{
		ugc.DrawBitmap(plots[selectedPlot]->getBitmap(), 0, 0, Width, Height);
	}
	else
	{
		for (int y = 0; y < cy; y++)
			for (int x = 0; x < cx; x++)
			{
				int index = y*cx + x;
				int w = plots[index]->getWidth();
				int h = plots[index]->getHeight();
				ugc.DrawBitmap(plots[index]->getBitmap(), x*w, y*h, w, h);
				
				if (selectedPlot == index)
				{
					ugc.SetDrawColor(255, 0, 0);
					ugc.DrawRectangle(x*w, y*h, w - 2, h - 2, 2);
				}
			}
	}
	
	
	
	
}
//------------------------------------------------------
int MainPlot::getGridWidth()
{
	return 3;
	int count = 1;
	int size = plots.size();
	if (size == 1) count = 1;
	else if (size>0)
	{
		count = sqrt(size);
		if (count*count != size) count++;
	}
	return count;
}

int MainPlot::getMinWidth(CRect& rect, int count)
{
	int w = rect.Width() / count;
	int h = rect.Height() / count;
	if (w > h) return h;
	else if (h > w) return w;

}

int MainPlot::getIndex(int x, int y)
{
	
	CRect rect;
	GetClientRect(&rect);

	int cx = 3;
	int cy = 2;
	int _x = x / (rect.Width()/cx);
	int _y = y / (rect.Height()/cy);
	if (_x >= cx) _x = cx - 1;
	if (_y >= cy) _y = cy - 1;
	return _y*cx + _x;
}
void MainPlot::ShowContextMenu()
{
	if (database->getCount() == 0) return;
	if (selectedPlot < 0) return;
	
	ribbonbar->ShowContextCategories(ID_PLOT_CONTEXT);
	ribbonbar->ActivateContextCategory(ID_PLOT_CONTEXT);
}
void MainPlot::UpdatePlots(bool reload)
{

	SetBounds();

	if (LargeMode)
	{
		Plot* plot = plots[selectedPlot];
		if(reload)
			plot->Reload(database);
		plot->Resize(Width, Height);
		plot->DrawPlot();
	}
	else
	{
		int w = Width / 3;
		int h = Height / 2;

		for (auto plot : plots)
		{
			if (reload)
				plot->Reload(database);
			plot->Resize(w, h);
			plot->DrawPlot();
		}
	}

	
	RedrawWindow();
}
//-------------------------------------------------------------------------------------------
void MainPlot::WritePlotToConfig(const Plot& plot)// PLOT_LIST on TABLE screen
{
	
	Ini ini("metabolograf_config.ini");
	ini.Write("Plots", "plot_list", plot.getPlotCode());
}

void MainPlot::LoadPlotFromConfig(Plot& plot) //PLOT_LIST on TABLE screen
{
	Ini ini("metabolograf_config.ini");

	int w = Width / 3;
	int h = Height / 2;
	string out = ini.Read("Plots", "plot_list", "plot(����������=\"1 ���\",y=\"�����������_�����������\")");
	try {
		plot.Run(database, out);
	}
	catch (...) {}

}

void MainPlot::WriteConfig()
{
	Ini ini("metabolograf_config.ini");

	for (int i = 0; i<plots.size(); i++)
	{
		string param = string("plot_") + to_string(i);
		ini.Write("Plots", param.c_str(), plots[i]->getPlotCode());
	}
}
//-------------------------------------------------------------------------------------------
void MainPlot::LoadConfig()
{
	Ini ini("metabolograf_config.ini");
	
	int w = Width / 3;
	int h = Height / 2;
	for (int i = 0; i<6; i++)
	{
		string param = string("plot_") + to_string(i);
		string out = ini.Read("Plots", param.c_str(), defaultVal[i]);
		AddPlot(out, w, h);

	}
	
}
//-------------------------------------------------------------------------------------------

void MainPlot::OnPlotConfigButton()
{
	if (selectedPlot < 0) return;
	ShowCustomizationDialog();
}


void MainPlot::OnEnlargePlotButton()
{
	if (selectedPlot < 0) return;
	ShowPlotWindow();
}


void MainPlot::OnClearPlot()
{
	if (selectedPlot < 0) return;
	plots[selectedPlot]->Clear();
	RedrawWindow();
	WriteConfig();
}


void MainPlot::OnDefaultPlotButton()
{
	plots[selectedPlot]->Run(database, defaultVal[selectedPlot]);
	plots[selectedPlot]->DrawPlot();
	RedrawWindow();
	WriteConfig();
}

void MainPlot::SetExperienceMode()
{
	for (auto& plot : plots)
	{
		plot->SetExperience(!plot->IsExperience());
		plot->Reload(database);
		plot->DrawPlot();
	}
	RedrawWindow();
}


bool MainPlot::GetExperienceStatus()
{
	if (plots.size() == 0) return false;
	if (plots[0] == nullptr) return false;
	
	return plots[0]->IsExperience();
}


void MainPlot::EnlargePlot()
{
	LargeMode = !LargeMode;
	UpdatePlots();
}
