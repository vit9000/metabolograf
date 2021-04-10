#pragma once

const int MinutValueSpiro = 30000;	// кол-во опросов спирографа за минуту
const int MinutValueCapno = 1500;	// кол-во опросов капнографа за минуту
const int MaxCikl = 10000;			// максимальное кол-во дыхательных циклов


struct CIKL_CAPNO // дыхательный цикл капнографа
{
	float Vi;		// объём вдоха
	float Ve;		// объём выдоха

	int EtCO2;		// концентрация CO2 в конце выдоха (мм)

	float FiCO2pr;	// концентрация CO2 на вдохе (% x 10)
	float EtCO2pr;	// концентрация CO2 в конце выдоха (% x 10)
	float FiO2pr;	//процент О2 на вдохе
	float EtO2pr;	//процент О2 на выдохе

	int CHD;		//частота дыхания

	int BegI;	//начало вдоха
	int BegE;	//начало выдоха
	int Li;		//длина вдоха
	int Le;		//длина выдоха
};



struct CIKL_SPIRO // дыхательный цикл спирографа
{
	float Vi;		// объём вдоха
	float Ve;		// объём выдоха

	int EtCO2;		// концентрация CO2 в конце выдоха (мм)

	float FiCO2pr;	// концентрация CO2 на вдохе (% x 10)
	float EtCO2pr;	// концентрация CO2 в конце выдоха (% x 10)
	float FiO2pr;	//процент О2 на вдохе
	float EtO2pr;	//процент О2 на выдохе

	float VEO_FiCO2pr;	// VEO_концентрация CO2 на вдохе (% x 10)
	float VEO_EtCO2pr;	// VEO_концентрация CO2 в конце выдоха (% x 10)
	float VEO_FiO2pr;	// VEO_процент О2 на вдохе
	float VEO_EtO2pr;	// VEO_процент О2 на выдохе

	float VEO_FiAX1pr;	// VEO_концентрация AX1 на вдохе (% x 10) 
	float VEO_EtAX1pr;	// VEO_концентрация AX1 в конце выдоха (% x 10)
	float VEO_FiAX2pr;	// VEO_процент AX2 на вдохе
	float VEO_EtAX2pr;	// VEO_процент AX2 на выдохе

	int CHD;		//частота дыхания

	int delta;		//длина цикла

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
	int AmntCikl;						//кол-во дыхательных циклов
	CIKL_SPIRO CiklSpiro[MaxCikl];		//массив структур дыхательного цикла

	int prEndCiklSpiro;					//признак окончания дыхательного цикла по спирографу
	int prEndCiklCapno;					//признак окончания дыхательного цикла по капнографу
	int WaitNewCiklGas;					//признак ожидания начала цикла по капнографу
	int WaitNewConc;					//признак ожидания конца  цикла по капнографу

	LONG spBytes;						//количество байт спиро
	int FlowPtr;						//указатель на массивы потока и объёма
	int FlowBase;						//Изолиния потока
	int adcFlow[MinutValueSpiro];		//массив потока за минуту (АЦП)
	float Flow[MinutValueSpiro];		//массив потока за минуту
	float Volume[MinutValueSpiro];		//массив объёмов за минуту	

	int PresPtr;
	int PresBase;						//Изолиния давления
	int adcPres[MinutValueSpiro];
	float Pres[MinutValueSpiro];

	LONG gasBytes;							//количество байт капно
	int PtrCapno;							//указатель на массивы карнограммы и оксиграммы
	float Kapnogramma[MinutValueCapno];		//массивы значения капнограммы за минуту
	float Oxigramma[MinutValueCapno];		//массивы значения оксиграммы  за минуты
	float Oxigramma0;						//текущее значение оксиграммы	
	float Kapnogramma0;						//текущее значение капнограммы

	int FiCO2;								// концентрация CO2 на вдохе (мм.рт.ст.)
	int EtCO2;								// концентрация CO2 в конце выдоха (мм.рт.ст.)
	float FiCO2pr;							// концентрация CO2 на вдохе (% x 10)
	float EtCO2pr;							// концентрация CO2 в конце выдоха (% x 10)

	int CHD;								//частота дыхания
	float FiO2pr;							//процент О2 на вдохе
	float EtO2pr;							//процент О2 на выдохе

