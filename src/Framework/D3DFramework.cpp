#include "StdAfx.h"
#include "D3DFramework.h"
#include "Foundation/Interface.h"
#include "Input/Input.h"
#include "Resource/ResourceMng.h"
#include "Framework/ViewMng.h"
#include "Framework/DebugInfoView.h"
#include "Foundation/Define.h"
#include "Foundation/EnvironmentVariable.h"	
#include "Graphics/Graphics.h"
#include "Window.h"
#include "GUI/CursorManager.h"

namespace Sophia
{


cD3DFramework* cD3DFramework::m_pInstance;
cD3DFramework* g_pApp=NULL;

cD3DFramework::cD3DFramework( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_bFullScreen(bFullScreen)
{
	m_RequestRect.left=0;
	m_RequestRect.top=0;
	m_RequestRect.right=nWidth;
	m_RequestRect.bottom=nHeight;
	


	g_pApp = this;	
	m_pInstance = this;

	m_bQuitLoop = FALSE;	

	m_pWindow = NULL;
	m_pGraphics = NULL;
	m_pInput = NULL;
	m_pResourceMng = NULL;
	m_pCursorManager = NULL;
	m_pView = NULL;

}
cD3DFramework::~cD3DFramework(void)
{
	m_listControlable.clear();
	m_listRenderable.clear();
	m_listProgressable.clear();


//	SAFE_RELEASE(Graphics::m_pDevice);
//	SAFE_RELEASE(m_pD3D9);
}



bool cD3DFramework::Initialize()
{
	m_hInstance = GetModuleHandle(NULL);

	char CurrPath[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH,CurrPath);
	EnvironmentVariable::GetInstance().SetString("CurrPath",std::string(CurrPath)+std::string("\\"));

	InitWindow();

	m_pGraphics = new Sophia::Graphics;	
	m_pGraphics->Init(m_hWndMain,m_hWndPresent,!m_bFullScreen,GetRequestRectWidth(),GetRequestRectHeight());

	m_pInput = new Input;
	AttachObject(m_pInput);
	if(!m_pInput->Initialize(m_hInstance,m_hWndMain,GetRequestRectWidth(),GetRequestRectHeight()))
	{
		MessageBox(m_hWndMain, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}
	

	m_pResourceMng = new cResourceMng;	
	m_pCursorManager = new CursorManager;
	m_pCursorManager->Initialize(m_hWndMain);
	return true;
}

void cD3DFramework::Finalize()
{		
	if (m_pCursorManager)
	{
		m_pCursorManager->Finalize();
		SAFE_DELETE(m_pCursorManager);
	}	

	SAFE_DELETE(m_pResourceMng);
	if (m_pInput)
	{
		DettachObject(m_pInput);
		m_pInput->Finalize();
		SAFE_DELETE( m_pInput );
	}

	if (m_pGraphics)
	{
		m_pGraphics->Finalize();
		SAFE_DELETE( m_pGraphics);
	}

	if (m_pWindow)
	{
		m_pWindow->Finalize();
		SAFE_DELETE( m_pWindow);
	}
}

void cD3DFramework::Run()
{	
	m_PrevFrameTime=GetTickCount();
 	while (!m_bQuitLoop) 
 	{
 		if(!m_pWindow->ProcessWindowMessage())
		{
			m_bQuitLoop = true;
		}
 		
		m_CurrFrameTime=GetTickCount();		
		m_DeltaFrameTime = m_CurrFrameTime - m_PrevFrameTime;
		m_AccumFrameTime += m_DeltaFrameTime;			
		Control();
		Update(m_DeltaFrameTime);		// Update
		Render();						// Render
		m_PrevFrameTime=m_CurrFrameTime;
		 		
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
	Graphics::m_pInstance->Begin();
	int temp = m_FpsMng.GetFPS();
	std::ostringstream stream;
	stream << "FPS " << temp << "\n";

	POINT pt;
	g_pInput->GetMouseDelta(pt.x,pt.y);
	stream << "MOUSE delta " << g_pInput->Mouse_IsMove() << "" << pt.x << " " << pt.y << "\n";
	m_pCursorManager->GetCursorPos(&pt);
	stream << "CURSOR pos " << pt.x << " " << pt.y << "\n";
	//stream << Graphics::g_pGraphics->m_WorldLightPosition.y << " "; 
	//stream << Graphics::g_pGraphics->m_WorldLightPosition.z << " ";
	stream << "RESOUCE " << m_pResourceMng->GetCount() << "\n";
	Graphics::m_pInstance->RenderDebugString(0,0,stream.str().c_str());

	std::list<IRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->ProcessRender();	
	}

	Graphics::m_pInstance->End();
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

void cD3DFramework::InitWindow()
{
	m_pWindow = new Window;
	m_pWindow->Initialize(m_RequestRect);
	m_hWndMain = m_pWindow->m_hWnd;
	m_hWndPresent = m_pWindow->m_hWnd;
}



}