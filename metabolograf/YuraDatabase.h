#ifndef YURADATABASE_H
#define YURADATABASE_H

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include "ydatabase.h"

class YuraDatabase
{
public:
    //YuraDatabase();
    //int InitShareMem();
    //void TermShareMem();


    bool Initialized;
    HANDLE g_hMapFile;
    LPVOID g_lpMapAddress;
    FDINFO* g_pdata;

    int PatientWeight;
    int PatientHeight;
    int PatientSex;
    double PatientAge;
    char FIO[128];
    char Info[128];

    //void GetPatientAge(const DATE VTime);
    //void Connect();

	string WStringToString(const wstring& wstr)
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	
	YuraDatabase()
	{
		Initialized = false;
		PatientWeight = 0;
		PatientHeight = 0;
		PatientSex = 1;
		PatientAge = 0;
	}
	//-------------------------------------------------------------------------------------------------
	void Connect()
	{
		InitShareMem();
		if (!Initialized) return;

		PatientHeight = (int)g_pdata->pac_height;

		PatientWeight = (int)g_pdata->pac_weight;
		PatientSex = (double)g_pdata->pac_sex;
		GetPatientAge(g_pdata->pac_birth);


		sprintf(FIO, "%s %s %s", WStringToString(g_pdata->pac_lname).c_str(), WStringToString(g_pdata->pac_fname).c_str(), WStringToString(g_pdata->pac_mname).c_str());
		sprintf(Info, "%s", WStringToString(g_pdata->pac_diagn).c_str());
	}
	//-------------------------------------------------------------------------------------------------

	int InitShareMem()
	{

		// Открываем общую память
		if (NULL == (g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "FDINFO")))
		{
			//printf("Could not open file-mapping object.\n");
			return FALSE;
		}

		// Получаем указатель на структуру в ней	
		if (NULL == (g_lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0)))
		{
			//printf("Could not map view of file.\n");
			return FALSE;
		}

		g_pdata = (FDINFO*)g_lpMapAddress;
		Initialized = true;
		return 1;
	}

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
	//------------------------------------------------------------------
	void GetPatientAge(const DATE VTime)
	{
		char str[64] = { 0 };
		SYSTEMTIME SystemTime;
		// Convert the Variant time to a System time value
		VariantTimeToSystemTime(VTime, &SystemTime);
		//int year = SystemTime.wYear;
		//int month = SystemTime.wMonth;
		//int day = SystemTime.wDay;
		//int hour = SystemTime.wHour;
		//int min = SystemTime.wMinute;
		//int sec = SystemTime.wSecond;

		time_t t = time(NULL);
		tm *tmp = localtime(&t);
		//file name of the protocol
		int day = tmp->tm_mday;
		int month = tmp->tm_mon + 1;
		int year = tmp->tm_year + 1900;

		PatientAge = year - SystemTime.wYear;
		if (month < SystemTime.wMonth)
			PatientAge--;
		else if (month == SystemTime.wMonth)
		{
			if (day<SystemTime.wDay)
				PatientAge--;
		}

		// _snprintf( str, sizeof(str)-1, "%4d:%2d;%2d - %2d:%2d:%2d", year, month, day, hour, min, sec);
		//return str;
	}
};

#endif // MONITORBLOCK_H