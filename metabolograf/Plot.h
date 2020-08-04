#ifndef Plot_h
#define Plot_h


//#include <windows.h>
#include <gdiplus.h>
#include "ugc.h"
#include <algorithm>
#include "Database.h"
#include "PlotParameter.h"
#include "Dictionary.h"

using namespace std;
using namespace Gdiplus;


namespace VitLib
{
	struct Bounds
	{
		int x;
		int y;
		int width;
		int height;
	};
};


class Plot
{
private:   
	int borderX;
	int borderY;
	int Width;
	int Height;
	vector<Color> ColorPalette;
	Color bgColor;
	Bitmap* bitmap;
	int TextSizeHeader;
	int TextSizeLegend;
	int TextSizeAxis;
	bool NeedMean;
	float UIZoom;
	string header;
	Variable_<MTime> varTime;
	string meanTime;
	
	string plot_type;
	bool experience;
	vector<int> marks;
	VitLib::Bounds plotRect;

	vector<PlotParameter> var_Y;
	vector<PlotParameter> var_X;
	string plot_code;

	
	
private:// удаленные конструктор копирования и оператор присваивания
	Plot(const Plot& plot) = delete;
	Plot& operator=(const Plot& plot) { return *this; };
public:
	const vector<PlotParameter>& get_var_Y() const { return var_Y; }
	const vector<PlotParameter>& get_var_X() const { return var_X; }
	const string& getPlotCode() const { return plot_code; }

	void ClearMarks() 
	{
		for (auto& m : marks) m = -1;
		DrawPlot();
	}
	const string getMeanTime() { return meanTime; };
	bool IsExperience() const { return experience; }
	void SetExperience(bool status) { experience = status; }
	const VitLib::Bounds& getPlotRect() const { return plotRect; };
	void Default();
	Plot(int width = 400, int height = 400);
	~Plot(){ delete bitmap;}
	void SetBackgroundColor(const Color& color) { bgColor = color; }
	void SetBackgroundColor(const int R, const int G, const int B) { bgColor = Color(R,G,B); }
	int getWidth() const { return Width; }
	int getHeight() const { return Height; }
	const string& getHeader() const { return header; }
	Bitmap* getBitmap() const { return bitmap; }
	void EnlargeUI() { UIZoom = 1.5; }
	void Resize(int width, int height);
	void DrawBitmap(HDC hdc) const;
	void DrawBitmap(HDC hdc, int x, int y, int width, int height) const;
	HBITMAP getHBITMAP() const;
	void SetMarkPosByPlotIndex(int type, int pos);
	void SetMarkPosByTime(int type, const MTime& time);
	void SetMarkPosByMouseCoordinate(int type, int pos);
	void SetMarkPosByTableIndex(Database* database, int type, int pos);
	int FromTableIndexToPlotIndex(Database* database, int type, int pos);//конвертер индекса строки таблицы в индекс графика
	int FromPlotIndexToTableIndex(int type, Database* database);//возвращем индекс строки в таблице по индексу графика
	int GetScreenDPI();
	void Run(Database* database, const string& code);
	void ErrorMsg(const string& msg);
	void Run(Database* database, const vector<string>& str);
	void Reload(Database* database);
	void Clear();
	int even(int number);
	int odd(int number);
	int DrawHeader(UGC& ugc);
	void CheckData();
	void DrawPlot();
	int GetStep(int& size);
	void DrawTimePlot();
	void DrawAxisY(UGC& ugc, PlotParameter& variable, const int& position, const VitLib::Bounds& plotRect);
	void DrawAxisX(UGC& ugc, PlotParameter& variable, const int& position, const VitLib::Bounds& plotRect);
	void DrawAxisTime(UGC& ugc, const VitLib::Bounds& plotRect);
	void DrawLegend(UGC& ugc, vector<string> var_Y, const VitLib::Bounds& rect);
	void DrawTwoParamPlot();
	

};


#endif
