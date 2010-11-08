#include "timer.h"

Timer::Timer(void)									// inicializácia èasovaèa
{
	mm_timer_start = 0;
	performance_timer_start = 0;

	// zistenie èi rýchli èasovaè je použite¾ný
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &(frekvencia) ))
	{
		// rýchli èasovaè nepodporovaný hardwarom
		performance_timer	= FALSE;					// nastav Performance Timer na false
		mm_timer_start		= timeGetTime();			// použitím timeGetTime() nastav štartovaciu hodnotu
		perioda				= 1.0f/1000.0f;				// nastav periodu na 1 ms
		frekvencia			= 1000;						// nastav frekvenciu na 1000 Hz
	}
	else
	{
		// rýchli èasovaè je prístupný
		QueryPerformanceCounter((LARGE_INTEGER *) &(performance_timer_start) );	//nastavenie štartovacej hodnoty
		performance_timer	= TRUE;				// nastav Performance Timer na TRUE
		perioda = (float) (1.0/((double)frekvencia));	// vypoèítanie periódy
	}
}

float Timer::GetTime(void)					// vráti èas v sekundách
{
	__int64 time;									// na uloženie aktuálnej hodnoty èasovaèa

	if (performance_timer)					// je použitý rýchli èasovaè (Performance Timer ) ?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);	// naèítanie aktuálnej hodnoty èasovaèa
		// vráti aktuálnu hodnotu minus štartovaciu hodnotu nasobenú periódou
		return ( (float) ( time - performance_timer_start) * perioda);
	}
	else
	{	return( (float) ( timeGetTime() - mm_timer_start) * perioda);}	// vráti poèet sekúnd od štartu
}

void Timer::Reset(void)
{	
	if (performance_timer)					// je použitý rýchli èasovaè (Performance Timer ) ?
	{	QueryPerformanceCounter((LARGE_INTEGER *) &(performance_timer_start) );}
	else
	{	mm_timer_start = timeGetTime();}
}