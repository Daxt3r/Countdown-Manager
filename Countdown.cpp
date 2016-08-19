// Countdown.cpp: Hauptprojektdatei.
	
	//Version V0.6.5 (Beta) (Stand 08.08.2016)
	
	//Update History
	//-------------------------------------------
	// # 19.08.2016: Initialisierung der Variablen in struct tWeekday hinzugefügt, womit ein Ausgabefehler behoben wurde (Version geändert von 0.6.4 zu 0.6.5)
	// # 08.08.2016: Fehler behoben, dass nicht alle Daten bei einem falsch eingelesenen Wert in die Eigenschaften Datei geschrieben werden (Version geändet von 0.6.3 zu 0.6.4) 
	//				 Es wird nun beim Einlesen geprüft ob die individuelle Feierabendszeit != 00:00 ist (Version geändert von 0.6.2 zu 0.6.3)
	//				 Fehler beim Einlesen der individuellen Uhrzeit behoben (Probleme beim Einlesen des Wertes "-") (Version geändert von 0.6.1 zu 0.6.2)
	// # 04.08.2016: Status des Programmes von Alpha zu Beta geändert
	//				 Es wird nun geprüft ob eine individuelle Zeit für den aktuellen Tag festgelegt, diese wird dann zur Berechnung verwendet (Version geändert von 0.6.0 zu 0.6.1) 
	//				 Funktion zum Einlesen der jeweiligen individuellen Werte für jeden Tag geschrieben (Version geändert von 0.5.4 zu 0.6.0)
	//				 Fehler beim Überschreiten der Endzeit behoben (Uhrzeit wurde trotzdem berechnet)
	//				 Fehler beim Überprüfen der Uhrzeit behoben (Version geändert von 0.5.2 zu 0.5.4)
	// # 02.08.2016: Funktion DatenEingeben.cpp überarbeitet und Programmcode in eigene Funktionen ausgelagert (Version geändert von 0.5.1 zu 0.5.2)
	// # 21.07.2016: Kritischen Fehler beim einlesen der Daten behoben (Es fehlten mehrere Abfragen auf NULL-Pointern) (Version geändert von 0.5.0 zu 0.5.1) 
	//				 Sämtliche Funktionen in den Modulen DatenEingeben.cpp und DatenEinlesen.cpp um die Sicherheitsabfragen erweitert (Version geändert von 0.4.2 zu 0.5.0)
	//				 Weiter am DatenHandler gearbeitet, folgende Funktionen hinzugefügt:
	//					- Prüfung von eingegebenen Werten
	//					- Prüfung von eingelesenen Werten aus der Eigenschaften.txt Datei (bei fehlenden oder falschen Daten werden Default Werte gesetzt)
    //				 Programmstruktur überarbeitet (Version geändert von 0.4.1 zu 0.4.2)
	// # 20.07.2016: Angefangen einen DatenHandler zu implementieren, welcher die Daten (eingelesen/eingegebene) auf Fehler prüft
	// # 08.07.2016: Großes Featur angefangen vorzubereiten, der Benutzer hat nun die Möglichkeit individuelle Feierabendszeiten für jeden
	//				 Wochentag festzulegen, sodass die standart Uhrzeit nicht immer geändert werden muss. 
	// # 08.07.2016: Fehler beim Eingeben der Sekunden zum Herunterfahren behoben (Version geändert von 0.4.0 zu 0.4.1) 
	// # 08.07.2016: Featur hinzugefügt, dass nun die Zeit angegeben werden kann, wie lange es dauern soll bis der Rechner runter fährt (Version geändert von 0.3.6 zu 0.4.0) 
	// # 16.06.2016: Featur hinzugefügt, dass nun Sekunden angezeigt werden können(je nach Benuzereinstellung).
	//-------------------------------------------

	/*Beschreibung: Das Programm ermittelt die Differenz zwischen der aktuellen Uhrzeit und der vom Benutzer festgelegten Endzeit 
	                Der Benutzer kann festlegen, ob beim erreichen der Endzeit, der Computer heruntergefahren werden soll oder nicht.
					Die Einstellung vom Benutzer werden in der Textdatei Eigenschaften.txt gespeichert. */
	//Erstellt am: 07.03.2016
	//Autor: Marcel Crone


