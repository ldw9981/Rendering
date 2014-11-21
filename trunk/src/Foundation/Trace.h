#pragma once
namespace Sophia
{

extern HWND g_TraceHWND;
extern bool g_TraceShowMsgBox;
// 전역 함수
extern void Trace(const char *format, ...);
extern void TraceMSGBOX(const char *format, ...);
HRESULT WINAPI DXUTTrace( const CHAR* strFile, DWORD dwLine, HRESULT hr,
	const CHAR* strMsg, bool bPopMsgBox );
///// 디버그 모드
#ifdef _DEBUG
#include <assert.h>

//#define new DEBUG_NEW
//#define malloc( size_t )	_malloc_dbg( size_t , _NORMAL_BLOCK, __FILE__, __LINE__ )

#define THIS_FILE                   __FILE__
#define ASSERT(e)                   assert(e)
#define ASSERT_VALID(o)             if (!o) assert(FALSE)

#define TRACE(sz)				Trace(sz)
#define TRACE0(sz)              Trace(sz)
#define TRACE1(sz, p1)          Trace(sz, p1)
#define TRACE2(sz, p1, p2)      Trace(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)  Trace(sz, p1, p2, p3)
#define TRACE4(sz, p1, p2, p3,p4)  Trace(sz, p1, p2, p3,p4)

#else // RELEASE == #ifdef !(_DEBUG) 

///// 릴리즈 모드

#define ASSERT(e)                   ((void)0)
#define ASSERT_VALID(o)             ((void)0)

#define TRACE(s)                    ((void)0)
#define TRACE0(s)                   ((void)0)   
#define TRACE1(f, a1)               ((void)0)
#define TRACE2(f, a1, a2)           ((void)0)
#define TRACE3(f, a1, a2, a3)       ((void)0)
#define TRACE4(f, a1, a2, a3, a4)   ((void)0)

#endif // _DEBUG

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, #x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, #x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

}