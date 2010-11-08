#include "StdAfx.h"
#include "D3DFramework.h"
#include "../Common/Interface.h"
#include "../Renderer/D3DGraphics.h"
#include "../Input/WinInput.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


cD3DFramework* g_pD3DFramework=NULL;


cD3DFramework::cD3DFramework( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_szTitleName(szTitleName),m_bFullScreen(bFullScreen)
{
	m_rtClient.left=0;
	m_rtClient.top=0;
	m_rtClient.right=nWidth;
	m_rtClient.bottom=nHeight;

	g_pD3DFramework = this;	
	m_szClassName = _T("cD3DFramework WndClass");			// default class name
	m_bQuitLoop=FALSE;	
}
cD3DFramework::~cD3DFramework(void)
{
	m_listControlable.clear();
	m_listRenderable.clear();
	m_listProgressable.clear();
}



void cD3DFramework::InitApplication()
{
//	m_wndclass.cbSize = sizeof(WNDCLASSEX);
	m_wndclass.style		= CS_HREDRAW | CS_VREDRAW;
	m_wndclass.lpfnWndProc	= WndProc;
	m_wndclass.cbClsExtra	= 0;
	m_wndclass.cbWndExtra	= 0;
	m_wndclass.hInstance	= m_hInstance;
	m_wndclass.hIcon		= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DENGINE));
	m_wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_wndclass.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	m_wndclass.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_D3DENGINE);
	m_wndclass.lpszClassName= m_szClassName;
//	m_wndclass.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	::RegisterClass(&m_wndclass);

	
	AdjustWindowRect(&m_rtClient,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd = ::CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_rtClient.right-m_rtClient.left ,
		m_rtClient.bottom - m_rtClient.top ,
		NULL, NULL,m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
	
	m_pGraphics = new cD3DGraphics;
	m_pGraphics->InitD3D();

	m_pInput = new cWinInput;

	AddObject(m_pInput);
	AddObject(m_pGraphics);

}

void cD3DFramework::CloseApplication()
{	
	delete m_pInput;
	delete m_pGraphics;	
}

void cD3DFramework::Run()
{
	g_pD3DFramework->InitApplication();
	g_pD3DFramework->OnInitApplication();
	m_PrevFrameTime=GetTickCount();
 	while (!m_bQuitLoop) 
 	{
		// 윈도우 메시지가 없으면 수행
 		if(!ProcessWindowMessage())
 		{
			m_CurrFrameTime=GetTickCount();		
			m_DeltaFrameTime = m_CurrFrameTime - m_PrevFrameTime;
			m_AccumFrameTime += m_DeltaFrameTime;
			ProcessProgress();
			ProcessRender();		
			m_PrevFrameTime=m_CurrFrameTime;
		} 		
 	}
	g_pD3DFramework->OnCloseApplication();
	g_pD3DFramework->CloseApplication();	
}
																																																									

void cD3DFramework::ProcessProgress()
{		
	list<cIControlable*>::iterator it_control=m_listControlable.begin();
	for ( ;it_control!=m_listControlable.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	

	list<cIProgressable*>::iterator it=m_listProgressable.begin();
	for ( ;it!=m_listProgressable.end() ; ++it )
	{
		(*it)->Update(m_DeltaFrameTime);
	}	
}

void cD3DFramework::ProcessRender()
{
	OnRenderStart();
	list<cIRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->Render();
	}
	OnRenderEnd();
}


BOOL cD3DFramework::ProcessWindowMessage()
{
	static MSG msg;
	//메시지 큐에 메시지가 있는지 검사한다.
	if(!::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		return FALSE;

	if(!::GetMessage( &msg, NULL, 0, 0 ))	
	{
		m_bQuitLoop=TRUE;//WM_Quit 
		return TRUE;
	}

	::TranslateMessage(&msg);	
	::DispatchMessage(&msg);
	return TRUE;
}


BOOL cD3DFramework::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}

BOOL cD3DFramework::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL cD3DFramework::OnWM_General( MSG& msg )
{
	return FALSE;
}


void cD3DFramework::OnRenderStart()
{
	m_pD3DDevice9->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pD3DDevice9->BeginScene();
}


void cD3DFramework::OnRenderEnd()
{
	m_pD3DDevice9->EndScene();
	m_pD3DDevice9->Present( NULL, NULL, NULL, NULL );	
}

BOOL cD3DFramework::AddObject( IUnknownObject* pIUnknownObject )
{
	BOOL bRet=FALSE;
	cIRenderable* pIR = dynamic_cast<cIRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_listRenderable.push_back(pIR);	bRet=TRUE;		
	}

	cIProgressable* pIP = dynamic_cast< cIProgressable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_listProgressable.push_back(pIP);	bRet=TRUE;
	}

	cIControlable* pIC = dynamic_cast< cIControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_listControlable.push_back(pIC);	bRet=TRUE;
	}
	return bRet;
}

void cD3DFramework::SetTitleName( LPCTSTR lpszString )
{
	assert(::IsWindow(m_hWnd));
	::SetWindowText(m_hWnd,lpszString);	
}


void cD3DFramework::ScreenToClient(LPRECT lpRect) const
{	
	::ScreenToClient(m_hWnd, (LPPOINT)lpRect);
	::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
}

void cD3DFramework::ClientToScreen(LPRECT lpRect) const
{	
	::ClientToScreen(m_hWnd, (LPPOINT)lpRect);
	::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);	
}


DWORD cD3DFramework::GetStyle() const
{ 
	assert(::IsWindow(m_hWnd)); 
	return (DWORD)GetWindowLong(m_hWnd, GWL_STYLE); 
}
DWORD cD3DFramework::GetExStyle() const
{ 
	assert(::IsWindow(m_hWnd)); 
	return (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE); 
}

void cD3DFramework::MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint /* = TRUE  */)
{
	::MoveWindow(m_hWnd,x,y,nWidth,nHeight,bRepaint);
}

void cD3DFramework::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	::MoveWindow(m_hWnd,lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,bRepaint);
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
		bProcess=g_pD3DFramework->OnWM_Keyboard(msg);		
	}
	else if ((WM_MOUSEFIRST < msg.message)&&(msg.message < WM_MOUSELAST))
	{
		bProcess=g_pD3DFramework->OnWM_Mouse(msg);
	}
	else
	{
		bProcess=g_pD3DFramework->OnWM_General(msg);	
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