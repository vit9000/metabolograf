// BigPlotDialog.cpp: ���� ����������
//

#include "stdafx.h"
#include "metabolograf.h"
#include "BigPlotDialog.h"
#include "afxdialogex.h"

// ���������� ���� BigPlotDialog

IMPLEMENT_DYNAMIC(BigPlotDialog, CDialogEx)

BigPlotDialog::BigPlotDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BIGPLOT_DIALOG, pParent)
{
	
}

BigPlotDialog::~BigPlotDialog()
{
	
}

void BigPlotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BigPlotDialog, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &BigPlotDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &BigPlotDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_AET, &BigPlotDialog::OnBnClickedAet)
	ON_BN_CLICKED(IDC_AT, &BigPlotDialog::OnBnClickedAt)
	ON_BN_CLICKED(IDC_MOC, &BigPlotDialog::OnBnClickedMoc)
	ON_BN_CLICKED(ID_EXPORT_MPK_BUTTON, &BigPlotDialog::OnBnClickedExportMpkButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &BigPlotDialog::OnBnClickedClearButton)
END_MESSAGE_MAP()


int BigPlotDialog::DoModal(Plot* _plot, bool Export)
{
	if (!Export)
		_Export = Export;
	//MODE = mode;
	database = Database::getInstance();
	plot = _plot;
	
	bigPlot.LoadValues({ database->getHeader().AeT , database->getHeader().AT , database->getHeader().MCO });

	return CDialogEx::DoModal();
	
}

BOOL BigPlotDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);
	DPIX dpix;
	rect.top += 30 * static_cast<int>(dpix);
	bigPlot.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_BIG_PLOT);
	bigPlot.Init(plot);

	CButton *button1 = (CButton*)GetDlgItem(IDC_AET);
	CButton *button2 = (CButton*)GetDlgItem(IDC_AT);
	CButton *button3 = (CButton*)GetDlgItem(IDC_MOC);
	button1->SetCheck(true);

	SetWindowText("����������� �����");
	if (!plot->IsExperience())
	{
		button1->ShowWindow(SW_HIDE);
		button2->ShowWindow(SW_HIDE);
		button3->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLEAR_BUTTON)->ShowWindow(SW_HIDE);
		SetWindowText("������");
	}
	if(!_Export)
	{
		GetDlgItem(ID_EXPORT_MPK_BUTTON)->ShowWindow(SW_HIDE);
		
	}
	else
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		
	return TRUE;
}

void BigPlotDialog::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	CDialogEx::OnOK();
}

void BigPlotDialog::OnBnClickedCancel()
{
	// TODO: �������� ���� ��� ����������� �����������
	CDialogEx::OnCancel();
}


void BigPlotDialog::OnBnClickedAet()
{
	bigPlot.SetType(0);
}


void BigPlotDialog::OnBnClickedAt()
{
	bigPlot.SetType(1);
}


