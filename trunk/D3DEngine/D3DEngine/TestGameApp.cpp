#include "StdAfx.h"
#include "TestGameApp.h"

#include "./Resource/ResourceMng.h"
#include "./Resource/RscTexture.h"

#include "./Scene/SceneMng.h"
#include "TestScene.h"
#include "MenuScene.h"
#include "GlobalScene.h"

cTestGameApp::cTestGameApp( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{
	m_pMenuScene=NULL;
	m_pTestScene=NULL;
	m_pGlobalScene=NULL;
	
}

cTestGameApp::~cTestGameApp(void)
{
}

void cTestGameApp::OnInitApplication()
{
	m_pMenuScene = new cMenuScene;
	m_pTestScene = new cTestScene;
	m_pGlobalScene = new cGlobalScene;
	GetSceneMng()->ChangeTopScene((cScene*)m_pMenuScene);
	AttachObject(m_pGlobalScene);
	

	
}

void cTestGameApp::OnCloseApplication()
{
	SAFE_DELETE(m_pMenuScene);
	SAFE_DELETE(m_pTestScene);
	SAFE_DELETE(m_pGlobalScene);
}


BOOL cTestGameApp::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}


BOOL cTestGameApp::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL cTestGameApp::OnWM_General( MSG& msg )
{
	switch(msg.message)
	{
	case WM_ACTIVATEAPP:
		if (!msg.wParam)
		{
			//MessageBox(NULL,L"the window is being deactivated.",L"알림",MB_OK);
		}
		else
		{
			//MessageBox(NULL,L"the window is being activated.",L"알림",MB_OK);
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
	cTestGameApp TestGameApp(L"spatial partitioning",FALSE,1024,768);
	TestGameApp.Run();
	return 0;
}