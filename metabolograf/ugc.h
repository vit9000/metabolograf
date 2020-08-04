#ifndef UGC_H
#define UGC_H
#define null NULL


#include <windows.h>
#include <GdiPlus.h>
#include <string>
#include <stdio.h>
#include "DPIX.h"

using namespace std;

//using namespace Gdiplus;

using Gdiplus::Bitmap;
using Gdiplus::Color;
using Gdiplus::RectF;
using Gdiplus::PointF;

/*#ifndef sprinf_s
#define sprintf_s sprintf
#endif
*/



struct UInput
{
    int x;
    int y;
    int pressedX;
    int pressedY;
    bool LBMouseDown;
    bool RBMouseDown;

    int scrollX;
    int scrollY;
};

enum class Align { CENTER, LEFT, RIGHT };
enum class Form { PAUSE, NO_CHANGE, ARROWUP, ARROWDOWN, BOLUS};


class UGC
{
public: 
	
	/*int CENTER;
	int LEFT;
	int RIGHT;*/
private:
	int WIDTH, HEIGHT;
	int TextSize;
	int RealTextSize;
	Align align;

	

	bool Bold;

	Gdiplus::Graphics *g;
	Gdiplus::Color color;
	wstring FontName;
	

	bool doublebuffering;
	
	
	HDC hdc;
	HDC Memhdc;
	HBITMAP Membitmap;
public:
	Gdiplus::Graphics *getGraphics() { return g; };

	UGC(Bitmap* bitmap)//, int width, int height)
	{
		
		doublebuffering = false;
		Init(bitmap->GetWidth(), bitmap->GetHeight());
		g = Gdiplus::Graphics::FromImage(bitmap);
		StartDraw();
	};
	//----------------------------------------
	UGC(CDC* pDC, int width, int height)
	{
		
		doublebuffering = true;
		Init(width, height);
		
		hdc = pDC->m_hDC;

		Memhdc = CreateCompatibleDC(hdc);
		Membitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(Memhdc, Membitmap);


		g = new Gdiplus::Graphics(Memhdc);
		StartDraw();
	};


	UGC(HDC& hdc, int width, int height)
	{
		
		doublebuffering = false;
		Init(width, height);
		//hdc = _hdc;
		
		g = new Gdiplus::Graphics(hdc);
		StartDraw();
	};
	//----------------------------------------
	void Init(int width, int height)
	{
		Bold = false;
		WIDTH = width,
		HEIGHT = height;
		TextSize = 12;
		RealTextSize = 12;
		//CENTER = 0;;
		//LEFT = 1;//Qt::AlignLeft;
		//RIGHT = 2;//Qt::AlignRight;
		align = Align::LEFT;
		//FontName = L"Times New Roman";
		FontName = L"Calibri";
	}
	//----------------------------------------
	~UGC()
	{
			EndDraw();
		
	}
	//----------------------------------------
	int getWidth() const { return WIDTH; }
	int getHeight() const { return HEIGHT; }
private:
	void StartDraw()
	{
		g->SetPageUnit(Gdiplus::UnitPixel);
		g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);

		SetDrawColor(255, 255, 255);
		FillRectangle(0, 0, WIDTH, HEIGHT);
	}
	//----------------------------------------
	void EndDraw()
	{	
		delete g;
		g = nullptr;
		if (doublebuffering)
		{
			BitBlt(hdc, 0, 0, WIDTH, HEIGHT, Memhdc, 0, 0, SRCCOPY);
			DeleteObject(Membitmap);
			DeleteDC(Memhdc);
			DeleteDC(hdc);
		}
	}
	//----------------------------------------
