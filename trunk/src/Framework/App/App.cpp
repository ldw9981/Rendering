
#include "App.h"
#include "../Interface.h"
#include "../Input/WinInput.h"
#include "../../Foundation//ResourceMng.h"
#include "../../Scene/SceneMng.h"
#include "../../Scene/DebugInfoScene.h"
#include "../../Foundation/Define.h"
#include <tchar.h>
#include <assert.h>
#include <d3d9.h>


namespace Application
{

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

App* App::Singleton = 0;

App::App( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_szTitleName(szTitleName),m_bFullScreen(bFullScreen)
{
	m_RequestRect.left=0;
	m_RequestRect.top=0;
	m_RequestRect.right=nWidth;
	m_RequestRect.bottom=nHeight;
	


	
	m_szClassName = _T("cD3DFramework WndClass");			// default class name
	m_bQuitLoop=FALSE;	
	
	App::Singleton = this;
	m_pD3D9 = NULL;
	m_SharedLPDIRECT3DDEVICE9 = NULL;
	memset(&m_D3DPP,0,sizeof(m_D3DPP));

}
App::~App(void)
{
	m_listControlable.clear();
	m_listRenderable.clear();
	m_listProgressable.clear();


	SAFE_RELEASE(m_SharedLPDIRECT3DDEVICE9);
	SAFE_RELEASE(m_pD3D9);
}



BOOL App::Open()
{
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,path);


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
	m_SharedHWND = ::CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		m_AdjustRect.right-m_AdjustRect.left ,
		m_AdjustRect.bottom - m_AdjustRect.top ,
		NULL, NULL,m_hInstance, NULL);

	ShowWindow(m_SharedHWND, SW_SHOWNORMAL);
	UpdateWindow(m_SharedHWND);

	InitD3D();

	m_pInput = new WinInput;
	m_pResourceMng = new ResourceMng;
	m_pSceneMng = new SceneMng;
	m_pDebugInfoScene = new cDebugInfoScene;

	AttachObject(m_pInput);
	AttachObject(m_pSceneMng);
	
	return TRUE;
}

void App::Close()
{	

	SAFE_DELETE( m_pDebugInfoScene );
	SAFE_DELETE( m_pSceneMng );
	SAFE_DELETE( m_pResourceMng );
	SAFE_DELETE( m_pInput );

}

void App::Run()
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
			ProcessControlableList();
			ProcessProgressableList(m_DeltaFrameTime);
			ProcessRenderableList();		
			m_PrevFrameTime=m_CurrFrameTime;
		} 		
 	}	
}


void App::ProcessControlableList()
{
	list<Framework::IControlable*>::iterator it_control=m_listControlable.begin();
	for ( ;it_control!=m_listControlable.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
	m_pDebugInfoScene->Control();
}


void App::ProcessProgressableList(DWORD elapseTime)
{	
	list<Framework::IUpdatable*>::iterator it=m_listProgressable.begin();

	m_pDebugInfoScene->Update(elapseTime);

	for ( ;it!=m_listProgressable.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}		
}

void App::ProcessRenderableList()
{
	m_SharedLPDIRECT3DDEVICE9->SetViewport(&m_ViewPortInfo);

	m_SharedLPDIRECT3DDEVICE9->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_SharedLPDIRECT3DDEVICE9->BeginScene();

	list<Framework::IRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->ProcessRender();
	}

	m_pDebugInfoScene->ProcessRender();

	m_SharedLPDIRECT3DDEVICE9->EndScene();
	m_SharedLPDIRECT3DDEVICE9->Present( NULL, NULL, NULL, NULL );	
}


BOOL App::ProcessWindowMessage()
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


BOOL App::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}

BOOL App::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL App::OnWM_General( MSG& msg )
{
	return FALSE;
}



