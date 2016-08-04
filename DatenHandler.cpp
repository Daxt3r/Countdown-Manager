#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <Windows.h>
#include <sys/stat.h>
#include "Eigenschaften.h"
#include "DatenEingeben.h"
#include "DatenHandler.h"


/* forward declaration */
bool ErrorFlag = false;

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft die eingegebene Uhrzeit des Benutzer auf folgende Kriterien:
// - Stimmt das Format der Uhrzeit --> hh:mm
// - Besteht die Stunde und Minute jeweils nur aus nummerischen Zeichen
// - Liegt die Stunde zwischen 00-23 und die Minute zwischen 00-59
//Parameter:    - *cBuffer = Wert der geprüft werden soll
//Rückgabewert: - 0 = Wenn eine Prüfung nicht in Ordnung war
//			    - 1 = Wenn die Prüfungen in Ordnung waren
int PruefeUhrzeit(char *cString)
{
	int nStd_tmp = 0;
	int nMin_tmp = 0;
	char cTmp[MAXLENGTH];

	strcpy(cTmp, cString); //cBuffer wird in cTmp kopiert, da strtok den Inhalt von cBuffer kaputt macht

	if( (!CheckInt(strtok(cString, ":"))) || (!CheckInt(strtok(NULL, "\0"))) ) //Es wird geprüft ob die Stunde nur aus nummerischen Werten besteht
	{
		printf("Die Uhrzeit besteht nicht nur aus nummerischen Werten!\n");
		return 0;
	}

	nStd_tmp = atoi(strtok(cTmp, ":"));
	nMin_tmp = atoi(strtok(NULL, "\0"));

	if((nStd_tmp < 0 ) || (nStd_tmp > 23))
	{
		printf("Der Stundenwert darf nur zwischen 00-23 liegen!\n");
		ErrorFlag = true;
	}
	if((nMin_tmp < 0) || (nMin_tmp > 59))
	{
		printf("Der Minutenwert darf nur zwischen 00-59 liegen!\n");
		ErrorFlag = true;
	}
	if(ErrorFlag)
		return 0;
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Es wird geprüft ob der eingelesene Wert richtig ist (bekannte Werte = True/true oder False/false)
//Parameter: - *cString = Wert der geprüft werden soll
//Rückgabewert: - 0 = Wenn eine Prüfung nicht in Ordnung war
//			    - 1 = Wenn die Prüfungen in Ordnung waren
int PruefeTrue_or_False(char *cString)
{
	if((strcmp(cString, "True") == 0) || (strcmp(cString, "true") == 0)) //Es wird geprüft ob der Benutzer Ja/ja eingegeben hat
		return 1;
	else if((strcmp(cString, "False") == 0) || (strcmp(cString, "false") == 0)) //Es wird geprüft ob der Benutzer Nein/nein eingegeben hat
		return 1;
	else
		return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die vom Benutzer eingegebenen Daten werden in die Eigenschaften.txt Datei geschrieben
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void UpdateEintraege(struct tEigenschaften *pEigenschaften)
{
	if(pEigenschaften->fpEigenschaften != NULL)
		fclose(pEigenschaften->fpEigenschaften);

	pEigenschaften->fpEigenschaften = Open_File(EIGENSCHAFTENFILE, "w");
	if(pEigenschaften->fpEigenschaften == NULL)
		return;

	//Die Uhrzeit wird in die Eigenschaftendatei geschrieben
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d\n", ENDSTDTAG, pEigenschaften->nEndStd);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d\n", ENDMINTAG, pEigenschaften->nEndMin);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n", SHUTDOWNTAG, pEigenschaften->cShutdown);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%d\n", SHUTDOWNTIMETAG, pEigenschaften->nShutdownTime);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n\n", SECONDSTAG, pEigenschaften->cSeconds);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n", WEEKDAY, pEigenschaften->cWeekday);
//	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_MO, pEigenschaften->pWday.nEndStd[0], pEigenschaften->pWday.nEndMin[0]);
//	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_DI, pEigenschaften->pWday.nEndStd[1], pEigenschaften->pWday.nEndMin[1]);
//	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_MI, pEigenschaften->pWday.nEndStd[2], pEigenschaften->pWday.nEndMin[2]);
//	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_DO, pEigenschaften->pWday.nEndStd[3], pEigenschaften->pWday.nEndMin[3]);
//	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d", WEEKDAY_FR, pEigenschaften->pWday.nEndStd[4], pEigenschaften->pWday.nEndMin[4]);
	
	fclose(pEigenschaften->fpEigenschaften); //Die Eigenschaften Datei wird geschlossen
}