public:
	void Clear()
	{
		g->Clear(color);
	}
	//---------------------------------------------------------------------

	void DrawBitmap(Gdiplus::Bitmap* bitmap, int x, int y, int width, int height)
	{	
		g->DrawImage(bitmap, x, y, width, height);
	}
	//---------------------------------------------------------------------

	void SetSpecColor(int color)
	{
		switch (color)
		{
		case 0:
			//SetDrawColor(254, 234, 6);
			SetDrawColor(217, 197, 25);
			break;
		case 1:
			SetDrawColor(235, 144, 55);
			break;
		case 2:
			SetDrawColor(116, 199, 229);
			break;
		case 3:
			SetDrawColor(238, 165, 157);
			break;

		case 4:
			SetDrawColor(120, 120, 120);
			break;

		case 5:
			SetDrawColor(239, 103, 9);
			break;

		case 6:
			SetDrawColor(216, 13, 6);
			break;

		case 7:
			SetDrawColor(94, 94, 94);
			break;

		case 8:
			SetDrawColor(62, 61, 61);
			break;

		}
	}

	void SetDrawColor(int A, int R, int G, int B)
	{
		color = Color(A, R, G, B);
	};
	//---------------------------------------
	void SetDrawColor(int R, int G, int B)
	{
		color = Color(R, G, B);
	};
	//---------------------------------------
	void SetDrawColor(Color _color)
	{
		color = _color;
	};
	//---------------------------------------
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		Gdiplus::SolidBrush blackBrush(color);
		Gdiplus::PointF point1((Gdiplus::REAL)x1, (Gdiplus::REAL)y1);
		Gdiplus::PointF point2((Gdiplus::REAL)x2, (Gdiplus::REAL)y2);
		Gdiplus::PointF point3((Gdiplus::REAL)x3, (Gdiplus::REAL)y3);
		Gdiplus::PointF points[3] = { point1, point2, point3 };
		g->FillPolygon(&blackBrush, points, 3);
	};
	//---------------------------------------




	void DrawForm(int x, int y, int width, int height, Form form)
	{
		
		//0 - pause, 1 - no change, 2 - up, 3 - down
		width = width/2*2;
		int temp = static_cast<int>(height*0.3);

		switch(form)
		{
			case Form::PAUSE:
				FillRectangle(x, y, width/3, height);
				FillRectangle(x+width, y, -width/3, height);
				break;
			case Form::NO_CHANGE:
				FillRectangle(x, y, width, height/3);
				FillRectangle(x, y+height, width, -height/3);
				break;
			case Form::ARROWUP:
				FillRectangle(x+temp, y+height/2, width-temp*2, height/2);
				FillTriangle(x, y+height/2, x+width, y+height/2, x+width/2, y);
				break;
			case Form::ARROWDOWN:
				FillRectangle(x+temp, y, width-temp*2, height/2);
				FillTriangle(x, y+height/2, x+width, y+height/2, x+width/2, y+height);
				break;
			case Form::BOLUS:
				FillTriangle(x, y, x+width, y, x+width/2, y+height/2);
				FillTriangle(x, y+height/2, x+width, y+height/2, x+width/2, y+height);
				break;
		}
	}
	//-------------------------------------------------------
	void DrawDashLine(int x1, int y1, int x2, int y2, int line_width = 1)
	{
		Gdiplus::Pen penCurrent(color);
		penCurrent.SetWidth(static_cast<Gdiplus::REAL>(DPIX().GetValue(line_width)));
		penCurrent.SetDashStyle(Gdiplus::DashStyleCustom);
		Gdiplus::REAL dashValues[4] = { 1, 2 };
		penCurrent.SetDashPattern(dashValues, 2);
		g->DrawLine(&penCurrent, x1, y1, x2, y2);
	}
	//-------------------------------------------------------
	void DrawLine(int x1, int y1, int x2, int y2, int line_width = 1)
	{
		Gdiplus::Pen penCurrent(color);
		penCurrent.SetWidth(static_cast<Gdiplus::REAL>(DPIX().GetValue(line_width)));

		g->DrawLine(&penCurrent, x1, y1, x2, y2);
	};
	//-------------------------------------------------------
	void DrawPunktLine(int x1, int y1, int x2, int y2, int line_width = 1)
	{


	};
	//-------------------------------------------------------
	void DrawPunktRectangle(int x, int y, int width, int height, int line_width = 1)
	{


	};

	//-------------------------------------------------------
	void DrawRectangle(int x, int y, int width, int height, int line_width = 1)
	{
		
		Gdiplus::Pen penCurrent(color);
		penCurrent.SetWidth(static_cast<Gdiplus::REAL>(line_width));
		g->DrawRectangle(&penCurrent, x, y, width, height);

	};
	//-------------------------------------------------------
	void FillRectangle(int x, int y, int width, int height, bool finishdraw = true)
	{
		Gdiplus::SolidBrush mySolidBrush(color);
		
		g->FillRectangle(&mySolidBrush, x, y, width, height);

	};
	//-------------------------------------------------------
	void FillObj(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{

	};
	//-------------------------------------------------------
	void DrawEllipse(int x, int y, int D, int line_width)
	{
		Gdiplus::Pen penCurrent(color);
		penCurrent.SetWidth(static_cast<Gdiplus::REAL>(line_width));
		g->DrawEllipse(&penCurrent, x, y, D, D);
	}
	//-------------------------------------------------------
	
	void FillEllipse(int x, int y, int D)
	{
		Gdiplus::SolidBrush mySolidBrush(color);
		g->FillEllipse(&mySolidBrush, x, y, D, D);		
	}
	//-------------------------------------------------------
	void SetTextSize(int size)
	{
		TextSize=size;
		RealTextSize = static_cast<int>(TextSize*1.2);

	}
	//-------------------------------------------------------
	int GetTextHeight(int size)
	{
		Gdiplus::Font font(FontName.c_str(), static_cast<Gdiplus::REAL>(size / (double)DPIX()), Gdiplus::FontStyleRegular);
		//Font font(L"Helvetica", TextSize, FontStyleRegular);

		int length = 1;

		PointF origin(0.0f, 0.0f);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		RectF boundRect;
		// Measure the string.
		g->MeasureString(L"A", length, &font, origin, &format, &boundRect);
		return static_cast<int>(boundRect.Height);
		//return RealTextSize;
	}
	//-------------------------------------------------------


	int GetTextHeight()
	{
		return GetTextHeight(TextSize);
		//return RealTextSize;
	}
	//-------------------------------------------------------
	int GetTextWidth(string str)
	{
		return GetTextWidth(str, TextSize);
	}

	int GetTextWidth(string str,int size)
	{
		
		Gdiplus::Font font(FontName.c_str(), static_cast<Gdiplus::REAL>(DPIX().GetValue(size)), Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
		//Font font(L"Helvetica", TextSize, FontStyleRegular);

		int length = str.length();
		
		PointF origin(0.0f, 0.0f);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		RectF boundRect;
		// Measure the string.
		g->MeasureString(StringToWString(str).c_str(), length, &font, origin, &format, &boundRect);
		return static_cast<int>(boundRect.Width);
	}
	//-------------------------------------------------------
	void DrawString(char* str, int x, int y)
	{
		string mstring = str;
		DrawString(mstring, x, y);
		
	}
	//-------------------------------------------------------
	void DrawVerticalString(string mstring, int x, int y)
	{
		g->TranslateTransform(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y)); // Set rotation point
		g->RotateTransform(-90); // Rotate text
		g->TranslateTransform(static_cast<Gdiplus::REAL>(-x), static_cast<Gdiplus::REAL>(-y)); // Reset translate transform
		DrawString(mstring, x, y);
		g->ResetTransform(); // Only needed if you reuse the Graphics object for multiple calls to DrawString

	}
	//-------------------------------------------------------
	void DrawString(string mstring, int x, int y)
	{

		Gdiplus::Font font(FontName.c_str(), static_cast<Gdiplus::REAL>(DPIX().GetValue(TextSize)), Bold?Gdiplus::FontStyleBold:Gdiplus::FontStyleRegular);
		//Font font(L"Helvetica", TextSize, FontStyleBold);
		g->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);//SetTextRenderingHint(TextRenderingHintAntiAlias);
		Gdiplus::SolidBrush brush(color);
		if (align == Align::RIGHT)
			x -= GetTextWidth(mstring);
		else if (align == Align::CENTER)
			x -= GetTextWidth(mstring) / 2;
		int length = static_cast<int>(mstring.length());
		g->DrawString(StringToWString(mstring).c_str(), length, &font, Gdiplus::PointF(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y)), &brush);
	};
	//-------------------------------------------------------
	void DrawSymbol(char symb, int x, int y)
	{		
		string str = {symb};
		DrawString(str, x, y);
	}
	//-------------------------------------------------------
	void DrawNumber(int number, int x, int y)
	{
		char buf[50];
		sprintf_s(buf, "%d", number);
		string str= buf;
		int len = str.length();
		DrawString(str, x, y);
	}
	//-------------------------------------------------------
	void DrawNumber(double number, int x, int y)
	{
		DrawString(ToString(number), x, y);
	}
	//-------------------------------------------------------
	void DrawNumber(double number, int x, int y, int count)
	{
		DrawString(ToString(number, count), x, y);
	}
	//-------------------------------------------------------
	string ToString(double number)
	{
		int sign = 1;
		if (number < 0) 
		{
			number = -number; 
			sign = -1;
		}
		if(number>99.999)
			return ToString(sign*number, 0);
		else if(number<0.001)
			return ToString(0, 0);
		else if (number<0.01)
				return ToString(sign*number, 3);
		else if(number<1.0)
			return ToString(sign*number, 2);
		
		return ToString(sign*number,1);
	}
	//-------------------------------------------------------		
	string ToString(double number, int count)
	{
		char buf[50];
		switch(count)
		{
			case 0:
				sprintf_s(buf, "%d", (int)number);
				break;
			case 1:
				sprintf_s(buf, "%.1f", number);
				break;
			case 2:
				sprintf_s(buf, "%.2f", number);
				break;
			case 3:
				sprintf_s(buf, "%.3f", number);
				break;
			case 4:
				sprintf_s(buf, "%.4f", number);
				break;
			case 5:
				sprintf_s(buf, "%.5f", number);
				break;
			default:
				sprintf_s(buf, "%.3f", number);
				break;
			}
			string str = buf;
			return str;
	}
	//-------------------------------------------------------
	void SetAlign(Align _align)
	{
		align = _align;
	}
	void SetBold(bool status)
	{
		Bold = status;
	}
	//-------------------------------------------------------
	void FillForm(int x, int y, int width, int height, int b=10)
	{
		Gdiplus::GraphicsPath path;
		path.AddLine(	x+b,		y,
						x+width-b,	y	);
		path.AddBezier( x+width-b,		y,
						x+width-b/2,	y+b/4,
						x+width-b/4,	y+b/2,
						x+width,		y+b	);

		path.AddLine(	x+width,		y+b,
						x+width,		y+height-b);
		
		path.AddBezier( x+width,		y+height-b,
						x+width-b/4,	y+height-b/2,
						x+width-b/2,	y+height-b/4,
						x+width-b,		y+height	);
		
		path.AddLine(	x+width-b,		y+height,
						x+b,			y+height);


		path.AddBezier( x+b,			y+height,
						x+b/2,			y+height-b/4,
						x+b/4,			y+height-b/2,
						x,				y+height-b	);
		
		path.AddLine(	x,				y+height-b,
						x,				y+b);

		path.AddBezier( x,				y+b,
						x+b/4,			y+b/2,
						x+b/2,			y+b/4,
						x+b,			y	);

		Gdiplus::SolidBrush brush(color);
		g->FillPath(&brush, &path);

		//FillRectangle(x,y,width,height);
	}
	//-------------------------------------------------------
	void FillFormTop(int x, int y, int width, int height, int b=10)
	{
		Gdiplus::GraphicsPath path;
		path.AddLine(	x+b,		y,
			x+width-b,	y	);
		path.AddBezier( x+width-b,		y,
						x+width-b/2,	y+b/8,
						x+width-b/8,	y+b/2,
						x+width,		y+b	);

		path.AddLine(	x+width,		y+b,
						x+width,		y+height);

		

		path.AddLine(	x+width,		y+height,
						x,				y+height);


		path.AddLine(	x,				y+height,
						x,				y+b);

		path.AddBezier( x,				y+b,
						x+b/8,			y+b/2,
						x+b/2,			y+b/8,
						x+b,			y	);

		Gdiplus::SolidBrush brush(color);
		g->FillPath(&brush, &path);
	}
	//-------------------------------------------------------
	void FillFormButtomInvert(int x, int y, int width, int height, int b=10)
	{
		
		Gdiplus::GraphicsPath path;

		path.AddLine(x,y, x+width, y);
		
		path.AddLine(x+width, y, x+width, y+height-b);

		path.AddBezier(	x+width,		y+height-b,
						x+width+b/6,	y+height-b/3,
						x+width+b/3,	y+height-b/6,
						x+width+b,		y+height);
		
		path.AddLine(x+width+b,	y+height, x-b, y+height);

		path.AddBezier(	x-b,	y+height,
						x-b/3,	y+height-b/6,
						x-b/6,	y+height-b/3,
						x,		y+height-b);

		//path.AddLine(x,		y+height-b, x, y+height+b);

		path.AddLine(x, y+height-b, x,y);
		
		Gdiplus::SolidBrush brush(color);
		g->FillPath(&brush, &path);

	}
    //-------------------------------------------------------
	wstring StringToWString(string str)
	{
		//CP_ACP
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		//выделяем память под wide char строку:
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwText, dwNum);
		wstring w = pwText;
		delete[] pwText;
		return w;
	}

	
};

#endif // UGC_H
