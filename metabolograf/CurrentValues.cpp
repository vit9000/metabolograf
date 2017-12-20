#include "stdafx.h"
#include "CurrentValues.h"

BEGIN_MESSAGE_MAP(CurrentValues, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CurrentValues::CurrentValues()
{
	selected = -1;
}


CurrentValues::~CurrentValues()
{
	//Clear();
	DestroyWindow();
}

void CurrentValues::Init(Database* _database)
{
	database = _database;
	SetBounds();
	if (database->variable_names.size()==0) return;

	//varnames.push_back("Время исследования");

	
	variable_names.push_back("Vвдоха");
	variable_names.push_back("Vвыдоха");
	variable_names.push_back("FiO2");
	variable_names.push_back("FetO2");
	variable_names.push_back("FiCO2");
	variable_names.push_back("FetCO2");
	variable_names.push_back("ЧД");
	variable_names.push_back("Дыхательный_коэффициент");
	variable_names.push_back("Минутное_потребление_O2");
	variable_names.push_back("Минутное_выделение_CO2");
	variable_names.push_back("Метаболизм_O2");
	variable_names.push_back("Метаболизм_CO2");
	variable_names.push_back("ЧСС");
	

	lables.push_back("Vвдоха (л)");
	lables.push_back("Vвыдоха (л)");
	lables.push_back("FiO2 (%)");
	lables.push_back("FetO2 (%)");
	lables.push_back("FiCO2 (%)");
	lables.push_back("FetCO2 (%)");
	lables.push_back("ЧД");
	lables.push_back("Дыхат. коэффициент");
	lables.push_back("Мин.потр.O2  (л)");
	lables.push_back("Мин.выд.CO2 (л)");
	lables.push_back("Метаб.(O2) (ккал)");
	lables.push_back("Метаб.(CO2) (ккал)");
	lables.push_back("ЧСС");
	
}
//------------------------------------------------------
void CurrentValues::SetSelected(int new_selected)
{
	if (new_selected>=database->getCount()) 
		selected = -1;
	else if (selected == new_selected) 
		return;
	else
		selected = new_selected;
	RedrawWindow();
}
//------------------------------------------------------
double CurrentValues::getDPIX()
{
	HDC hdcScreen = ::GetDC(NULL);
	double iDPI = 96.; // assume failure
	if (hdcScreen) {
		iDPI = ::GetDeviceCaps(hdcScreen, LOGPIXELSX);
		::ReleaseDC(NULL, hdcScreen);
	}
	return iDPI/96.;
}
//------------------------------------------------------
void CurrentValues::OnPaint()
{
	CWnd::OnPaint();

	double dpiX = getDPIX();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());

	ugc.g->Clear(Color(225, 225, 225));


	//ugc.SetDrawColor(100, 100, 100);
	//ugc.FillRectangle(0, 0, Width, 20 * dpiX);

	ugc.SetTextSize(10*dpiX);
	ugc.SetAlign(ugc.CENTER);

	//ugc.SetDrawColor(255, 255, 255);
	//ugc.DrawString("Выбранные значения", Width/2, 0);

	
	//if(selected==-1)
	
	int w = Width ;
	int space = ugc.GetTextHeight();
	int startY = 0;// space * 2;

	//ugc.DrawLine(w / 2, startY, w / 2, variable_names.size() / 2 * space * 3+startY);
	const int border = 2 * dpiX;
	for (int i = 0; i < variable_names.size(); i++)
	{
		string& vname = variable_names[i];
		if (vname=="ЧСС" && database->variables.count(vname) == 0) continue;//если чсс не учитывается и нет такой переменной то рисовать не надо
		
		int yi = i / 2;
		yi *= (space * 2.7);
		yi += startY+border;

		int xi = 0;
		if (i % 2 != 0)
		{
			xi = w / 2;
			//if(i>1)
			//	ugc.DrawLine(0, yi-space/2, Width, yi-space/2);
		}
		ugc.SetDrawColor(60, 60, 60);
		ugc.FillRectangle(xi+2*dpiX, yi, w/2-4*dpiX, space*2.5);
		ugc.SetDrawColor(255, 255, 255);
		ugc.FillRectangle(xi + 2 * dpiX + border, yi+space, w / 2 - 4 * dpiX - 2*border, space*1.5-border);
		ugc.SetDrawColor(225, 225, 225);
		ugc.SetTextSize(9 * dpiX);
		ugc.DrawString(lables[i], xi+w/4, yi+2*dpiX);
		ugc.SetTextSize(22*dpiX);

		
		double value = 0;
		if (selected != -1)
			value = database->variables[vname][selected];
		ugc.SetDrawColor(235, 235, 235);
		//ugc.FillRectangle(xi + 2 * dpiX, yi+space, w / 2 - 4 * dpiX, space);
		ugc.SetDrawColor(0, 0, 0);

		ugc.Bold = true;
		ugc.DrawString(ToString(vname,value), xi+w/4, yi+space*0.6);
		ugc.Bold = false;
	}

	
	
	int yi = (variable_names.size()) / 2;
	yi *= (space * 2.7);
	yi += startY+border;

	int xi = w / 2;
	
	ugc.SetDrawColor(60, 60, 60);
	ugc.FillRectangle(xi + 2 * dpiX, yi, w / 2 - 4 * dpiX, space*2.5);
	ugc.SetDrawColor(255, 255, 255);
	ugc.FillRectangle(xi + 2 * dpiX + border, yi + space, w / 2 - 4 * dpiX - 2 * border, space*1.5 - border);
	ugc.SetDrawColor(225, 225, 225);
	ugc.SetTextSize(9 * dpiX);
	ugc.DrawString("Вдох-Выдох (%)", xi + w / 4, yi + 2 * dpiX);
	ugc.SetTextSize(22 * dpiX);


	double value = 0;
	
	//ugc.SetDrawColor(235, 235, 235);
	//ugc.FillRectangle(xi + 2 * dpiX, yi + space, w/2 - 4 * dpiX, space);
	ugc.SetDrawColor(0, 0, 0);

	double result = 0;
	if(database->getCount()>0)
	{
		double Vim = database->variables["Vвдоха"].mean(database->checked);
		double Vexm = database->variables["Vвыдоха"].mean(database->checked);
		if(Vim!=0)
			result = (Vim - Vexm)/ Vim * 100.;
	}
	ugc.Bold = true;
	ugc.DrawString(DoubleToString(result), xi + w / 4, yi + space*0.6);
	ugc.Bold = false;
	ugc.SetAlign(ugc.LEFT);
}
//------------------------------------------------------
void CurrentValues::SetBounds()
{
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();
}
void CurrentValues::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();

	
}

string CurrentValues::DoubleToString(double val)
{
	stringstream ss;
	ss << fixed;
	int count=0;
	int sign = 1;
	if (val < 0)
	{
		sign = -1;
		val = -val;
	}

	if (val < 0.001)
		ss << 0;
	else
	{
		if (val < 0) count = 4;
		else if (val < 1) count = 3;
		else if (val < 100) count = 1;
		ss << setprecision(count);
		ss << val*sign;
	}
	
	return ss.str();
}

string CurrentValues::ToString(string var_name, double value)
{
	if (value < 0) return string("НД");

	int precision = 3;
	if (var_name == "Метаболизм_O2" ||
		var_name == "Метаболизм_CO2" ||
		var_name == "ДМП" || var_name == "ЧСС")
		return to_string(static_cast<int>(value));

	else if (var_name == "ЧД" || var_name == "ЧД_old" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2")
		precision = 1;
	else if (var_name == "Дыхательный_коэффициент")
	{
		if (value < 0.01) return string("НД");
		precision = 2;
	}

	stringstream ss;
	ss << setprecision(precision) << fixed << value;

	return ss.str();
}