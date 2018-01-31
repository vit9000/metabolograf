
#include "stdafx.h"
#include "Export.h"
#include <Windows.h>

Export::Export(Database *_database)
{
    database = _database;
    CalcIdealWeight();
	char buffer[MAX_PATH];
	GetCurrentDirectory(sizeof(buffer), buffer);

    curDir = buffer;
	curDir.append("\\");
}
//-------------------------------------------------------------------------------------------------
void Export::ReplacePoint(char *buf, int length)
{
      for(int i=0; i<length; i++)
      {
        if(buf[i]=='.') buf[i]=',';
      }
}


void Export::ExportDataString()
{
	if (database->getHeader().count<1)
		return;
	string t = "Данные базы данных";
	FILE *file = CreateExportFile(t, "html");
	if (file <= 0) return;

	page_width = 800;
	char bbuf[1024];
	//write html-header with coding
	sprintf(bbuf, "<html><head><meta http-equiv='Content-Type' content='text/html; charset=windows-1251'>\n<style type='text/css'>\n");
	fwrite(bbuf, strlen(bbuf), 1, file);
	//border-bottom:dashed; border-bottom-width:1px;
	sprintf(bbuf, "TABLE {width: %dpx;    border-collapse: collapse; }A:link {text-decoration: none; color: black; }A:visited { text-decoration: none; color: black;}A:active { text-decoration: none; color: black;}A:hover {text-decoration: none; color: red; border-bottom: 1px dashed;}\n</style>\n</head> \n<center>\n", page_width);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<table border=1><tr><td>ФИО<td>Дата исследования<td>Время исследования<td>Пол<td>Возраст<td>Рост<td>Вес<td>Потребление кислорода<td>Выделение углекислого газа<td>Частота дыхания<td>Дыхательный объем<td>Минутный объем дыхания<td>Дыхательный коэффициент<td>Дыхательный эквивален по O2<td>Дыхательный эквивален по CO2<td>ДМП<td>Метаболизм безбелковый<td>Метаболизм белковый<td>Б<td>Ж<td>У<td>Harris-Benedict<td>Muffin-Jeor<td>Katch-MacArdle");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%s", database->getHeader().PatientName);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d.%d.%d", database->getHeader().Day, database->getHeader().Month, database->getHeader().Year);
	fwrite(bbuf, strlen(bbuf), 1, file);
	const auto& zero_datetime = database->getDatetime(0);
	const auto& last_datetime = database->getDatetime(database->getCount() - 1);
	sprintf(bbuf, "<td>%d:%d-%d:%d", zero_datetime.getHour(), zero_datetime.getMinute(), last_datetime.getHour(), last_datetime.getMinute());
	fwrite(bbuf, strlen(bbuf), 1, file);



	sprintf(bbuf, "<td>%d", database->getHeader().PatientSex);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)database->getHeader().PatientAge);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", database->getHeader().PatientHeight);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", database->getHeader().PatientWeight);
	fwrite(bbuf, strlen(bbuf), 1, file);

	CalculateMeanValues();
	sprintf(bbuf, "<td>%.1lf", VO2 * 1000);
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%.1lf", VCO2 * 1000);
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%.1lf", RespRate);
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)(tidalVol*1000.));
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%.1lf", tidalVol*RespRate);
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%.2lf", breathK_sredn);
	ReplacePoint(bbuf, strlen(bbuf));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)((Vexp*RespRate) / VO2));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)((Vexp*RespRate) / VCO2));
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)DMP);
	fwrite(bbuf, strlen(bbuf), 1, file);

	double pbody_O2CO2 = VO2 * 3.94062 + VCO2 * 1.10638; // KKAL - O2+CO2
	pbody_O2CO2 = pbody_O2CO2 * 60 * 24;//24 hours

	sprintf(bbuf, "<td>%d", (int)pbody_O2CO2);
	fwrite(bbuf, strlen(bbuf), 1, file);


	double NBK = 25.;
	double DUK = 1.;
	double CH = 0;
	double P = 0;
	double F = 0;
	double BK = 0;
	double BrK = 0;
	double k_pbody = 1;
	//РЕКОМЕНДАЦИИ
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
	//Диета 1
	NBK = 22.86207;
	DUK = 1.2251;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);

	sprintf(bbuf, "<td>%d", (int)p_body);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)P);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)F);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)CH);
	fwrite(bbuf, strlen(bbuf), 1, file);





	database->CalculateMetab();

	sprintf(bbuf, "<td>%d", (int)database->getCalculatedMetab().Benedict);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)database->getCalculatedMetab().Muffin_Jeor);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>%d", (int)database->getCalculatedMetab().Katch_MacArdle);
	fwrite(bbuf, strlen(bbuf), 1, file);





	sprintf(bbuf, "</table>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	fclose(file);
	ShellExecute(0, NULL, file_name.c_str(), NULL, NULL, SW_RESTORE);
     
	
}
//-------------------------------------------------------------
void Export::CalcPFCH(double *P, double *F, double *CH, double *p_body, double *BK, double DUK, double NBK, double *BrK)
{

	*P = MOP24 / 
	( 1 + (DUK*4.463*(NBK/4.1 +1))/(5.047*DUK + 5.047) +
	(4.463*(NBK/4.1-DUK))/(4.735+DUK*4.735) );
	
	*CH = (DUK*(*P)*(NBK*4.463/4.1+4.463))/(5.047*DUK+5.047);
	
	*F = (4.463*(*P)*(NBK/4.1-DUK))/(4.735+DUK*4.735); 
	
		//P = calculator->data.TotalN * 6.25;
	*p_body = 5.047*(*CH) + 4.735*(*F) + 4.463*(*P);
        if(*p_body<=0) *p_body=1;

	*BK = ((*CH)+0.718*(*F)+0.802*(*P))/MOP24;
	//проверка
//	double _DUK = 5.047*(*CH)/(4.735*(*F) + 4.463*(*P));
//	double _NBK = (5.047*(*CH) + 4.735*(*F))/(4.463*(*P)/4.1);

//        double temp = DUK +_DUK;
//        temp = NBK + _NBK;
        double o2 =  *P + *CH +  *F;
        double co2 =  *CH + 0.802*(*P) + 0.718*(*F);
        *BrK =  co2/o2;

        koefficient1 = MOP24 - MCOP24 - 0.198*(*P);
        koefficient2 = -0.718 * MOP24 + MCOP24 - 0.084*(*P);

	*P = (*P)*4.463/4.1;
	*CH = (*CH)*5.047/4.1;
	*F = (*F)*4.735/9.3;


}
//--------------------------------------------------------------------------
//void Export::CalcPFCHUrea(double *P, double *F, double *CH, double *p_body, double MOP24, double MCOP24, double urea, double koefficient)
void Export::CalcPFCHUrea(double *P, double *F, double *CH, double *p_body, double urea, double koefficient)
{
       //urea mmol

       //перевели в г
       urea *=60.06/1000;
       double TotalN = urea/2.143 *  koefficient;
       double bpody_o2_co2 = MOP24 * (5.047-0.312/0.282) + MCOP24 * (0.312/0.282); // KKAL - O2+CO2
       *p_body =  bpody_o2_co2 + (-0.584 + 0.312*0.198/0.282) * TotalN * 5.941;

       double y = TotalN * 5.941; //белок
       double z = (MOP24-MCOP24-0.198*y)/0.282;//жиры
       //double x = (-0.718*MOP24 + MCOP24 - 0.084*y)/0.282;//углеводы
       double x = MOP24-z-y;

        koefficient1 = MOP24 - MCOP24 - 0.198*(y);
        koefficient2 = -0.718 * MOP24 + MCOP24 - 0.084*(y);

	*P = y*4.463/4.1;
	*F = z*4.735/9.3;
        *CH = x*5.047/4.1;

        //*CH = (*p_body-(y*4.463+z*4.735))/4.1;

        //x=0;
}
//--------------------------------------------------------------------------
//void Export::CalcPFCH_NBK (double *P, double *F, double *CH, double *p_body, double MOP24, double MCOP24, double NBK)
void Export::CalcPFCH_NBK(double *P, double *F, double *CH, double *p_body, double NBK)
{
       //*p_body = MOP24 * (5.047-0.312/0.282) + MCOP24 * (0.312/0.282); // KKAL - O2+CO2
       double y = 1/(1-0.718) * ((4.735-5.047*0.718)*MOP24 + (5.047-4.735)*MCOP24) / ((4.463/4.1)*(NBK/6.25) + (1-0.718)*((1-0.802)*4.735 +5.047*(0.802-0.718)));

       *p_body =  MOP24 * (5.047-0.312/0.282) + MCOP24 * (0.312/0.282) + (-0.584 + 0.312*0.198/0.282) * y;

       double z = (MOP24-MCOP24-0.198*y)/0.282;//жиры
       if(z<0) z=0;
       //double x = (-0.718*MOP24 + MCOP24 - 0.084*y)/0.282;//углеводы
       double x = MOP24-z-y;
       koefficient1 = MOP24 - MCOP24 - 0.198*(y);
       koefficient2 = -0.718 * MOP24 + MCOP24 - 0.084*(y);

	*P = y*4.463/4.1;
	*F = z*4.735/9.3;
        *CH = x*5.047/4.1;
        //*CH = (*p_body-(y*4.463+z*4.735))/4.1;
}
//--------------------------------------------------------------------------
void Export::CalcIdealWeight()
		{
			int h = database->getHeader().PatientHeight;
			if(database->getHeader().PatientAge>=18)//adult
			{
				IdealWeight=  h * h * 0.00225;
				return;
			}
			//child
			if(database->getHeader().PatientSex==1)//male
			{
				if(h<79)
					IdealWeight = -16.1027+0.4475*h-0.0013*h*h;
				else if(h<93)
					IdealWeight = -5.3046+0.179*h+0.0003*h*h;
				else if(h<127)
					IdealWeight = 26.4124-0.4715*h+0.0037*h*h;
				else//Рост больше 126,9см у мальчиков
					IdealWeight = 84.0659-1.3693*h+0.0071*h*h;
				
			}
			else//female
			{
				if(h<77)//Рост до 77,1 см у девочек
					IdealWeight = -4.9105+0.0909*h+0.0014*h*h;
					//вес девочка1 = -4,9105+0,0909*x+0,0014*x^2

				else if(h<92)//Рост больше 77,1см и до 91,7 см у девочек
					IdealWeight =  84.984-1.9711*h+0.0131*h*h;
					//вес девочка б1 = 84,984-1,9711*x+0,0131*x^2

				else if(h<127)//Рост больше 91,7 см и до 127,2см у девочек
					IdealWeight = 13.9217-0.2465*h+0.0026*h*h;
					//вес девочка б2,5 = 13,9217-0,2465*x+0,0026*x^2

				else//Рост больше 127,2см у девочек
					IdealWeight = 338.3651-5.0824*h+0.0206*h*h;
					//вес девочка б8 = 338,3651-5,0824*x+0,0206*x^2
			}
}
//-----------------------------------------------------------------------
FILE * Export::CreateExportFile(string type, string extension)
{
	string path = curDir+string("files");
	CreateDirectory(path.c_str(), NULL);
	
	file_name = path+string("\\")+string(database->getHeader().PatientName);
	CreateDirectory(file_name.c_str(), NULL);
	   
	
	for(int i=0; i<100; i++)
	{
		stringstream ss;
		ss << path << "\\" << string(database->getHeader().PatientName) + "\\";

		ss << type << " "
			<< database->getHeader().Day << "_"
			<< database->getHeader().Month << "_"
			<< database->getHeader().Year << " ("
			<< database->getDatetime(0).getTime().getHour() << "_"
			<< database->getDatetime(0).getTime().getMinute() << ")";
		if(i>0)
		{
			ss << " (" << i << ")";
		}
		ss << "." << extension;
		file_name = ss.str();
		if(!PathFileExists(file_name.c_str()))
			break;

	}
	FILE *file = fopen(file_name.c_str(), "w");

	if (file == NULL)
	{
		MessageBox(0, file_name.c_str(), "Ошибка создания файла", MB_OK | MB_ICONERROR);
		return 0;
	}


	return file;
		
}
//-----------------------------------------------------------------------
void  Export::WriteHeader(FILE *file)
{

	page_width=800;
	char bbuf[1024];
	//write html-header with coding
	sprintf(bbuf, "<html><head><meta http-equiv='Content-Type' content='text/html; charset=windows-1251'>\n<style type='text/css'>\n");
	fwrite(bbuf, strlen(bbuf), 1, file);
         //border-bottom:dashed; border-bottom-width:1px;
	sprintf(bbuf, "TABLE {width: %dpx;    border-collapse: collapse; }A:link {text-decoration: none; color: black; }A:visited { text-decoration: none; color: black;}A:active { text-decoration: none; color: black;}A:hover {text-decoration: none; color: red; border-bottom: 1px dashed;}\n</style>\n</head> \n<center>\n", page_width);
	fwrite(bbuf, strlen(bbuf), 1, file);



	int len = curDir.length();
	

	sprintf(bbuf, "<table bordercolor=white>\n <tr><td><img src='..\\..\\Images\\logo.png'><br><a href=lanamedica.ru><center><b>www.lanamedica.ru</b></center></a> <td align=center><font size=5><b>Исследование выполнено на оборудовании <br><a href='http://lanamedica.ru/new/catalog/spirolan-m'/>СПИРОЛАН-М</a></b></font>\n</table>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	//delete [] curDir1251;

	sprintf(bbuf, "<hr align=center width=%d size=2 color=#000/>\n",page_width);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<table bordercolor=white>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td width=25%%\%>Дата исследования:<td width=30%%><font size=4><b>");
	fwrite(bbuf, strlen(bbuf), 1, file);
	const auto& zero_dt = database->getDatetime(0);
	const auto& last_dt = database->getDatetime(database->getCount() - 1);
	sprintf(bbuf, "%s %s-%s</b></font><td width=10%%>\n", zero_dt.getDateStringRU().c_str(), zero_dt.getTimeString().c_str(), last_dt.getTimeString().c_str());// database->getHour(0), database->getMinute(0), database->getHour(database->getHeader().count - 1), database->getMinute(database->getHeader().count - 1));
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<td width=25%%>Возраст:<td width=15%%><font size=4><b>%d лет</b></font>\n", (int)database->getHeader().PatientAge);
	fwrite(bbuf, strlen(bbuf), 1, file);
	//|

	sprintf(bbuf, "<tr><td>ФИО:<td><font size=4><b>%s</b></font><td>\n", database->getHeader().PatientName);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<td>Вес:<td><font size=4><b>%d кг</b></font>\n", (int)database->getHeader().PatientWeight);
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td>Пол:<td><font size=4><b>%s</b></font><td>\n", database->getHeader().PatientSex ? "мужской" : "женский");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<td>Рост:<td><font size=4><b>%d см</b></font>\n", (int)database->getHeader().PatientHeight);
	fwrite(bbuf, strlen(bbuf), 1, file);

	//|
	sprintf(bbuf, "</table>\n");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<hr align=center width=%d size=2 color=#000/>\n",page_width);
	fwrite(bbuf, strlen(bbuf), 1, file);

	//Header of body components
	sprintf(bbuf, "<br><table bordercolor=black border=0 ><tr><td colspan=4 align=center bgcolor=black><font size=4 color=white>Состав тела</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	double bmi = (double)database->getHeader().PatientWeight/(double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.));
	double norma_l = 0;
	double norma_h = 0;
	char diagnos[1024];
	if(database->getHeader().PatientAge>18)
	{
		norma_l = (18.6 * (double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.)));
		norma_h = (24.9 * (double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.)));

		if(bmi>=40)
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Ожирение III степени</font>");
	    else if(bmi>=35)
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Ожирение II степени</font>");
		else if(bmi>=30)
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Ожирение I степени</font>");
	    else if(bmi>=25)
			sprintf(diagnos,"<td width=35%% bgcolor=orange align=center><font size=4 color=white>Избыточная масса тела</font>");
	    else if(bmi>=18.5)
			sprintf(diagnos,"<td width=35%% bgcolor=green align=center><font size=4 color=white>Норма</font>");
	    else if(bmi>=16)
			sprintf(diagnos,"<td width=35%% bgcolor=orange align=center><font size=4 color=white>Дефицит массы тела</font>");
	    else
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Выраженный дефицит массы</font>");

	}
	else if(database->getHeader().PatientAge>=8)                 //children
	{
		double child_bmi[2][11][4]=
		{
			{
				{12.2, 13.2, 18.8, 22.3},
				{13.0, 13.7, 19.8, 23.4},
				{13.4, 14.2, 20.7, 23.4},
				{13.8, 14.6, 20.8, 22.9},
				{14.8, 16.0, 21.5, 23.4},
				{15.2, 15.6, 22.1, 24.4},
				{16.2, 17.0, 23.2, 26.0},
				{16.9, 17.6, 23.2, 27.6},
				{16.9, 17.8, 22.8, 24.2},
				{17.1, 17.8, 23.4, 25.7},
				{17.6, 18.3, 23.5, 25.0}

			} ,
			{
				{12.5, 14.2, 19.3, 22.6},
				{12.8, 13.7, 19.4, 21.6},
				{13.9, 14.6, 21.4, 25.0},
				{14.0, 14.3, 21.2, 23.1},
				{14.6, 14.8, 22.0, 24.8},
				{15.6, 16.2, 21.7, 24.5},
				{16.1, 16.7, 22.6, 25.7},
				{17.0, 17.8, 23.1, 25.9},
				{17.8, 18.5, 23.7, 26.0},
				{17.6, 18.6, 23.7, 25.8},
				{17.6, 18.6, 24.0, 26.8}
			}
		};
		int age =  database->getHeader().PatientAge-8;
		int sex=  database->getHeader().PatientSex;
		norma_l = ((child_bmi[sex][age][0]+0.1) * (double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.)));
		norma_h = ((child_bmi[sex][age][1]-0.1) * (double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.)));

		if(bmi >= child_bmi[sex][age][3])
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Тяжелое ожирение</font>");
		else if(bmi >= child_bmi[sex][age][2])
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Избыточный вес</font>");
		else if(bmi >= child_bmi[sex][age][1])
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Норма</font>");
		else if(bmi >= child_bmi[sex][age][0])
			sprintf(diagnos,"<td width=35%% bgcolor=red align=center><font size=4 color=white>Недостаточный вес</font>");
		else
			sprintf(diagnos,"<td width=30%% bgcolor=red align=center><font size=4 color=white>Сильно недостаточный вес</font>");
	}
	else sprintf(diagnos," ");

	sprintf(bbuf, "<tr><td width=40%%>Индекс массы тела:<td width=25%%><font size=4><b>%.1lf</b></font>%s\n", bmi, diagnos);
	fwrite(bbuf, strlen(bbuf), 1, file);

	//Lean body mass
	double lbm;
	if(database->getHeader().PatientSex==1)
	{
		lbm = (1.1 * database->getHeader().PatientWeight)-(128*((double)(database->getHeader().PatientWeight*database->getHeader().PatientWeight) / (double)(database->getHeader().PatientHeight*database->getHeader().PatientHeight)));
	}
	else
		lbm = ((1.07 * database->getHeader().PatientWeight)-148*((double)(database->getHeader().PatientWeight*database->getHeader().PatientWeight) / (double)(database->getHeader().PatientHeight*database->getHeader().PatientHeight)));
	sprintf(bbuf, "<tr><td>Тощая масса тела:<td><font size=4><b>%.1lf кг</b></font><td>\n", lbm);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Идеальная масса тела:<td><font size=4><b>%.1lf кг</b></font><td>\n", IdealWeight);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Рекомендуемая масса тела:<td><font size=4><b>%.1lf-%.1lf кг</b></font><td>\n", norma_l, norma_h);
	fwrite(bbuf, strlen(bbuf), 1, file);

	//FAT
	double fat =
		64.5 - 848/bmi + 0.079*database->getHeader().PatientAge
		- 16.5*database->getHeader().PatientSex + 0.05*database->getHeader().PatientSex*database->getHeader().PatientAge
		+ 39.0*database->getHeader().PatientSex/bmi;

	//(calculator->data.PatientWeight-lbm)/calculator->data.PatientWeight*100;
	int ages[5][2]=
	{
		{18,29}, {30,39}, {40,49}, {50,59}, {60,100}
	};
	int male[5][2]=
	{
		{14,20}, {15,21}, {17,23}, {18,24}, {19,25}
	};
	int female[5][2]=
	{
		{20,28}, {21,29}, {22,30}, {23,31}, {24,32}
	};
	int a=0;
	//поиск возрастной группы
	for(a=0; a<5; a++)
		if(database->getHeader().PatientAge>=ages[a][0] && database->getHeader().PatientAge<=ages[a][1])
			break;
	
	if(database->getHeader().PatientAge>=18)
	{
		if(database->getHeader().PatientSex==1)
		{
			if(fat > male[a][1])
				sprintf(diagnos,"<td bgcolor=red align=center><font size=4 color=white>Больше нормы");
			else if(fat < male[a][0])
				sprintf(diagnos,"<td bgcolor=orange align=center><font size=4 color=white>Меньше нормы");
			else
				sprintf(diagnos,"<td bgcolor=green align=center><font size=4 color=white>Норма");
                        norma_h =  male[a][1];
                        norma_l =  male[a][0];
		}
		else
		{
			if(fat > female[a][1])
				sprintf(diagnos,"<td bgcolor=red align=center><font size=4 color=white>Больше нормы");
			else if(fat < female[a][0])
				sprintf(diagnos,"<td bgcolor=orange align=center><font size=4 color=white>Меньше нормы");
			else
				sprintf(diagnos,"<td bgcolor=green align=center><font size=4 color=white>Норма");
                        norma_h =  female[a][1];
                        norma_l =  female[a][0];
		}
	 }
	 else
		sprintf(diagnos,"<td>");

	double Height2 = (database->getHeader().PatientHeight/100.);
	Height2*=Height2;
	for(int i=0; i<2; i++)
	{
		if(database->getHeader().PatientSex==1)
		{
			target[i] = 
			(-848*Height2 + 39.*database->getHeader().PatientSex*Height2) /
			(male[a][i] - 64.5 - 0.079*database->getHeader().PatientAge + 16.5*database->getHeader().PatientSex - 0.05*database->getHeader().PatientSex*database->getHeader().PatientAge);
		}
		else
		{
			target[i] = 
			(-848*Height2 + 39.*database->getHeader().PatientSex*Height2) /
			(female[a][i] - 64.5 - 0.079*database->getHeader().PatientAge + 16.5*database->getHeader().PatientSex - 0.05*database->getHeader().PatientSex*database->getHeader().PatientAge);
		}
	}

	
	if(database->getHeader().PatientAge>=18)
	{
		sprintf(bbuf, "<tr><td>Жировая масса:<td><font size=4><b>%.1lf%%</b></font>%s\n", fat, diagnos);
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<tr><td>Рекомендуемая жировая масса:<td><font size=4><b>%.1lf-%.1lf%%</b></font><td>\n", norma_l, norma_h);
		fwrite(bbuf, strlen(bbuf), 1, file);
	}
	//teloslozhenie=0;// 1-астеник 2-нормо 3-гипер
	if(database->getHeader().PatientSex==1) //Мужской пол
	{
		if(database->getHeader().PatientWrist<18) teloslozhenie=1;
		else if(database->getHeader().PatientWrist<=20) teloslozhenie=2;
		else teloslozhenie=3;
	}
	else
	{
		if(database->getHeader().PatientWrist<16) teloslozhenie=1;
		else if(database->getHeader().PatientWrist<=17) teloslozhenie=2;
		else teloslozhenie=3;
	}
	switch(teloslozhenie)
	{
		case 1:
			sprintf(diagnos,"<td bgcolor=gray align=center><font size=4 color=white>Астеник");
			break;
		case 2:
			sprintf(diagnos,"<td bgcolor=gray align=center><font size=4 color=white>Нормостеник");
			break;
		case 3:
			sprintf(diagnos,"<td bgcolor=gray align=center><font size=4 color=white>Гиперстеник");
			break;
        }
	sprintf(bbuf, "<tr><td>Окружность запястья:<td><font size=4><b>%.1lf см</b></font>%s<tr><td height=5></table>\n",database->getHeader().PatientWrist, diagnos);
	fwrite(bbuf, strlen(bbuf), 1, file);

}
//--------------------------------------------------------
void Export::WriteCalcutatedMetab(FILE *file)
{
	char bbuf[1024];
	database->CalculateMetab();
	sprintf(bbuf, "<br>\n<table bordercolor=black border=0><tr><td colspan=3 align=center bgcolor=black><font size=4 color=white>Расчетный основной обмен (ккал/сутки)</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td width=33%% align=center>Harris-Benedict\n<td width=33%% align=center>Muffin-Jeor\n<td width=33%% align=center>Katch-MacArdle");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td align=center><font size=4>%d\n<td align=center><font size=4>%d\n<td align=center><font size=4>%d</font><tr><td height=5></table>", (int)database->getCalculatedMetab().Benedict, (int)database->getCalculatedMetab().Muffin_Jeor, (int)database->getCalculatedMetab().Katch_MacArdle);
	fwrite(bbuf, strlen(bbuf), 1, file);
}
//-----------------------------------------------------------
void Export::CalculateMeanValues()
{
	VO2=0;
	VCO2=0;
	p_body=0;
	breathK=0.;
	tidalVol=0.;
	RespRate=0.;
	DMP=0.;
	Vexp=0.;

	const Variable_<uint8_t>& checked =  database->getChecked();
	tidalVol = database->getVariable("Vвдоха").mean(checked);
	Vexp = database->getVariable("Vвыдоха").mean(checked);
	RespRate = database->getVariable("ЧД").mean(checked);
	if(database->getHeader().OsrednenieType2)
	{
		VO2 += database->getVariable("Потребление_O2").mean(checked);
		VCO2 += database->getVariable("Выделение_CO2").mean(checked);
		VO2*= RespRate;
		VCO2*=RespRate;
	}
	else
	{
		VO2 += database->getVariable("Минутное_потребление_O2").mean(checked);
		VCO2 += database->getVariable("Минутное_выделение_CO2").mean(checked);
	}
	MOP24 = VO2*60*24;
	MCOP24 = VCO2*60*24;

	if(Vexp>0 && RespRate>0)
		DMP = (int)(VO2*1000/(Vexp * RespRate));

	breathK_sredn = MCOP24/MOP24;
	if(breathK_sredn>1.0)
	{
		MCOP24 = MOP24;
	}

}
//-----------------------------------------------------------------------
void Export::WriteMesuredValues(FILE *file)
{
	CalculateMeanValues();
	char bbuf[1024];
	sprintf(bbuf, "</table>\n<br>\n<table bordercolor=black border=0><tr><td colspan=2 align=center bgcolor=black><font size=4 color=white>Параметры непрямой калориметрии</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);
	if(DMP<33)
	{
		sprintf(bbuf, "<tr><td colspan=2 align=center><font size=4 color=red>У пациента неэффективная вентиляция!</font>");
		fwrite(bbuf, strlen(bbuf), 1, file);
	}
	if(DMP>55)
	{
		sprintf(bbuf, "<tr><td colspan=2 align=center><font size=4 color=red>Пациент находился в режиме гипервентиляции!</font>");
		fwrite(bbuf, strlen(bbuf), 1, file);
	}
	sprintf(bbuf, "<tr><td>Потребление кислорода\n<td align=center><font size=4><b>%.1lf мл/мин.\n", VO2*1000);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Выделение углекислого газа\n<td align=center><font size=4><b>%.1lf мл/мин.\n", VCO2*1000);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Частота дыхания\n<td align=center><font size=4><b>%.1lf в мин.\n", RespRate);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Дыхательный объем\n<td align=center><font size=4><b>%d мл\n", (int)(tidalVol*1000.));
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Минутный объем дыхания\n<td align=center><font size=4><b>%.1lf л/мин.\n", tidalVol*RespRate);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Дыхательный коэффициент\n<td align=center><font size=4><b>%.2lf\n<tr><td height=5>\n", breathK_sredn);
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Дыхательный эквивалент по O2\n<td align=center><font size=4><b>%d\n<tr><td height=5>\n", (int)((Vexp*RespRate)/VO2));
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>Дыхательный эквивалент по CO2\n<td align=center><font size=4><b>%d\n<tr><td height=5>\n", (int)((Vexp*RespRate)/VCO2));
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td>ДМП\n<td align=center><font size=4><b>%d\n<tr><td height=5></table>\n", (int)DMP);
	fwrite(bbuf, strlen(bbuf), 1, file);
	//для таблицы метаболизм считаем если ДК норм по НБК 150, если ДК не норм то по протоколу лечебного питания №1
}
//------------------------------------------
void Export::StandartExport(bool fitness)
{
	if(database->getHeader().count<1)
		return;

	string t = "Протокол питания по конституции пациента";
	if(fitness)
		t="Фитнес-протокол";
	FILE *file = CreateExportFile(t, "html");
	if(file<=0) return;

	WriteHeader(file);
	WriteCalcutatedMetab(file);
	WriteMesuredValues(file);

	char bbuf[1024];
	double NBK = 25.;
	double DUK = 1.;
	double CH = 0;
	double P = 0;
	double F = 0;
	double BK = 0;
	double BrK=0;

	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        double bmi = (double)database->getHeader().PatientWeight/(double)((database->getHeader().PatientHeight/100.)*(database->getHeader().PatientHeight/100.));

	//РЕКОМЕНДАЦИИ
	sprintf(bbuf, "\n<br>\n<table bordercolor=black border=0><tr><td align=center bgcolor=black><font size=4 color=white>Рекомендации</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	if(bmi>=25 && database->getHeader().PatientAge>18 && fitness)   //ИЗБЫТОК МАССЫ ТЕЛА
	{
		sprintf(bbuf, "<tr><td style=\'text-align:justify\'><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Для того, что бы снизить свою массу тела за счет <b>жировой прослойки</b> выполняйте следующие рекомендации.\n");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<table width=100% bordercolor=white><tr><td bgcolor=green><CENTER><font size=4 color=white>Питание:</font></table>");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "&nbsp;&nbsp;&nbsp;&nbsp;Рекомендуется 5-разовое питание объемами 200 мл в соответствии с представленными ниже суточными нормами <b>потребленных калорий</b>. Следует распределить объем пищи на день в процентном соотношении 20/10/40/10/20. Перед приемом пищи рекомендуется пить очищенную воду объемом не менее 2,5 л порциями по 200 мл.\n");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "&nbsp;&nbsp;&nbsp;&nbsp;Для выбора <b>потребляемых</b> соответствующих диете калорий Вам необходимо рассчитать <b>использованные калории</b> (суточную нагрузку) с учетом всех Ваших видов деятельности в течение дня. Количество <b>использованных калорий</b> в течение дня должны превышать <b>потребленные калории</b>.\n\n");
		fwrite(bbuf, strlen(bbuf), 1, file);
	}
	else if(database->getHeader().PatientAge>18)  //НОРМА И ДЕФИЦИТ МАССЫ ТЕЛА
	{
		sprintf(bbuf, "<tr><td><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Не рекомендуется снижение массы тела ниже <b>%d</b> кг\n", (int)(target[0]));
		fwrite(bbuf, strlen(bbuf), 1, file);
	}

	double k_pbody;
	if(teloslozhenie==3)
	{
        //Диета 1
		CalcPFCH(&P, &F, &CH, &p_body, &BK, 0.429, 6.15, &BrK);
        if(bmi>=25  && database->getHeader().PatientAge>18 && fitness)
	        sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Если избыток веса появляется при употреблении хлеба и сладостей, число <b>потребляемых</b> ккал не может превосходить <b>%d</b>, при соотношении белков : жиров : углеводов - 40&#37:30&#37:30&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        else
			sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вы гиперстеник. Ваша диета содержит <b>%d</b> ккал, при соотношении белков : жиров : углеводов - 40&#37:30&#37:30&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=1200/p_body;
        sprintf(bbuf, "При этом Ваша диета не может содержать менее <b>1200</b> ккал, что соответствует Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody), BrK);
		fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
	}
	//Диета 2
	else if(teloslozhenie==1)
	{
		CalcPFCH(&P, &F, &CH, &p_body, &BK, 1.5, 12.3, &BrK);
        if(bmi>=25 && database->getHeader().PatientAge>18 && fitness)
			sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Если избыток веса появляется при употреблении жирного мяса и рыбы, число <b>потребляемых</b> ккал не может превосходить <b>%d</b>, при соотношении белков : жиров : углеводов - 25&#37:15&#37:60&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        else
			sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вы астеник. Ваша диета содержит <b>%d</b> ккал, при соотношении белков : жиров : углеводов - 25&#37:15&#37:60&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);

		fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=1200/p_body;
        sprintf(bbuf, "При этом Ваша диета не может содержать менее <b>1200</b> ккал, что соответствует Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
		fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
	}
	else if(teloslozhenie==2)
	{
        //Диета 3
		CalcPFCH(&P, &F, &CH, &p_body, &BK, 0.667, 6.15, &BrK);
        if(bmi>=25 && database->getHeader().PatientAge>18 && fitness)
			sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Если избыток веса появляется из-за малой подвижности и избытка потребляемой еды, число <b>потребляемых</b> ккал не может превосходить <b>%d</b>, при соотношении белков : жиров : углеводов - 40&#37:20&#37:40&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        else
			sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вы нормостеник. Ваша диета содержит <b>%d</b> ккал, при соотношении белков : жиров : углеводов - 40&#37:20&#37:40&#37 ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);

		fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=1200/p_body;
        sprintf(bbuf, "При этом Ваша диета не может содержать менее <b>1200</b> ккал, что соответствует Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
		fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
	}
	if(bmi>=25 && database->getHeader().PatientAge>18 && fitness)
	{
		sprintf(bbuf, "<br><font size=4><br>&nbsp;&nbsp;&nbsp;&nbsp;Из полученного диапазона <b>потребляемых</b> ккал, Вы выбираете суточную диету на <b>250-500 ккал меньше использованных</b> с необходимым для Вас соотношением белков:жиров:углеводов. Выбранная разница <b>использованных</b> и <b>потребленных</b> калорий соответствует <b>потерям 200 - 400 г</b>  жира в неделю. Не пытайтесь сбросить больше!\n");
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Целевая масса тела: нижняя граница - <b>%d</b> кг, верхняя граница - <b>%d</b> кг.\n", target[0], target[1]);
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br><font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Если сбрасывать <b>по 200 г жира в неделю</b>, то время достижения требуемой массы тела: \n");
		fwrite(bbuf, strlen(bbuf), 1, file);

		if(database->getHeader().PatientWeight-target[1]>0)
			sprintf(bbuf, "нижняя граница - <b>%d</b> нед., верхняя граница - <b>%d</b> нед.", (int)((database->getHeader().PatientWeight-target[0])/0.2), (int)((database->getHeader().PatientWeight-target[1])/0.2));
		else
			sprintf(bbuf, "нижняя граница - <b>%d</b> нед., верхняя граница - <b>уже достигнута</b>.", (int)((database->getHeader().PatientWeight-target[0])/0.2));
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br>&nbsp;&nbsp;&nbsp;&nbsp;Если сбрасывать <b>по 400 г жира в неделю</b>, то время достижения требуемой массы тела: \n");
		fwrite(bbuf, strlen(bbuf), 1, file);

		if(database->getHeader().PatientWeight-target[1]>0)
			sprintf(bbuf, "нижняя граница - <b>%d</b> нед., верхняя граница - <b>%d</b> нед.\n", (int)((database->getHeader().PatientWeight-target[0])/0.4), (int)((database->getHeader().PatientWeight-target[1])/0.4));
		else
			sprintf(bbuf, "нижняя граница - <b>%d</b> нед., верхняя граница - <b>уже достигнута</b>.\n", (int)((database->getHeader().PatientWeight-target[0])/0.4));
		fwrite(bbuf, strlen(bbuf), 1, file);

		if(database->getHeader().PatientWeight-target[1]>0)
			sprintf(bbuf, "<br>&nbsp;&nbsp;&nbsp;&nbsp;Скорость достижения <b>верхней границы целевой массы тела</b>: от <b>%d</b> до <b>%d</b> недель (в зависимости от дефицита потраченных калорий в сутки - от 250 до 500 ккал). ", (int)((database->getHeader().PatientWeight-target[1])/0.4), (int)((database->getHeader().PatientWeight-target[1])/0.2));
		else
			sprintf(bbuf, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<b>Верхняя граница целевой массы тела</b> уже достигнута. ");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<br>&nbsp;&nbsp;&nbsp;&nbsp;Скорость достижения <b>нижней границы целевой массы тела</b>: от <b>%d</b> до <b>%d</b> недель (в зависимости от дефицита потраченных калорий в сутки - от 250 до 500 ккал).  \n", (int)((database->getHeader().PatientWeight-target[0])/0.4), (int)((database->getHeader().PatientWeight-target[0])/0.2));
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br>&nbsp;&nbsp;&nbsp;&nbsp;Не злоупотребляйте пищей, вызывающей избыточное выделение желудочного сока. Употребляйте продукты с низким (10-40) гликемическим индексом (ГИ). Продукты со средним (40-70) ГИ употребляют ограниченными порциями. Продукты с высоким (свыше 70) ГИ необходимо ограничивать и сочетать с белками, жирами или другими углеводистыми продуктами с низким ГИ. Употребляйте белковую пищу и продукты с высоким ГИ в первые 20 минут после тренировок.");
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br><br>&nbsp;&nbsp;&nbsp;&nbsp;При использовании выбранной Вами диеты метаболизм, измеренный на метаболографе, должен повышаться, а дыхательный коэффициент быть меньше 0,8. Вместе с этим окружность Вашей талии должна достигнуть величины меньше половины Вашего роста - %d см.", database->getHeader().PatientHeight/2);
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br><table width=100% bordercolor=white><tr><td bgcolor=blue ><CENTER><font size=4 color=white>Движение:</font></table>");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Поддерживайте адекватную физическую активность - ходите, выполняйте легкую зарядку и силовые упражнения.");
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<table width=100% bordercolor=white><tr><td bgcolor=orange><CENTER><font size=4 color=white>Расслабление:</font></table>");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Стресс увеличивает жировые запасы.  Принимайте контрастный душ. Спите не менее восьми часов. Посещайте массаж или самостоятельно ежедневно выполняйте миофасциальное расслабление и стретч основных мышц тела.");
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<table width=100% bordercolor=white><tr><td bgcolor=green><CENTER><font size=4 color=white>Очищение:</font></table>");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Не реже 1 раза в неделю посещение сауны или хаммама. Не реже 1 раза в 2 месяца посещение СПА процедур с лимфодренажным и очищающим эффектом.");
		fwrite(bbuf, strlen(bbuf), 1, file);

		sprintf(bbuf, "<br><br><b>Справка.</b> Низкий ГИ у продуктов из  цельной пшеницы, ячменя, ржи, а также бобовые, лимоны, яблоки, сливы, персики, апельсины, грейпфруты, груши, вишня, авокадо, курага, абрикосы свежие, кабачки, перец, грибы, шпинат, листовая зелень, ");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "лук-порей, репчатый лук, брюссельская капуста, цветная капуста, брокколи,  стручковая фасоль,  горох желтый дробленый, баклажаны, помидоры, огурцы, чеснок, орехи,  семечки посолнуха, молоко и молочные продукты,  черный шоколад черный (70% какао), йогурт,  перловка,  фруктоза.  ");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "Средний ГИ имеют макароны из белой муки, овсянка, лапша, пита, зерновой ржаной хлеб, рис басмати (белый и коричневый), гречка, попкорн, овсяное печенье, кукуруза, отварной картофель, сырая морковь, зеленый горошек, манго, киви, виноград, недозрелые бананы, свекла, инжир, финики, фруктовые соки, джем. ");
		fwrite(bbuf, strlen(bbuf), 1, file);
		sprintf(bbuf, "Высокий ГИ у следующих продуктов: белый хлеб, зерновой хлеб, ржаные хлебцы, хлебные палочки, печенье, кукурузные хлопья и палочки, пшеничные крекеры.");
		fwrite(bbuf, strlen(bbuf), 1, file);
	}

	sprintf(bbuf, "<tr><td height=5></table>\n");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "\n<br>\n<table bordercolor=gray border=0><tr><td align=center bgcolor=gray><font size=4 color=white>Список литературы.</font>");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td style=\'text-align:justify\'>1.Robergs R.A., Roberts S.O. Exercise physiology. Exercise, performance, and clinical applications. St. Louis: Mosby-Year Book, 1997.<br>2.D.J. Jenkins, T.M. Wolever, R.H. Taylor, H. Barker, H. Fielden, J.M. Baldwin, A.C. Bowling, H.C. Newman, A.L. Jenkins and D.V. Goff, Glycemic index of foods: a physiological basis for carbohydrate exchange, American Journal of Clinical Nutrition, Vol. 34, 362—366. 1981.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>3.K. Foster-Powell, S.H.A. Holt, J.C. Brand-Miller, International table of glycemic index and glycemic load values: 2002, American Journal of Clinical Nutrition, Vol. 76, No. 1, 5-56, 2002.<br>4.Бергер М., Старостина Е. Г. Практика инсулинотерапии. 1990. — 365 с., С. 364—365.<br>5.Ф. К. Хэтфилд . Всестороннее руководство по развитию силы. Красноярск, 1992, 288 с.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>6.Sh. Lieberman, N.P. Bruning  Dare to Lose: 4 Simple Steps to A Better Body.   Penguin Group (USA) Incorporated, 2003, 368p.<br>7. Wolcott W, Fahey T. The Metabolic Typing Diet. Broadway Books, New York. 2000.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>8.WHO. Physical status: the use and interpretation of anthropometry. Report of a WHO Expert Committee. WHO Technical Report Series 854. Geneva: World Health Organization, 1995.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>9.WHO. Obesity: preventing and managing the global epidemic. Report of a WHO Consultation. WHO Technical Report Series 894. Geneva: World Health Organization, 2000.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>10.ASHWELL, M. & GIBSON, S. 2016. Waist to height ratio as an indicator of \'early health risk\': simpler and more predictive than using a \'matrix\' based on BMI and waist circumference BMJ Open, 6:e010159.doi:10.1136/bmjopen-2015-010159.");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<br>11.Coners, H.; Himmelmann, G.W.; Hebebrand, J.; Heseker, H.; Remschmidt, H.; Schafer, H.:Perzentilenkurven fur den Body-Mass-Index zur Gewichtsbeurteilung bei Kindern und Jugendlichen ab einem Alter von zehn Jahren. Kinderarzt, Bd. 27 (8), 1996, S. 1002-1007.  </table>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td height=5></table></html>");
	fwrite(bbuf, strlen(bbuf), 1, file);

	CloseFile(file);
}