void BigPlotDialog::OnBnClickedMoc()
{
	bigPlot.SetType(2);
}
//+++++++++++++++++++++++++
//++++++++++++++++++++++++
void BigPlotDialog::ExportButtonClick()
{
	//A a = {};
	if (database->getCount() == 0) return;
	if (database->getHeader().StartTest <= 0 || database->getHeader().EndTest <= 0)
	{
		MessageBox("������� ����������, ��� ��� ������������ ��������� �������", "��������", MB_OK | MB_ICONINFORMATION);
		return;
	}

	string header = "��������-������";
	/*for(int i=0; i<3; i++)
		if (bigPlot.getValue(i) <= 0)
		{
			MessageBox("���������� ��������� ��� �����", "��������", MB_OK | MB_ICONINFORMATION);
			return;
		}
	*/
	database->getHeader().AeT = bigPlot.getValue(0);
	database->getHeader().AT = bigPlot.getValue(1);
	database->getHeader().MCO = bigPlot.getValue(2);

	Export *pictexport = new Export(database);
	

	FILE *file = pictexport->CreateExportFile(header, "html");
	if (file <= 0) return;

	string dir= pictexport->file_name.substr(0, pictexport->file_name.length()-5);
	CreateDirectory(dir.c_str(), NULL);

	vector<string> code;
	if (database->getHeader().AeT && database->getHeader().HR)
	{
		code.push_back("������ (���������� = \"20 ���\", y = ���, �������=\"������� ��������� ����������\"");
		code.push_back("������ (���������� = \"20 ���\", y = SD, �������=\"SD\"");
	}
	if (database->getHeader().AT)
	{
		code.push_back("������ (���������� = \"20 ���\", y = ��������������_����������_O2, �������=\"����.���������� �� O2\", y = ��������������_����������_CO2, �������=\"����.���������� �� CO2\")");
		code.push_back("������ (���������� = \"20 ���\", y = FetO2, �������=\"EtO2\", y = FetCO2, ������� = \"EtO2\"");
	}
	if (database->getHeader().MCO)
		code.push_back("������ (���������� = \"20 ���\", y = �����������_O2_��_��_���, �������=\"����������� O2 (��/��/���)\")");


	pictexport->WriteHeader(file);
	char bbuf[1024];

	for(size_t i=0; i<code.size(); ++i)
	{
		string pictname = dir + string("\\plot") + to_string(i+1) + string(".png");
		CImage image;
		Plot plot(800,300);
		plot.SetExperience(true);
		plot.SetMarkPosByTableIndex(database, 0, database->getHeader().AeT);
		plot.SetMarkPosByTableIndex(database, 1, database->getHeader().AT);
		plot.SetMarkPosByTableIndex(database, 2, database->getHeader().MCO);

		//plot.Set
		plot.Run(database, code[i]);
		
		plot.DrawPlot();
		image.Attach(plot.getHBITMAP());
		image.Save(pictname.c_str(), ImageFormatPNG);
		sprintf(bbuf, "<br><table bordercolor=white >\n <tr><td><img src='%s' width=%d></table>\n", pictname.c_str(), pictexport->page_width);
		fwrite(bbuf, strlen(bbuf), 1, file);
	}


	
	

	//int len = pictname.length();
	/*
	int i = 0;
	for (i = dir.length(); i>0; i--)
	{
		if (dir[i] == '\\') break;

	}
	//wcscpy(dir, dir + i + 1);
	*/

	
	
	string temp = ExportMPKTable();
	fwrite(temp.c_str(), temp.length(), 1, file);
	//ExportPANOTable(file);
	
	pictexport->CloseFile(file);
	delete  pictexport;
	pictexport = NULL;

	CDialogEx::OnOK();
	
}
//---------------------------------------------------------------------------

void BigPlotDialog::print(std::iostream& out, const PrintOptions& options)
{
	double KER = (options.power>0) ? (double)options.delta / (double)options.power : 0;
	double KPD = (options.power>0) ? (double)options.mop / (double)options.power : 0;
	string template_ = "<td align=center>";
	out.precision(2);
	out << fixed;
	out << "<tr>"
		<< template_ << options.time;
	if (database->getHeader().HR)
	{
		out << template_ << static_cast<int>(options.HR)
			<< template_ << static_cast<int>(60000. / options.HR)
			<< template_ << static_cast<int>(options.SD);
	}
	out	<< template_ << options.mop
		<< template_ << options.mcop
		<< template_ << options.VO2
		<< template_ << options.VCO2
		<< template_ << options.EtO2
		<< template_ << options.EtCO2	
		<< template_ << (double)options.mop / database->getHeader().PatientWeight
		<< template_ << static_cast<int>(options.power)
		<< template_ << options.power / database->getHeader().PatientWeight
		<< template_ << (to_string(options.delta))
		<< template_ << KER
		<< template_ << KPD;

	
}

void BigPlotDialog::PrintEnd2Minutes(std::iostream& out, int lastmop)
{
	{//��������� ������� ��������� �� ������ �����������
	 //�� ������������
		MTime time = database->getDatetime(database->getHeader().EndTest).getTime() + 120;//���������� 2 ������ ����� ������������
		MTime time30 = database->getDatetime(database->getHeader().EndTest).getTime() + 30;//���������� 2 ������ ����� ������������
		PrintOptions options;
		//int count = 0;
		int start = 0;
		int end = 0;
		int end30 = 0;
		for (end30 = start = end = database->getHeader().EndTest; end < database->getCount(); ++end)// ���� ���������� �����
		{
			//MTime temp = database->datetime[end].getTime();
			if (database->getDatetime(end).getTime() < time30)
				end30++;
			if (database->getDatetime(end).getTime() >= time)
				break;
			
		}
		
		--end;
		options.mop = database->getVariable("��������_�����������_O2").mean(start, end)*1000.;
		options.mcop = database->getVariable("��������_���������_CO2").mean(start, end)*1000.;
		if (database->getHeader().HR)
		{
			options.HR = database->getVariable("���").mean(start, end);
			options.SD = database->getVariable("SD").mean(start, end30);//(60000. / database->variables["���"]).SD(start, end30);
		}
		options.VO2 = database->getVariable("��������������_����������_O2").mean(start, end);
		options.VCO2 = database->getVariable("��������������_����������_CO2").mean(start, end);
		options.EtO2 = database->getVariable("FetO2").mean(start, end);
		options.EtCO2 = database->getVariable("FetCO2").mean(start, end);


		options.power = 0;
		options.time = "2 ������ ����� �����";
		print(out, options);
	}
}

