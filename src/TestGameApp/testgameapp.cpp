#include "StdAfx.h"
#include "TestGameApp.h"

#include "Resource/ResourceMng.h"
#include "D3D9Server/RscTexture.h"

#include "Scene/ViewMng.h"
#include "TestView.h"
#include "MenuView.h"
#include "Framework/D3DFramework.h"
#include "D3D9Server/Server.h"
#include "D3D9Server/MaterialEx.h"
#include "Foundation/HeapValidator.h"

using namespace D3D9;

TestGameApp::TestGameApp( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{

	
}

TestGameApp::~TestGameApp(void)
{
}

bool TestGameApp::Open()
{
	if(!cD3DFramework::Open())
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
	m_pD3D9Server->LoadHLSL(strHLSL.c_str());
	
	m_pView = new cMenuView;
	m_pView->SetViewPortInfo(0,0,GetRequestRectWidth(),GetRequestRectHeight());
	AttachObject(m_pView);
	m_pView->Enter();

	
	return true;
}

void TestGameApp::Close()
{
	m_pView->Leave();
	DettachObject(m_pView);
	SAFE_DELETE(m_pView);
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
	//HeapValidator::SetBreakAlloc(53612);	
	
	char buffer[256];
	::GetCurrentDirectory(256,buffer);
	OutputDebugString(buffer);

	TestGameApp TestGameApp("TestGameApp",false,1024,768);
	if(TestGameApp.Open())
	{
		TestGameApp.Run();
	}
	TestGameApp.Close();
	
	HeapValidator::CheckMemory();
	return 0;
}