#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <Windows.h>
#include <sys/stat.h>
#include "Eigenschaften.h"
#include "DatenEingeben.h"
#include "DatenEinlesen.h"

using namespace System;

/* forward decleration */
//-------------------------------------------------------------------------------------------------------------------------------
void Init_tEigenschaften(struct tEigenschaften *pEigenschaften);
void FirstStartDialog(struct tEigenschaften *pEigenschaften);
int Countdown(struct tEigenschaften *pEigenschaften);
void EndzeitErmitteln(struct tEigenschaften *pEigenschaften, int nAktTag);
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
int main(array<System::String ^> ^args)
{
	struct tEigenschaften pEigenschaften;
	char cShutdown[MAXLENGTH] = "";
	int nReturn = 0;

	Init_tEigenschaften(&pEigenschaften);

	pEigenschaften.fpEigenschaften = Open_File("Eigenschaften.txt", "r");
	if(pEigenschaften.fpEigenschaften == NULL)
		FirstStartDialog(&pEigenschaften);
	
	/*In der Schleife wird die Differenz bis zum Feierabend berechnet*/
	while(!nReturn)
	{
		nReturn = Countdown(&pEigenschaften);
		if(nReturn == -1)
		{
			system("Pause");
			return 0;
		}

		Sleep(1000);
		system("cls");
	}

	if(strcmp(pEigenschaften.cShutdown, "true") == 0)
	{
		sprintf(cShutdown, "shutdown.exe -s -t %d", pEigenschaften.nShutdownTime);
		system(cShutdown); //Befehl um den Computer herunter zu fahren
		return 0;
	}
	else
		printf("Schoenen Feierabend. . .\n");

	system("Pause");
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Es werden die Variablen der Struktur tEigenschaften initialisiert
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
void Init_tEigenschaften(struct tEigenschaften *pEigenschaften)
{
	int i = 0;

	pEigenschaften->fpEigenschaften = NULL;
	pEigenschaften->nEndStd = 0;
	pEigenschaften->nEndMin = 0;
	strcpy(pEigenschaften->cShutdown, "");
	strcpy(pEigenschaften->cSeconds, "");
	strcpy(pEigenschaften->cLastChange, "");
	strcpy(pEigenschaften->cWeekday, "");
	pEigenschaften->nShutdownTime = 30;

	for(i = 0; i < MAXDAYS; i++)
	{
		pEigenschaften->pWday.nDaySet[i] = 0;
		pEigenschaften->pWday.nEndStd[i] = 0;
		pEigenschaften->pWday.nEndMin[i] = 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
//Das Programm wurde zum ersten Mal gestartet. Der Benutzer legt in dieser Funktion die Werte der Endzeit fest.
//Parameter: - *fpEigenschaften = Zeiger auf die Textdatei Eigenschaften.txt
//			 - *pEigenschaften  = Zeiger auf die Struktur tEigenschaften
void FirstStartDialog(struct tEigenschaften *pEigenschaften)
{
	pEigenschaften->fpEigenschaften = Open_File("Eigenschaften.txt", "w"); //Eigenschaften Textdatei wird erstellt

	printf("Willkommen beim Countdown-Manager (CM)\n\n");
	printf("Dies ist Ihr erster Start des CM's. Daher muessen Sie zuerst ein\npaar Eigenschaften fuer die Nutzung festlegen.\n\n");

	DatenEingeben(pEigenschaften); //Die Daten werden vom Benutzer eingegeben
}

//-------------------------------------------------------------------------------------------------------------------------------
//Die Funktion berechnet die Differenz zwischen der aktuellen Uhrzeit und der Endzeit und überprüft ob die Endzeit erreicht wurde
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
//Rückgabewert: - true  = Endzeit wurde erreicht
//				- false = Endzeit wurde noch nicht erreicht
//				- -1    = Endzeit wurde überschritten
int Countdown(struct tEigenschaften *pEigenschaften)
{
	char cBuffer[MAXLENGTH] = "";
	struct tm *tmnow;
	int tmp_std = 0;
	int tmp_min = 0;
	int tmp_sec = 59;

	time_t tnow;
	
	time(&tnow);
	tmnow = localtime(&tnow);
	
	//Es wird geprüft ob die Datei Eigenschaften.txt verändert wurde
	stat(EIGENSCHAFTENFILE , &pEigenschaften->attribut); //Die Instanz pEigenschaften->attribut bekommt die Datei "Eigenschaften.txt" zugewiesen
	strcpy(cBuffer, ctime(&pEigenschaften->attribut.st_mtime)); //Letztes Änderungsdatum wird abgefragt, mittels ctime formatiert und in cBuffer kopiert
	if(strcmp(cBuffer, pEigenschaften->cLastChange) != 0) //Datei wurde verändert, es werden die Eigenschaften neu eingelesen
	{
		EigenschaftenEinlesen(pEigenschaften);
		strcpy(pEigenschaften->cLastChange, cBuffer);
	}
	else
		strcpy(pEigenschaften->cLastChange, cBuffer);

	//Es wird geprüft ob für den Tag eine individuelle Zeit angegeben wurde, oder ob die standartd Zeit verwendet werden soll
	if(strcmp(pEigenschaften->cWeekday, "true") == 0)
			EndzeitErmitteln(pEigenschaften, tmnow->tm_wday);

	tmp_min = pEigenschaften->nEndMin - tmnow->tm_min; // - pEigenschaften->nBreak;  //Differenz zwischen Endminute und aktueller Minute wird berechnet
	
	if(tmp_min < 0) //Aktuelle Uhrzeit ist größer als Endzeit daher kleiner 0
	{
		tmp_min = tmp_min + 60;  //Differenz zwischen 60 Minuten und Endminute wird berechnet, da tmp_min kleiner 0 ist
		tmp_std = pEigenschaften->nEndStd - tmnow->tm_hour - 1; //Differenz zwischen Endstunde und aktueller Stunde wird berechnet -1 wenn Minuten größer als Endzeit
		if(tmp_std < 0)
			tmp_std = 0;
	}
	else
		tmp_std = pEigenschaften->nEndStd - tmnow->tm_hour; //Differenz zwischen Endstunde und aktueller Stunde wird berechnet		

	if(tmp_std == 0 && tmp_min == 0) //Wenn beides auf 0 ist, wurde die Endzeit erreicht
		return true;
	
	if(tmp_std < 0) //Es wird geprüft ob die Endzeit bereits überschritten wurde
	{
		printf("Endzeit wurde bereits erreicht, bzw. ueberschritten!\n");
		return -1;
	}

	if(strcmp(pEigenschaften->cSeconds, "true") == 0)
		printf("%.2d:%.2d:%.2d\n", tmp_std, tmp_min, tmp_sec - tmnow->tm_sec); //Differenz mit Sekunden wird ausgegeben
	else
		printf("%.2d:%.2d\n", tmp_std, tmp_min); //Differenz wird ausgegeben
	
	return false;
}

//-------------------------------------------------------------------------------------------------------------------------------
//Es wurde für einen oder mehrere Tage eine individuelle Zeit festgelegt. Es wurd nun geprüft ob für den aktuellen Tag die
//individuelle Uhrzeit festgelegt wurde
//Parameter: - *pEigenschaften = Zeiger auf die Struktur tEigenschaften
//			 - nAktTag		   = Nummer des aktueller Tag
//Anmerkung: Der Wert nAktTag wird -1 genommen, da das Array der Struktur tm (time.h) etwas andere Werte verwendet
//			 Werte in der tm Struktur: Sonntag (pos. 0) Montag (pos. 1) Dienstag (pos. 2) ...
//			 Werte in meiner Struktur: Sonntag (n. a. ) Montag (pos. 0) Dienstag (pos. 1) ...
void EndzeitErmitteln(struct tEigenschaften *pEigenschaften, int nAktTag)
{
	if(pEigenschaften->pWday.nDaySet[nAktTag-1] == 1)
	{
		pEigenschaften->nEndStd = pEigenschaften->pWday.nEndStd[nAktTag-1];
		pEigenschaften->nEndMin = pEigenschaften->pWday.nEndMin[nAktTag-1];
	}

	return;
}