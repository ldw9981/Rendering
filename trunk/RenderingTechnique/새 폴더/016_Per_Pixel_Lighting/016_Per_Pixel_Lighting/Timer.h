#ifndef _TIMER_H
#define _TIMER_H

#include "init.h"

class Timer{
	__int64			frekvencia;								// frekvencia �asova�a
	float			perioda;								// peri�da �asova�a
	unsigned long	mm_timer_start;							// �tart �tandartn�ho �asova�a - v milisekund�ch
	bool			performance_timer;						// pou�it� r�chli �asova� ? | Using The Performance Timer?
	__int64			performance_timer_start;				// �tartovn� hodnota r�chleho �asova�a
public:
	Timer(void);										// inicializuje casovac
	float GetTime(void);					// vr�ti �as v sekund�ch
	void Reset(void);						// resetuje cas
};


#endif