void BigPlotDialog::Print1Minute(std::iostream& out)
{
	{//��������� ������� ��������� �� ������ �����������
	 //�� ������������
		PrintOptions options;
		MTime time = database->getDatetime(database->getHeader().StartTest).getTime() - 60;//���������� 1 ������ �� ������ ������������
		MTime time30 = database->getDatetime(database->getHeader().StartTest).getTime() - 30;
		//int count = 0;
		int start = 0;
		int end = 0;
		int start30=0;
		for (start30 = end = start = database->getHeader().StartTest; start >= 0; --start)// ���� ���������� �����
		{
			MTime temp = database->getDatetime(start).getTime();
			if (database->getDatetime(start).getTime() > time30)
				--start30;

			if (database->getDatetime(start).getTime() <= time)
				break;
			
		}
		
		if (start < 0) start = 0;
		options.mop = database->getVariable("��������_�����������_O2").mean(start, end)*1000.;
		options.mcop = database->getVariable("��������_���������_CO2").mean(start, end)*1000.;
		if (database->getHeader().HR)
		{
			options.HR = database->getVariable("���").mean(start, end);
			options.SD = database->getVariable("SD").mean(start30, end);//(60000./database->variables["���"]).SD(start, end);
			
		}
		options.VO2 = database->getVariable("��������������_����������_O2").mean(start, end);
		options.VCO2 = database->getVariable("��������������_����������_CO2").mean(start, end);
		options.EtO2 = database->getVariable("FetO2").mean(start, end);
		options.EtCO2 = database->getVariable("FetCO2").mean(start, end);


		options.power = 0;
		options.time = "1 ������ �� �����";
		print(out, options);
	}
}


