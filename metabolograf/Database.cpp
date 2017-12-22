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
	hdata.StartTest = 0;// ������ ��������
	hdata.EndTest = 0;//����� ��������
	hdata.AeT = 0;//�������� �����
	hdata.AT = 0;//���������� �����
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

	variable_names.push_back("�������");
	variable_names.push_back("���");
	variable_names.push_back("����");
	variable_names.push_back("���");
	variable_names.push_back("V�����");
	variable_names.push_back("V������");
	variable_names.push_back("��");
	variable_names.push_back("FiO2");
	variable_names.push_back("FetO2");
	variable_names.push_back("FiCO2");
	variable_names.push_back("FetCO2");
	variable_names.push_back("V�����_���_��");
	variable_names.push_back("V������_���_��");
	variable_names.push_back("�����������_O2");
	variable_names.push_back("���������_CO2");
	variable_names.push_back("�����������_O2_��_��_���");
	variable_names.push_back("���������_CO2_��_��_���");
	variable_names.push_back("��������_�����������_O2");
	variable_names.push_back("��������_���������_CO2");
	variable_names.push_back("��������������_����������_O2");
	variable_names.push_back("��������������_����������_CO2");
	variable_names.push_back("�����������_�����������");
	variable_names.push_back("����������_O2");
	variable_names.push_back("����������_CO2");
	variable_names.push_back("���");
	variable_names.push_back("��������_����������");


	if (hdata.HR == 1)
	{
		variable_names.push_back("���");
		variable_names.push_back("RR");
		variable_names.push_back("SD");
	}
	else hdata.HR = 0;
}
//---------------------------------------------------------------------
void Database::CalculateMetab()
{
	//Harris-Benedict equation
	double oo = 0.;//�������� �����
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

		MessageBox(0, fname, "������ ���������� �����", MB_OK);

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

	// ������ �������� �������. �������� ����, ������, �������
	out.write((char *)H.getPtr(), sizeof(int)*count);
	out.write((char *)M.getPtr(), sizeof(int)*count);
	out.write((char *)S.getPtr(), sizeof(int)*count);
	// ������ ������������ �����
	out.write((char *)checked.getPtr(), sizeof(bool)*count);
	
	//������ �����������
	out.write((char *)variables["V�����"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["V������"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["��"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FiO2"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FetO2"].getPtr(), sizeof(double)*count);
	out.write((char *)variables["FiCO2"].getPtr(), sizeof(double)*count);
	// � CO2 ������ �������, ��� ����������� �������, ��������� ����������� �� 100
	Variable FetCO2 = variables["FetCO2"] / 100;
	out.write((char *)FetCO2.getPtr(), sizeof(double)*count);
	if (hdata.HR)
		out.write((char *)variables["���"].getPtr(), sizeof(double)*count);
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
		variables["���"] = var;
	}

	CalculateParameters(); // ��������� ������ �������������� ����������

	in.close();
	//������������� ������������ ��������� ������������
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
	variables["�������"] = Variable(hdata.PatientAge);
	variables["�������"].SetType(Scalar);
	variables["���"] = Variable(static_cast<double>(hdata.PatientSex));
	variables["���"].SetType(Scalar);
	variables["����"] = Variable(static_cast<double>(hdata.PatientHeight));
	variables["����"].SetType(Scalar);
	variables["���"] = Variable(static_cast<double>(hdata.PatientWeight));
	variables["���"].SetType(Scalar);
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
	//������ ����������� ��������� � ��������� ��2
	if (hdata.Filter_TVolume && (variables["V�����"][index] >= VOL_INSP_MAX || variables["V�����"][index] <= VOL_INSP_MIN))
	{
		status = false;
	}
	else if ((hdata.Filter_BrKoef) && (variables["�����������_�����������"][index]>2.5 || variables["�����������_�����������"][index]<0.5))
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
		variables["V�����_���_��"] = variables["V�����"] - (0.3*childTV + ADS) / 1000.;//��� �������� ������������
		variables["V������_���_��"] = variables["V������"] - (0.3*childTV + ADS) / 1000.;//��� �������� ������������

	}
	else
	{
		variables["V�����_���_��"] = variables["V�����"] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//��� �������� ������������
		variables["V������_���_��"] = variables["V������"] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//��� �������� ������������
	}
	Variable Vexp = variables["V������_���_��"];
	Variable Vinsp = variables["V�����_���_��"];;
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



	variables["��������_����������"] = Vexp * variables["��"];

	variables["�����������_O2"] = Vinsp * variables["FiO2"] / 100 - Vexp * variables["FetO2"] / 100;//����������� �� ��������� � ������� ����������� ����
	variables["���������_CO2"] = Vexp*variables["FetCO2"] / 100.;// - Volinsp*data.FiCO2[Circle];//��������� ����������� ���� � ������� ��� ����
	variables["��������_�����������_O2"] = variables["�����������_O2"] * variables["��"]; //metab->RespRate;//����������� �� ��������� � ���
	variables["��������_���������_CO2"] = variables["���������_CO2"] * variables["��"];//metab->RespRate;//��������� ����������� ���� � ���

	variables["�����������_O2_��_��_���"] = variables["��������_�����������_O2"] * 1000 / hdata.PatientWeight;//variables["���"];
	variables["���������_CO2_��_��_���"] = variables["��������_���������_CO2"] * 1000 / hdata.PatientWeight;//variables["���"];;

	variables["��������������_����������_O2"] = Vexp / variables["�����������_O2"];
	variables["��������������_����������_CO2"] = Vexp / variables["���������_CO2"];

	// ������ ������������ ������������
	variables["�����������_�����������"] = variables["��������_���������_CO2"] / variables["��������_�����������_O2"];
	double *BrK_Ptr = variables["�����������_�����������"].getPtr();
	Variable tempMOP = variables["��������_�����������_O2"];
	Variable tempMCOP = variables["��������_���������_CO2"];
	for (int i = 0; i < variables["��������_���������_CO2"].size(); i++)
	{
		if (BrK_Ptr[i] < 0) BrK_Ptr[i] = 0.0;
		if (variables["��������_�����������_O2"].getPtr()[i] < 0.01 || variables["��������_���������_CO2"].getPtr()[i] < 0.01)
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
	variables["����������_O2"] = temp;

	temp = tempMOP * 3.94062 + tempMCOP * 1.10638; // KKAL - O2+CO2
	temp = temp * 60 * 24;//24 hours
	variables["����������_CO2"] = temp;

	variables["���"] = (variables["��������_�����������_O2"] * 1000 / (variables["V������"] * variables["��"]));

	if (hdata.HR && variables["SD"].size() == 0)
		FillSD();
}
//--------------------------------------------------------------------------
void Database::FillSD()
{
	if (getCount() <= 0) return;
	if (!hdata.HR) return;

	variables["RR"] = 60000. / variables["���"];
	variables["SD"].resize(getCount());

	int start = 0;

	// ������ ������� 30 ��������� �������
	MTime starttime = datetime[0].getTime() + 30;
	for (; start < getCount(); ++start)
	{
		if (datetime[start].getTime() > starttime)
		{
			--start;
			break;
		}
	}
	//�������� � 30 ��������� �������
	Variable sd(getCount());
	for (int i = start; i < getCount(); ++i)
	{//���� �� ��������

		CalculateSD(i, false);

	}
}
//--------------------------------------------------------------------------
void Database::CalculateSD(int i, bool checktime)
{
	if (i < 0) return;
	variables["RR"][i] = 60000. / variables["���"][i];
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
		variables["V�����_���_��"][i] = variables["V�����"][i] - (0.3*childTV + ADS) / 1000.;//��� �������� ������������
		variables["V������_���_��"][i] = variables["V������"][i] - (0.3*childTV + ADS) / 1000.;//��� �������� ������������

	}
	else
	{
		variables["V�����_���_��"][i] = variables["V�����"][i] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//��� �������� ������������
		variables["V������_���_��"][i] = variables["V������"][i] - (2.2*(hdata.PatientHeight - 100) + ADS) / 1000.;//��� �������� ������������
	}

	double Vexp = variables["V������_���_��"][i];
	double Vinsp = variables["V�����_���_��"][i];
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



	variables["��������_����������"][i] = Vexp * variables["��"][i];

	variables["�����������_O2"][i] = Vinsp * variables["FiO2"][i] / 100 - Vexp * variables["FetO2"][i] / 100;//����������� �� ��������� � ������� ����������� ����
	variables["���������_CO2"][i] = Vexp*variables["FetCO2"][i] / 100.;// - Volinsp*data.FiCO2[Circle];//��������� ����������� ���� � ������� ��� ����
	variables["��������_�����������_O2"][i] = variables["�����������_O2"][i] * variables["��"][i]; //metab->RespRate;//����������� �� ��������� � ���
	variables["��������_���������_CO2"][i] = variables["���������_CO2"][i] * variables["��"][i];//metab->RespRate;//��������� ����������� ���� � ���

	variables["�����������_O2_��_��_���"][i] = variables["��������_�����������_O2"][i] * 1000 / hdata.PatientWeight;//variables["���"];
	variables["���������_CO2_��_��_���"][i] = variables["��������_���������_CO2"][i] * 1000 / hdata.PatientWeight;//variables["���"];;

	variables["��������������_����������_O2"][i] = Vexp / variables["�����������_O2"][i];
	variables["��������������_����������_CO2"][i] = Vexp / variables["���������_CO2"][i];

	// ������ ������������ ������������
	variables["�����������_�����������"][i] = variables["��������_���������_CO2"][i] / variables["��������_�����������_O2"][i];
	double& BrK_Ptr = variables["�����������_�����������"][i];
	double tempMOP = variables["��������_�����������_O2"][i];
	double tempMCOP = variables["��������_���������_CO2"][i];

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
	variables["����������_O2"][i] = temp;

	temp = tempMOP * 3.94062 + tempMCOP * 1.10638; // KKAL - O2+CO2
	temp = temp * 60 * 24;//24 hours
	variables["����������_CO2"][i] = temp;

	variables["���"][i] = (variables["��������_�����������_O2"][i] * 1000 / (variables["V������"][i] * variables["��"][i]));


	if (hdata.HR)// && (datetime[i].getTime() > datetime[0].getTime() + 29))
	{
		CalculateSD(i);
	}
}
//---------------------------------------------------------------------
void Database::DeleteUncheckedValues() // ������� �� ���������� ��������, � ������� ����� �����
{
	int j = 0;
	
	for (int i = 0; i<variables["V�����"].size(); i++)
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
string  Database::GetHTMLHeader() // ��������� ��������� ��� �������� ���������� �� �������
{
	stringstream ss("");
	int page_width = 800;
	char bbuf[1024];
	//write html-header with coding
	ss << "<html><head><meta http-equiv='Content-Type' content='text/html; charset=windows-1251'>\n<style type='text/css'>\n";
	ss << "TABLE {width: %dpx;    border-collapse: collapse; }A:link {text-decoration: none; color: black; }A:visited { text-decoration: none; color: black;}A:active { text-decoration: none; color: black;}A:hover {text-decoration: none; color: red; border-bottom: 1px dashed;}\n</style>\n</head> \n<center>\n";
	ss << "<table width=" << page_width << " bordercolor=white>\n <tr><td><img src='images\\logo.png'><br><a href=lanamedica.ru><center><b>www.lanamedica.ru</b></center></a> <td align=center><font size=5><b>������������ ��������� �� ������������ <br><a href=http://lanamedica.ru/new/catalog/spirolan-m/>��������-�</a></b></font>\n</table>";
	ss << "<hr align=center width=" + to_string(page_width) + " size=2 color=#000/>\n";
	ss << "<table width=" << page_width << " bordercolor=white>";
	ss << "<tr><td width=25%%\%>���� ������������:<td width=30%%><font size=4><b>";
	ss << datetime[0].getDatetimeString() << " -<br>" << datetime[datetime.size() - 1].getDatetimeString() << endl;
	ss << "<td width=25%%>�������:<td width=15%%><font size=4><b>" << static_cast<int>(hdata.PatientAge) << " ���</b></font>\n";
	ss << "<tr><td>���:<td><font size=4><b>" << hdata.PatientName << "</b></font>\n";
	ss << "<td>���:<td><font size=4><b>" << static_cast<int>(hdata.PatientWeight) << " ��</b></font>\n";
	ss << "<tr><td>���:<td><font size=4><b>" << (hdata.PatientSex ? "�������" : "�������") << "</b></font>\n";
	ss << "<td>����:<td><font size=4><b>" << static_cast<int>(hdata.PatientHeight) << " ��</b></font>\n";
	ss << "</table>\n";

	ss << "<hr align=center width=" << page_width << " size=2 color=#000/>\n";

	ss << "<table width=" << page_width << " bordercolor=white><tr><td align=left>";

	return ss.str();


}