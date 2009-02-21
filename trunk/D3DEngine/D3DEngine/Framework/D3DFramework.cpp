#include "StdAfx.h"
#include "D3DFramework.h"
#include "../Common/Interface.h"
#include "../Input/WinInput.h"
#include "../Resource/ResourceMng.h"
#include "../Scene/SceneMng.h"
#include "../Log/DebugInfoScene.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


cD3DFramework* g_pD3DFramework=NULL;


cD3DFramework::cD3DFramework( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_szTitleName(szTitleName),m_bFullScreen(bFullScreen)
{
	m_RequestRect.left=0;
	m_RequestRect.top=0;
	m_RequestRect.right=nWidth;
	m_RequestRect.bottom=nHeight;
	


	g_pD3DFramework = this;	
	m_szClassName = _T("cD3DFramework WndClass");			// default class name
	m_bQuitLoop=FALSE;	
	
	m_pD3D9 = NULL;
	m_pD3DDevice = NULL;
	memset(&m_D3DPP,0,sizeof(m_D3DPP));

}
cD3DFramework::~cD3DFramework(void)
{
	m_listControlable.clear();
	m_listRenderable.clear();
	m_listProgressable.clear();


	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D9);
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

	m_AdjustRect = m_RequestRect;
	AdjustWindowRect(&m_AdjustRect,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd = ::CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_AdjustRect.right-m_AdjustRect.left ,
		m_AdjustRect.bottom - m_AdjustRect.top ,
		NULL, NULL,m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	InitD3D();

	m_pInput = new cWinInput;
	m_pResourceMng = new cResourceMng;
	m_pSceneMng = new cSceneMng;
	m_pDebugInfoScene = new cDebugInfoScene;

	AttachObject(m_pInput);
	AttachObject(m_pSceneMng);
}

void cD3DFramework::CloseApplication()
{	
	SAFE_DELETE( m_pDebugInfoScene );
	SAFE_DELETE( m_pSceneMng );
	SAFE_DELETE( m_pResourceMng );
	SAFE_DELETE( m_pInput );
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
			ProcessControlableList();
			ProcessProgressableList(m_DeltaFrameTime);
			ProcessRenderableList();		
			m_PrevFrameTime=m_CurrFrameTime;
		} 		
 	}
	g_pD3DFramework->OnCloseApplication();
	g_pD3DFramework->CloseApplication();	
}

void cD3DFramework::ProcessControlableList()
{
	list<cIControlable*>::iterator it_control=m_listControlable.begin();
	for ( ;it_control!=m_listControlable.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
	m_pDebugInfoScene->Control();
}


void cD3DFramework::ProcessProgressableList(DWORD elapseTime)
{	
	list<cIUpdatable*>::iterator it=m_listProgressable.begin();

	m_pDebugInfoScene->Update(elapseTime);

	for ( ;it!=m_listProgressable.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}		
}

void cD3DFramework::ProcessRenderableList()
{
	m_pD3DDevice->SetViewport(&m_ViewPortInfo);

	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pD3DDevice->BeginScene();

	list<cIRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->ProcessRender();
	}

	m_pDebugInfoScene->ProcessRender();

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );	
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
	cIRenderable* pIR = dynamic_cast<cIRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_listRenderable.push_back(pIR);
		pIR->m_ItRenderable = --m_listRenderable.end();	
	}

	cIUpdatable* pIP = dynamic_cast< cIUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_listProgressable.push_back(pIP);
		pIP->m_ItProgressable = --m_listProgressable.end();	
	}

	cIControlable* pIC = dynamic_cast< cIControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_listControlable.push_back(pIC);
		pIC->m_ItControlable = --m_listControlable.end();	
	}
}

void cD3DFramework::DettachObject( IUnknownObject* pIUnknownObject )
{
	cIRenderable* pIR = dynamic_cast<cIRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_listRenderable.erase(pIR->m_ItRenderable);
	}

	cIUpdatable* pIP = dynamic_cast< cIUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_listProgressable.erase(pIP->m_ItProgressable);
	}

	cIControlable* pIC = dynamic_cast< cIControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_listControlable.erase(pIC->m_ItControlable);
	}	
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

void cD3DFramework::InitD3D()
{
	// 1) D3D를 생성한다.
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );

	// 2) Device를 생성을 위한 Parameter를 설정한다.

	m_D3DPP.Windowed	 = TRUE;
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;

	// Zbuffer사용
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;


	// 3) Device를 생성한다.

	m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pD3DDevice );

	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);

	m_pD3DDevice->GetViewport(&m_ViewPortInfo);

	D3DMATERIAL9 mtrl;	
	ZeroMemory(&mtrl,sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 1.0f;
	mtrl.Diffuse.a = 1.0f;

	mtrl.Ambient.r = 1.0f;
	mtrl.Ambient.g = 1.0f;
	mtrl.Ambient.b = 1.0f;
	mtrl.Ambient.a = 1.0f;

	m_pD3DDevice->SetMaterial(&mtrl);


	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type	=	D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Direction = D3DXVECTOR3(0.0f,-1.0f,1.0f);		// 타겟 방향
	//	light.Range = 1000.0f;

	m_pD3DDevice->SetLight(0,&light);
	m_pD3DDevice->LightEnable(0,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
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