//------------------------------------------------------------------------------------
void Export::DietaExport()
{
        if(database->getHeader().count<1)
        return;

        FILE *file = CreateExportFile("Протоколы лечебного питания (приказы МЗ", "html");
        if(file<=0) return;

        WriteHeader(file);
        WriteMesuredValues(file);

        char bbuf[1024];
        double NBK = 25.;
	double DUK = 1.;
	double CH = 0;
	double P = 0;
	double F = 0;
	double BK = 0;
        double BrK=0;
        double k_pbody=1;
        //РЕКОМЕНДАЦИИ
	sprintf(bbuf, "\n<br>\n<table border=0 bordercolor=black><tr><td align=center bgcolor=black><font size=4 color=white>Расчет дневного калоража и количества макронутриентов по утвержденным нормам лечебного питания</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);



        CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        //Диета 1
        NBK = 22.86207;
        DUK = 1.2251;
        CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
	//CalcPFCH(&P, &F, &CH, &p_body, &BK, MOP24, 1.2251,  22.86207);
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Основной вариант стандартной диеты (диеты 1, 2, 3, 5, 6, 7, 9, 10, 12,13,14, 15)<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г.<br><br>",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        //Диета 2
        NBK = 23.33333;
        DUK = 1.263992;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вариант диеты с механическим и химическим щажением (диеты 1б, 4б, 4в, 5п [I вариант])<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. <br><br>",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        //Диета 3
        NBK = 17.56957;
        DUK = 0.974643;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вариант диеты с повышенным количеством белка (диеты 4э, 4аг, 5п [II вариант], 7в, 7г, 9б, 10б, 11, R-I, R-II)<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. <br><br>",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        //Диета 4
        NBK = 58.2;
        DUK = 1.610791;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вариант диеты с пониженным количеством белка (диеты 7б, 7а)<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. <br><br>",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        //Диета 5
        NBK = 15.71333;
        DUK = 0.629386;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        P *= 0.618;
        F *= 0.618;
        CH*= 0.618;
        p_body*= 0.618;
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вариант диеты с пониженной калорийностью (диеты 8, 8а, 8о, 9а, 10с)<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. <br><br>",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        //Диета 6
        NBK = 21.58889;
        DUK = 1.136784;
	CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
        P *=1.429;
        F *=1.429;
        CH*=1.429;
        p_body *=1.429;
        sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Вариант диеты с повышенным количеством белка и повышенной калорийностью (диета 11)<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал., расчетный дыхательный коэффициент %.1lf. \n", (int)P, (int)F, (int)CH, (int)p_body, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г.",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);

        sprintf(bbuf, "</table></table>\n");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "\n<br>\n<table bordercolor=gray border=0><tr><td align=center bgcolor=gray><font size=4 color=white>Список литературы.</font>");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td style=\'text-align:justify\'>1.Приказ Минздрава РФ от 5 августа 2003 г. N 330.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>2.Приказ Министерства здравоохранения и социального развития РФ от 26 апреля 2006 г. N 316.");
	fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<br>3.Приказ Минздрава России № 395н от 21 июня 2013 г..");
	fwrite(bbuf, strlen(bbuf), 1, file);

	sprintf(bbuf, "<tr><td height=5></table>");
	fwrite(bbuf, strlen(bbuf), 1, file);


        CloseFile(file);
}
//------------------------------------------------------------------------------------
void Export::CustomDietaExport(double NBK, double DUK)
{
        if(database->getHeader().count<1)
        return;

        FILE *file = CreateExportFile("Формируемый протокол питания", "html");
        if(file<=0) return;

        WriteHeader(file);
        WriteCalcutatedMetab(file);
        WriteMesuredValues(file);

        char bbuf[1024];

	double CH = 0;
	double P = 0;
	double F = 0;
	double BK = 0;
        double BrK=0;
        double k_pbody=1;
        //РЕКОМЕНДАЦИИ
	sprintf(bbuf, "\n<br>\n<table border=0 bordercolor=black><tr><td align=center bgcolor=black><font size=4 color=white>Расчет дневного калоража и количества макронутриентов по выбранной диете</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);

        if(DUK==0)
        {
                CalcPFCH_NBK(&P, &F, &CH, &p_body, NBK);
                sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Ваша диета содержит <b>%d</b> ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. \n", (int)p_body, (int)P, (int)F, (int)CH);
        }
        else
        {
                CalcPFCH(&P, &F, &CH, &p_body, &BK, DUK, NBK, &BrK);
                sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Ваша диета содержит <b>%d</b> ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        }
	//sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Рекомендуемая диета<br>Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., энергетическая ценность <b>%d</b> ккал.\n", (int)P, (int)F, (int)CH, (int)p_body);
        //sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Ваша диета содержит <b>%d</b> ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г., расчетный дыхательный коэффициент %.1lf. \n", (int)p_body, (int)P, (int)F, (int)CH, BrK);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=1200/p_body;
        sprintf(bbuf, "При этом Ваша диета не может содержать менее <b>1200</b> ккал, что соответствует Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г.",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td height=5></table>");
	fwrite(bbuf, strlen(bbuf), 1, file);


        CloseFile(file);
}
//------------------------------------------------------------------------------------
void Export::UreaDietaExport(double Urea, double koefficient)
{
        if(database->getHeader().count<1)
        return;

		FILE *file = CreateExportFile("Формируемый протокол питания", "html");
        if(file<=0) return;

        WriteHeader(file);
        WriteCalcutatedMetab(file);
        WriteMesuredValues(file);

        char bbuf[1024];

	double CH = 0;
	double P = 0;
	double F = 0;
	double BK = 0;
        double k_pbody=1;
        //РЕКОМЕНДАЦИИ
	sprintf(bbuf, "\n<br>\n<table border=0 bordercolor=black><tr><td align=center bgcolor=black><font size=4 color=white>Расчет дневного калоража и количества макронутриентов по выбранной диете</font><tr><td>");
	fwrite(bbuf, strlen(bbuf), 1, file);


        CalcPFCHUrea(&P, &F, &CH, &p_body, Urea, koefficient);

	sprintf(bbuf, "<font size=4>&nbsp;&nbsp;&nbsp;&nbsp;Ваша диета содержит <b>%d</b> ккал, что в граммах составляет: Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г.\n", (int)p_body, (int)P, (int)F, (int)CH);
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=1200/p_body;
        sprintf(bbuf, "При этом Ваша диета не может содержать менее <b>1200</b> ккал, что соответствует Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г. ",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
        k_pbody=100/p_body;
        sprintf(bbuf, "Каждые 100 ккал должны примерно состоять из Б = <b>%d</b> г, Ж = <b>%d</b> г, У= <b>%d</b> г.",(int)(P*k_pbody), (int)(F*k_pbody), (int)(CH*k_pbody));
        fwrite(bbuf, strlen(bbuf), 1, file);
	sprintf(bbuf, "<tr><td height=5></table>");
	fwrite(bbuf, strlen(bbuf), 1, file);


        CloseFile(file);
}
//------------------------------------------------------------------------------------
void Export::CloseFile(FILE *file)
{
        char bbuf[512];

        sprintf(bbuf, "<br><table bordercolor=white><tr><b>Включенные опции при формировании протокола:</b> ");
	fwrite(bbuf, strlen(bbuf), 1, file);
        if(database->getHeader().UseExpiratoryVolume)
        {
                sprintf(bbuf, "<tr><td>Использовать объем вдоха");
                fwrite(bbuf, strlen(bbuf), 1, file);
        }
        if(database->getHeader().Filter_TVolume)
        {
                sprintf(bbuf, "<tr><td>Фильтр дыхательных объемов,");
                fwrite(bbuf, strlen(bbuf), 1, file);
        }
        if(database->getHeader().Filter_BrKoef)
        {
                sprintf(bbuf, "<tr><td>Фильтр дых.коэффициента");
                fwrite(bbuf, strlen(bbuf), 1, file);
        }
        if(database->getHeader().OsrednenieType2)
        {
                sprintf(bbuf, "<tr><td>Осреднение тип 2.");
	        fwrite(bbuf, strlen(bbuf), 1, file);
        }
        sprintf(bbuf, "</table> ");
	fwrite(bbuf, strlen(bbuf), 1, file);



        int y =0;
        time_t t=time(NULL);
	tm *tmp=localtime(&t);
        y = (int)tmp->tm_year+1900;
        sprintf(bbuf, "<br><table border=0 bordercolor=white><tr><td bgcolor=black height=2><tr><td align=center><font size=3>%d© ООО <a href=http://lanamedica.ru/new/company/contacts/>Ланамедика</a>, Санкт-Петербург, Россия. </font></table></html>", y);
	fwrite(bbuf, strlen(bbuf), 1, file);

	fclose(file);
	ShellExecute(0, NULL, file_name.c_str(), NULL, NULL, SW_RESTORE);
}
//------------------------------------------------------------------------------------
void Export::ExportExcel(const vector<string>& parameters)
{

     FILE *file = CreateExportFile("Данные", "xls");
	
     char bbuf[1024];
     sprintf(bbuf, "<html><meta http-equiv='content-type' content='text/html; charset=windows-1251' /><table><tr><td>Имя пациента<td align=center>%s", database->getHeader().PatientName);
     fwrite(bbuf, strlen(bbuf), 1, file);
     sprintf(bbuf, "<tr><td>Возраст пациента<td align=center>%d", (int)database->getHeader().PatientAge);
     fwrite(bbuf, strlen(bbuf), 1, file);
     sprintf(bbuf, "<tr><td>Пол пациента<td align=center>%s", (database->getHeader().PatientSex==1)?"мужской":"женский");
     fwrite(bbuf, strlen(bbuf), 1, file);
     sprintf(bbuf, "<tr><td>Рост пациента<td align=center>%d", database->getHeader().PatientHeight);
     fwrite(bbuf, strlen(bbuf), 1, file);
     sprintf(bbuf, "<tr><td>Вес пациента<td align=center>%d</table>", database->getHeader().PatientWeight);
     fwrite(bbuf, strlen(bbuf), 1, file);

	 sprintf(bbuf, "<table border=1><tr><td>Время аст.<td>Время");
	 fwrite(bbuf, strlen(bbuf), 1, file);
	 for (const string& param : parameters)
	 {
		 string temp = string("<td>") + param;
		 sprintf(bbuf, temp.c_str());
		 fwrite(bbuf, strlen(bbuf), 1, file);
	 }

	 for (int i = 0; i<database->getHeader().count; i++)
	 {
		 if (database->getChecked(i))
		 {
			 stringstream ss;
			 ss << fixed;
			 ss << "<tr><td align = center>" << database->getDatetime(i).getTimeString().c_str();
			 ss << "<td align = center>" << (database->getDatetime(i).getTime()-database->getDatetime(0).getTime()).getString().c_str();

			 for (const string& param : parameters)
			 {
				 ss << "<td align=center>" << setprecision(3) << database->getVariable(param)[i];
			 }
			 ss << endl;
				

			 string temp = ss.str();
			 ReplaceSymbols(temp, '.', ',');
			 fwrite(temp.c_str(), temp.length(), 1, file);
		 }
	 }


     /*sprintf(bbuf, "<table><tr><td>Время<td>Vвдоха (л)<td>Vвыдоха (л)<td>FiO2<td>FetO2<td>FiCO2<td>FetCO2<td>ЧД<td>Поглощение O2 (мл)<td>Выделение CO2 (мл)<td>ДМП<td>ДК");
     fwrite(bbuf, strlen(bbuf), 1, file);
	 
     for(int i=0; i<database->getHeader().count; i++)
     {
          if(database->getChecked(i))
          {
			  stringstream ss;
			  ss << fixed;
			  ss << "<tr><td align = center>" << database->getDatetime(i).getTimeString().c_str()
				  << "<td align=center>" << setprecision(3) << database->getVariable("Vвдоха")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("Vвыдоха")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("FiO2")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("FetO2")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("FiCO2")[i].
				  << "<td align=center>" << setprecision(3) << database->getVariable("FetCO2")[i].
				  << "<td align=center>" << setprecision(3) << database->getVariable("ЧД")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("Потребление_O2")[i]
				  << "<td align=center>" << setprecision(3) << database->getVariable("Выделение_CO2")[i]
				  << "<td align=center>" << setprecision(3) << static_cast<int>(database->getVariable("ДМП")[i])
				  << "<td align=center>" << setprecision(3) << database->getVariable("Дыхательный_коэффициент")[i]
				  << endl;

			  string temp = ss.str();
			  ReplaceSymbols(temp, '.', ',');
			  fwrite(temp.c_str(), temp.length(), 1, file);
          }
     }
	 */
     sprintf(bbuf, "</table>");
     fwrite(bbuf, strlen(bbuf), 1, file);
	 CloseFile(file);  
}
//-------------------------------------------------
void Export::ReplaceSymbols(char *str, char a, char b)
{
        int len = strlen(str);
        for(int j=0;j<len; j++)
        {
            if(str[j]==a) str[j]=b;
        }
}

void Export::ReplaceSymbols(string& str, char a, char b)
{
	int len = str.length();
	for (int j = 0; j < len; j++)
	{
		if (str[j] == a) str[j] = b;
	}
}