#pragma once

const int MinutValueSpiro = 30000;	// ���-�� ������� ���������� �� ������
const int MinutValueCapno = 1500;	// ���-�� ������� ���������� �� ������
const int MaxCikl = 10000;			// ������������ ���-�� ����������� ������


struct CIKL_CAPNO // ����������� ���� ����������
{
	float Vi;		// ����� �����
	float Ve;		// ����� ������

	int EtCO2;		// ������������ CO2 � ����� ������ (��)

	float FiCO2pr;	// ������������ CO2 �� ����� (% x 10)
	float EtCO2pr;	// ������������ CO2 � ����� ������ (% x 10)
	float FiO2pr;	//������� �2 �� �����
	float EtO2pr;	//������� �2 �� ������

	int CHD;		//������� �������

	int BegI;	//������ �����
	int BegE;	//������ ������
	int Li;		//����� �����
	int Le;		//����� ������
};



struct CIKL_SPIRO // ����������� ���� ����������
{
	float Vi;		// ����� �����
	float Ve;		// ����� ������

	int EtCO2;		// ������������ CO2 � ����� ������ (��)

	float FiCO2pr;	// ������������ CO2 �� ����� (% x 10)
	float EtCO2pr;	// ������������ CO2 � ����� ������ (% x 10)
	float FiO2pr;	//������� �2 �� �����
	float EtO2pr;	//������� �2 �� ������

	float VEO_FiCO2pr;	// VEO_������������ CO2 �� ����� (% x 10)
	float VEO_EtCO2pr;	// VEO_������������ CO2 � ����� ������ (% x 10)
	float VEO_FiO2pr;	// VEO_������� �2 �� �����
	float VEO_EtO2pr;	// VEO_������� �2 �� ������

	float VEO_FiAX1pr;	// VEO_������������ AX1 �� ����� (% x 10) 
	float VEO_EtAX1pr;	// VEO_������������ AX1 � ����� ������ (% x 10)
	float VEO_FiAX2pr;	// VEO_������� AX2 �� �����
	float VEO_EtAX2pr;	// VEO_������� AX2 �� ������

	int CHD;		//������� �������

	int delta;		//����� �����

	int HR;
	int RR;

	int begi;
	int ende;

	float VO2;
	float VCO2;
	float MTB;
};


struct METAB_DATA
{
	int AmntCikl;						//���-�� ����������� ������
	CIKL_SPIRO CiklSpiro[MaxCikl];		//������ �������� ������������ �����

	int prEndCiklSpiro;					//������� ��������� ������������ ����� �� ����������
	int prEndCiklCapno;					//������� ��������� ������������ ����� �� ����������
	int WaitNewCiklGas;					//������� �������� ������ ����� �� ����������
	int WaitNewConc;					//������� �������� �����  ����� �� ����������

	LONG spBytes;						//���������� ���� �����
	int FlowPtr;						//��������� �� ������� ������ � ������
	int FlowBase;						//�������� ������
	int adcFlow[MinutValueSpiro];		//������ ������ �� ������ (���)
	float Flow[MinutValueSpiro];		//������ ������ �� ������
	float Volume[MinutValueSpiro];		//������ ������� �� ������	

	int PresPtr;
	int PresBase;						//�������� ��������
	int adcPres[MinutValueSpiro];
	float Pres[MinutValueSpiro];

	LONG gasBytes;							//���������� ���� �����
	int PtrCapno;							//��������� �� ������� ����������� � ����������
	float Kapnogramma[MinutValueCapno];		//������� �������� ����������� �� ������
	float Oxigramma[MinutValueCapno];		//������� �������� ����������  �� ������
	float Oxigramma0;						//������� �������� ����������	
	float Kapnogramma0;						//������� �������� �����������

	int FiCO2;								// ������������ CO2 �� ����� (��.��.��.)
	int EtCO2;								// ������������ CO2 � ����� ������ (��.��.��.)
	float FiCO2pr;							// ������������ CO2 �� ����� (% x 10)
	float EtCO2pr;							// ������������ CO2 � ����� ������ (% x 10)

	int CHD;								//������� �������
	float FiO2pr;							//������� �2 �� �����
	float EtO2pr;							//������� �2 �� ������

