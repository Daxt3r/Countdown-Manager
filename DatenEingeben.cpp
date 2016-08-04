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


//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktionen zur Eingabe der Daten werden aufgerufen
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void DatenEingeben(struct tEigenschaften *pEigenschaften)
{
	EingabeArbeitsende(pEigenschaften);
	EingabeShutdown(pEigenschaften);
	EingabeSeconds(pEigenschaften);
	EingabeWeekTime(pEigenschaften);
	DatenSchreiben(pEigenschaften);
}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt die Uhrzeit seines Feierabends ein
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeArbeitsende(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	char cTmp[MAXLENGTH];

	do
	{
		printf("Geben Sie die Uhrzeit (hh:mm) Ihres Arbeitsende ein: ");
		fgets(cBuffer, MAXLENGTH, stdin); //Die Endzeit wird vom Benutzer eingegeben
		SetNewCharacter(cBuffer, '\n', '\0');
		if((cBuffer[2] != ':') || (strlen(cBuffer) != 5)) //Es wird geprüft, ob an der zweiten Stelle von cBuffer ein Doppelpunkt angegeben wurde
		{
			printf("\nFormat der Uhrzeit stimmt nicht ueberein! Richtiges Format: hh:mm\n");
			strcpy(cBuffer, "");
			continue;
		}

		strcpy(cTmp, cBuffer); //cBuffer wird in cTmp kopiert, da strtok den Inhalt von cBuffer kaputt macht
		if(!PruefeUhrzeit(cBuffer)) //Die Uhrzeit wird auf das richtige Format geprüft
			strcpy(cBuffer, "");

	} while(strcmp(cBuffer, "") == 0);

	pEigenschaften->nEndStd = atoi(strtok(cTmp, ":"));	//Stunde wird vom Eingabe String getrennt
	pEigenschaften->nEndMin = atoi(strtok(NULL, "\0"));		//Minute wird vom Eingabe String getrennt
}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt an, ob der Rechner heruntergefahren werden soll beim erreichen des Feierabends
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeShutdown(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	printf("\nSoll der Rechner nach erreichen des Arbeitsende heruntergefahren werden?\nGeben Sie \"true\" oder \"false\" ein: ");
	do
	{
		fgets(cBuffer, MAXLENGTH, stdin); //Der Benutzer gibt an ob der Rechner runtergefahren werden soll
		SetNewCharacter(cBuffer, '\n', '\0');
		if((strcmp(cBuffer, "True") == 0) || (strcmp(cBuffer, "true") == 0)) //Es wird geprüft ob der Benutzer Ja/ja eingegeben hat
		{
			strcpy(pEigenschaften->cShutdown, "true");
			EingabeShutdownTime(pEigenschaften);
		}
		else if((strcmp(cBuffer, "False") == 0) || (strcmp(cBuffer, "false") == 0))
			strcpy(pEigenschaften->cShutdown, "false"); //Der Benutzer hat Nein/nein eingegeben
		else
		{
			printf("Unbekannter Wert: %s\n", cBuffer);
			printf("Geben Sie \"true\" oder \"flase\" ein: ");
			strcpy(pEigenschaften->cShutdown, "");
		}
	} while(strcmp(pEigenschaften->cShutdown, "") == 0);

}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt an, wie lange es dauern soll bis der Rechner herunterfährt
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeShutdownTime(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH];

	printf("\nWie lange soll es dauern, bis der Rechner herunterfaehrt?\nGeben Sie die Zeit in Sekunden an: ");
	do
	{
		fgets(cBuffer, MAXLENGTH, stdin); //Der Benutzer gibt an wie lange es dauern soll, bis der Rechner runter fährt
		SetNewCharacter(cBuffer, '\n', '\0');
		if(CheckInt(cBuffer)) //Es wird geprüft ob der Benutzer eine ShutdownTime >= 0 eingegeben hat
		{
			pEigenschaften->nShutdownTime = atoi(cBuffer);
			return;
		}
		else
		{
			printf("\nUngueltiger Wert: %s\n", cBuffer);
			printf("Geben Sie eine Zahl groesser gleich 0 ein: ");
			pEigenschaften->nShutdownTime = -1;
		}
	} while(pEigenschaften->nShutdownTime < 0);
}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt an, ob er die Sekunden angezeigt haben möchte oder nicht
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeSeconds(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	printf("\nSollen die Sekunden zusaetzlich zur berechneten Uhrzeit (hh:mm:ss)\nangezeigt werden?\nGeben Sie \"true\" oder \"false\" ein: ");
	do
	{
		fgets(cBuffer, MAXLENGTH, stdin); //Der Benutzer gibt an ob die Sekunden angezeigt werden sollen in der Konsole
		SetNewCharacter(cBuffer, '\n', '\0');
		if((strcmp(cBuffer, "True") == 0) || (strcmp(cBuffer, "true") == 0)) //Es wird geprüft ob der Benutzer Ja/ja eingegeben hat
			strcpy(pEigenschaften->cSeconds, "true");
		else if((strcmp(cBuffer, "False") == 0) || (strcmp(cBuffer, "false") == 0)) //Es wird geprüft ob der Benutzer Nein/nein eingegeben hat
			strcpy(pEigenschaften->cSeconds, "false");
		else
		{
			printf("Unbekannter Wert: %s\n", cBuffer);
			printf("Geben Sie \"true\" oder \"flase\" ein: ");
			strcpy(pEigenschaften->cShutdown, "");
		}
	}while(strcmp(pEigenschaften->cSeconds, "") == 0);
}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt an, ob er für bestimmte Wochentage eine andere Uhrzeit für den Feierabend festlegen möchte
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeWeekTime(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	printf("\nMoechten Sie eine individuelle Uhrzeit fuer einen/mehrere Wochentage festlegen?\nGeben Sie \"true\" oder \"false\" ein: ");
	do
	{
		fgets(cBuffer, MAXLENGTH, stdin); //Der Benutzer gibt an ob die Sekunden angezeigt werden sollen in der Konsole
		SetNewCharacter(cBuffer, '\n', '\0');
		if((strcmp(cBuffer, "True") == 0) || (strcmp(cBuffer, "true") == 0)) //Es wird geprüft ob der Benutzer Ja/ja eingegeben hat
		{
			strcpy(pEigenschaften->cWeekday, "true");
			EingabeWeekDayTime(pEigenschaften);
		}
		else if((strcmp(cBuffer, "False") == 0) || (strcmp(cBuffer, "false") == 0)) //Es wird geprüft ob der Benutzer Nein/nein eingegeben hat
			strcpy(pEigenschaften->cWeekday, "false");
		else
		{
			printf("Unbekannter Wert: %s\n", cBuffer);
			printf("Geben Sie \"true\" oder \"flase\" ein: ");
			strcpy(pEigenschaften->cWeekday, "");
		}
	}while(strcmp(pEigenschaften->cWeekday, "") == 0);
}

