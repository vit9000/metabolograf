#pragma once
#include "Histogram.h"
#include <sstream>

// VolumeErrorDialog dialog

class VolumeErrorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(VolumeErrorDialog)

public:
	void Init(Database *database);
	VolumeErrorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~VolumeErrorDialog();
	void Apply() { hist.Apply(); }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = undefined };
#endif

protected:
	CComboBox combo;
	Histogram hist;
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo();
};