	int MSS;	//��� ��������� ������� ������� �������� �����
				// 1 - ����������� �������� ������ (�� ���������)
				// 0 - ������ ������� �������� �����
	int NoLine;	// 1 - ����� ������ ����� ��� ������� �������� ����� �� ����������
	int OCC;	// 1 - ����� �������������� ������� ������ ��������� � ��������� "��������"
	int Apnea;	// 1 - ������� �������� �� �������������� � ������� 20 ���.
	int Wave;	// 1 - ���������� ������� ����������� (� 4-5 �����)
				// 0 - ���������� ������� ����������  (� 4-5 �����)
	int I_E;	// 1 - ��� ����
				// 0 - ��� �����
	int Cal0;	// 1 - ��� ������� ���������� ������� ������������ CO2
	int CalG;	// 1 - ��� ������� ���������� �������� ������ ��������� CO2

	int Run;	// 1 - ��������� ������
				// 0 - ��������� ��������
	int BTPS;	// 1 - ���������� ����� BTPS
				// 0 - �� ���������� ����� BTPS
	int NEO;	// 1 - ������ �������� � ������������ ������ 
				// 0 - �������������� �� �������� ������ (���������)
	int MNT;	// 1 - ������� ����� ����
				// 0 - ������� ����� ����
	int RTE;	// 1 - ������� �������� RTE (���������)  
				// 0 - �������� �������� RTE  
	int Optic;	// �� ������������
	int Clbr0;	// 1 ��������� �������� ���������� ������� ������������ CO2
				// 0 ������������ ����� ����������.
	int MSTR;	// 1 - Master
				// 0 - Slave (���������)

	int No_O2;	// 1 - ������ ��������� �� ��������� � ������
	int Er_O2;	// 1 - ������ ��������� ���������, �� ��������� ��� ������

	int Pkam;	// ������� �������� � ������������� ������ (��.��.��.)
	int Patm;	// ������� ����������� �������� (��.��.��.)
	int Tkam;	// ������� ����������� � ������������� ������ CO2 (����. C)

	float UAnalog[5];	// ���������� ���������� ��������

	float Upump;		// ������� ���������� �� ����������� (� * 10)
	int Ipump;			// ������� ������������ ��� ������������ (��)	
	int Pump_Run_Time;	// ������� ��������� ����������� (�)
	int After_Cal;		// ����� �� ��������� ���������� �������� CO2 (�)

	int Ser_Num;		// �������� ����� ������
	float SW_Rev;		// ������ ������������ ����������� ������

	unsigned int id;
	unsigned int sts;

	unsigned int VEO_O2;
	unsigned int VEO_CO2;
	unsigned int VEO_N2O;
	unsigned int VEO_AX1;
	unsigned int VEO_AX2;

	unsigned int slow0;
	unsigned int slow1;
	unsigned int slow2;
	unsigned int slow3;
	unsigned int slow4;
	unsigned int slow5;

	LONG N2OPerc;
	LONG MinVol;

	int spt; // ����������� ��������� ����������� (��+)
	int spp; // ���.�������� ��������� ����������� (��+)
	int sph; // ���.��������� ��������� ����������� (��+)

	float CO2_Gain; // ����������� �������� CO2 (��������������� ����� ���������� CO2)
	float CO2_Level; // ������������ ��2 � ��������� ������� (��� ���������� ��2)

	float PIP; // �������� ����� ����� (��.��.��)
	float PEEP; // �������� ����� ������ (��.��.��)
	float Fres; // ����� ���� ������� Raw
	float Ppl; // �������� ����� (��.��.��)
	float Raw; // ���������
	float Raw2; // ��������� ������������ ����� ���������� �������
	float Cst1; // ��������� ����� ����� �����
	float Cst2; // ��������� ����� ���� ��� BTPS - �� ������������
	float Cst3; // ��������� ����� ����� - �� ������������
	float Cdyn; // ��������� ������������
	float Aupr; // ������� ������ (������� ������������ �� PIP)
	float Aupr2; // ������� ������ (������� ������������ �� ����� �������� �������� - ������������ ����� �� ������)
	float Ain; // ������ �� ����� (�� �����)
	float Aex; // ������ �� ������ (����� �����)

	DATE TimeBeg; // ����� ������ ����� (������ �����)

	float Tau; // ���������� ������� (����� ����� ������� ����� ����������� �� 36,8% �� ����������)
	float Tin; // ����� �����
	float Tex; // ����� ������

	LONG tc0; // GetTickCount � ������ ������

	float fVsum[2];						// ��������� ����� ����� � ������ (0-����)

	int RR;
	int HR;

	int mrdone;

	unsigned char pac_height; // ���� ��������
	unsigned char pac_weight; // ��� ��������
	unsigned char pac_age; // �������
	unsigned char pac_sex; // ���

	int vds_mask; // ����� �������� ������������ �����

	int HRnew;

};

