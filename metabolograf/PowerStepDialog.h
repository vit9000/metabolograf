#pragma once

#include "afxwin.h"
#include <string>
#include <sstream>
// PowerStepDialog dialog

class PowerStepDialog : public CDialogEx
{
	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_POWER_STEP_DIALOG };
	#endif
	DECLARE_DYNAMIC(PowerStepDialog)

public:
	PowerStepDialog(CWnd* pParent = NULL) { pos = 0; }   // standard constructor
	virtual ~PowerStepDialog() {}
	BOOL OnInitDialog();

	void update()
	{
		char temp[10];
		sprintf(temp, "%d", slider.GetPos());
		edit.SetWindowTextA(temp);
	}
	int getValue()
	{
		return pos;
	}
	
	
protected:
	CEdit edit;
	CSliderCtrl slider;
	int pos=0;

	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMReleasedcaptureSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditbox();
};
