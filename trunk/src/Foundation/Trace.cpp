#include <memory.h>
#include <crtdbg.h>
#include <tchar.h>
#include "Trace.h"

HWND g_TraceHWND;

void Trace(TCHAR *format, ...)
{
	va_list argptr;
	TCHAR    buf[512];

	memset(buf, 0, 512);
	va_start(argptr, format);	
	int i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	OutputDebugString(buf);
}

void TraceMSGBOX(TCHAR *format, ...)
{
	va_list argptr;
	TCHAR    buf[512];

	memset(buf, 0, 512);
	va_start(argptr, format);	
	int i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	int rval = MessageBox(g_TraceHWND, _T("TraceMSGBOX"), buf, MB_YESNO|MB_TOPMOST);
	if (rval == IDYES) PostMessage(g_TraceHWND, WM_CLOSE, 0, 0);		

}