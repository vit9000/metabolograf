#ifndef YDATABASE_H
#define YDATABASE_H

typedef unsigned char BYTE;
typedef long LONG;
typedef double DATE;
//typedef wchar_t TCHAR;

typedef struct
{
       LONG pac_id;
       LONG exp_id;
       wchar_t pac_code[32];
       wchar_t pac_dep[32];
       wchar_t pac_lname[32];
       wchar_t pac_fname[32];
       wchar_t pac_mname[32];
       BYTE pac_sex;
       DATE pac_birth;
       DATE expdt;
       BYTE pac_height;
       BYTE pac_weight;
       wchar_t pac_diagn[60];
       long action; // предпологаемое действие 0-нет файлов 1-сохранить фaйл в FileName;
       LONG doc_id;
       wchar_t doc_name[32];
       long sub_type;
       wchar_t doc_concl[200];
       LONG temp;
       LONG pres;
       int IKCH;
       int IPL;
       int ACT;
       int CAT;
       wchar_t file_name[255];

}
FDINFO;

#endif // MONITORBLOCK_H