
#include "stdafx.h"
#include "Plot.h"

Plot::Plot(int width, int height)
{
	plot_code.clear();
	marks.resize(3, -1);
	experience = false;
	bitmap = nullptr;
	UIZoom = 1;

	Default();
	Resize(width, height);


	ColorPalette.push_back(Color(148, 0, 211));
	ColorPalette.push_back(Color(255, 127, 0));
	ColorPalette.push_back(Color(0, 0, 128));
	ColorPalette.push_back(Color(205, 205, 0));
	ColorPalette.push_back(Color(128, 0, 0));
	ColorPalette.push_back(Color(0, 128, 0));


	bgColor = Color(255, 255, 255);
}
//---------------------------------------------------------------------
void Plot::Default()
{
	m_iStartExperience = -1;
	m_iEndExperience = -1;
	plot_type.clear();
	header.clear();
	meanTime = "1 ���";
	NeedMean = false;
	marks = { -1,-1,-1 };
	varTime.clear();
	var_Y.clear();
	var_X.clear();
}
//---------------------------------------------------------------------
void Plot::Resize(int width, int height)
{
	if (bitmap)
		delete bitmap;
	Width = width;
	Height = height;
	bitmap = new Bitmap(Width, Height);

	double ind = height / 400.;
	TextSizeHeader = static_cast<int>(16 * ind / 2 * 2 * UIZoom);
	TextSizeLegend = static_cast<int>(14 * ind / 2 * 2 * UIZoom);
	TextSizeAxis = static_cast<int>(13 * ind / 2 * 2 * UIZoom);
	borderX = static_cast<int>(60 * ind * UIZoom);
	borderY = static_cast<int>(20 * ind * UIZoom);
}
//---------------------------------------------------------------------
void Plot::DrawBitmap(HDC hdc) const
{
	DrawBitmap(hdc, 0, 0, getWidth(), getHeight());
}
//---------------------------------------------------------------------
void Plot::DrawBitmap(HDC hdc, int x, int y, int width, int height) const
{
	UGC ugc(hdc, width, height);
	ugc.DrawBitmap(bitmap, x, y, width, height);
}
//---------------------------------------------------------------------
HBITMAP Plot::getHBITMAP() const
{
	HBITMAP hbitmap;
	bitmap->GetHBITMAP(bgColor, &hbitmap);
	return hbitmap;
}
//---------------------------------------------------------------------
void Plot::SetMarkPosByPlotIndex(int type, int pos)
{
	marks[type] = pos;
}
//----------------------------------------------------------------------
void Plot::SetMarkPosByTime(int type, const MTime& time)
{
	int i;
	for (i = 0; i < varTime.size(); i++)
	{
		if (time == varTime[i])
			break;
		else if (varTime[i] > time)
		{
			i--;
			break;
		}
	}
	marks[type] = i;
}
//----------------------------------------------------------------------
void Plot::SetMarkPosByMouseCoordinate(int type, int pos)
{
		if (plot_type != "TimePlot") return;
		int size = varTime.size();
		auto step = GetStepX(size);

		double result = pos / step;
		double temp = (double)(pos) / step-result;
		if (temp > 0.5)
			result++;
		if (result > size) result = size - 1;
		if (type < static_cast<int>(marks.size()))
			marks[type] = static_cast<int>(result);

		result;
}
//-------------------------------------------------------------------------------------------------------
void Plot::SetMarkPosByTableIndex(Database* database, int type, int pos)
{
	marks[type] = FromTableIndexToPlotIndex(database, pos);
}
//-------------------------------------------------------------------------------------------------------
int Plot::FromTableIndexToPlotIndex(Database* database, int pos)//��������� ������� ������ ������� � ������ �������
{
	if (pos < 0 || pos >= static_cast<int>(database->getCount()))
		return -1;

	MTime time = database->getDatetime(pos).getTime() - database->getDatetime(0).getTime();
	
	int result = -1;
	//------------
	if (time >= varTime[0])
	{
		for (int i = 0; i < varTime.size(); i++)
		{
			MTime& temptime = varTime[i];
			if (temptime > time)
			{
				if (NeedMean) return result + 1;
				else return result;

			}
			result = i;
		}
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------------
int Plot::FromPlotIndexToTableIndex(int type, Database* database)//��������� ������ ������ � ������� �� ������� �������
{
	if (type < 0 || type >= static_cast<int>(marks.size()))
		return -1;
	int p = marks[type];
	if (p < 0 || p >= varTime.size())
		return -1;
	
	MTime& time = varTime[p];
	
	for (int i = 0; i < static_cast<int>(database->getCount()); i++)
	{
		MTime temptime = database->getDatetime(i).getTime() - database->getDatetime(0).getTime();
		if (temptime > time)
		{
			return i - 1;
		}
	}
	return -1;
}
//-------------------------------------------------------------------------------------------------------
int Plot::GetScreenDPI()
{
	HDC hdcScreen = ::GetDC(NULL);
	int iDPI = -1; // assume failure
	if (hdcScreen) {
		iDPI = ::GetDeviceCaps(hdcScreen, LOGPIXELSX);
		::ReleaseDC(NULL, hdcScreen);
	}
	return iDPI;
}
//---------------------------------------------------------------------
void Plot::Run(Database* database, const string& code)
{
	plot_code = code;
	vector<string> elements;//�������� ������ �� ��������, ��� ���������� � ���������� ���������
	Dictionary dict;
	string word = "";

	for (int i = 0; i<static_cast<int>(code.length()); i++)//�����������
	{
		if (code[i] == '\"')
		{
			int j = code.find('\"', i + 1);
			if (j == -1)
				throw runtime_error("������ ����������: ������� ������ ���� �������.");
			string temp = code.substr(i + 1, j - i - 1);
			i = j;

			if (!word.empty())
				elements.push_back(word);//��������� � �����
			elements.push_back(temp);

		}
		else if (code[i] == ' ' || dict.operators.count(ToString(code[i]))>0)//��������� ������ ��� ����� �������������� ������
		{
			if (!word.empty())
				elements.push_back(word);//��������� � �����
			if (code[i] != ' ')
				elements.push_back(ToString(code[i]));//� �������������� ������
			word.clear();
		}
		else//����� ������� � �����
		{
			word.append(ToString(code[i]));
			if (i == code.length() - 1)
				elements.push_back(word);
		}
	}
	Run(database, elements);
}
//--------------------------------------------------
void Plot::ErrorMsg(const string& msg)
{
	UGC ugc(bitmap);
	ugc.SetDrawColor(bgColor);
	ugc.FillRectangle(0, 0, Width, Height);
	ugc.SetDrawColor(0, 0, 0);
	int textsize = static_cast<int>(20 * Height / 400.);
	ugc.SetTextSize(textsize);
	ugc.SetAlign(Align::CENTER);
	ugc.DrawString(msg, Width / 2, Height / 2 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(Align::LEFT);
	throw runtime_error(msg);
}
//-------------------------------------------------------------------------------------------------------
void Plot::Run(Database* database, const vector<string>& str)
{
	Default();
	int zerotime_index = 0;
	VariableBOOL checked(database->getChecked());


	//���� �������� �����������
	//if (experience)
	{
		if (database->getHeader().StartTest != 0)
		{
			zerotime_index = database->getHeader().StartTest;//������������� ������� ������� �� ��������
			//for (int i = 0; i < database->getHeader().StartTest; i++)
				//checked[i] = false;// ������� ����� �� ������ �����
		}
		else
			experience = false;

		if (database->getHeader().EndTest != 0)
		{
			//for (int i = database->getHeader().EndTest + 1; i < static_cast<int>(database->getCount()); i++)
				//checked[i] = false;//������� ����� ����� ����� �����
		}

	}

	meanTime = "";
	varTime = database->GetTimeFromZero(zerotime_index).getWithoutUnchecked(checked);//����������� ����� 

	vector<int> intervals;

	vector<PlotParameter>* var = nullptr;

	if (str.size() > 5)//!!!!!!!!!!!!!!!!!
	{
		int start(2), end(str.size() - 1);

		for (int i = 2; i < end; i += 4)
		{
			if (i + 2>end)
				throw runtime_error("�������������� ������ ��������� ������� '" + str[0] + "'");

			const string& p_name = str[i];
			const string& p_value = str[i + 2];

			if (p_name == "header" || p_name == "���������")// �������� ��������� �������
			{
				header = p_value;
			}
			else if (p_name == "mean" || p_name == "����������")// �������� �������� ����������. �� ��������� ��� 1 ���.
			{
				if (var_Y.size() > 0) continue;//���� �������� mean ������������ ��� ����� ���������� x ��� y, �� �� ������������
				if (meanTime.length() > 0) continue;//����������� ������ ������ �������� ����������

				meanTime = p_value;//��������� �������� ��������� ����������
				NeedMean = false;
				if (meanTime != "1 sec" && meanTime != "1 �" && meanTime != "1 ���" && meanTime != "1 c")// ���� ���� ���������, �� ������� ��������� � �����
				{
					Algorithm alg;
					NeedMean = true;
					intervals = alg.getIntervalsByTime(varTime, meanTime);
				}

			}
			else if (p_name == "y" || p_name == "�" || p_name == "x" || p_name == "�")// �������� �������� ���������� �� �����������
			{
				if (p_name == "y" || p_name == "�")
					var = &var_Y;
				else
					var = &var_X;

				int tick = 0;

				int j = i + 2;
				for (j; j<end; j += 2)
				{
					string p_value2 = str[j];
					if (!database->isVariableExists(p_value2))//���� ��� ����� ����������
						continue;
					//throw runtime_error("������: ���������� '" + p_value2 + "' �� �������.");
					//������� ���������� ��� ������ �����
					Variable varT = database->getVariable(p_value2).getWithoutUnchecked(checked);
					//��������� ���������� � ��������������� �����������
					if (tick == 0)
						var->push_back(PlotParameter(p_value2, intervals, varT));
					else
						(*var)[var->size() - 1].AddVariable(p_value2, intervals, varT);
					tick++;

					if (j + 1 < end && str[j + 1] != "&") break;
				}
				i = j - 2;

			}
			else
			{
				if (!var) throw runtime_error("�������������� ������ ��������� ������� '" + str[0] + "'");

				if (var->size() > 0)//���� ��� �������� ����������
				{
					int j = i + 2;
					for (j; j<end; j += 2)
					{
						string p_value2 = str[j];
						(*var)[var->size() - 1].setParameter(p_name, p_value2);	//����� ������ � ������� �������������� ���������		
						if (j + 1 < end && str[j + 1] != "&") break;
					}
					i = j - 2;
				}
			}
		}
	}
	if (var_X.size() == 0)
	{
		plot_type = "TimePlot";
		//experience = true;
	}
	else
	{
		//experience = false;
		plot_type = "TwoParamPlot";
	}

	//if (experience)
	{
		SetMarkPosByTableIndex(database, 0, database->getHeader().AeT);
		SetMarkPosByTableIndex(database, 1, database->getHeader().AT);
		SetMarkPosByTableIndex(database, 2, database->getHeader().MCO);
		m_iStartExperience = FromTableIndexToPlotIndex(database, database->getHeader().StartTest);
		m_iEndExperience = FromTableIndexToPlotIndex(database, database->getHeader().EndTest);
	}
	CheckData();
}
//-------------------------------------------------------------------------------------------------------
void Plot::Reload(Database* database)
{
	try
	{
		Run(database, plot_code);
	}
	catch (...) {}
}
//-------------------------------------------------------------------------------------------------------
void Plot::Clear()
{
	UGC ugc(bitmap);
	ugc.SetDrawColor(bgColor);
	ugc.Clear();
	plot_code.clear();
}
//-------------------------------------------------------------------------------------------------------
int Plot::even(int number)
{
	int result = 0;
	for (int i = 0; i <= number; i++)
		if (i % 2 == 0) result++;
	return result;
}
//-------------------------------------------------------------------------------------------------------
int Plot::odd(int number)
{
	int result = 0;
	for (int i = 0; i <= number; i++)
		if (i % 2 != 0) result++;
	return result;
}
//-------------------------------------------------------------------------------------------------------
int Plot::DrawHeader(UGC& ugc)
{
	if (!header.empty())
	{
		ugc.SetTextSize(TextSizeHeader);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetAlign(Align::CENTER);
		ugc.DrawString(header, Width / 2, 2);
		ugc.SetAlign(Align::LEFT);
		return ugc.GetTextHeight(TextSizeHeader);//borderY;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
void Plot::CheckData()
{
	//�������� ������ ���� ������ ��� ������
	if (var_Y.size() == 0)
		ErrorMsg("������: ��� ������.");//�� ������ 4 ����������
	if (var_Y.size() > 6)
		ErrorMsg("������ �� ������������ ����������� ����� 6 ����������.");//�� ������ 4 ����������
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawPlot()
{
	//�������� ������ ���� ������ ��� ������
	try {
		CheckData();
	}
	catch (...) { return; }

	if (plot_type == "TimePlot")
	{
		//experience = true;
		DrawTimePlot();
	}
	else //if(plot_type == "TwoParamPlot")
	{
		//experience = false;
		DrawTwoParamPlot();
	}
}
//-------------------------------------------------------------------------------------------------------
double Plot::GetStepX(int size)
{
	size--;
	if (size > plotRect.width) size = plotRect.width;
	double step = plotRect.width;
	if(size>0)
		step = static_cast<double>(plotRect.width) / size;

	if (step > DPIX()(20.))
		step = DPIX()(20.);

	return step;
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawTimePlot()
{
	vector<PlotParameter>& variables = var_Y;
	//���������� � ����������
	UGC ugc(bitmap);
	ugc.SetDrawColor(bgColor);
	ugc.Clear();

	//������ ���������
	int headerHeight = DrawHeader(ugc);
	//������������ � ������ �������
	int e = even(variables.size()) - 1;
	int o = odd(variables.size());
	
	int axisX_height = ugc.GetTextWidth("00:00", TextSizeAxis);

	plotRect = { borderX / 10 + borderX * o,
		Height - borderY / 2 - axisX_height,
		Width - borderX / 5 - borderX * o - borderX * e,
		static_cast<int>(Height - borderY*1.5 - headerHeight - axisX_height) };

	plotRect.x += 5;
	plotRect.width -= 15;


	int size = varTime.size();
	auto x_step = GetStepX(size);
	//������ ����� ������� ������� �������
	ugc.SetDrawColor(125, 125, 125);
	ugc.DrawRectangle(plotRect.x - 2, plotRect.y - plotRect.height - 2, plotRect.width + 4, plotRect.height + 4);
	if (m_iStartExperience >= 0)
	{
		ugc.SetDrawColor(235, 235, 235);
		int x = static_cast<int>(m_iStartExperience * x_step);
		
		int width = (m_iEndExperience >= 0 ? static_cast<int>(m_iEndExperience * x_step) : plotRect.width);
		ugc.FillRectangle(plotRect.x + x , plotRect.y - plotRect.height, width - x, plotRect.height);
	}

	ugc.SetAlign(Align::RIGHT);
	int countPlots = 0;
	vector<string> legends;
	for (int i = 0; i < static_cast<int>(variables.size()); i++)
	{
		//draw axises
		DrawAxisY(ugc, variables[i], i, plotRect);

		// ������������ ������
		double start = variables[i].getStart();
		double end = variables[i].getEnd();
		double scale = variables[i].getZoom(plotRect.height);

		//�������� ���� ���������� ������
		for (int v = 0; v < static_cast<int>(variables[i].count()); v++)
		{

			legends.push_back(variables[i].getLegend(v));

			ugc.SetDrawColor(ColorPalette[countPlots]);
			// ������ ������
			Variable& var = variables[i].getVar(v);
			//������������ � �������� ����������. ���� �������� ������, ��� ������ �������, �� �������� �� ������ �������
			
			for (int j = 1; j < size; j++)
			{
				double y0 = var[j - 1];
				if (y0 > end) y0 = end;
				if (y0 < start) y0 = start;
				y0 = (y0 - start)*scale;

				double y1 = var[j];
				if (y1 > end) y1 = end;
				if (y1 < start) y1 = start;
				y1 = (y1 - start)*scale;
				
				ugc.DrawLine(static_cast<int>(plotRect.x + (j - 1)*x_step), static_cast<int>(plotRect.y - y0), static_cast<int>(plotRect.x + j * x_step), static_cast<int>(plotRect.y - y1), 2);
			}

			countPlots++;
		}

	}
	/*
	Draw Marks
	*/
	vector<string> v{ "���", "����", "���" };
	ugc.SetBold(true);
	for (int c = 0; c<3; c++)//!!!!!!!!!!!!!!!!!!!!!
	{
		if (marks[c] >= 0)
		{
			if (c == 0)
				ugc.SetDrawColor(255, 0, 0);
			else if (c == 1)
				ugc.SetDrawColor(0, 155, 0);
			else
				ugc.SetDrawColor(0, 0, 255);
			int x = static_cast<int>(plotRect.x + marks[c] * x_step);
			int y = plotRect.y - plotRect.height;
			ugc.FillRectangle(x, y, DPIX()(2), plotRect.height);
			if (experience)
				ugc.DrawString(v[c], x + 2, plotRect.y - DPIX()(18));
		}
	}
	ugc.SetBold(false);
	
	DrawAxisTime(ugc, plotRect);
	DrawLegend(ugc, legends, plotRect);
}
//-------------------------------------------------------------------------------------------------------
double Plot::GetStepY(double range)
{
	auto f = [](int range) 
	{
		if (range % 6 == 0)
		{
			return range / 6;
		}
		else if (range % 4 == 0)
		{
			return range / 4;
		}
		else if (range % 5 == 0)
		{
			return range / 5;
		}
		return range / 10;
	};


	if (range > (int)range / 10 * 10)
	{
		int r = (int)(range * 10);
		double result = static_cast<double>(f(r)) / 10.;
		return result;
	}
	return f((int)range);
	

}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawAxisY(UGC& ugc, PlotParameter& variable, int position, const VitLib::Bounds& plotRect)
{
	int x;

	int space = 1;
	if (position % 2 == 0)//������ ������ - ������� � 0. ��� �������� �����
	{
		ugc.SetAlign(Align::RIGHT);
		x = plotRect.x - borderX*(position / 2) - 3;
		space = -1;
	}
	else//�������� ������. ��� �������� ������
	{
		ugc.SetAlign(Align::LEFT);
		x = plotRect.x + borderX*(position / 2) + plotRect.width + 3;
		space = 1;
	}

	double range = variable.getRange();
	double start = variable.getStart();
	double end = variable.getEnd();
	double step = GetStepY(range);
	
	ugc.SetTextSize(TextSizeAxis);
	//x += space;
	int w = ugc.GetTextWidth(ugc.ToStringAlt(end), TextSizeAxis);
	ugc.SetDrawColor(Color(0, 0, 0));
	for (double number = start; number <= end; number += step)
	{
		int yi = plotRect.y - (int)((number-start) * (plotRect.height/range));
		ugc.DrawNumberAlt(number, x, yi - ugc.GetTextHeight() / 2);
		if (position % 2 == 0)
			ugc.DrawLine(x, yi, x+2, yi);
		else
			ugc.DrawLine(x - 2, yi, x, yi);
	}

	ugc.SetDrawColor(Color(0, 0, 0));
	ugc.SetTextSize(TextSizeLegend);
	string text = "";
	ugc.SetAlign(Align::CENTER);
	for (int i = 0; i < static_cast<int>(variable.count()); i++)
	{
		text += variable.getLegend(i);
		if (i < static_cast<int>(variable.count()) - 1)
			text += " & ";
	}

	//int w = ugc.GetTextWidth(ugc.ToString(start + range), TextSizeAxis);
	int h = ugc.GetTextHeight();
	if (space < 0)
		x -= (w + h*5/6);//borderX;
	else
		x += (w);//borderX - ugc.TextSize*1.75;


	//ugc.FillRectangle(x, plotRect.y - plotRect.height / 2, 1, 20);
	ugc.DrawVerticalString(text, x, plotRect.y - plotRect.height / 2);
	ugc.SetAlign(Align::LEFT);
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawAxisX(UGC& ugc, PlotParameter& variable, int position, const VitLib::Bounds& plotRect)
{
	//int step = ugc.GetTextWidth("12345", TextSizeAxis);
	//int countPointsInAxis = plotRect.width / step;
	int x = plotRect.x;
	int y = plotRect.y + position*(ugc.GetTextHeight(TextSizeAxis) + ugc.GetTextHeight(TextSizeLegend));
	int space = 4;
	double range = variable.getRange();
	double start = variable.getStart();
	double end = variable.getEnd();
	double step = GetStepY(range);

	ugc.SetTextSize(TextSizeAxis);
	int textHeight = static_cast<int>(ugc.GetTextHeight()*1.3);
	ugc.SetAlign(Align::CENTER);
	ugc.SetDrawColor(Color(0, 0, 0));
	for (double number = start; number <= end; number += step)
	{
		int xi = plotRect.x + (int)((number - start) * (plotRect.width / range));

		ugc.DrawNumberAlt(number, xi, y + 4);
		ugc.DrawDashLine(xi, plotRect.y + 4, xi, plotRect.y + 6);
	}
	
	ugc.SetDrawColor(Color(0, 0, 0));
	ugc.SetTextSize(TextSizeLegend);
	string text = "";
	//ugc.SetAlign(Align::CENTER);
	for (int i = 0; i < static_cast<int>(variable.count()); i++)
	{
		text += variable.getLegend(i);
		if (i < static_cast<int>(variable.count()) - 1)
			text += " & ";
	}
	ugc.DrawString(text, x + plotRect.width / 2, y + textHeight);
	ugc.SetAlign(Align::LEFT);
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawAxisTime(UGC& ugc, const VitLib::Bounds& plotRect)
{
	ugc.SetTextSize(TextSizeAxis);

	int countPointsInAxis = ugc.GetTextHeight() * 2;
	int text_height = ugc.GetTextWidth("000 m 00 s");//���������� ������ �������

	int size = varTime.size();
	auto x_step = GetStepX(size);//plotRect.width / varTime.size();
	countPointsInAxis = static_cast<int>(countPointsInAxis /x_step);
	if (countPointsInAxis <= 0) countPointsInAxis = 1;

	int x = plotRect.x;
	int y = plotRect.y;

	ugc.SetDrawColor(0, 0, 0);

	ugc.SetAlign(Align::RIGHT);
	for (int i = 0; i < plotRect.width; i += countPointsInAxis)
	{
		if (i >= varTime.size()) break;

		string time = varTime[i].getStringAlt();
		ugc.SetDrawColor(0, 0, 0);
		ugc.DrawVerticalString(time, x + static_cast<int>(i*x_step) - ugc.GetTextHeight() / 2, y + TextSizeAxis);
		ugc.SetDrawColor(155, 155, 155);
		ugc.DrawDashLine(static_cast<int>(x + i * x_step), plotRect.y, static_cast<int>(x + i * x_step), plotRect.y - plotRect.height);
		//ugc.DrawLine(x + i*x_step, y + 2, x + i*x_step, y + 4);
	}
	ugc.SetAlign(Align::LEFT);
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawLegend(UGC& ugc, vector<string> var_Y, const VitLib::Bounds& rect)
{
	int y = rect.y - rect.height;
	int x = rect.x + 5;
	ugc.SetAlign(Align::LEFT);
	ugc.SetTextSize(TextSizeLegend);
	int textheight = ugc.GetTextHeight(TextSizeLegend + 1);

	ugc.SetDrawColor(155, 255, 255, 255);
	int legendWidth = 0;
	for (auto& varName : var_Y)
	{
		legendWidth = max(legendWidth, ugc.GetTextWidth(varName));

	}
	ugc.FillRectangle(rect.x, y, textheight / 2 + legendWidth + 8, static_cast<int>(var_Y.size() * textheight));


	/*if (experience && plot_type == "TimePlot")
	{
		int yi = static_cast<int>(var_Y.size() + 1) * textheight + 12;
		vector<string> v{ "���", "����", "���" };
		vector<Color> c{ Color(255,0,0), Color(0,255,0), Color(0,0,255) };
		int width = rect.width;
		width /= 3;
		for (int i = 0; i < 3; i++)
		{
			int xi = x + i * width;
			ugc.SetDrawColor(0, 0, 0);
			ugc.DrawString(v[i], xi + textheight, y - yi);
			ugc.SetDrawColor(c[i]);
			ugc.FillRectangle(xi, y - yi, textheight / 2, textheight / 2);
		}
	}*/

	



	for (int i = 0; i < static_cast<int>(var_Y.size()); i++)
	{
		ugc.SetDrawColor(ColorPalette[i]);
		int yi = i * textheight;
		ugc.FillRectangle(x, y + yi + 4, textheight/2, textheight/2);
		ugc.SetDrawColor(50, 50, 50);
		string temp = var_Y[i];
		for (char& c : temp)
			if (c == '_') c = ' ';
		ugc.DrawString(temp, x + textheight/2 + 3, y + yi);
	}
	
}
//-------------------------------------------------------------------------------------------------------
void Plot::DrawTwoParamPlot()//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	//���������� � ����������
	UGC ugc(bitmap);
	ugc.SetDrawColor(bgColor);
	ugc.Clear();

	//������ ���������
	int headerHeight = DrawHeader(ugc);
	//������������ � ������ �������

	int axisX_height = ugc.GetTextHeight(TextSizeAxis) + ugc.GetTextHeight(TextSizeLegend);
	axisX_height *= var_X.size();

	int e = even(var_Y.size()) - 1;
	int o = odd(var_Y.size());

	plotRect = { borderX / 10 + borderX,
		Height - borderY / 2 - axisX_height,
		Width - borderX / 5 - borderX * o - borderX * e,
		static_cast<int>(Height - borderY*1.5 - headerHeight - axisX_height) };

	plotRect.x += 5;
	plotRect.width -= 15;

	//������ ����� ������� ������� �������
	ugc.SetDrawColor(125, 125, 125);
	ugc.DrawRectangle(plotRect.x - 2, plotRect.y - plotRect.height - 2, plotRect.width + 4, plotRect.height + 4);

	ugc.SetAlign(Align::RIGHT);
	int countPlots = 0;
	vector<string> legends;
	Bitmap temp_bitmap(plotRect.width, plotRect.height);
	UGC temp_ugc(&temp_bitmap);
	temp_ugc.SetDrawColor(bgColor);
	temp_ugc.Clear();

	int xi = 0;
	for (int yi = 0; yi < (int)var_Y.size(); yi++)//��������� ������ ���� �������� �� Y
	{
		// ������������ ������
		double startX = var_X[xi].getStart();
		double endX = var_X[xi].getEnd();
		double scaleX = var_X[xi].getZoom(plotRect.width);
		double startY = var_Y[yi].getStart();
		double endY = var_Y[yi].getEnd();
		double scaleY = var_Y[yi].getZoom(plotRect.height);

		//�������� ���� ���������� ������
		for (int v = 0; v < static_cast<int>(var_Y[yi].count()); v++)
		{
			//������������ � �������� ����������. ���� �������� ������, ��� ������ �������, �� �������� �� ������ �������
			int sizeX = var_X[xi].size(v);
			if (sizeX > plotRect.width) 
				sizeX = plotRect.width;

			legends.push_back(var_Y[yi].getLegend(v));

			temp_ugc.SetDrawColor(ColorPalette[countPlots]);
			// ������ ������
			Variable& varX = var_X[xi].getVar(0);
			Variable& varY = var_Y[yi].getVar(v);
			int x_step = plotRect.width;;
			if(sizeX>0) x_step/= sizeX;

			//int point = plotRect.height / 4.0 / sizeX * dpiX;
			int point = plotRect.height / 40;
			if (point < DPIX()(2)) point = DPIX()(2);
			if (point > DPIX()(8)) point = DPIX()(8);

			int sizeY = varY.size();
			for (int j = 0; j < sizeX, j<sizeY; j++)
			{
				double x = varX[j];

				if (x < startX)
					x = startX;
				if (x > endX)
					x = endX;
				x = (x - startX)*scaleX;
				x -= point / 2;

				double y = varY[j];
				if (y > endY)
					y = endY;
				if (y < startY)
					y = startY;
				y = (y - startY)*scaleY;
				y += point / 2;

				//temp_ugc.FillRectangle(x, temp_ugc.HEIGHT-y, point, point);
				temp_ugc.FillEllipse((int)x, temp_ugc.getHeight() - (int)y, point);
			}
			countPlots++;
		}
	}
	ugc.DrawBitmap(&temp_bitmap, plotRect.x, plotRect.y - plotRect.height, plotRect.width, plotRect.height);
	

	for (int i = 0; i < (int)var_Y.size(); i++)
		DrawAxisY(ugc, var_Y[i], i, plotRect);
	//for (size_t i = 0; i<var_X.size(); i++)
	DrawAxisX(ugc, var_X[0], 0, plotRect);

	if (legends.size() > 1)
		DrawLegend(ugc, legends, plotRect);
}
//-------------------------------------------------------------------------------------------------------

