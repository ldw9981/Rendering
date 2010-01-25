#include "TestGameApp.h"
#include "../Framework/App/App.h"
//#include "./Resource/ResourceMng.h"
//#include "./Resource/RscTexture.h"
#include "../Foundation/Define.h"
#include "../Scene/SceneMng.h"
#include "TestScene.h"
#include "MenuScene.h"
#include "GlobalScene.h"

namespace Application
{

TestGameApp::TestGameApp( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:App(szTitleName,bFullScreen,nWidth,nHeight)
{

	m_pMenuScene=NULL;
	m_pTestScene=NULL;
	m_pGlobalScene=NULL;

}

TestGameApp::~TestGameApp(void)
{
}

BOOL TestGameApp::Open()
{
	App::Open();

	m_pMenuScene = new cMenuScene;
	m_pTestScene = new TestScene;
	m_pGlobalScene = new cGlobalScene;
	GetSceneMng()->ChangeTopScene((Scene*)m_pMenuScene);

	AttachObject(m_pGlobalScene);
	

	return TRUE;
}

void TestGameApp::Close()
{
	App::Close();

	SAFE_DELETE(m_pMenuScene);
	SAFE_DELETE(m_pTestScene);
	SAFE_DELETE(m_pGlobalScene);

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







}

