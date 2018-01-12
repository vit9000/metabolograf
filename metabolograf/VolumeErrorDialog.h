#pragma once
#include "Histogram.h"
#include <sstream>

// VolumeErrorDialog dialog

class VolumeErrorDialog : public CDialogEx
{
	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = undefined };
	#endif
	DECLARE_DYNAMIC(VolumeErrorDialog)

public:
	void Init();
	VolumeErrorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~VolumeErrorDialog();
	void Apply() { hist.Apply(); }

private:
	CComboBox combo;
	Histogram hist;

	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeCombo();
	DECLARE_MESSAGE_MAP()

};
