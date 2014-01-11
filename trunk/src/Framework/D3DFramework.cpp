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
#include "Window.h"



cD3DFramework* g_pApp=NULL;


cD3DFramework::cD3DFramework( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight )
:m_bFullScreen(bFullScreen)
{
	m_RequestRect.left=0;
	m_RequestRect.top=0;
	m_RequestRect.right=nWidth;
	m_RequestRect.bottom=nHeight;
	


	g_pApp = this;	

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



bool cD3DFramework::Initialize()
{
	m_hInstance = GetModuleHandle(NULL);

	char CurrPath[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH,CurrPath);
	EnvironmentVariable::GetInstance().SetString("CurrPath",std::string(CurrPath)+std::string("\\"));

	m_pWindow = new Window;
	m_pWindow->Initialize(m_RequestRect);

	m_pD3D9Server = new D3D9::Server;	
	m_pD3D9Server->Init(!m_bFullScreen,GetRequestRectWidth(),GetRequestRectHeight());

	m_pInput = new Input;
	if(!m_pInput->Initialize(m_hInstance,m_pWindow->m_hWnd,GetRequestRectWidth(),GetRequestRectHeight()))
	{
		MessageBox(m_pWindow->m_hWnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

	m_pResourceMng = new cResourceMng;

	AttachObject(m_pInput);
	return true;
}

void cD3DFramework::Finalize()
{		
	SAFE_DELETE( m_pResourceMng );
	SAFE_DELETE( m_pInput );

	m_pD3D9Server->Finalize();
	SAFE_DELETE( m_pD3D9Server);


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
	D3D9::Server::g_pServer->Begin();
	int temp = m_FpsMng.GetFPS();
	std::ostringstream stream;
	stream << "FPS " << temp << " ";
	stream << "LIGHT" << D3D9::Server::g_pServer->m_WorldLightPosition.x << " ";
	stream << D3D9::Server::g_pServer->m_WorldLightPosition.y << " "; 
	stream << D3D9::Server::g_pServer->m_WorldLightPosition.z << " ";
	stream << "RESOUCE " << m_pResourceMng->GetCount() << " ";
	D3D9::Server::g_pServer->RenderDebugString(0,0,stream.str().c_str());

	std::list<IRenderable*>::iterator it=m_listRenderable.begin();
	for ( ;it!=m_listRenderable.end() ; ++it )
	{
		(*it)->ProcessRender();	
	}

	D3D9::Server::g_pServer->End();
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

