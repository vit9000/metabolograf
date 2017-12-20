#ifndef EXPORT_H
#define EXPORT_H


#include "Database.h"
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"



class Export
{
public:
    Export(Database *_database);
    ~Export()
    {
        database = NULL;
    }
    void ExportDataString();

    void StandartExport(bool fitness);
    void DietaExport();
    void CustomDietaExport(double NBK, double DUK);
    void UreaDietaExport(double Urea, double koefficient);
    void CalcPFCH(double *P, double *F, double *CH, double *p_body, double *BK, double MOP24, double DUK, double NBK, double *BrK);
    void CalcPFCHUrea(double *P, double *F, double *CH, double *p_body, double MOP24, double MCOP24, double urea, double koefficient);
    void CalcPFCH_NBK (double *P, double *F, double *CH, double *p_body, double MOP24, double MCOP24, double NBK);

    void CalcIdealWeight();
    void ReplacePoint(char *buf, int length);
    FILE * CreateExportFile(string type, string extension);
    void WriteHeader(FILE *file);
    void WriteCalcutatedMetab(FILE *file);
    void CalculateMeanValues();
    void WriteMesuredValues(FILE *file);
    void CloseFile(FILE *file);

    void ExportExcel();
    void ReplaceSymbols(char *str, char a, char b);
	void ReplaceSymbols(string& str, char a, char b);

    int page_width;
    string file_name;
    int target[2];
    double IdealWeight;
    Database *database;
    string curDir;



    double VO2;
    double VCO2;
    double p_body;
    double breathK;
    double tidalVol;
    double Vexp;
    double RespRate;
    double DMP;
    int teloslozhenie;

    double koefficient1;
    double koefficient2;

    double MOP24;
    double MCOP24;
    double breathK_sredn;
};

#endif // MONITORBLOCK_H