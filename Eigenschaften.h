#ifndef EIGENSCHAFTEN_H
#define EIGENSCHAFTEN_H

#define EIGENSCHAFTENFILE "Eigenschaften.txt"

#define ENDSTDTAG "endstd"
#define ENDMINTAG "endmin"
#define SECONDSTAG "seconds"
#define SHUTDOWNTAG "shutdown"
#define SHUTDOWNTIMETAG "shutdowntime"
#define WEEKDAY "weekday"
#define WEEKDAY_MO "mo"
#define WEEKDAY_DI "di"
#define WEEKDAY_MI "mi"
#define WEEKDAY_DO "do"
#define WEEKDAY_FR "fr"

#define MAXLENGTH 50

#define DEFAULT_ENDSTUNDE 16
#define DEFAULT_ENDMINUTE 30
#define DEFAULT_FUNKTION "false"
#define DEFAULT_SHUTDOWNTIME 30
#define MAXDAYS 5


/* In dieser Struktur werden die jeweiligen festgelegten Zeiten für einen Tag gespeichert */
struct tWeekday
{
	int nDaySet[MAXDAYS];
	int nEndStd[MAXDAYS];
	int nEndMin[MAXDAYS];
};

struct tEigenschaften
{
	FILE *fpEigenschaften;
	int nEndStd;
	int nEndMin;
	char cShutdown[5];
	int  nShutdownTime; //Default Wert sind 30 Sekunden
	char cSeconds[5];
	char cLastChange[MAXLENGTH];
	char cWeekday[5];
	struct tWeekday pWday; //Instanz der Struktur tWeekday

	struct stat attribut; //Mittels der Struktur stat können die Attribute einer Datei abgefragt werden
};

#endif