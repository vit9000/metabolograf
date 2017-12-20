// NutritionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "metabolograf.h"
#include "NutritionDialog.h"
#include "afxdialogex.h"


// NutritionDialog dialog

IMPLEMENT_DYNAMIC(NutritionDialog, CDialogEx)

NutritionDialog::NutritionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUSTOM_NUTRITION_DIALOG, pParent)
{

}

NutritionDialog::~NutritionDialog()
{
}

void NutritionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO1, comboBox);
	DDX_Control(pDX, IDC_EDIT_1, edit1);
	DDX_Control(pDX, IDC_EDIT_2, edit2);
	DDX_Control(pDX, IDC_EDIT_3, edit3);
	DDX_Control(pDX, IDC_STATIC_1, label1);
	DDX_Control(pDX, IDC_STATIC_2, label2);
	DDX_Control(pDX, IDC_STATIC_3, label3);
	
	DDX_Control(pDX, IDC_PREVIEW1, preview1);
	DDX_Control(pDX, IDC_PREVIEW2, preview2);
}





BEGIN_MESSAGE_MAP(NutritionDialog, CDialogEx)
	
	ON_CBN_SELCHANGE(IDC_COMBO1, &NutritionDialog::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &NutritionDialog::OnBnClickedOk)
	
	ON_EN_CHANGE(IDC_EDIT_1, &NutritionDialog::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_2, &NutritionDialog::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_3, &NutritionDialog::OnEnChangeEdit)
	ON_BN_CLICKED(IDINFO, &NutritionDialog::OnBnClickedInfo)
END_MESSAGE_MAP()


// NutritionDialog message handlers

BOOL NutritionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	comboBox.AddString("����� � ��������� ������������ �,�,�");
	comboBox.AddString("����� � ������������ � ������� �����");
	comboBox.AddString("����� ������ �� ��� � ����.����� �����");
	comboBox.SetCurSel(0);
	Select(0);
	preview1.SetWindowTextA("");
	preview2.SetWindowTextA("");


	
	/*
	if (PatientAge >= 0.01 && PatientAge<0.06)
		UreaNormal->Caption = "�����: 10-17 �����/���";
	if (PatientAge >= 0.06 && PatientAge<1)
		UreaNormal->Caption = "�����: 33-67 �����/���";
	if (PatientAge >= 1 && PatientAge<4)
		UreaNormal->Caption = "�����: 67-133 �����/���";
	if (PatientAge >= 4 && PatientAge<8)
		UreaNormal->Caption = "�����: 133-200 �����/���";
	if (PatientAge >= 8 && PatientAge <= 15)
		UreaNormal->Caption = "�����: 200-300 �����/���";
	else
		UreaNormal->Caption = "�����: 333-587,7 �����/���";

	koefficient->Caption = "1," + AnsiString(KoefBar->Position);
	*/
	return TRUE;
}


void NutritionDialog::Select(int index)
{
	label1.ShowWindow(SW_HIDE);
	label2.ShowWindow(SW_HIDE);
	label3.ShowWindow(SW_HIDE);
	edit1.ShowWindow(SW_HIDE);
	edit2.ShowWindow(SW_HIDE);
	edit3.ShowWindow(SW_HIDE);
	edit1.SetWindowTextA("");
	edit2.SetWindowTextA("");
	edit3.SetWindowTextA("");
	preview1.SetWindowTextA("");
	preview1.SetWindowTextA("");

	switch (index)
	{
	case 0:
		label1.SetWindowTextA("����� (%)");
		label1.ShowWindow(SW_SHOW);

		label2.SetWindowTextA("���� (%)");
		label2.ShowWindow(SW_SHOW);

		label3.SetWindowTextA("�������� (%)");
		label3.ShowWindow(SW_SHOW);
		edit1.ShowWindow(SW_SHOW);
		edit2.ShowWindow(SW_SHOW);
		edit3.ShowWindow(SW_SHOW);
		edit1.Clear();
		edit2.Clear();
		edit3.Clear();
		break;
	case 1:
		label1.SetWindowTextA("�������� �������� (�����)");
		label1.ShowWindow(SW_SHOW);
		edit1.ShowWindow(SW_SHOW);
		edit1.Clear();
		label2.SetWindowTextA("����.����� ���.������ �����");
		label2.ShowWindow(SW_SHOW);
		edit2.ShowWindow(SW_SHOW);
		edit2.SetWindowTextA("1.0");
		
		break;
	case 2:

		label1.SetWindowTextA("���");
		label1.ShowWindow(SW_SHOW);
		edit1.ShowWindow(SW_SHOW);
		edit1.SetWindowTextA("150");
		
		break;
	}
}

void NutritionDialog::OnCbnSelchangeCombo1()
{
	Select(comboBox.GetCurSel());
}


