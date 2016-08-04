#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <Windows.h>
#include <sys/stat.h>
#include "Eigenschaften.h"
#include "DatenEinlesen.h"
#include "DatenEingeben.h"
#include "DatenHandler.h"


bool DatenFlag = false; // - false = Es wurden keine Daten eingelesen
						// - true  = Es wurden Daten eingelesen
bool FalscheDatenFlag = false;	// - false = Es wurden keine Daten eingelesen
								// - true  = Es wurden Daten eingelesen

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion liest die Eigenschaften aus der Eigenschaften.txt Datei in die Struktur tEigenschaften ein
//Parameter: - *fpEigenschaften = Zeiger auf die Textdatei Eigenschaften.txt
//			 - *pEigenschaften  = Zeiger auf die Struktur tEigenschaften
void EigenschaftenEinlesen(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	char *pTmp;

	//Es wird geprüft ob die Eigenschaftsdatei bereits geöffnet ist, da entweder das Programm gestartet wurde
	//oder eine Änderung der Daten stattgefunden hat
	pEigenschaften->fpEigenschaften = Open_File(EIGENSCHAFTENFILE, "r");
	if(pEigenschaften->fpEigenschaften == NULL)
		return;

	while(fgets(cBuffer, sizeof(cBuffer), pEigenschaften->fpEigenschaften) != NULL)
	{
		SetNewCharacter(cBuffer, '\n', '\0');
		pTmp = strtok(cBuffer, "=");
		if(pTmp != NULL)
		{
			if(strcmp(pTmp, ENDSTDTAG) == 0) //Endstunde wird eingelesen
				EndStundeEinlesen(pEigenschaften);
			else if(strcmp(pTmp, ENDMINTAG) == 0) //Endminute wird eingelesen
				EndMinuteEinlesen(pEigenschaften);
			else if(strcmp(pTmp, SHUTDOWNTAG) == 0) //Es wird eingelesen ob der Rechner heruntergefahren werden soll
				ShutdownEinlesen(pEigenschaften);
			else if(strcmp(pTmp, SHUTDOWNTIMETAG) == 0) //Die Zeit zum Herunterfahren des Rechners wird eingelesen
				ShutdownSecondEinlesen(pEigenschaften);
			else if(strcmp(pTmp, SECONDSTAG) == 0) //Es wird eingelesen ob die Sekunden angezeigt werden sollen
				SecondEinlesen(pEigenschaften);
			else if(strcmp(pTmp, WEEKDAY) == 0)
			{
				WeekdayEinlesen(pEigenschaften);
				if(strcmp(pEigenschaften->cWeekday, "true") == 0)
					WeekdayTimeEinlesen(pEigenschaften);
			}
			DatenFlag = true;
		}
	}

	if(FalscheDatenFlag) //Es wurden falsche Daten aus der Eigenschaften.txt Datei eingelesen, dadurch wurden Defaultwerte gesetzt
	{					 //welche nin die Datei geschrieben werden müssen
		UpdateEintraege(pEigenschaften);
		return;
	}

	if(!DatenFlag) //Es wird geprüft ob Daten aus der Eigenschaften Datei eingelesen werden konnten
	{
		printf("Es konnten keine Daten aus der Eigenschaften.txt Datei eingelesen werden!\n");
		pEigenschaften->fpEigenschaften = Open_File("Eigenschaften.txt", "w");
		if(pEigenschaften->fpEigenschaften == NULL)
			return;
		DatenEingeben(pEigenschaften);
	}
	if(pEigenschaften->fpEigenschaften != NULL)
		fclose(pEigenschaften->fpEigenschaften); //Zeiger auf die Textdatei wird geschlossen

	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *cWert = Eingelesener Wert
//			 - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EndStundeEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;

	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if((atoi(pWert) < 0 ) || (atoi(pWert) > 23))
		{
			printf("Der eingelesene Wert: %s ist nicht in Ordnung! Die Wert wird auf den DEFAULTWERT (%d) gesetzt!\n", ENDSTDTAG, DEFAULT_ENDSTUNDE);
			pEigenschaften->nEndStd = DEFAULT_ENDSTUNDE;
			FalscheDatenFlag = true;
			return;
		}
		pEigenschaften->nEndStd = atoi(pWert);
	}
	else
	{
		printf("Der eingelesene Wert: %s ist nicht in Ordnung! Die Wert wird auf den DEFAULTWERT (%d) gesetzt!\n", ENDSTDTAG, DEFAULT_ENDSTUNDE);
		pEigenschaften->nEndStd = DEFAULT_ENDSTUNDE;
		FalscheDatenFlag = true;
	}

	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *cWert = Eingelesener Wert
