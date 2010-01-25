#pragma once
#include <Windows.h>

extern HWND g_TraceHWND;

// ���� �Լ�
extern void Trace(TCHAR *format, ...);
extern void TraceMSGBOX(TCHAR *format, ...);

///// ����� ���
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

///// ������ ���

#define ASSERT(e)                   ((void)0)
#define ASSERT_VALID(o)             ((void)0)

#define TRACE(s)                    ((void)0)
#define TRACE0(s)                   ((void)0)   
#define TRACE1(f, a1)               ((void)0)
#define TRACE2(f, a1, a2)           ((void)0)
#define TRACE3(f, a1, a2, a3)       ((void)0)
#define TRACE4(f, a1, a2, a3, a4)   ((void)0)

#endif // _DEBUG