std::string  BigPlotDialog::ExportMPKTable()
{
	
	int zerotime_index = database->getHeader().StartTest;
	int end_index = database->getHeader().EndTest;
	
	Variable_ <MTime> varTime;// = database->GetTimeFromZero(zerotime_index).getWithoutUnchecked(checked);//����������� ����� 
	Variable_<uint8_t> checked(database->getCount(), 0);

	for (int i = zerotime_index; i <= end_index; ++i)
	{
		if (database->getChecked(i))
		{
			varTime.append(database->getDatetime(i).getTime());
			checked[i] = true;
		}
	}

	Algorithm alg;
	auto intervals = alg.getIntervalsByTime(varTime, "30 sec");
	Variable mop = database->getVariable("��������_�����������_O2").getWithoutUnchecked(checked).meanByIntervals(intervals);
	Variable mcop = database->getVariable("��������_���������_CO2").getWithoutUnchecked(checked).meanByIntervals(intervals);
	

	Variable HR;
	Variable SD;
	if (database->getHeader().HR)
	{
		HR = database->getVariable("���").getWithoutUnchecked(checked).meanByIntervals(intervals);
		SD = database->getVariable("SD").getWithoutUnchecked(checked).meanByIntervals(intervals);
	}
	
	Variable VO2 = database->getVariable("��������������_����������_O2").getWithoutUnchecked(checked).meanByIntervals(intervals);
	Variable VCO2 = database->getVariable("��������������_����������_CO2").getWithoutUnchecked(checked).meanByIntervals(intervals);
	Variable EtO2 = database->getVariable("FetO2").getWithoutUnchecked(checked).meanByIntervals(intervals);
	Variable EtCO2 = database->getVariable("FetCO2").getWithoutUnchecked(checked).meanByIntervals(intervals);

	
	std::stringstream out;
	
	out << "<br><table bordercolor=black border=1 style='border-bottom=none'> \
			<tr><td colspan=17 align=center bgcolor=black><font size=4 color=white>���������� ���������</font>";
	string template_ = "<td align=center valign=top><font size=2>";
	out << "<tr>"
		<<	template_<< "<b>����� �����</b>"
		<< (database->getHeader().HR ? template_+string("<b>���, 1/���.</b>") : "")
		<< (database->getHeader().HR ? template_ + string("<b>RR, ��</b>") : "")
		<< (database->getHeader().HR ? template_ + string("<b>SD, ��</b>") : "")
		<< template_ << "<b>�������. O2 (��/���.)</b>"
		<< template_ << "<b>�����. CO2 (��/���.)</b>"
		<< template_ << "<b>��O2</b>"
		<< template_ << "<b>��CO2</b>"
		<< template_ << "<b>EtO2</b>"
		<< template_ << "<b>EtCO2</b>"
		<< template_ << "<b>�������. O2/��, ��/���/�� ����� ����</b>"
		<< template_ << "<b>W, ��</b>"
		<< template_ << "<b>W/��, ��/�� ����� ����</b>"
		<< template_ << "<b>�����-����� �����-������ �� 1 ���.</b>"
		<< template_ << "<b>�����-������ ����-������ ������ (���)</b>"
		<< template_ << "<b>�������� ���=<br>�����-����� �2 (��/���.)/ �������� (��)</b>";
	
	Print1Minute(out);

	MTime time_research(0,0,30);
	size_t size = mop.size();//�������� � ����� 2 ������
	PrintOptions options;
	
	for (int i = 0; i < size; ++i)
	{
		//������� ����������� �2 � ��������� ��2
		options.mop = static_cast<int>(mop[i] * 1000);
		options.mcop = static_cast<int>(mcop[i] * 1000);
		options.VO2 = VO2[i];
		options.VCO2 = VCO2[i];
		options.EtO2 = EtO2[i];
		options.EtCO2 = EtCO2[i];
		
		if (database->getHeader().HR)
		{
			options.HR = HR[i];
			options.SD = SD[i];
		}
		//����� ����� ��� �������
		options.time = time_research.getString();
		time_research = time_research + 30; // ����������� ����� ������������ �� 30 ������ (������������� ��������+)
		
		// ������. �������. �� 1 ������
		options.delta = 0;
		if (i >= 4)
			options.delta = (mop[i] * 1000 + mop[i - 1] * 1000) / 2 - (mop[i - 2] * 1000 + mop[i - 3] * 1000) / 2;

		// ��������
		options.power = database->getHeader().PowerStep * (i/ 4 + 1);
		
		print(out, options);
	}
	
	PrintEnd2Minutes(out, options.mop);
	out << "</table>";
	//------
	double imt = 1;
	if (database->getHeader().PatientSex == 1)
		imt = 50. + (database->getHeader().PatientHeight - 150.) *0.75 + (database->getHeader().PatientAge - 21.) / 4.;
	else
		imt = 50. + (database->getHeader().PatientHeight - 150.) * 0.32 + (database->getHeader().PatientAge - 24.) / 5.;
	//------

	
	
	if (database->getHeader().AeT > 0)
	{
		out << "<br><table bordercolor=black border=1 style='border-bottom=none'><tr><td colspan=2 align=center bgcolor=black><font size=4 color=white>����� ��������� ��������� (���)</font>";
		out << "<tr><td>����� ���������� ��� (AeT)<td align=center>" << (database->getDatetime(database->getHeader().AeT).getTime() - database->getDatetime(database->getHeader().StartTest).getTime()).getStringAlt();
		double MOP = database->getVariable("��������_�����������_O2")[database->getHeader().AeT];
		out << "<tr><td>����������� �2 �� ��� (AeT), �/���.<td align=center>" << MOP;
		if (database->getHeader().HR)
		{
			out << "<tr><td>��� ��� ��� (AeT), 1/���.<td align=center>" << static_cast<int>(database->getVariable("���")[database->getHeader().AeT]);
			out << "<tr><td>SD ��� ��� (AeT), ��<td align=center>" << static_cast<int>(database->getVariable("SD")[database->getHeader().AeT]);
			//SD ���  AeT, ��
		}
		out << "<tr><td>������������� ����������� O2 �� ��� (AeT), ��/���/�� ����� ����<td align=center>" << (MOP * 1000 / database->getHeader().PatientWeight);
		out << "<tr><td>������������� ����������� O2 �� ��� (AeT), ��/���/�� ��������� ����� ����<td align=center>" << (MOP * 1000 / imt);
		int pos = FindInterval(intervals, database->getHeader().AeT - database->getHeader().StartTest);
		double power = database->getHeader().PowerStep * (pos / 4 + 1);
		out << "<tr><td>�������� �� ��� (AeT), ��<td align=center>" << int(power);
		out << "<tr><td>������������� �������� �� ��� (AeT), ��/�� ����� ����<td align=center>" << ((double)power / (double)database->getHeader().PatientWeight);
		out << "</table>";
	}
	if (database->getHeader().AT)
	{
		out << "<br><table bordercolor=black border=1 style='border-bottom=none'><tr><td colspan=2 align=center bgcolor=black><font size=4 color=white>����� ����������� ��������� (����)</font>";
		out << "<tr><td>����� ���������� ���� (AnT)<td align=center>" << (database->getDatetime(database->getHeader().AT).getTime() - database->getDatetime(database->getHeader().StartTest).getTime()).getStringAlt();
		double MOP = database->getVariable("��������_�����������_O2")[database->getHeader().AT];
		out << "<tr><td>����������� �2 �� ���� (AnT), �/���.<td align=center>" << MOP;
		if (database->getHeader().HR)
		{
			out << "<tr><td>��� ��� ���� (AnT), 1/���.<td align=center>" << static_cast<int>(database->getVariable("���")[database->getHeader().AT]);
			out << "<tr><td>SD ��� ���� (AnT), ��<td align=center>" << static_cast<int>(database->getVariable("SD")[database->getHeader().AT]);
		}
		//SD ���  AnT, ��
		out << "<tr><td>������������� ����������� O2 �� ���� (AnT), ��/���/�� ����� ����<td align=center>" << (MOP * 1000 / database->getHeader().PatientWeight);
		out << "<tr><td>������������� ����������� O2 �� ���� (AnT), ��/���/�� ��������� ����� ����<td align=center>" << (MOP * 1000 / imt);
		int pos = FindInterval(intervals, database->getHeader().AT - database->getHeader().StartTest);
		double power = database->getHeader().PowerStep * (pos / 4 + 1);
		out << "<tr><td>�������� �� ���� (AnT), ��<td align=center>" << int(power);
		out << "<tr><td>������������� �������� �� ���� (AnT), ��/�� ����� ����<td align=center>" << ((double)power / (double)database->getHeader().PatientWeight);
		out << "</table>";
	}

	if (database->getHeader().MCO)
	{
		out << "<br><table bordercolor=black border=1 style='border-bottom=none'><tr><td colspan=2 align=center bgcolor=black><font size=4 color=white>������������ ����������� ��������� (���)</font>";
		out << "<tr><td>����� ���������� ���<td align=center>" << (database->getDatetime(database->getHeader().MCO).getTime() - database->getDatetime(database->getHeader().StartTest).getTime()).getStringAlt();
		double MOP = database->getVariable("��������_�����������_O2")[database->getHeader().MCO];
		out << "<tr><td>���, �/���.<td align=center>" << MOP;
		if (database->getHeader().HR)
		{
			out << "<tr><td>��� ��� ���, 1/���.<td align=center>" << static_cast<int>(database->getVariable("���")[database->getHeader().MCO]);
			out << "<tr><td>SD ��� ���, ��<td align=center>" << static_cast<int>(database->getVariable("SD")[database->getHeader().MCO]);
			//SD ���  ���, ��
		}
		out << "<tr><td>������������� ���, ��/���/�� ����� ����<td align=center>" << (MOP * 1000 / database->getHeader().PatientWeight);
		out << "<tr><td>������������� ���, ��/���/�� ��������� ����� ����<td align=center>" << (MOP * 1000 / imt);
		int pos = FindInterval(intervals, database->getHeader().MCO - database->getHeader().StartTest);
		double power = database->getHeader().PowerStep * (pos / 4 + 1);
		out << "<tr><td>������������ �������� ��������, ��<td align=center>" << int(power);
		out << "<tr><td>������������� ������������ �������� ��������, ��/�� ����� ����<td align=center>" << ((double)power / (double)database->getHeader().PatientWeight);
		out << "</table>" << endl;
	}
	
	

	
	


	return out.str();
}
//-----------------------------------------------------------------------

void BigPlotDialog::OnBnClickedExportMpkButton()
{
	ExportButtonClick();
}


void BigPlotDialog::OnBnClickedClearButton()
{
	bigPlot.ClearMarks();
}
