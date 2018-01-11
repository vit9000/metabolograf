#pragma once
#include "afxwin.h"
#include "MTime.h"
#include "ugc.h"
#include "Database.h"


class TimerWindow :
	public CWnd
{
public:
	TimerWindow();
	~TimerWindow();

	void StartRec();
	void StartTest();
	void StopRec();
	void StopTest();
	void Update();

protected:
	UINT_PTR timer;
	int Width;
	int Height;
	MTime start_test;
	MTime start_rec;
	string test;
	string rec;
	int FontSize;

	//functions
	void Default() {
		start_rec.Zero();
		start_test.Zero();
		rec = start_rec.getStringAlt();
		test.clear();
	};
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
	void SetBounds();
	void OnTimer(UINT_PTR nIdEvent);
	DECLARE_MESSAGE_MAP()
};

