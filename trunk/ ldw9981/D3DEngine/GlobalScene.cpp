#include "StdAfx.h"
#include "GlobalScene.h"
#include "./GUI/GUIButton.h"
#include "./Scene/SceneMng.h"
#include "./Framework/D3DFramework.h"

#include "TestGameApp.h"


cGlobalScene::cGlobalScene(void)
{
	SetViewPortInfo(0,0,128,128);

	m_pBtNextScene = new cGUIButton((cINotifiable*)this);
	m_pBtNextScene->Create(L"ButtonNextScene.bmp");
	m_pBtNextScene->SetPos(0,0);
	AttachObject(m_pBtNextScene);

	m_pBtExit = new cGUIButton((cINotifiable*)this);
	m_pBtExit->Create(L"ButtonExit.bmp");
	m_pBtExit->SetPos(0,0+64);
	AttachObject(m_pBtExit);

	
}

cGlobalScene::~cGlobalScene(void)
{
	DettachObject(m_pBtNextScene);
	SAFE_DELETE(m_pBtNextScene);

	DettachObject(m_pBtExit);
	SAFE_DELETE(m_pBtExit);
}

void cGlobalScene::Open( void* arg )
{

}

void cGlobalScene::Close()
{

}

void cGlobalScene::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{
	if (pSource== m_pBtNextScene)
	{
		cTestGameApp* p = (cTestGameApp*)g_pD3DFramework;

		if (g_pD3DFramework->GetSceneMng()->IsTopScene((cScene*)p->m_pMenuScene))
		{
			g_pD3DFramework->GetSceneMng()->ChangeTopScene((cScene*)p->m_pTestScene);
		}
		else if (g_pD3DFramework->GetSceneMng()->IsTopScene((cScene*)p->m_pTestScene))
		{
			g_pD3DFramework->GetSceneMng()->ChangeTopScene((cScene*)p->m_pMenuScene);
		}
	}
	else if (pSource == m_pBtExit)
	{
		g_pD3DFramework->QuitLoop();
	}
}

void cGlobalScene::ProcessRender()
{
	cScene::ProcessRender();

}