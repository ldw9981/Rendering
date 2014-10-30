#include "StdAfx.h"
#include "Trace.h"
namespace Sophia
{

HWND g_TraceHWND;
bool g_TraceShowMsgBox;
void Trace(const char *format, ...)
{
	va_list argptr;
	char    buf[512];

	memset(buf, 0, 512);
	va_start(argptr, format);	
	int i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	OutputDebugString(buf);
}

void TraceMSGBOX(const char *format, ...)
{
	va_list argptr;
	char    buf[512];

	memset(buf, 0, 512);
	va_start(argptr, format);	
	int i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	int rval = MessageBox(g_TraceHWND,buf, "TraceMSGBOX", MB_YESNO|MB_TOPMOST);
	if (rval == IDYES) PostMessage(g_TraceHWND, WM_CLOSE, 0, 0);		

}

void TraceError(const char lpszFunction) 
{ 
	char szBuf[80]; 
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	sprintf_s(szBuf, 
		"%s failed with error %d: %s", 
		lpszFunction, dw, lpMsgBuf); 

	LocalFree(lpMsgBuf);
	OutputDebugString(szBuf);

	int ret=MessageBox(NULL, szBuf,"Error 계속 진행하겠습니까?", MB_YESNO);

	if (ret==IDNO)
	{
		ExitProcess(dw); 
	}

}

HRESULT WINAPI DXUTTrace( const CHAR* strFile, DWORD dwLine, HRESULT hr, const CHAR* strMsg, bool bPopMsgBox )
{
	bool bShowMsgBoxOnError = g_TraceShowMsgBox;
	if( bPopMsgBox && bShowMsgBoxOnError == false )
		bPopMsgBox = false;

	return DXTrace( strFile, dwLine, hr, strMsg, bPopMsgBox );
}

}