#ifndef METAB_H
#define METAB_H


#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "metabdata.h"


class Metab
{
public:
	double Vol_insp;
	double Vol_exsp;
	double RespRate;
	double RespRateOld;
	double FiO2;
	double FetO2;
	double FiCO2;
	double FetCO2;
	int HR;

private:
	bool DemoReserv;
	bool Demo;

	HANDLE g_hMapFile;
	LPVOID g_lpMapAddress;
	METAB_DATA* g_pdata;

	int Circle;
	int lastCircle;
	bool NewDataRecieved;

	

	bool Initialized;
	int FirstCircle;

public:
	Metab();
	~Metab();
	bool Init();
	bool Connect();
	void ReadData();
	void RestoreMode();
	void Disconnect();
	bool IsNewDataRecieved() const { return NewDataRecieved; }
	void NewDataReaded() { NewDataRecieved = false; }
	void SetDemoMode();
	
private:
	int InitShareMem();
	void TermShareMem();
};

#endif // MONITORBLOCK_H