void NutritionDialog::Calculate(bool save)
{
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	// TODO: �������� ���� ��� ����������� �����������
	Export *dietexport = new Export(database);
	dietexport->CalculateMeanValues();

	int index = comboBox.GetCurSel();
	preview1.SetWindowTextA("");
	preview2.SetWindowTextA("");

	if (index == 0)
	{
		double CH = 1;
		double F = 1;
		double P = 1;
		if (edit1.GetWindowTextLengthA() == 0 || edit2.GetWindowTextLengthA() == 0 || edit3.GetWindowTextLengthA() == 0)
		{
			
			goto END;
		}
		CString temp;
		edit1.GetWindowTextA(temp);
		P = atof(temp.GetBuffer());
		edit2.GetWindowTextA(temp);
		F = atof(temp.GetBuffer());
		edit3.GetWindowTextA(temp);
		CH = atof(temp.GetBuffer());
		
		
		if (P + F + CH != 100)
			goto END;

		double DUK = CH / (F + P);
		double NBK = ((CH + F)*4.1) / (P);
		double BrK = 0;
		if (!save)
		{
			double BK = 0;
			double p_body = 0;
			dietexport->CalcPFCH(&P, &F, &CH, &p_body, &BK, dietexport->MOP24, DUK, NBK, &BrK);
			char bbuf[1000];
			sprintf(bbuf, "�������������� �������� %d ����", (int)p_body);
			CString t(bbuf);
			preview1.SetWindowTextA(t);
			sprintf(bbuf, "�=%d �, �=%d �, �=%d �., ��������� ��: %.1lf", (int)P, (int)F, (int)CH, BrK);
			t = CString(bbuf);
			preview2.SetWindowTextA(t);
		}
		else
			dietexport->CustomDietaExport(NBK, DUK);

		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	}        //
	else if (index == 1)
	{
		double k = 1;
		double u = 1;
		//k = CustomDietForm->koefficient->Caption.ToDouble();;
		//u = CustomDietForm->UBox->Text.ToDouble();

		CString temp;
		edit1.GetWindowTextA(temp);
		u = atof(temp.GetBuffer());
		edit2.GetWindowTextA(temp);
		k = atof(temp.GetBuffer());
		
		if (k <= 0 || u <= 0) goto END;


		if (!save)
		{
			double CH = 0;
			double P = 0;
			double F = 0;
			double BK = 0;
			double p_body = 0;
			dietexport->CalcPFCHUrea(&P, &F, &CH, &p_body, dietexport->MOP24, dietexport->MCOP24, u, k);
			char bbuf[100];


			if (CH <= 0 || F <= 0)
			{
				sprintf(bbuf, "���. �����������: %.1lf", dietexport->breathK_sredn);
				CString t(bbuf);
				preview1.SetWindowTextA(t);
				preview2.SetWindowTextA("����������� ������� ���");
			}
			else
			{
				char bbuf[1000];
				sprintf(bbuf, "�������������� �������� %d ����", (int)p_body);
				CString t(bbuf);
				preview1.SetWindowTextA(t);
				sprintf(bbuf, "�=%d �, �=%d �, �=%d �.", (int)P, (int)F, (int)CH);
				t = CString(bbuf);
				preview2.SetWindowTextA(t);


				GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
			}
			
		}
		else
		{
			dietexport->UreaDietaExport(u, k);
			GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		}

		

	}
	else if (index == 2)
	{
		//int NBK = NBKBar->Position;
		CString temp;
		edit1.GetWindowTextA(temp);
		int NBK = atof(temp.GetBuffer());
		if (NBK <= 0) goto END;

		if (!save)
		{
			double CH = 0;
			double P = 0;
			double F = 0;

			double p_body = 0;
			dietexport->CalcPFCH_NBK(&P, &F, &CH, &p_body, dietexport->MOP24, dietexport->MCOP24, NBK);
			char bbuf[100];
			if (CH <= 0 || F <= 0)
			{
				sprintf(bbuf, "���. �����������: %.1lf", dietexport->breathK_sredn);
				CString t(bbuf);
				preview1.SetWindowTextA(t);
				preview2.SetWindowTextA("����������� ������� ���");
			}
			else
			{
				char bbuf[1000];
				sprintf(bbuf, "�������������� �������� %d ����", (int)p_body);
				CString t(bbuf);
				preview1.SetWindowTextA(t);
				sprintf(bbuf, "�=%d �, �=%d �, �=%d �.", (int)P, (int)F, (int)CH);
				t = CString(bbuf);
				preview2.SetWindowTextA(t);


				GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			dietexport->CustomDietaExport(NBK, 0);
			GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		}

	}

END:

	delete dietexport;
	dietexport = NULL;
}

void NutritionDialog::OnBnClickedOk()
{
	Calculate(true);
	CDialogEx::OnOK();
}



void NutritionDialog::OnEnChangeEdit()
{
	Calculate(false);
	// TODO:  ���� ��� ������� ���������� RICHEDIT, �� ������� ���������� �� �����
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  �������� ��� �������� ����������
}


void NutritionDialog::OnBnClickedInfo()
{
	CString msg;
	switch (comboBox.GetCurSel())
	{
	case 0:
		msg = "���������� ������ ����������� ������, ����� � ���������. ��������, �����: 30, ����: 30, ��������: 40. ��������� ����������� ������ ��������������� 100%.";
		break;
	case 1:
	{
		double PatientAge = database->hdata.PatientAge;
		if (PatientAge >= 0.01 && PatientAge<0.06)
			msg = "����� ��������: 10-17 �����/���";
		if (PatientAge >= 0.06 && PatientAge<1)
			msg = "����� ��������: 33-67 �����/���";
		if (PatientAge >= 1 && PatientAge<4)
			msg = "����� ��������: 67-133 �����/���";
		if (PatientAge >= 4 && PatientAge<8)
			msg = "����� ��������: 133-200 �����/���";
		if (PatientAge >= 8 && PatientAge <= 15)
			msg = "����� ��������: 200-300 �����/���";
		else
			msg = "����� ��������: 333-587,7 �����/���";
	}
		break;
	case 2:
		msg = "������������� �������� ���: 50-300";
		break;
	}
	MessageBox(msg, "����������", MB_OK);
}