//			 - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EndMinuteEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;

	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if((atoi(pWert) < 0) || (atoi(pWert) > 59))
		{
			printf("Der eingelesene Wert: %s ist nicht in Ordnung! Die Wert wird auf den DEFAULTWERT (%d) gesetzt!\n", ENDMINTAG, DEFAULT_ENDMINUTE);
			pEigenschaften->nEndMin = DEFAULT_ENDMINUTE;
			FalscheDatenFlag = true;
			return;
		}
		pEigenschaften->nEndMin = atoi(pWert);
	}
	else
	{
		printf("Der eingelesene Wert: %s ist nicht in Ordnung! Die Wert wird auf den DEFAULTWERT (%d) gesetzt!\n", ENDMINTAG, DEFAULT_ENDMINUTE);
		pEigenschaften->nEndMin = DEFAULT_ENDMINUTE;
		FalscheDatenFlag = true;
	}
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *cWert = Eingelesener Wert
//			 - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void ShutdownEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;

	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if(!PruefeTrue_or_False(pWert))
		{
			printf("Das true/false des Wertes: %s ist nicht in Ordnung! Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", SHUTDOWNTAG, DEFAULT_FUNKTION);
			strcpy(pEigenschaften->cShutdown, DEFAULT_FUNKTION);
			FalscheDatenFlag = true;
			return;
		}
		strcpy(pEigenschaften->cShutdown, pWert);
	}
	else
	{
		printf("Das true/false des Wertes: %s ist nicht in Ordnung! Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", SHUTDOWNTAG, DEFAULT_FUNKTION);
		strcpy(pEigenschaften->cShutdown, DEFAULT_FUNKTION);
		FalscheDatenFlag = true;
	}
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void ShutdownSecondEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;
	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if(atoi(pWert) < 0)
		{
			printf("Der eingelesene Wert: %s ist kleiner 0. Die Zeit wird auf den DEFAULTWERT (%d) gesetzt!\n", SHUTDOWNTIMETAG, DEFAULT_SHUTDOWNTIME);
			pEigenschaften->nShutdownTime = DEFAULT_SHUTDOWNTIME;
			FalscheDatenFlag = true;
			return;
		}
		pEigenschaften->nShutdownTime = atoi(pWert);
	}
	else
	{
		printf("Der eingelesene Wert: %s ist kleiner 0. Die Zeit wird auf den DEFAULTWERT (%d) gesetzt!\n", SHUTDOWNTIMETAG, DEFAULT_SHUTDOWNTIME);
		pEigenschaften->nShutdownTime = DEFAULT_SHUTDOWNTIME;
		FalscheDatenFlag = true;
	}
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void SecondEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;

	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if(!PruefeTrue_or_False(pWert))
		{
			printf("Das true/false des Wertes: %s ist nicht in Ordnung! Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", SECONDSTAG, DEFAULT_FUNKTION);
			strcpy(pEigenschaften->cSeconds, DEFAULT_FUNKTION);
			FalscheDatenFlag = true;
			return;
		}
		strcpy(pEigenschaften->cSeconds, pWert);
	}
	else
	{
		printf("Das true/false des Wertes: %s ist nicht in Ordnung! Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", SECONDSTAG, DEFAULT_FUNKTION);
		strcpy(pEigenschaften->cSeconds, DEFAULT_FUNKTION);
		FalscheDatenFlag = true;
	}
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void WeekdayEinlesen(struct tEigenschaften *pEigenschaften)
{
	char *pWert;
	
	pWert = strtok(NULL, "\n");
	if(pWert != NULL)
	{
		if(!PruefeTrue_or_False(pWert))
		{
			printf("Der eingelesene Wert: %s ist nicht in Ordnung. Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", WEEKDAY, DEFAULT_FUNKTION);
			strcpy(pEigenschaften->cWeekday, DEFAULT_FUNKTION);
			FalscheDatenFlag = true;
			return;
		}
		strcpy(pEigenschaften->cWeekday, pWert);
	}
	else
	{
		printf("Der eingelesene Wert: %s ist nicht in Ordnung. Der Wert wird auf den DEFAULTWERT (%s) gesetzt!\n", WEEKDAY, DEFAULT_FUNKTION);
		strcpy(pEigenschaften->cWeekday, DEFAULT_FUNKTION);
		FalscheDatenFlag = true;
	}
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion prüft den eingelesnene Parameter auf richtigkeit und bearbeitet diesen entsprechend
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void WeekdayTimeEinlesen(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	char cTmp[MAXLENGTH] = "";
	char *pTmp;
	int i = 0;

	char cTage[5][5] = {"mo", "di", "mi", "do", "fr"};

	while(fgets(cBuffer, sizeof(cBuffer), pEigenschaften->fpEigenschaften) != NULL)
	{
		SetNewCharacter(cBuffer, '\n', '\0');
		pTmp = strtok(cBuffer, "=");		
		if(pTmp != NULL)
		{
			for(i = 0; i < MAXDAYS; i++)
			{
				if(strcmp(pTmp, cTage[i]) == 0)
				{
					pTmp = strtok(NULL, "\0");
					if(pTmp != NULL)
					{
						strcpy(cTmp, pTmp);
						if(!PruefeUhrzeit(pTmp)) //Die Uhrzeit wird auf das richtige Format geprüft
						{
							strcpy(cBuffer, "");
							break;
						}
						pEigenschaften->pWday.nEndStd[i] = atoi(strtok(cTmp, ":"));
						pEigenschaften->pWday.nEndMin[i] = atoi(strtok(NULL, "\0"));
						pEigenschaften->pWday.nDaySet[i] = 1;
						break;
					}
					else
					{
						pEigenschaften->pWday.nDaySet[i] = 0;
						break;
					}
				}
			}
		}
	}

	return;
}