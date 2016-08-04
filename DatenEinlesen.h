#ifndef DATENEINLESEN_H
#define DATENEINLESEN_H

void EigenschaftenEinlesen(struct tEigenschaften *pEigenschaften);
void EndStundeEinlesen(struct tEigenschaften *pEigenschaften);
void EndMinuteEinlesen(struct tEigenschaften *pEigenschaften);
void ShutdownEinlesen(struct tEigenschaften *pEigenschaften);
void ShutdownSecondEinlesen(struct tEigenschaften *pEigenschaften);
void SecondEinlesen(struct tEigenschaften *pEigenschaften);
void WeekdayEinlesen(struct tEigenschaften *pEigenschaften);
void WeekdayTimeEinlesen(struct tEigenschaften *pEigenschaften);

#endif