#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "util.h"

using namespace std;

//#pragma warning( disable: C4996 ) // C4996 ist Nummer der Warnung diese wird ausgeschaltet

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Öffnen eine die Datei mit dem angegebenen Pfad und Modus
// Parameter: - char cPfad   = Pfad der Datei 
//            - char cMode   = Modus in dem die Datei geöffnet werden soll
// Rückgabewert: - Open_File = Zeiger auf die Datei, die geöffnet werden sollte
FILE *Open_File(char cPfad[], char cMode[])
{
	FILE *Open_File;

	Open_File = fopen(cPfad, cMode);

	if (Open_File == NULL) //Wenn die Datei nicht geöffnet werden konnte, wird diese Fehlermeldung ausgegeben
		printf("Datei %s konnte nicht geoeffnet werden\n", cPfad);
	
	return Open_File;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Das zu suchende Zeichen (cSearch_Symbole) wird in dem String (cBuffer[]) durch ein anderes Zeichen (new_symbole) ersetzt
// Parameter: - char cBuffer[]       = Zeichenkette, inder nach den Zeichen gesucht werden soll
//            - char cSearch_Symbole = Zeichen nachdem gesucht werden soll		
//            - char cNew_Symbole    = Zeichen durch das das gesuchte Zeichen ersetzt werden soll  
void SetNewCharacter(char cBuffer[], char cSearch_Symbole, char cNew_Symbole)
{
	char *ptr;
							
	ptr = strchr(cBuffer, cSearch_Symbole);
	
	if (ptr != NULL)
		*ptr = cNew_Symbole;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es wird die Häufigkeit eines bestimmten Wortes (cSearch_Word[]) in einem Satz/Text (cString[]) ermitteln
// Parameter: - char cString      = Satz/Text indem das Wort gezählt werden soll 
//            - char cSearch_Word = Das Wort, welches gezählt werden soll
// Rückgabewert: - i = Anzahl der gezählten Wörter 
//Funktion erweitern, dass mehrere Wörter gezählt werden können.
int GetWords(char cString[], char cSearch_Word[])
{
	int i = 0;

	if(strstr(cString, cSearch_Word) != NULL)
		i++;

	return i;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Trennt einen Satz/Text (cString[]) anhand eines angegebenen Wortes (cResearch_Criteria[]) und zählt die Trennungen
// Parameter: - char cString[]            = Satz/Text, indem nach dem Wort gesucht werden soll
//            - char cResearch_Criteria[] = Zu suchende Wort in dem Satz/Text          
// Rückgabewert: - nCounter = Anzahl der getrennten Sätze
// Anmerkung: Funktion anpassen, dass sie auch anders einsetzbar ist
// z.B um Records durch | zu trennen und kein int als return <-- macht einfach keinen Sinn!!
int Split_String(char cString[], char cResearch_Criteria[])
{
	int nCounter = 0;
	char *pPtr;

	// Trennt das erste Wort anhand einer der Suchkriterien
	pPtr = strtok(cString, cResearch_Criteria);									
	while(pPtr != NULL)
	{
		nCounter++;
		pPtr = strtok(NULL, cResearch_Criteria); // Zur weiterbenutzung der Funktion muss NULL anstelle cstring angegeben werden
	}
	return nCounter;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Zählt die Zeilen in der Datei auf die *pFile zeigt
// Parameter: - FILE *pFile = Zeiger auf die Datei
// Rückgabewert: - nZeilen  = Anzahl der gezählten Zeilen
int GetLines(FILE *pFile)
{
	char cBuffer[3000];
	int nZeilen = 0;

	while(fgets(cBuffer, sizeof(cBuffer), pFile) != NULL)
		nZeilen++;

	return nZeilen;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Zählt die Zeilen und Zeichen in der Datei auf die *pFile zeigt
// Parameter: - FILE *pFile = Zeiger auf die Datei
//			  - *nZeichen   = Zeiger auf die Speicher, andem der Wert gespeichert wird
// Rückgabewert: - nZeilen  = Anzahl der gezählten Zeilen
int GetLinesAndSize(FILE *pFile, unsigned int *nZeichen)
{
	char cBuffer[3000];
	int nZeilen = 0;

	while(fgets(cBuffer, sizeof(cBuffer), pFile) != NULL)
	{
		nZeilen++;
		*nZeichen += strlen(cBuffer) + 1;
	}

	return nZeilen;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es wird der längste Satz / Wort in der Datei auf die *pFile zeigt ermittelt
// Parameter: - FILE *pFile = Zeiger auf die Datei
// Rückgabewert: - nMaxLaenge  = Gibt die Länge des längesten Satzes / Wortes aus der Datei *pFile zurück 
int GetMaxLength(FILE *pFile)
{
	char cBuffer[3000];
	unsigned int nMaxLaenge = 0;

	while(fgets(cBuffer, sizeof(cBuffer), pFile) != NULL)
		if(nMaxLaenge < strlen(cBuffer))
			nMaxLaenge = strlen(cBuffer);

	return nMaxLaenge;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Prüft ob in einer Zeichenkette(*cBuf) nur nummerische Zeichen enthalten sind
// Parameter: - char *cString = Zeichenkette welche geprüft werden soll
// Rückgabewert: 0 = Nicht nur nummerische Zeichen 
//               1 = Nur nummerische Zeichen
int CheckInt(char *cString)
{
	if(cString == NULL)
		return 0;
	while (*cString != '\0')
	{
		if ((*cString < '0') || (*cString > '9'))
			return 0;
		cString++;
	}
	return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: zum Prüfen, ob ein char[] nur numerische Zeichen und 1 x '.' enthält
// Parameter: - char *cString = Zeichenkette welche geprüft werden soll
// Rückgabewert: 0 = Enthält mehr als einen Punkt und/oder ist nicht nummerisch
//				 1 = Enthält nur einen Punkt und ist nur nummerisch
int CheckFloat(char *cString)
{
	int nPoint_Count = 0;
	
	while (*cString != '\0')
	{
		if ((nPoint_Count > 1) && (*cString < '0') || (*cString > '9'))
			return 0;
		
		if(*cString == '.')
			nPoint_Count++;

		cString++;
	}
	return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es wird ein Array mittels dem Quicksort sortiert.
// Parameter: - nZahlen[] = Integer Array
//			  - L		  = Erste Position im Array (0)
//			  - R         = Letzte Element des Arrays (Maxelemente-1)
void Qsort_int(int nZahlen[], int L, int R)
{
	int L1;
	int R1;

	Divide_int(nZahlen, L, R, &L1, &R1); 

	if (L < L1)
		Qsort_int(nZahlen, L, L1); // Rekursiver Aufruf
	if (R1 < R)
		Qsort_int(nZahlen, R1, R); // Rekursiver Aufruf
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Pivoelement wird ermittelt und anhand diesen Elementes die linke und rechte Seite auf der linken Seite
// stehen alle Elemente die <Pivoelement und auf der rechten Seite stehen alle Zahlen die >Pivoelement
// Parameter: - nZahlen[] = Integer Array 
//			  - L		  = Erste Position im Array (0)
//			  - R         = Letzte Element des Arrays (Maxelemente-1)
//			  - *L1		  = Startposition des linken Zeigers (0)
//			  - *R1       = Startposition des rechten Zeigers (Maxelemente-1)
void Divide_int(int nZahlen[], int L, int R, int *L1, int *R1)
{
	int nPivo;

	nPivo = nZahlen[(L+R)/2];		// Das Element welches in der Mitte ist, wird berechnet
	
	*R1=L;
	*L1=R;

	while (*R1 <= *L1)
	{
		while (nZahlen[*R1] < nPivo) //Es wird nach einer Zahl gesucht, die >nPivo ist
			(*R1)++; //Zeiger wird erhöht
		while (nPivo < nZahlen[*L1]) //Es wird nach einer Zahl gesucht, die <nPivo ist
			(*L1)--; //Zeiger wird verringert
		if (*R1 < *L1) //Rechter Pointer < Linker Pointer -> beide Zeiger zeigen noch nicht auf das gleiche Element und haben sich noch nicht überschnitten
		{
			Swap_int(nZahlen, *R1, *L1); //Vorher ermittelte Zahlen werden im Array getauscht
			(*R1)++; //Zeiger wird erhöht
			(*L1)--; //Zeiger wird verringert 
		}
		else if (*R1 == *L1)
		{
			(*R1)++; //Zeiger wird erhöht
			(*L1)--; //Zeiger wird verringert 
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es werden zwei Elemente des Arrays v[] an der Stelle i und j getauscht
// Parameter: - v[] = int Array mit den Zahlen
//	  	      - i   = Position der ersten zu tauschenden Zahl
//	 		  - j   = Position der zweiten zu tauschenden Zahl
// Anmerkung: Kein return Wert, da ein Array ein Zeiger ist!
void Swap_int(int nZahlen[], int i, int j)
{
	int tmp;

	tmp=nZahlen[i];
	nZahlen[i]=nZahlen[j];
	nZahlen[j]=tmp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es wird ein Array mittels dem Quicksort sortiert.
// Parameter: - cDaten[]  = char Array
//			  - L		  = Erste Position im Array (0)
//			  - R         = Letzte Element des Arrays (Maxelemente-1)
void Qsort_char(char *cDaten[], int L, int R)
{
	int L1;
	int R1;

	Divide_char(cDaten, L, R, &L1, &R1);

	if (L < L1)
		Qsort_char(cDaten, L, L1); // Rekursiver Aufruf
	if (R1 < R)
		Qsort_char(cDaten, R1, R); // Rekursiver Aufruf
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Pivoelement wird ermittelt und anhand diesen Elementes die linke und rechte Seite auf der linken Seite
// stehen alle Elemente die <Pivoelement und auf der rechten Seite stehen alle Zahlen die >Pivoelement
// Parameter: - cDaten[]  = char Array 
//			  - L		  = Erste Position im Array (0)
//			  - R         = Letzte Element des Arrays (Maxelemente-1)
//			  - *L1		  = Startposition des linken Zeigers (0)
//			  - *R1       = Startposition des rechten Zeigers (Maxelemente-1)
void Divide_char(char *cDaten[], int L, int R, int *L1, int *R1)
{
	char cPivo[500] = "";

	strcpy(cPivo, cDaten[(L+R)/2]);		// Das Element welches in der Mitte ist, wird berechnet und in cPivo kopiert
	*R1=L;
	*L1=R;

	while (*R1 <= *L1)
	{
		while (strcmp(cDaten[*R1], cPivo) < 0) //Es wird nach einem Namen gesucht, der >nPivo ist
			(*R1)++; // Zeiger wird erhöht
		while (strcmp(cDaten[*L1], cPivo) > 0) //Es wird nach einem Namen gesucht, der <nPivo ist
			(*L1)--; // Zeiger wird verringert
		if (*R1 < *L1) // Rechter Pointer < Linker Pointer -> beide Zeiger zeigen noch nicht auf das gleiche Element und haben sich noch nicht überschnitten
		{			   // Daten werden getauscht 
			Swap_char(cDaten, *R1, *L1); //Vorher ermittelte Zahlen werden im Array getauscht
			(*R1)++; //Zeiger wird erhöht
			(*L1)--; //Zeiger wird verringert 
		}
		else if (*R1 == *L1)
		{
			(*R1)++; //Zeiger wird erhöht
			(*L1)--; //Zeiger wird verringert 
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es werden zwei Elemente des Arrays cDaten[] an der Stelle i und j getauscht
// Parameter: - cDaten[] = char Array mit den Daten
//	  	      - i		 = Position der ersten zu tauschenden Daten
//	 		  - j		 = Position der zweiten zu tauschenden Daten
// Anmerkung: Kein return Wert, da ein Array ein Zeiger ist!
void Swap_char(char *cDaten[], int i, int j)
{
	char  *cTmp;

	cTmp = cDaten[i];
	cDaten[i] = cDaten[j];
	cDaten[j] = cTmp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Funktion: Es wird geprüft ob die Daten einer Funktion ordentlich sortiert wurden.
//			 Bei einem Fehler werden die jeweiligen Datensätze + deren Zeile ausgegeben.
// Parameter: -*pFile: Zeiger auf die Datei, in der die sortierten Daten sind
void CheckSortDatas(FILE *pFile)
{
	char cBufferNew[2000] = " ";
	char cBufferOld[2000] = " ";
	
	int error_flag = 0;
	int i = 0, n = 0;

	while(fgets(cBufferNew, sizeof(cBufferNew), pFile) != NULL)
	{
		SetNewCharacter(cBufferNew, '\n', '\0');
		if(strcmp(cBufferNew, cBufferOld) > 0);
		else
		{
			printf("Zeile %d: %s \nZeile %d: %s\n\n", i, cBufferOld, i+1, cBufferNew);
			error_flag = 1;
		}
		strcpy(cBufferOld, cBufferNew);
		i++;
	}

	if(!error_flag)
		printf("Die Sortierung ist in Ordnung!\n");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//Ähnliche Funktion wie strtok(), nur werden hier bei einer Leerstelle im String auch die leeren Werte zurückgegeben
//Parameter: - *cSatz   = Adresse des Satzes, der Bearbeitet werden soll
//			 - cTrenner = Zeichen, anhand desen der Satz getrennt werden soll
//			 - *pSatz	= Bereitgestellter Speicher in dem der abgetrennte Wert gespeichert wird
char *SatzZerlegen(char *cSatz, char cTrenner, char *pSatz)
{
	static char *pTmp;
	char cTmp;

	strcpy(pSatz, "");

	//Es wird geprüft ob ein neuer String angegeben wurde, oder ob mit dem Alten weiter gearbeitet werden soll
	if(cSatz == NULL)
		cSatz = pTmp;
	else
		pTmp = cSatz;

	//cSatz wird solange durchlaufen bis das Ende erreicht wurde oder der Satztrenner
	while(*pTmp != '\0')
	{
		if(*pTmp != cTrenner)
		{
			cTmp = *pTmp;
			strcat(pSatz, &cTmp);
			*pTmp++;	//Es wird auf den nächsten Buchstaben im Satz zugegriffen 
		}
		else
		{
			*pTmp++;	//Wird erhöht, damit beim erneuten Aufruf, das Trennzeichen nicht als erstes steht
			return pSatz; //Abgetrennter Satz wird zurück gegeben
		}
	}
	return NULL;
}