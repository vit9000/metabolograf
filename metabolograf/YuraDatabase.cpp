#include "stdafx.h"
#include "YuraDatabase.h"

using namespace std;
YuraDatabase::YuraDatabase()
{
      Initialized = false;
      PatientWeight =0;
      PatientHeight =0;
      PatientSex =1;
      PatientAge =0;
}
//-------------------------------------------------------------------------------------------------
void YuraDatabase::Connect()
{
    InitShareMem();
    if(!Initialized) return;

    PatientHeight =  (int)g_pdata->pac_height;

    PatientWeight =  (int)g_pdata->pac_weight;
    PatientSex = (double)g_pdata->pac_sex ;
    GetPatientAge(g_pdata->pac_birth);
	

    sprintf(FIO, "%s %s %s", WStringToString(g_pdata->pac_lname).c_str(), WStringToString(g_pdata->pac_fname).c_str(), WStringToString(g_pdata->pac_mname).c_str());
    sprintf(Info, "%s", WStringToString(g_pdata->pac_diagn).c_str());
}
//-------------------------------------------------------------------------------------------------

int YuraDatabase::InitShareMem()
{

	 // Открываем общую память
	if(NULL == (g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "FDINFO")))
	{ 
		//printf("Could not open file-mapping object.\n");
		return FALSE;
	} 

	// Получаем указатель на структуру в ней	
	if(NULL == (g_lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS,	0, 0, 0)))
	{ 
		//printf("Could not map view of file.\n");
		return FALSE;
	} 

	g_pdata = (FDINFO*)g_lpMapAddress;
        Initialized =true;
	return 1;
}

void YuraDatabase::TermShareMem()
{
	if(Initialized)
	{
		UnmapViewOfFile(g_lpMapAddress);
		CloseHandle(g_hMapFile);
		g_pdata = NULL;
                Initialized =false;
	}
}
//------------------------------------------------------------------
void YuraDatabase::GetPatientAge(const DATE VTime )
{
   char str[64] = {0};
    SYSTEMTIME SystemTime;
    // Convert the Variant time to a System time value
    VariantTimeToSystemTime( VTime, &SystemTime );
    //int year = SystemTime.wYear;
    //int month = SystemTime.wMonth;
    //int day = SystemTime.wDay;
    //int hour = SystemTime.wHour;
    //int min = SystemTime.wMinute;
    //int sec = SystemTime.wSecond;

    time_t t=time(NULL);
    tm *tmp=localtime(&t);
        //file name of the protocol
        int day =   tmp->tm_mday;
        int month =  tmp->tm_mon+1 ;
        int year =   tmp->tm_year+1900;

    PatientAge = year - SystemTime.wYear;
    if(month < SystemTime.wMonth)
      PatientAge--;
    else if(month == SystemTime.wMonth)
    {
        if(day<SystemTime.wDay)
           PatientAge--;
    }

   // _snprintf( str, sizeof(str)-1, "%4d:%2d;%2d - %2d:%2d:%2d", year, month, day, hour, min, sec);
   //return str;
}


