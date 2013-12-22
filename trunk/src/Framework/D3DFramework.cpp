#include "StdAfx.h"
#include "D3DFramework.h"
#include "Foundation//Interface.h"
#include "WinInput/WinInput.h"
#include "Resource/ResourceMng.h"
#include "Scene/ViewMng.h"
#include "Scene/DebugInfoView.h"
#include "Foundation/Define.h"
#include "EnvironmentVariable.h"	
#include "D3D9Server/Server.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


cD3DFramework* g_pApp=NULL;


cD3DFramework::cD3DFramework( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_szTitleName(szTitleName),m_bFullScreen(bFullScreen)
{
	m_RequestRect.left=0;
	m_RequestRect.top=0;
	m_RequestRect.right=nWidth;
	m_RequestRect.bottom=nHeight;
	


	g_pApp = this;	
	m_szClassName = "cD3DFramework WndClass";			// default class name
	m_bQuitLoop=FALSE;	



}
cD3DFramework::~cD3DFramework(void)
{
	m_listControlable.clear();
	m_listRenderable.clear();
	m_listProgressable.clear();


//	SAFE_RELEASE(m_pD3DDevice);
//	SAFE_RELEASE(m_pD3D9);
}



bool cD3DFramework::Open()
{
	char CurrPath[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH,CurrPath);
	EnvironmentVariable::GetInstance().SetString("CurrPath",std::string(CurrPath)+std::string("\\"));


	InitWindow();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

//	InitD3D();

	m_pD3D9Server = new D3D9::Server;
	m_pD3D9Server->Init();

	m_pInput = new cWinInput;
	m_pResourceMng = new cResourceMng;
//	m_pView = new cView;
//	m_pView->SetViewPortInfo(0,0,GetRequestRectWidth(),GetRequestRectHeight());

	AttachObject(m_pInput);
//	AttachObject(m_pView);
	return true;
}

void cD3DFramework::Close()
{		
	
//	SAFE_DELETE( m_pView );
	SAFE_DELETE( m_pResourceMng );
	SAFE_DELETE( m_pInput );

	m_pD3D9Server->Uninit();
	SAFE_DELETE( m_pD3D9Server);
}

void cD3DFramework::Run()
{	
	m_PrevFrameTime=GetTickCount();
 	while (!m_bQuitLoop) 
 	{
		// 윈도우 메시지가 없으면 수행
 		if(!ProcessWindowMessage())
 		{
			m_CurrFrameTime=GetTickCount();		
			m_DeltaFrameTime = m_CurrFrameTime - m_PrevFrameTime;
			m_AccumFrameTime += m_DeltaFrameTime;			
			Control();
			Update(m_DeltaFrameTime);		// Update
			Render();						// Render
			m_PrevFrameTime=m_CurrFrameTime;
		} 		
 	}	
}

void cD3DFramework::Control()
{
	std::list<IControlable*>::iterator it_control=m_listControlable.begin();
	for ( ;it_control!=m_listControlable.end() ; ++it_control )
	{
		(*it_control)->Control();
	}
}

void cD3DFramework::Update(DWORD elapseTime)
{
	std::list<IUpdatable*>::iterator it=m_listProgressable.begin();
	for ( ;it!=m_listProgressable.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}		
}

void cD3DFramework::Render()
{
	m_pView->SetViewPort();
	D3D9::Server::g_pServer->Begin();
	int temp = m_FpsMng.GetFPS();
	std::ostringstream stream;
	stream << "FPS " << temp;
	D3D9::Server::g_pServer->RenderDebugString(stream.str().c_str());

	std::list<IRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->ProcessRender();	
	}

	D3D9::Server::g_pServer->End();
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



void cD3DFramework::AttachObject( IUnknownObject* pIUnknownObject )
{
	IRenderable* pIR = dynamic_cast<IRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_listRenderable.push_back(pIR);
		pIR->m_ItRenderable = --m_listRenderable.end();	
	}

	IUpdatable* pIP = dynamic_cast< IUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_listProgressable.push_back(pIP);
		pIP->m_ItProgressable = --m_listProgressable.end();	
	}

	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_listControlable.push_back(pIC);
		pIC->m_ItControlable = --m_listControlable.end();	
	}
}

void cD3DFramework::DettachObject( IUnknownObject* pIUnknownObject )
{
	IRenderable* pIR = dynamic_cast<IRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_listRenderable.erase(pIR->m_ItRenderable);
	}

	IUpdatable* pIP = dynamic_cast< IUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_listProgressable.erase(pIP->m_ItProgressable);
	}

	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_listControlable.erase(pIC->m_ItControlable);
	}	
}

void cD3DFramework::SetTitleName( const char* lpszString )
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



void cD3DFramework::InitWindow()
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
	::RegisterClassA(&m_wndclass);

	m_AdjustRect = m_RequestRect;
	AdjustWindowRect(&m_AdjustRect,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd = ::CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_AdjustRect.right-m_AdjustRect.left ,
		m_AdjustRect.bottom - m_AdjustRect.top ,
		NULL, NULL,m_hInstance, NULL);
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