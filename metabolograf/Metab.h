#ifndef METAB_H
#define METAB_H


#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "metabdata.h"


class Metab
{
public:
    
	

	bool DemoReserv;
	bool Demo;

	HANDLE g_hMapFile;
	LPVOID g_lpMapAddress;
	METAB_DATA* g_pdata;

	int Circle;
	int lastCircle;
	bool NewDataRecieved;

	double Vol_insp;
	double Vol_exsp;
	double RespRate;
	double RespRateOld;
	double FiO2;
	double FetO2;
	double FiCO2;
	double FetCO2;
	int HR;

	bool Initialized;
	int FirstCircle;


	Metab()
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
	void Disconnect()
	{
		if (g_pdata != 0)
			g_pdata->sts = 1;
		TermShareMem();

	}
	~Metab()
	{
		TermShareMem();

	}
	//-------------------------------------------------------------------------------------------------
	void TermShareMem()
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
	bool Init()
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
	void SetDemoMode()
	{
		Demo = true;
	}
	//-----------------------------------------------------------------------------------------------------------------
	void RestoreMode()
	{
		Demo = DemoReserv;
	}
	//-----------------------------------------------------------------------------------------------------------------
	int InitShareMem()
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
	bool Connect()
	{
		if (Initialized) return true;

		return Init();
	}
	//-----------------------------------------------------------------------------------------------------------------

	void ReadData()
	{

		if (Demo)//g_pdata== NULL || g_pdata->AmntCikl <= 0) 
		{
			ReadDataDemo();
			return;
		}
		if (g_pdata == NULL || g_pdata->AmntCikl <= 0)
		{
			return;
		}
		//printf("%d\n", g_pdata->AmntCikl);

		Circle = g_pdata->AmntCikl;
		
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
			if ((FiO2<15 || FiO2>100) || (FetO2<5 || FetO2>100) || (FiO2 < FetO2))
			{
				NewDataRecieved = false;
				return;
			}
		}
		NewDataRecieved = true;


	}
	int o2 = 0;

	//#include "Global.h"
	void ReadDataDemo()
	{
		//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].CHD;
		Circle++;
		NewDataRecieved = true;
		o2++;
		if (o2>4)o2 = 0;

		Vol_insp = 0.8;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Vi;
		Vol_exsp = 0.8;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Ve;

		FiO2 = 21.0;
		//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiO2pr;
		if (o2 == 0)
		{
			RespRate = 9;
			Vol_insp = 0.005;
			Vol_exsp = 0.005;
			FetO2 = 13.2;
			FetCO2 = 36.;
		}
		else if (o2 == 1) {
			RespRate = 14;
			Vol_insp = 0.5;
			Vol_exsp = 0.55;
			FetO2 = 13.2;
			FetCO2 = 36.;
		}
		else if (o2 == 2) {
			RespRate = 12;
			Vol_insp = 0.7;
			Vol_exsp = 0.7;
			FetO2 = 12.9;
			FetCO2 = 34.;
		}
		else if (o2 == 3) {
			RespRate = 16;
			Vol_insp = 0.6;
			Vol_exsp = 0.6;
			FetO2 = 13.1;
			FetCO2 = 36.;
		}
		else if (o2 == 4) {
			RespRate = 18;
			Vol_insp = 0.55;
			Vol_exsp = 0.55;
			FetO2 = 13.2;
			FetCO2 = 35.0;
		}


		FiCO2 = 0;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiCO2pr/10;
				  //FetCO2 = 35;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].EtCO2;

	}

};

#endif // MONITORBLOCK_H