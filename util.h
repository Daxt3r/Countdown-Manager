#ifndef UTIL_H
#define UTIL_H

//#pragma warning( disable: C4996 ) // C4996 ist Nummer der Warnung diese wird ausgeschaltet

FILE *Open_File(char cPfad[], char cMode[]);
void SetNewCharacter(char cBuffer[], char cSearch_Symbole, char cNew_Symbole);
int GetWords(char cString[], char cSearch_Word[]);
int Split_String(char cString[], char cResearch_Criteria[]);
int GetLines(FILE *pFile);
int GetLinesAndSize(FILE *pFile, unsigned int *nZeichen);
int GetMaxLength(FILE *pFile);
int CheckInt(char *cString);
int CheckFloat(char *cString);
void Qsort_int(int nZahlen[], int L, int R);
void Divide_int(int nZahlen[], int L, int R, int *L1, int *R1);
void Swap_int(int nZahlen[], int i, int j);
void Qsort_char(char *cDaten[], int L, int R);
void Divide_char(char *cDaten[], int L, int R, int *L1, int *R1);
void Swap_char(char *cDaten[], int i, int j);
void CheckSortDatas(FILE *pFile);
char *SatzZerlegen(char *cSatz, char cTrenner, char *pSatz);

#endif
