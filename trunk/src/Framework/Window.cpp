#include "StdAfx.h"
#include "Window.h"
#include "D3DFramework.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Window::Window(void)
{
	m_szTitleName = "cD3DFramework m_szTitleName";			// default class name
		m_szClassName = "cD3DFramework m_szClassName";			// default class name
}


Window::~Window(void)
{
}
void Window::SetTitleName( const char* lpszString )
{
	assert(::IsWindow(m_hWnd));
	::SetWindowText(m_hWnd,lpszString);	
}


void Window::ScreenToClient(LPRECT lpRect) const
{	
	::ScreenToClient(m_hWnd, (LPPOINT)lpRect);
	::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
}

void Window::ClientToScreen(LPRECT lpRect) const
{	
	::ClientToScreen(m_hWnd, (LPPOINT)lpRect);
	::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);	
}


DWORD Window::GetStyle() const
{ 
	assert(::IsWindow(m_hWnd)); 
	return (DWORD)GetWindowLong(m_hWnd, GWL_STYLE); 
}
DWORD Window::GetExStyle() const
{ 
	assert(::IsWindow(m_hWnd)); 
	return (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE); 
}

void Window::MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint /* = TRUE  */)
{
	::MoveWindow(m_hWnd,x,y,nWidth,nHeight,bRepaint);
}

void Window::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	::MoveWindow(m_hWnd,lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,bRepaint);
}



void Window::Initialize(RECT& request)
{
	//	m_wndclass.cbSize = sizeof(WNDCLASSEX);
	m_wndclass.style		= CS_HREDRAW | CS_VREDRAW;
	m_wndclass.lpfnWndProc	= WndProc;
	m_wndclass.cbClsExtra	= 0;
	m_wndclass.cbWndExtra	= 0;
	m_wndclass.hInstance	= g_pApp->m_hInstance;
	m_wndclass.hIcon		= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DENGINE));
	m_wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_wndclass.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	m_wndclass.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_D3DENGINE);
	m_wndclass.lpszClassName= m_szClassName;
	//	m_wndclass.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	::RegisterClassA(&m_wndclass);

	m_AdjustRect = request;
	AdjustWindowRect(&m_AdjustRect,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd = ::CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_AdjustRect.right-m_AdjustRect.left ,
		m_AdjustRect.bottom - m_AdjustRect.top ,
		NULL, NULL,g_pApp->m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}



BOOL Window::ProcessWindowMessage()
{
	static MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Translate the message and dispatch it to WindowProc()
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If the message is WM_QUIT, exit the while loop
	if(msg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}

void Window::Shutdown()
{

}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static MSG msg;
	static HDC hdc;
	static BOOL bProcess;
	msg.hwnd=hWnd;	msg.message=message;	msg.wParam=wParam;	msg.lParam=lParam;

	if ((WM_KEYFIRST < msg.message)&&(msg.message < WM_KEYLAST))
	{
		bProcess=g_pApp->OnWM_Keyboard(msg);		
	}
	else if ((WM_MOUSEFIRST < msg.message)&&(msg.message < WM_MOUSELAST))
	{
		bProcess=g_pApp->OnWM_Mouse(msg);
	}
	else
	{
		bProcess=g_pApp->OnWM_General(msg);	
	}

	// 처리 안된 메세지만 처리한다.
	if (!bProcess)
	{
		switch (message)
		{				
		case WM_PAINT:
			hdc = ::BeginPaint(hWnd, &ps);
			// TODO: 여기에 그리기 코드를 추가합니다.
			::EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0); 
			break;
		default:
			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}			
	}
	return 0;
}