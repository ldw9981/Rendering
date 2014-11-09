#include "StdAfx.h"
#include "TestGameApp.h"

#include "Resource/ResourceMng.h"
#include "Graphics/RscTexture.h"

#include "Framework/ViewMng.h"
#include "TestView.h"
#include "MenuView.h"
#include "Framework/D3DFramework.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Foundation/HeapValidator.h"

using namespace Sophia;
TestGameApp::TestGameApp( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{

	
}

TestGameApp::~TestGameApp(void)
{
}

bool TestGameApp::Initialize()
{
	if(!cD3DFramework::Initialize())
		return false;
	
	std::string Path;
	Path = EnvironmentVariable::GetInstance().GetString("CurrPath");

	std::string::size_type index=Path.length();
	for (int i=0;i<3;i++)
	{
		index=Path.rfind("\\",index-1,1);
	}	
	Path  = Path.substr ( 0 ,index+1);		
	Path  += "Data\\";

	EnvironmentVariable::GetInstance().SetString("DataPath",Path.c_str());
	

	std::string strHLSL=Path;
	strHLSL+= "hlsl.fx";
	m_pGraphics->LoadHLSL(strHLSL.c_str());
	

	m_pView = new cMenuView;
	AttachObject(m_pView);
	m_pView->Enter();

	
	return true;
}

void TestGameApp::Finalize()
{
	if (m_pView)
	{
		m_pView->Leave();
		DettachObject(m_pView);
		SAFE_DELETE(m_pView);
	}			
	cD3DFramework::Finalize();
}


BOOL TestGameApp::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}


BOOL TestGameApp::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL TestGameApp::OnWM_General( MSG& msg )
{
	switch(msg.message)
	{
	case WM_ACTIVATE:    
		if(msg.wParam == WA_ACTIVE)    
		{        
			m_pInput->SetRequestAcquire(true);
		}    
		if(msg.wParam == WA_INACTIVE)
		{        
			m_pInput->UnAcquire();
		}
		return TRUE;
	
	}
	return FALSE;
}





int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	HeapValidator::SetDbgFlag();
	//HeapValidator::SetBreakAlloc(762074);

	TestGameApp TestGameApp("TestGameApp",false,1024,768);
	if(TestGameApp.Initialize())
	{
		TestGameApp.Run();
	}
	TestGameApp.Finalize();
	
	HeapValidator::CheckMemory();
	return 0;
}