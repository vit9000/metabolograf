#ifndef Structures_h
#define Structures_h


struct HeaderDatabase
{
		int count;
        int PatientWeight;
        int PatientHeight;
        int PatientSex;
        double PatientAge;
        char PatientName[128];
        char AdditionalInformation[226];
		int8_t PowerStep;

		int8_t HR;
		int StartTest;// ������ ��������
		int EndTest;//����� ��������
		int AeT;//�������� �����
		int AT;//���������� �����
		int MCO;//maximum oxygen comsumption
		
		double PatientWrist;
        bool UseExpiratoryVolume;
        bool Filter_TVolume;
        bool Filter_BrKoef;
        bool OsrednenieType2;
        bool UseMeanVolume;
        bool UseInspiratoryVolume;
        int Day;
        int Month;
        int Year;
};
#endif