void App::AttachObject( Framework::IUnknown* pIUnknown )
{
	Framework::IRenderable* pIR = dynamic_cast<Framework::IRenderable*>(pIUnknown);
	if(pIR != NULL)
	{
		m_listRenderable.push_back(pIR);
		pIR->m_ItRenderable = --m_listRenderable.end();	
	}

	Framework::IUpdatable* pIP = dynamic_cast< Framework::IUpdatable *>(pIUnknown);
	if(pIP != NULL)
	{
		m_listProgressable.push_back(pIP);
		pIP->m_ItProgressable = --m_listProgressable.end();	
	}

	Framework::IControlable* pIC = dynamic_cast< Framework::IControlable *>(pIUnknown);
	if(pIC != NULL)
	{
		m_listControlable.push_back(pIC);
		pIC->m_ItControlable = --m_listControlable.end();	
	}
}

void App::DettachObject( Framework::IUnknown* pIUnknown )
{
	Framework::IRenderable* pIR = dynamic_cast<Framework::IRenderable*>(pIUnknown);
	if(pIR != NULL)
	{
		m_listRenderable.erase(pIR->m_ItRenderable);
	}

	Framework::IUpdatable* pIP = dynamic_cast< Framework::IUpdatable *>(pIUnknown);
	if(pIP != NULL)
	{
		m_listProgressable.erase(pIP->m_ItProgressable);
	}

	Framework::IControlable* pIC = dynamic_cast< Framework::IControlable *>(pIUnknown);
	if(pIC != NULL)
	{
		m_listControlable.erase(pIC->m_ItControlable);
	}	
}

void App::SetTitleName( LPCTSTR lpszString )
{
	assert(::IsWindow(m_SharedHWND));
	::SetWindowText(m_SharedHWND,lpszString);	
}


void App::ScreenToClient(LPRECT lpRect) const
{	
	::ScreenToClient(m_SharedHWND, (LPPOINT)lpRect);
	::ScreenToClient(m_SharedHWND, ((LPPOINT)lpRect)+1);
}

void App::ClientToScreen(LPRECT lpRect) const
{	
	::ClientToScreen(m_SharedHWND, (LPPOINT)lpRect);
	::ClientToScreen(m_SharedHWND, ((LPPOINT)lpRect)+1);	
}


DWORD App::GetStyle() const
{ 
	assert(::IsWindow(m_SharedHWND)); 
	return (DWORD)GetWindowLong(m_SharedHWND, GWL_STYLE); 
}
DWORD App::GetExStyle() const
{ 
	assert(::IsWindow(m_SharedHWND)); 
	return (DWORD)GetWindowLong(m_SharedHWND, GWL_EXSTYLE); 
}

void App::MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint /* = TRUE  */)
{
	::MoveWindow(m_SharedHWND,x,y,nWidth,nHeight,bRepaint);
}

void App::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	::MoveWindow(m_SharedHWND,lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,bRepaint);
}

void App::InitD3D()
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
		m_SharedHWND,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_SharedLPDIRECT3DDEVICE9 );

	m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_ZENABLE,TRUE);

	m_SharedLPDIRECT3DDEVICE9->GetViewport(&m_ViewPortInfo);

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

	m_SharedLPDIRECT3DDEVICE9->SetMaterial(&mtrl);


	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type	=	D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Direction = D3DXVECTOR3(0.0f,-1.0f,1.0f);		// 타겟 방향
	//	light.Range = 1000.0f;

	m_SharedLPDIRECT3DDEVICE9->SetLight(0,&light);
	m_SharedLPDIRECT3DDEVICE9->LightEnable(0,TRUE);
	m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_LIGHTING,TRUE);
	
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
		bProcess=App::Instance()->OnWM_Keyboard(msg);		
	}
	else if ((WM_MOUSEFIRST < msg.message)&&(msg.message < WM_MOUSELAST))
	{
		bProcess=App::Instance()->OnWM_Mouse(msg);
	}
	else
	{
		bProcess=App::Instance()->OnWM_General(msg);	
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


}