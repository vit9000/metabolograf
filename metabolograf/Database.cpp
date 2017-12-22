#include "stdafx.h"
#include "Database.h"

Database::Database()
{
	Ini ini("metabolograf_config.ini");
	ADS = ini.Read("Statics", "AdditionslDeadSpace", 0.0);
	Default();

}
//---------------------------------------------------------------------
Database::~Database()
{
	Ini ini("metabolograf_config.ini");
	ini.Write("Statics", "AdditionslDeadSpace", ADS);
}
//---------------------------------------------------------------------
size_t Database::getCount()
{
	return hdata.count;
}
//---------------------------------------------------------------------
void Database::Default()
{

	ZeroMemory((void*)&hdata, sizeof(hdata));
	sprintf(hdata.PatientName, "");
	hdata.UseMeanVolume = true;
	hdata.UseInspiratoryVolume = false;
	hdata.PatientAge = 40;
	hdata.PatientWeight = 70;
	hdata.PatientHeight = 170;
	hdata.PatientSex = 1;
	hdata.PatientWrist = 20;
	hdata.UseExpiratoryVolume = false;
	hdata.Filter_TVolume = false;
	hdata.Filter_BrKoef = false;
	hdata.OsrednenieType2 = false;

	hdata.Day = 0;
	hdata.Month = 0;
	hdata.Year = 0;
	hdata.count = 0;
	hdata.StartTest = 0;// начало нагрузки
	hdata.EndTest = 0;//конец нагрузки
	hdata.AeT = 0;//аэробный порог
	hdata.AT = 0;//анаэробный порог
	hdata.MCO = 0;//maximum oxygen comsumption
	hdata.PowerStep = 0;
	hdata.HR = false;


	datetime.clear();
	checked.clear();
	variables.clear();

	DefaultVarNames();
	AddPatientParametersToVariables();
}
//---------------------------------------------------------------------
Variable_<MTime> Database::GetTimeFromZero(int zerotime)
{
	Variable_<MTime> temp;
	size_t size = datetime.size();
	if (size == 0) return temp;

	temp.resize(size);
	for (int i = 0; i < size; i++)
	{

		temp[i] = datetime[i].getTime() - datetime[zerotime].getTime();
	}
	return temp;
}
//---------------------------------------------------------------------
void Database::DefaultVarNames()
{
	variable_names.clear();

	variable_names.push_back("Возраст");
	variable_names.push_back("Пол");
	variable_names.push_back("Рост");
	variable_names.push_back("Вес");
	variable_names.push_back("Vвдоха");
	variable_names.push_back("Vвыдоха");
	variable_names.push_back("ЧД");
	variable_names.push_back("FiO2");
	variable_names.push_back("FetO2");
	variable_names.push_back("FiCO2");
	variable_names.push_back("FetCO2");
	variable_names.push_back("Vвдоха_без_МП");
	variable_names.push_back("Vвыдоха_без_МП");
	variable_names.push_back("Потребление_O2");
	variable_names.push_back("Выделение_CO2");
	variable_names.push_back("Потребление_O2_мл_кг_мин");
	variable_names.push_back("Выделение_CO2_мл_кг_мин");
	variable_names.push_back("Минутное_потребление_O2");
	variable_names.push_back("Минутное_выделение_CO2");
	variable_names.push_back("Вентиляционный_эквивалент_O2");
	variable_names.push_back("Вентиляционный_эквивалент_CO2");
	variable_names.push_back("Дыхательный_коэффициент");
	variable_names.push_back("Метаболизм_O2");
	variable_names.push_back("Метаболизм_CO2");
	variable_names.push_back("ДМП");
	variable_names.push_back("Минутная_вентиляция");


	if (hdata.HR == 1)
	{
		variable_names.push_back("ЧСС");
		variable_names.push_back("RR");
		variable_names.push_back("SD");
	}
	else hdata.HR = 0;
}
//---------------------------------------------------------------------
void Database::CalculateMetab()
{
	//Harris-Benedict equation
	double oo = 0.;//основной обмен
	double temperature = 36.6;
	if (hdata.PatientSex == 1)
		oo = 66.5 + (13.7*hdata.PatientWeight) + (5 * hdata.PatientHeight) - (6.8*hdata.PatientAge);
	else
		oo = 655.0 + (9.5*hdata.PatientWeight) + (1.8*hdata.PatientHeight) - (4.7*hdata.PatientAge);

	double k = 0.0;
	if (temperature >= 41.) k = 1.4;
	else if (temperature >= 40.) k = 1.3;
	else if (temperature >= 39.) k = 1.2;
	else if (temperature >= 38.) k = 1.1;
	else k = 1.0;

	//oo= (oo+(oo*0.4))*k;
	Benedict = oo;


	double koef = -161;
	if (hdata.PatientSex == 0)
		koef = 5;
	oo = 9.99 * hdata.PatientWeight + 6.25 * hdata.PatientHeight - 4.92 * hdata.PatientAge + koef;
	Muffin_Jeor = oo;


	double LBM = 0.0;
	if (hdata.PatientSex == 1)
		LBM = (1.10 *hdata.PatientWeight) - 128 * hdata.PatientWeight*hdata.PatientWeight / (100 * hdata.PatientHeight*hdata.PatientHeight);
	else
		LBM = (1.07 * hdata.PatientWeight) - 148 * hdata.PatientWeight*hdata.PatientWeight / (100 * hdata.PatientHeight*hdata.PatientHeight);
	//Hallynck TH Soep HH et al. Should clearance be normalised to body surface or to lean body mass? Br J Clin Pharmacol. 1981; 11: 523-526.
	oo = 370 + 21.6 * LBM;
	Katch_MacArdle = oo;
}
//---------------------------------------------------------------------
int Database::getHour(int index)
{
	return datetime[index].getTime().getHour();
}
//---------------------------------------------------------------------
int Database::getMinute(int index)
{
	return datetime[index].getTime().getMinute();
}
//---------------------------------------------------------------------
bool Database::SaveFile(const char *fname)
{
	ofstream out;
	out.open(fname, ios::out | ios::binary);

	if (!out.is_open())
	{

		MessageBox(0, fname, "Ошибка сохранения файла", MB_OK);

		return false;
	}

	out.write((char *)&hdata, sizeof(hdata));


	int count = hdata.count;
	Variable_<int> H(count), M(count), S(count);
	for (int i = 0; i<count; i++)
	{
		H[i] = datetime[i].getTime().getHour();
		M[i] = datetime[i].getTime().getMinute();
		S[i] = datetime[i].getTime().getSecond();
	}

	// запись массивов времени. Отдельно часы, минуты, секунды
	out.write((char *)H.getPtr(), sizeof(int)*count);
	out.write((char *)M.getPtr(), sizeof(int)*count);
	out.write((char *)S.getPtr(), sizeof(int)*count);
	// запись поставленных галок
	out.write((char *)checked.getPtr(), sizeof(bool)*count);
	
	//запись показателей
	out.write((char *)variables["Vвдоха"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["Vвыдоха"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["ЧД"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FiO2"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FetO2"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FiCO2"].getPtr(), sizeof(double)*count);
	// с CO2 другая история, для ненарушения формата, сохраняем разделенный на 100
	Variable FetCO2 = variables["FetCO2"] / 100;
	out.write((char *)FetCO2.getPtr(), sizeof(double)*count);
	if (hdata.HR)
		out.write((char *)variables["ЧСС"].getPtr(), sizeof(double)*count);
	out.close();
	return true;
}
//---------------------------------------------------------------------
void Database::SetVariable(string name, Variable& var, bool constant)
{
	if (variables.count(name) == 0)
		variable_names.push_back(name);
	var.setConst(constant);
	variables[name] = var;
}
//---------------------------------------------------------------------
bool Database::OpenFile(const char *fname)
{
	ifstream in(fname, ios::in | ios::binary);
	if (!in)
		return false;

	datetime.clear();
	checked.clear();
	variables.clear();
	variable_names.clear();
	DefaultVarNames();

	in.read((char *)&hdata, sizeof(hdata));
	int size = hdata.count;

	int* t = new int[size * 3];
	in.read((char *)t, sizeof(int)*size * 3);
	for (int i = 0; i<size; i++)
	{
		datetime.append(Datetime(hdata.Year, hdata.Month, hdata.Day, t[i], t[i + size], t[i + size * 2]));
	}
	delete[] t;

	in.read((char*)checked.getPtr(size), sizeof(bool)*size);
	AddPatientParametersToVariables();

	Variable var;
	var.resize(size);


	for (int i = 4; i<4 + 7; i++)
	{
		in.read((char *)var.getPtr(), sizeof(double)*size);
		variables[variable_names[i]] = var;
	}
	variables["FetCO2"] = variables["FetCO2"] * 100.;

	if (hdata.HR && !in.eof())
	{
		in.read((char *)var.getPtr(), sizeof(double)*size);
		variables["ЧСС"] = var;
	}

	CalculateParameters(); // выполняем расчет дополнительных параметров

	in.close();
	//устанавливаем рассчитанные параметры константными
	for (map<string, Variable>::iterator it = variables.begin();
		it != variables.end();
		++it)
	{
		it->second.setConst(true);
	}
	return true;
}
//---------------------------------------------------------------------
void Database::AddPatientParametersToVariables()
{
	variables["Возраст"] = Variable(hdata.PatientAge);
	variables["Возраст"].SetType(Scalar);
	variables["Пол"] = Variable(static_cast<double>(hdata.PatientSex));
	variables["Пол"].SetType(Scalar);
	variables["Рост"] = Variable(static_cast<double>(hdata.PatientHeight));
	variables["Рост"].SetType(Scalar);
	variables["Вес"] = Variable(static_cast<double>(hdata.PatientWeight));
	variables["Вес"].SetType(Scalar);
}
//---------------------------------------------------------------------
void Database::Filter(int index)
{
	double VOL_INSP_MIN;
	double VOL_INSP_MAX;
	if (hdata.PatientAge<12)
		VOL_INSP_MIN = 0.0;
	else
		VOL_INSP_MIN = 0.05;
	VOL_INSP_MAX = 20.0;

	bool status = true;
	//РАСЧЕТ ПОТРЕБЛЕНИЯ КИСЛОРОДА И ВЫДЕЛЕНИЯ СО2
	if (hdata.Filter_TVolume && (variables["Vвдоха"][index] >= VOL_INSP_MAX || variables["Vвдоха"][index] <= VOL_INSP_MIN))
	{
		status = false;
	}
	else if ((hdata.Filter_BrKoef) && (variables["Дыхательный_коэффициент"][index]>2.5 || variables["Дыхательный_коэффициент"][index]<0.5))
		status = false;

	checked[index] = status;
}
//--------------------------------------------------------------------------
void Database::Clear()
{
	vector<string> todelete;
	for (auto &it : variables)
	{
		if (!it.second.isConst())
			todelete.push_back(it.first);

	}
	for (auto& s : todelete)
		variables.erase(s);


	DefaultVarNames();
}
//----------------------------------------------------------------------------
double Database::ChildTV(int x/*month*/)
{

	if (x>12)
		return (300. - 70.) / ((14. - 1)*12.)*(double)x + 54;
	return 24.03636364 + 6.15757576*(double)x + (-0.19393939)*pow((double)x, 2);
}
//--------------------------------------------------------------------------
void Database::CalculateParameters()
{
	if (getCount() == 0) return;

	Variable Volinsp, Volexp;
	if (hdata.PatientAge <= 14)
	{
		double childTV = ChildTV(hdata.PatientAge * 12);
		variables["Vвдоха_без_МП"] = variables["Vвдоха"] - (0.3*childTV + ADS) / 1000.;//без мертвого пространства
		variables["Vвыдоха_без_МП"] = variables["Vвыдоха"] - (0.3*childTV + ADS) / 1000.;//без мертвого пространства

	}
	else
	{
		variables["Vвдоха_без_МП"] = variables["Vвдоха"] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//без мертвого пространства
		variables["Vвыдоха_без_МП"] = variables["Vвыдоха"] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//без мертвого пространства
	}
	Variable Vexp = variables["Vвыдоха_без_МП"];
	Variable Vinsp = variables["Vвдоха_без_МП"];;
	if (hdata.UseExpiratoryVolume)
		Vinsp = Vexp;
	else if (hdata.UseInspiratoryVolume)
		Vexp = Vinsp;
	else if (hdata.UseMeanVolume)
	{
		Variable temp = (Vinsp + Vexp) / 2;
		Vinsp = temp;
		Vexp = temp;
	}



	variables["Минутная_вентиляция"] = Vexp * variables["ЧД"];

	variables["Потребление_O2"] = Vinsp * variables["FiO2"] / 100 - Vexp * variables["FetO2"] / 100;//потребление по кислорода в текущий дыхательный цикл
	variables["Выделение_CO2"] = Vexp*variables["FetCO2"] / 100.;// - Volinsp*data.FiCO2[Circle];//выделение углекислого газа в текущий дых цикл
	variables["Минутное_потребление_O2"] = variables["Потребление_O2"] * variables["ЧД"]; //metab->RespRate;//потребление по кислорода в мин
	variables["Минутное_выделение_CO2"] = variables["Выделение_CO2"] * variables["ЧД"];//metab->RespRate;//выделение углекислого газа в мин

	variables["Потребление_O2_мл_кг_мин"] = variables["Минутное_потребление_O2"] * 1000 / hdata.PatientWeight;//variables["Вес"];
	variables["Выделение_CO2_мл_кг_мин"] = variables["Минутное_выделение_CO2"] * 1000 / hdata.PatientWeight;//variables["Вес"];;

	variables["Вентиляционный_эквивалент_O2"] = Vexp / variables["Потребление_O2"];
	variables["Вентиляционный_эквивалент_CO2"] = Vexp / variables["Выделение_CO2"];

	// РАСЧЕТ ДЫХАТЕЛЬНОГО КОЭФФИЦИЕНТА
	variables["Дыхательный_коэффициент"] = variables["Минутное_выделение_CO2"] / variables["Минутное_потребление_O2"];
	double *BrK_Ptr = variables["Дыхательный_коэффициент"].getPtr();
	Variable tempMOP = variables["Минутное_потребление_O2"];
	Variable tempMCOP = variables["Минутное_выделение_CO2"];
	for (int i = 0; i < variables["Минутное_выделение_CO2"].size(); i++)
	{
		if (BrK_Ptr[i] < 0) BrK_Ptr[i] = 0.0;
		if (variables["Минутное_потребление_O2"].getPtr()[i] < 0.01 || variables["Минутное_выделение_CO2"].getPtr()[i] < 0.01)
			BrK_Ptr[i] = 0.0;
		if (tempMCOP.getPtr()[i] < 0) tempMCOP.getPtr()[i] = 0;
		if (tempMOP.getPtr()[i] < 0) tempMOP.getPtr()[i] = 0;
		if (BrK_Ptr[i] > 1.0)
		{
			tempMCOP.getPtr()[i] = tempMOP.getPtr()[i];
		}

	}



	Variable temp = tempMOP * 20000;//KKAL - O2
	temp = temp * 60 * 24;//24 hours
	temp = temp / 4.18 / 1000;
	variables["Метаболизм_O2"] = temp;

	temp = tempMOP * 3.94062 + tempMCOP * 1.10638; // KKAL - O2+CO2
	temp = temp * 60 * 24;//24 hours
	variables["Метаболизм_CO2"] = temp;

	variables["ДМП"] = (variables["Минутное_потребление_O2"] * 1000 / (variables["Vвыдоха"] * variables["ЧД"]));

	if (hdata.HR && variables["SD"].size() == 0)
		FillSD();
}
//--------------------------------------------------------------------------
void Database::FillSD()
{
	if (getCount() <= 0) return;
	if (!hdata.HR) return;

	variables["RR"] = 60000. / variables["ЧСС"];
	variables["SD"].resize(getCount());

	int start = 0;

	// сперва находим 30 секундную отметку
	MTime starttime = datetime[0].getTime() + 30;
	for (; start < getCount(); ++start)
	{
		if (datetime[start].getTime() > starttime)
		{
			--start;
			break;
		}
	}
	//начинаем с 30 секундной отметки
	Variable sd(getCount());
	for (int i = start; i < getCount(); ++i)
	{//идем по значению

		CalculateSD(i, false);

	}
}
//--------------------------------------------------------------------------
void Database::CalculateSD(int i, bool checktime)
{
	if (i < 0) return;
	variables["RR"][i] = 60000. / variables["ЧСС"][i];
	variables["SD"][i] = 0;
	if (i == 0) return;

	if (!checktime || (datetime[i].getTime() >= datetime[0].getTime() + 30))
	{
		MTime backtime = datetime[i].getTime() - 31;
		int j = i;
		MTime t;
		while (j >= 0 && (t = datetime[j].getTime()) > backtime)
			--j;

		variables["SD"][i] = variables["RR"].SD(j + 1, i);
	}

}
//-----------------------------------------------------------------------------------------------------------
void Database::CalculateParameters(int i)
{

	double Volinsp, Volexp;
	if (hdata.PatientAge <= 14)
	{
		double childTV = ChildTV(hdata.PatientAge * 12);
		variables["Vвдоха_без_МП"][i] = variables["Vвдоха"][i] - (0.3*childTV + ADS) / 1000.;//без мертвого пространства
		variables["Vвыдоха_без_МП"][i] = variables["Vвыдоха"][i] - (0.3*childTV + ADS) / 1000.;//без мертвого пространства

	}
	else
	{
		variables["Vвдоха_без_МП"][i] = variables["Vвдоха"][i] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//без мертвого пространства
		variables["Vвыдоха_без_МП"][i] = variables["Vвыдоха"][i] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//без мертвого пространства
	}

	double Vexp = variables["Vвыдоха_без_МП"][i];
	double Vinsp = variables["Vвдоха_без_МП"][i];
	if (hdata.UseExpiratoryVolume)
		Vinsp = Vexp;
	else if (hdata.UseInspiratoryVolume)
		Vexp = Vinsp;
	else if (hdata.UseMeanVolume)
	{
		double temp = (Vinsp + Vexp) / 2;
		Vinsp = temp;
		Vexp = temp;
	}



	variables["Минутная_вентиляция"][i] = Vexp * variables["ЧД"][i];

	variables["Потребление_O2"][i] = Vinsp * variables["FiO2"][i] / 100 - Vexp * variables["FetO2"][i] / 100;//потребление по кислорода в текущий дыхательный цикл
	variables["Выделение_CO2"][i] = Vexp*variables["FetCO2"][i] / 100.;// - Volinsp*data.FiCO2[Circle];//выделение углекислого газа в текущий дых цикл
	variables["Минутное_потребление_O2"][i] = variables["Потребление_O2"][i] * variables["ЧД"][i]; //metab->RespRate;//потребление по кислорода в мин
	variables["Минутное_выделение_CO2"][i] = variables["Выделение_CO2"][i] * variables["ЧД"][i];//metab->RespRate;//выделение углекислого газа в мин

	variables["Потребление_O2_мл_кг_мин"][i] = variables["Минутное_потребление_O2"][i] * 1000 / hdata.PatientWeight;//variables["Вес"];
	variables["Выделение_CO2_мл_кг_мин"][i] = variables["Минутное_выделение_CO2"][i] * 1000 / hdata.PatientWeight;//variables["Вес"];;

	variables["Вентиляционный_эквивалент_O2"][i] = Vexp / variables["Потребление_O2"][i];
	variables["Вентиляционный_эквивалент_CO2"][i] = Vexp / variables["Выделение_CO2"][i];

	// РАСЧЕТ ДЫХАТЕЛЬНОГО КОЭФФИЦИЕНТА
	variables["Дыхательный_коэффициент"][i] = variables["Минутное_выделение_CO2"][i] / variables["Минутное_потребление_O2"][i];
	double& BrK_Ptr = variables["Дыхательный_коэффициент"][i];
	double tempMOP = variables["Минутное_потребление_O2"][i];
	double tempMCOP = variables["Минутное_выделение_CO2"][i];

	if (BrK_Ptr<0) BrK_Ptr = 0.0;
	if (tempMOP<0.01 || tempMCOP<0.01)
		BrK_Ptr = 0.0;
	if (tempMCOP<0) tempMCOP = 0;
	if (tempMOP <0) tempMOP = 0;
	if (BrK_Ptr>1.0)
	{
		tempMCOP = tempMOP;
	}

	double temp = tempMOP * 20000;//KKAL - O2
	temp = temp * 60 * 24;//24 hours
	temp = temp / 4.18 / 1000;
	variables["Метаболизм_O2"][i] = temp;

	temp = tempMOP * 3.94062 + tempMCOP * 1.10638; // KKAL - O2+CO2
	temp = temp * 60 * 24;//24 hours
	variables["Метаболизм_CO2"][i] = temp;

	variables["ДМП"][i] = (variables["Минутное_потребление_O2"][i] * 1000 / (variables["Vвыдоха"][i] * variables["ЧД"][i]));


	if (hdata.HR)// && (datetime[i].getTime() > datetime[0].getTime() + 29))
	{
		CalculateSD(i);
	}
}
//---------------------------------------------------------------------
void Database::DeleteUncheckedValues() // удаляет из переменных значения, с которых галки сняты
{
	int j = 0;
	
	for (int i = 0; i<variables["Vвдоха"].size(); i++)
	{
		if (!checked[i])
		{
			for (map<string, Variable>::iterator it = variables.begin();
				it != variables.end();
				++it)
			{
				it->second.erase(i - j);
			}
			datetime.erase(i - j);
			checked.erase(i - j);
			j++;
		}
	}
	//delete [] checked;
	//checked = NULL;
}
//---------------------------------------------------------------------
string  Database::GetHTMLHeader() // временный заголовок для экспотра протоколов из скрипта
{
	stringstream ss("");
	int page_width = 800;
	char bbuf[1024];
	//write html-header with coding
	ss << "<html><head><meta http-equiv='Content-Type' content='text/html; charset=windows-1251'>\n<style type='text/css'>\n";
	ss << "TABLE {width: %dpx;    border-collapse: collapse; }A:link {text-decoration: none; color: black; }A:visited { text-decoration: none; color: black;}A:active { text-decoration: none; color: black;}A:hover {text-decoration: none; color: red; border-bottom: 1px dashed;}\n</style>\n</head> \n<center>\n";
	ss << "<table width=" << page_width << " bordercolor=white>\n <tr><td><img src='images\\logo.png'><br><a href=lanamedica.ru><center><b>www.lanamedica.ru</b></center></a> <td align=center><font size=5><b>Исследование выполнено на оборудовании <br><a href=http://lanamedica.ru/new/catalog/spirolan-m/>СПИРОЛАН-М</a></b></font>\n</table>";
	ss << "<hr align=center width=" + to_string(page_width) + " size=2 color=#000/>\n";
	ss << "<table width=" << page_width << " bordercolor=white>";
	ss << "<tr><td width=25%%\%>Дата исследования:<td width=30%%><font size=4><b>";
	ss << datetime[0].getDatetimeString() << " -<br>" << datetime[datetime.size() - 1].getDatetimeString() << endl;
	ss << "<td width=25%%>Возраст:<td width=15%%><font size=4><b>" << static_cast<int>(hdata.PatientAge) << " лет</b></font>\n";
	ss << "<tr><td>ФИО:<td><font size=4><b>" << hdata.PatientName << "</b></font>\n";
	ss << "<td>Вес:<td><font size=4><b>" << static_cast<int>(hdata.PatientWeight) << " кг</b></font>\n";
	ss << "<tr><td>Пол:<td><font size=4><b>" << (hdata.PatientSex ? "мужской" : "женский") << "</b></font>\n";
	ss << "<td>Рост:<td><font size=4><b>" << static_cast<int>(hdata.PatientHeight) << " см</b></font>\n";
	ss << "</table>\n";

	ss << "<hr align=center width=" << page_width << " size=2 color=#000/>\n";

	ss << "<table width=" << page_width << " bordercolor=white><tr><td align=left>";

	return ss.str();


}