	int MSS;	//тип выбранной системы системы удаления влаги
				// 1 - гидрофобный дисковый фильтр (по умолчанию)
				// 0 - другая система удаления влаги
	int NoLine;	// 1 - линия забора пробы или система удаления влаги не подключены
	int OCC;	// 1 - когда пневматическая система модуля находится в состоянии "окклюзии"
	int Apnea;	// 1 - дыхание пациента не регистрируется в течении 20 сек.
	int Wave;	// 1 - передаются отсчёты капнограммы (в 4-5 байте)
				// 0 - передаются отсчёты оксиграммы  (в 4-5 байте)
	int I_E;	// 1 - идёт вдох
				// 0 - идёт выдох
	int Cal0;	// 1 - идёт процесс калибровки нулевой концентрации CO2
	int CalG;	// 1 - идёт процесс калибровки усиления канала измерения CO2

	int Run;	// 1 - состояние работы
				// 0 - состояние ожидания
	int BTPS;	// 1 - установлен режим BTPS
				// 0 - не установлен режим BTPS
	int NEO;	// 1 - модуль работает в неонатальном режиме 
				// 0 - модульработает во взрослом режиме (умолчание)
	int MNT;	// 1 - включён режим горы
				// 0 - сброшен режим горы
	int RTE;	// 1 - включен алгоритм RTE (умолчание)  
				// 0 - выключен алгоритм RTE  
	int Optic;	// не используется
	int Clbr0;	// 1 требуется провести калибровку нулевой концентрации CO2
				// 0 сбрасывается после калибровки.
	int MSTR;	// 1 - Master
				// 0 - Slave (умолчание)

	int No_O2;	// 1 - датчик кислорода не подключён к модулю
	int Er_O2;	// 1 - датчик кислорода подключён, но требуется его замена

	int Pkam;	// текущее давление в измерительной камере (мм.рт.ст.)
	int Patm;	// текущее атмосферное давление (мм.рт.ст.)
	int Tkam;	// текущая температура в измерительной камере CO2 (град. C)

	float UAnalog[5];	// напряжения аналоговых сигналов

	float Upump;		// текущее напряжение на компрессоре (В * 10)
	int Ipump;			// текущий потребляемый ток компрессором (мА)	
	int Pump_Run_Time;	// текущая наработка компрессора (ч)
	int After_Cal;		// время от последней калибровки усиления CO2 (ч)

	int Ser_Num;		// серийный номер модуля
	float SW_Rev;		// версия программного обеспечения модуля

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

	int spt; // Температура измереная спирометром (СП+)
	int spp; // Атм.Давление измереная спирометром (СП+)
	int sph; // Отн.Влажность измереная спирометром (СП+)

	float CO2_Gain; // Коэффициент усиления CO2 (устанавливается после калибровки CO2)
	float CO2_Level; // Концентрация СО2 в эталонном баллоне (для калибровки СО2)

	float PIP; // Давление конца вдоха (см.вд.ст)
	float PEEP; // Давление конца выдоха (см.вд.ст)
	float Fres; // Поток джля расчета Raw
	float Ppl; // Давление плато (см.вд.ст)
	float Raw; // Резистанс
	float Raw2; // Резистанс рассчитанный через постоянную времени
	float Cst1; // Комплайнс через объем вдоха
	float Cst2; // Комплайнс через вдох без BTPS - не используется
	float Cst3; // Комплайнс через выдох - не используется
	float Cdyn; // Комплайнс динамический
	float Aupr; // Упругая работа (площадь треугольника до PIP)
	float Aupr2; // Упругая работа (площадь треугольника до точки перегиба давления - максимальный поток на выдохе)
	float Ain; // Работа на вдохе (до паузы)
	float Aex; // Работа на выдохе (после паузы)

	DATE TimeBeg; // Время начала цикла (начала вдоха)

	float Tau; // Постоянная времени (время через которое объем уменьшается до 36,8% от начального)
	float Tin; // Время вдоха
	float Tex; // Время выдоха

	LONG tc0; // GetTickCount в начале работы

	float fVsum[2];						// Суммарный объем вдоха и выдоха (0-вдох)

	int RR;
	int HR;

	int mrdone;

	unsigned char pac_height; // рост пациента
	unsigned char pac_weight; // вес пациента
	unsigned char pac_age; // возраст
	unsigned char pac_sex; // пол

	int vds_mask; // объем мертвого пространства маски

	int HRnew;

};