//-------------------------------------------------------------------------------------------------------------------------------
//Der Benutzer gibt die Uhrzeit für die abweichende Feierabendszeit ein
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void EingabeWeekDayTime(struct tEigenschaften *pEigenschaften)
{
	char cWochenTag[6][20] = {"Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag"};
	char cBuffer[MAXLENGTH] = "";
	char cTmp[MAXLENGTH] = "";
	int i;
	
	printf("\nGeben Sie die individuelle Uhrzeit (hh:mm) fuer jeden Tag ein.\nSollten Sie fuer einen Tag keine Zeit angeben wollen geben Sie \"-\" ein. Fuer diesen Tag wird dann die standart Uhrzeit verwendet.\n");
	for(i = 0; i < 5; i++)
	{
		do
		{
			printf("\n%s: ", cWochenTag[i]);
			fgets(cBuffer, MAXLENGTH, stdin); //Der Benutzer gibt an ob die Sekunden angezeigt werden sollen in der Konsole
			SetNewCharacter(cBuffer, '\n', '\0');
			if(cBuffer[0] == '-') //Es wird geprüft ob ein "-" angegeben wurde -> Es wurde keine individuele Uhrzeit angegeben
				break; 
			else if((cBuffer[2] != ':')  || (strlen(cBuffer) != 5)) //Es wird geprüft, ob an der zweiten Stelle von cBuffer ein Doppelpunkt angegeben wurde
			{
				printf("\nFormat der Uhrzeit (hh:mm) stimmt nicht, oder unbekannter Wert: %s\n", cBuffer);
				strcpy(cBuffer, "");
			}

			strcpy(cTmp, cBuffer); //Sicherheitskopie wird erstellt
			if(!PruefeUhrzeit(cBuffer)) //Die Uhrzeit wird auf das richtige Format geprüft
				strcpy(cBuffer, "");

		} while(strcmp(cBuffer, "") == 0);

		if(cTmp[0] == '-')
		{
			pEigenschaften->pWday.nEndStd[i] = 0;	//Es wurde keine individuelle Zeit festgelegt
			pEigenschaften->pWday.nEndMin[i] = 0;	//Es wurde keine individuelle Zeit festgelegt
		}
		else
		{
			pEigenschaften->pWday.nEndStd[i] = atoi(strtok(cTmp, ":"));	//Stunde wird vom Eingabe String getrennt
			pEigenschaften->pWday.nEndMin[i] = atoi(strtok(NULL, "\0"));	//Minute wird vom Eingabe String getrennt
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die vom Benutzer eingegebenen Daten werden in die Eigenschaften.txt Datei geschrieben
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void DatenSchreiben(struct tEigenschaften *pEigenschaften)
{
	//Die Uhrzeit wird in die Eigenschaftendatei geschrieben
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d\n", ENDSTDTAG, pEigenschaften->nEndStd);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d\n", ENDMINTAG, pEigenschaften->nEndMin);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n", SHUTDOWNTAG, pEigenschaften->cShutdown);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%d\n", SHUTDOWNTIMETAG, pEigenschaften->nShutdownTime);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n\n", SECONDSTAG, pEigenschaften->cSeconds);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%s\n", WEEKDAY, pEigenschaften->cWeekday);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_MO, pEigenschaften->pWday.nEndStd[0], pEigenschaften->pWday.nEndMin[0]);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_DI, pEigenschaften->pWday.nEndStd[1], pEigenschaften->pWday.nEndMin[1]);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_MI, pEigenschaften->pWday.nEndStd[2], pEigenschaften->pWday.nEndMin[2]);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d\n", WEEKDAY_DO, pEigenschaften->pWday.nEndStd[3], pEigenschaften->pWday.nEndMin[3]);
	fprintf(pEigenschaften->fpEigenschaften, "%s=%.2d:%.2d", WEEKDAY_FR, pEigenschaften->pWday.nEndStd[4], pEigenschaften->pWday.nEndMin[4]);
	fclose(pEigenschaften->fpEigenschaften); //Die Eigenschaften Datei wird geschlossen

	printf("\nVielen Danke! Die Differenz bis zu Ihrem verdienten\nFeierabend wird Ihnen nun angezeigt.\n");
	system("PAUSE");
	system("cls"); //Ausgaben in der Konsole werden gelöscht
}