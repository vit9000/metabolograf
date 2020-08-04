#pragma once
#include "BigPlot.h"
#include "Export.h"
#include "DPIX.h"

// диалоговое окно BigPlotDialog
class BigPlotDialog : private CDialogEx
{

	bool m_bExport;
	Plot* m_pPlot;
	Database* m_pDatabase;
	BigPlot m_BigPlot;
	
	DECLARE_DYNAMIC(BigPlotDialog)
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_BIGPLOT_DIALOG };
	#endif

public:
	BigPlotDialog(CWnd* pParent = NULL);   
	virtual ~BigPlotDialog();
	int DoModal(Plot* _plot, bool Export = false);
	const BigPlot& getBigPlot() { return m_BigPlot; }
	afx_msg void OnBnClickedExportMpkButton();
	afx_msg void OnBnClickedClearButton();
protected:

	struct PrintOptions
	{
		string time = ""; 
		int mop = 0; 
		int mcop = 0; 
		int delta = 0;
		double power = 0; 
		double HR = 0;
		double VO2 = 0;
		double VCO2 = 0;
		double EtO2 = 0;
		double EtCO2 = 0;
		int SD = 0;	
	};

	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX); 
	void ExportButtonClick();
	std::string ExportMPKTable();
	void print(std::iostream& out, const PrintOptions& options);
	void Print1Minute(std::iostream& out);
	void PrintEnd2Minutes(std::iostream& out, int lastmop);
	int FindInterval(vector<int>& v, int p);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAet();
	afx_msg void OnBnClickedAt();
	afx_msg void OnBnClickedMoc();
	DECLARE_MESSAGE_MAP()

};
