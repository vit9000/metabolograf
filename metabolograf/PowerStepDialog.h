#pragma once

#include "afxwin.h"
#include <string>
#include <sstream>
// PowerStepDialog dialog

class PowerStepDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PowerStepDialog)

public:
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
	BOOL OnInitDialog();
	PowerStepDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~PowerStepDialog();
	CEdit edit;
	CSliderCtrl slider;
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWER_STEP_DIALOG};
#endif

protected:
	int pos=0;
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMReleasedcaptureSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditbox();
};
