#pragma once
#include <windows.h>

class DPIX
{
protected:

public:
	static double dpix;

	DPIX()
	{
	}

	static int GetDPI() { return static_cast<int>(dpix * 96); }

	static void SetDPI(int dpi)
	{
		dpix = static_cast<double>(dpi) / 96.;
	}

	static void SetDisplayDPI()
	{
		HDC hdcScreen = ::GetDC(NULL);
		dpix = 1.; // assume failure
		if (hdcScreen)
		{
			dpix = (double)::GetDeviceCaps(hdcScreen, LOGPIXELSX);
			::ReleaseDC(NULL, hdcScreen);
			dpix /= 96;
		}
	}

	double GetValue(double val) const
	{
		return dpix * val;
	}
	int getIntegerValue(double val)
	{
		return static_cast<int>(dpix*val);
	}

	int operator ()(double val) const
	{
		return static_cast<int>(dpix*val);
	}

	operator double() const
	{
		return dpix;
	}

};