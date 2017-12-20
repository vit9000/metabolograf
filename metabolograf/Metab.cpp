#include "Metab.h"

Metab::Metab()
{
    DemoReserv=false;
	Demo = false;
	Initialized = false;
	FiO2 = 0.;
	FetO2 = 0.;
	FiCO2 = 0.;
	FetCO2 = 0.;
	
	RespRate = 0;
	Circle = 0;
        FirstCircle=0;
	lastCircle = -1;
	NewDataRecieved = false;
	Vol_insp = 0.;
	Vol_exsp = 0.;
	RespRate = 0;

	g_pdata = 0;


}
//-------------------------------------------------------------------------------------------------
void Metab::Disconnect()
{
    if(g_pdata!=0)
        g_pdata->sts =1;
        //g_pdata->FiCO2;
    TermShareMem();

}
Metab::~Metab()
{
	TermShareMem();

}
//-------------------------------------------------------------------------------------------------
void Metab::TermShareMem()
{
	if(Initialized)
	{
		UnmapViewOfFile(g_lpMapAddress);
		CloseHandle(g_hMapFile);
		g_pdata = NULL;
                Initialized =false;
	}
}
//-------------------------------------------------------------------------------------------------
bool Metab::Init()
{
       if(InitShareMem()==1)
	{
		Demo = false;
                DemoReserv=false;
		printf("Connected\n");
		return true;
	}
	else
	{
		printf("Demo\n");
                DemoReserv=true;
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
	if(NULL == (g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "METAB_DATA")))
	{ 
		printf("Could not open file-mapping object.\n"); 
		return FALSE;
	} 

	// Получаем указатель на структуру в ней	
	if(NULL == (g_lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS,	0, 0, 0)))
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
#ifdef _WIN32
    
	return Init();
#endif	
	return false;
}
//-----------------------------------------------------------------------------------------------------------------

void Metab::ReadData()
{

	if(Demo)//g_pdata== NULL || g_pdata->AmntCikl <= 0) 
	{
		ReadDataDemo();
		return;
	}
	if(g_pdata== NULL || g_pdata->AmntCikl <= 0) 
	{
		return;
	}
	//printf("%d\n", g_pdata->AmntCikl);

        Circle = g_pdata->AmntCikl;
	if(Circle==lastCircle) return;//если новые данные не пришли


        lastCircle = Circle;


	Vol_insp = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Vi;
	Vol_exsp = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Ve;
	
	RespRate = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].CHD;

	if(g_pdata->CiklSpiro[g_pdata->AmntCikl-1].VEO_FiO2pr>0)
	{
		FiO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].VEO_FiO2pr;
		FetO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].VEO_EtO2pr;
		FiCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].VEO_FiCO2pr/10;
		FetCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].VEO_EtCO2pr/10;
	}
	else
	{
		FiO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiO2pr;
		FetO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].EtO2pr;
		FiCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiCO2pr/10;
		FetCO2 = g_pdata->CiklSpiro[g_pdata->AmntCikl-1].EtCO2;
                if((FiO2<15 || FiO2>100) || (FetO2<5 || FetO2>100))
                {
                   NewDataRecieved = false;
                   return;
                }
	}
	NewDataRecieved = true;


}
int o2=0;

//#include "Global.h"
void Metab::ReadDataDemo()
{
      	//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].CHD;
        Circle++;
       	NewDataRecieved = true;
	o2++;
      	if(o2>4)o2=0;

	Vol_insp = 0.8;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Vi;
	Vol_exsp = 0.8;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].Ve;

	FiO2 = 21.0;
	//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiO2pr;
	if(o2 == 0)
	{
                RespRate = 9;
		Vol_insp = 0.005;
		Vol_exsp = 0.005;
		FetO2=13.2;
		FetCO2=36.;
	}
	else if(o2 == 1){
                RespRate = 14;
		Vol_insp = 0.5;
		Vol_exsp = 0.55;
		FetO2=13.2;
		FetCO2=36.;
	}
	else if(o2 == 2) {
        RespRate = 12;
		Vol_insp = 0.7;
		Vol_exsp = 0.7;
		FetO2=12.9;
		FetCO2=34.;
	}
	else if(o2 == 3) {
                RespRate = 16;
		Vol_insp = 0.6;
		Vol_exsp = 0.6;
		FetO2=13.1;
		FetCO2=36.;
	}
	else if(o2 == 4) {
                RespRate = 18;
		Vol_insp = 0.55;
		Vol_exsp = 0.55;
		FetO2=13.2;
		FetCO2=35.0;
	}


	FiCO2 = 0;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].FiCO2pr/10;
	//FetCO2 = 35;//g_pdata->CiklSpiro[g_pdata->AmntCikl-1].EtCO2;

}