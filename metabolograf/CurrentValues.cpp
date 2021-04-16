#include "stdafx.h"
#include "CurrentValues.h"

BEGIN_MESSAGE_MAP(CurrentValues, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CurrentValues::CurrentValues(const Metab &metab)
	: m_rMetab(metab)
{
	selected = -1;
}


CurrentValues::~CurrentValues()
{
	//Clear();
	DestroyWindow();
}

void CurrentValues::Init()
{
	database = Database::getInstance();
	SetBounds();
	if (database->getVariableNames().size()==0) return;

	//varnames.push_back("����� ������������");

	
	variable_names.push_back("V�����");
	variable_names.push_back("V������");
	variable_names.push_back("FiO2");
	variable_names.push_back("FetO2");
	variable_names.push_back("FiCO2");
	variable_names.push_back("FetCO2");
	variable_names.push_back("��");
	variable_names.push_back("�����������_�����������");
	variable_names.push_back("��������_�����������_O2");
	variable_names.push_back("��������_���������_CO2");
	variable_names.push_back("����������_O2");
	variable_names.push_back("����������_CO2");
	variable_names.push_back("���");
	

	lables.push_back("V����� (�)");
	lables.push_back("V������ (�)");
	lables.push_back("FiO2 (%)");
	lables.push_back("FetO2 (%)");
	lables.push_back("FiCO2 (%)");
	lables.push_back("FetCO2 (%)");
	lables.push_back("��");
	lables.push_back("�����. �����������");
	lables.push_back("���.����.O2  (�)");
	lables.push_back("���.���.CO2 (�)");
	lables.push_back("�����.(O2) (����)");
	lables.push_back("�����.(CO2) (����)");
	lables.push_back("���");
	
}
//------------------------------------------------------
void CurrentValues::SetSelected(int new_selected)
{
	if (new_selected>= static_cast<int>(database->getCount()))
		selected = -1;
	else if (selected == new_selected) 
		return;
	else
		selected = new_selected;
	RedrawWindow();
}
//------------------------------------------------------

void CurrentValues::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());

	ugc.getGraphics()->Clear(Color(225, 225, 225));

	ugc.SetTextSize(DPIX()(10));
	ugc.SetAlign(Align::CENTER);

	//ugc.SetDrawColor(255, 255, 255);
	//ugc.DrawString("��������� ��������", Width/2, 0);

	
	//if(selected==-1)
	
	int w = Width ;
	int space = ugc.GetTextHeight();
	int startY = 0;// space * 2;

	//ugc.DrawLine(w / 2, startY, w / 2, variable_names.size() / 2 * space * 3+startY);
	const int border = DPIX()(2);
	for (size_t i = 0; i < variable_names.size(); i++)
	{
		string& vname = variable_names[i];
		//if (vname=="���" && !database->isVariableExists(vname)) continue;//���� ��� �� ����������� � ��� ����� ���������� �� �������� �� ����
		
		int yi = static_cast<int>(i / 2);
		yi *= static_cast<int>(space * 2.7);
		yi += startY+border;
		
		int xi = 0;
		if (i % 2 != 0)
		{
			xi = w / 2;
			//if(i>1)
			//	ugc.DrawLine(0, yi-space/2, Width, yi-space/2);
		}
		ugc.SetDrawColor(60, 60, 60);
		ugc.FillRectangle(static_cast<int>(xi + DPIX()(2)), yi, static_cast<int>(w/2- DPIX()(4)), static_cast<int>(space*2.5));
		ugc.SetDrawColor(255, 255, 255);
		ugc.FillRectangle(static_cast<int>(xi + DPIX()(2) + border), yi+space, static_cast<int>(w / 2 - DPIX()(4) - 2*border), static_cast<int>(space*1.5-border));
		ugc.SetDrawColor(225, 225, 225);
		ugc.SetTextSize(DPIX()(9));
		ugc.DrawString(lables[i], xi+w/4, static_cast<int>(yi+DPIX()(2)));
		ugc.SetTextSize(DPIX()(22));

		
		double value = -1;
		if (selected != -1 && database->isVariableExists(vname))
			value = database->getVariable(vname)[selected];
		else if (vname == "���")
			value = m_rMetab.HR;

		ugc.SetDrawColor(235, 235, 235);
		ugc.SetDrawColor(0, 0, 0);

		ugc.SetBold(true);
		ugc.DrawString(ToString(vname,value), xi+w/4, static_cast<int>(yi+space*0.6));
		ugc.SetBold(false);
	}

	
	
	int yi = (variable_names.size()) / 2;
	yi *= static_cast<int>(space * 2.7);
	yi += startY+border;

	int xi = w / 2;
	
	ugc.SetDrawColor(60, 60, 60);
	ugc.FillRectangle(static_cast<int>(xi + DPIX()(2)), yi, static_cast<int>(w / 2 - DPIX()(4)), static_cast<int>(space*2.5));
	ugc.SetDrawColor(255, 255, 255);
	ugc.FillRectangle(static_cast<int>(xi + DPIX()(2) + border), yi + space, static_cast<int>(w / 2 - DPIX()(4) - 2 * border), static_cast<int>(space*1.5 - border));
	ugc.SetDrawColor(225, 225, 225);
	ugc.SetTextSize(DPIX()(9));
	ugc.DrawString("����-����� (%)", xi + w / 4, static_cast<int>(yi + DPIX()(2)));
	ugc.SetTextSize(DPIX()(22));


	double value = 0;
	
	//ugc.SetDrawColor(235, 235, 235);
	//ugc.FillRectangle(xi + 2 * dpiX, yi + space, w/2 - 4 * dpiX, space);
	ugc.SetDrawColor(0, 0, 0);

	
	ugc.SetBold(true);
	//ugc.DrawString(DoubleToString(CalculateErr()), xi + w / 4, yi + space*0.6);
	//ugc.SetTextSize(18 * dpiX);
	ugc.DrawString(DoubleToString(CalculateErr()), xi + w / 8, static_cast<int>(yi + space*0.6));
	ugc.SetDrawColor(140, 140, 140);
	
	ugc.DrawString(DoubleToString(OldCalculateErr()), xi + w * 3 / 8, static_cast<int>(yi + space*0.6));
	ugc.SetBold(false);
	ugc.SetAlign(Align::LEFT);
}
//------------------------------------------------------
double CurrentValues::OldCalculateErr()
{
	if (database->getCount() == 0) return 0;
	double result = 0;
	
	double Vim = database->getVariable("V�����").getWithoutUnchecked(database->getChecked()).mean();
	double Vex = database->getVariable("V������").getWithoutUnchecked(database->getChecked()).mean();

	if (Vim != 0)
		result = (Vim - Vex) / Vim * 100.;

	return result;
}
//------------------------------------------------------
double CurrentValues::CalculateErr()
{
	if (database->getCount() == 0) return 0;
	double result = 0;
	int count = 0;
	for (size_t i = 0; i < database->getCount(); ++i)
	{
		if (database->getChecked(i))
		{
			const double &Vim = database->getVariable("V�����")[i];
			const double &Vex = database->getVariable("V������")[i];
			if (Vim != 0 && Vex !=0)
			{
				result += (Vim - Vex) / Vim * 100.;
				++count;
			}
		}
	}

	result = result / count;
	return result;
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
		if (val < 0) count = 3;
		//else if (val < 1) count = 3;
		else if (val < 100) count = 1;
		ss << setprecision(count);
		ss << val*sign;
	}
	
	return ss.str();
}

string CurrentValues::ToString(string var_name, double value)
{
	if (value < 0) return string("--");

	int precision = 3;
	if (var_name == "����������_O2" ||
		var_name == "����������_CO2" ||
		var_name == "���" || var_name == "���")
		return to_string(static_cast<int>(value));

	else if (var_name == "��" || var_name == "��_old" || var_name == "FiO2" || var_name == "FetO2" || var_name == "FiCO2" || var_name == "FetCO2")
		precision = 1;
	else if (var_name == "�����������_�����������")
	{
		if (value < 0.01) return string("��");
		precision = 2;
	}

	stringstream ss;
	ss << setprecision(precision) << fixed << value;

	return ss.str();
}