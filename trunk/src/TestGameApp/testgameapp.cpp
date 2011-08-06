#include "StdAfx.h"
#include "TestGameApp.h"

#include "Resource/ResourceMng.h"
#include "D3D9Server/RscTexture.h"

#include "Scene/ViewMng.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/D3DFramework.h"
#include "D3D9Server/Server.h"
#include "Framework/environmentVariable.h"
#include "Foundation/HeapValidator.h"

using namespace D3D9;

TestGameApp::TestGameApp( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{
	m_pMenuScene=NULL;
	m_pTestScene=NULL;
	m_pGlobalScene=NULL;
	
}

TestGameApp::~TestGameApp(void)
{
}

bool TestGameApp::Open()
{
	if(!cD3DFramework::Open())
		return false;
	
	string Path;
	Path = EnvironmentVariable::GetInstance().GetString("CurrPath");

	string::size_type index=Path.length();
	for (int i=0;i<3;i++)
	{
		index=Path.rfind("\\",index-1,1);
	}	
	Path  = Path.substr ( 0 ,index+1);		
	Path  += "Data\\";

	EnvironmentVariable::GetInstance().SetString("DataPath",Path.c_str());
	

	string strHLSL=Path;
	strHLSL+= "hlsl.fx";
	m_pD3D9Server->LoadHLSL(strHLSL.c_str());
	

	m_pMenuScene = new cMenuView;
	m_pTestScene = new cTestView;
	m_pGlobalScene = new cGlobalView;
	GetSceneMng()->ChangeTopScene((cView*)m_pTestScene);
	AttachObject(m_pGlobalScene);
	return true;
}

void TestGameApp::Close()
{
	delete  m_pGlobalScene;	
	delete	m_pTestScene;
	delete	m_pMenuScene;

	cD3DFramework::Close();
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
	case WM_ACTIVATEAPP:
		if (!msg.wParam)
		{
			//MessageBox(NULL,"the window is being deactivated.","알림",MB_OK);
		}
		else
		{
			//MessageBox(NULL,"the window is being activated.","알림",MB_OK);
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
	//MemoryValidator::SetBreakAlloc(133);	
	
	char buffer[256];
	::GetCurrentDirectory(256,buffer);

	TestGameApp TestGameApp("TestGameApp",FALSE,1024,768);
	if(TestGameApp.Open())
	{
		TestGameApp.Run();
	}
	TestGameApp.Close();
	
	HeapValidator::CheckMemory();
	return 0;
}