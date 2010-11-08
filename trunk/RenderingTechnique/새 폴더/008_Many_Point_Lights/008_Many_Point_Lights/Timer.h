#ifndef _TIMER_H
#define _TIMER_H

#include "init.h"

class Timer{
	__int64			frekvencia;								// frekvencia èasovaèa
	float			perioda;								// perióda èasovaèa
	unsigned long	mm_timer_start;							// štart štandartného èasovaèa - v milisekundách
	bool			performance_timer;						// použitý rýchli èasovaè ? | Using The Performance Timer?
	__int64			performance_timer_start;				// štartovná hodnota rýchleho èasovaèa
public:
	Timer(void);										// inicializuje casovac
	float GetTime(void);					// vráti èas v sekundách
	void Reset(void);						// resetuje cas
};


#endif