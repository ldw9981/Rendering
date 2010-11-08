#include "timer.h"

Timer::Timer(void)									// inicializ�cia �asova�a
{
	mm_timer_start = 0;
	performance_timer_start = 0;

	// zistenie �i r�chli �asova� je pou�ite�n�
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &(frekvencia) ))
	{
		// r�chli �asova� nepodporovan� hardwarom
		performance_timer	= FALSE;					// nastav Performance Timer na false
		mm_timer_start		= timeGetTime();			// pou�it�m timeGetTime() nastav �tartovaciu hodnotu
		perioda				= 1.0f/1000.0f;				// nastav periodu na 1 ms
		frekvencia			= 1000;						// nastav frekvenciu na 1000 Hz
	}
	else
	{
		// r�chli �asova� je pr�stupn�
		QueryPerformanceCounter((LARGE_INTEGER *) &(performance_timer_start) );	//nastavenie �tartovacej hodnoty
		performance_timer	= TRUE;				// nastav Performance Timer na TRUE
		perioda = (float) (1.0/((double)frekvencia));	// vypo��tanie peri�dy
	}
}

float Timer::GetTime(void)					// vr�ti �as v sekund�ch
{
	__int64 time;									// na ulo�enie aktu�lnej hodnoty �asova�a

	if (performance_timer)					// je pou�it� r�chli �asova� (Performance Timer ) ?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);	// na��tanie aktu�lnej hodnoty �asova�a
		// vr�ti aktu�lnu hodnotu minus �tartovaciu hodnotu nasoben� peri�dou
		return ( (float) ( time - performance_timer_start) * perioda);
	}
	else
	{	return( (float) ( timeGetTime() - mm_timer_start) * perioda);}	// vr�ti po�et sek�nd od �tartu
}

void Timer::Reset(void)
{	
	if (performance_timer)					// je pou�it� r�chli �asova� (Performance Timer ) ?
	{	QueryPerformanceCounter((LARGE_INTEGER *) &(performance_timer_start) );}
	else
	{	mm_timer_start = timeGetTime();}
}