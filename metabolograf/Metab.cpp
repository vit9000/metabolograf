#include "stdafx.h"
#include "Metab.h"


Metab::Metab()
{
	DemoReserv = false;
	Demo = false;
	Initialized = false;
	FiO2 = 0.;
	FetO2 = 0.;
	FiCO2 = 0.;
	FetCO2 = 0.;

	RespRate = 0;
	Circle = 0;
	FirstCircle = 0;
	lastCircle = -1;
	NewDataRecieved = false;
	Vol_insp = 0.;
	Vol_exsp = 0.;
	RespRate = 0;
	HR = 0;
	g_pdata = 0;


}
//-------------------------------------------------------------------------------------------------
void Metab::Disconnect()
{
	if (g_pdata != 0)
		g_pdata->sts = 1;
	TermShareMem();

}
Metab::~Metab()
{
	TermShareMem();
}
//-------------------------------------------------------------------------------------------------
void Metab::TermShareMem()
{
	if (Initialized)
	{
		UnmapViewOfFile(g_lpMapAddress);
		CloseHandle(g_hMapFile);
		g_pdata = NULL;
		Initialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool Metab::Init()
{
	if (InitShareMem() == 1)
	{
		Demo = false;
		DemoReserv = false;
		printf("Connected\n");
		return true;
	}
	else
	{
		printf("Demo\n");
		DemoReserv = true;
		Demo = true;
		return false;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Metab::SetDemoMode()
{
	Demo = true;
}
//-----------------------------------------------------------------------------------------------------------------
void Metab::RestoreMode()
{
	Demo = DemoReserv;
}
//-----------------------------------------------------------------------------------------------------------------
int Metab::InitShareMem()
{

	// Открываем общую память
	if (NULL == (g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "METAB_DATA")))
	{
		printf("Could not open file-mapping object.\n");
		return FALSE;
	}

	// Получаем указатель на структуру в ней	
	if (NULL == (g_lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0)))
	{
		printf("Could not map view of file.\n");
		return FALSE;
	}

	g_pdata = (METAB_DATA*)g_lpMapAddress;
	Initialized = true;

	return 1;
}
//-----------------------------------------------------------------------------------------------------------------
bool Metab::Connect()
{
	if (Initialized) return true;

	return Init();
}
//-----------------------------------------------------------------------------------------------------------------

void Metab::ReadData()
{

	if (Demo)//g_pdata== NULL || g_pdata->AmntCikl <= 0) 
	{
		return;
	}
	if (g_pdata == NULL)
	{
		return;
	}
	//printf("%d\n", g_pdata->AmntCikl);

	Circle = g_pdata->AmntCikl;
	HR = g_pdata->HR;

	if (g_pdata->AmntCikl <= 0)
		return;

	if (Circle == lastCircle) return;//если новые данные не пришли


	lastCircle = Circle;


	Vol_insp = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].Vi;
	Vol_exsp = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].Ve;
	//ЧСС
	RespRateOld = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].CHD;
	int& bege = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].begi;
	int& endi = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].ende;
	int len = endi - bege + (endi < bege ? 30000 : 0);// -плюсуем длину буфера если перешли через его край
	double time = static_cast<double>(len) / 300.;// -время ДЦ
	RespRate = 60. / time;// -реальная ЧД

	int rr = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].RR;
	HR = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].HR;//!!!!!!!!!!!!!!!!!!!!! HEART RATE

	if (g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].VEO_FiO2pr>0)
	{
		FiO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].VEO_FiO2pr;
		FetO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].VEO_EtO2pr;
		FiCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].VEO_FiCO2pr / 10;
		FetCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].VEO_EtCO2pr / 10;
	}
	else
	{
		FiO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].FiO2pr;
		FetO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].EtO2pr;
		FiCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].FiCO2pr / 10;
		FetCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl - 1].EtCO2;


		double tempco2 = FetCO2 / 760. * 100.;
		if ((FiO2<15 || FiO2>100) || (FetO2<5 || FetO2>100) || (FiO2 < FetO2) || (FetCO2 / 760. * 100. > 10))
		{
			NewDataRecieved = false;
			return;
		}
	}
	NewDataRecieved = true;


}
