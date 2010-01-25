#include "GlobalScene.h"
#include "../GUI/GUIButton.h"
#include "../Scene/SceneMng.h"
#include "../Framework/App/App.h"
#include "../Foundation/Define.h"

#include "TestGameApp.h"


cGlobalScene::cGlobalScene(void)
{
	SetViewPortInfo(0,128,128,128+128);

	m_pBtNextScene = new cGUIButton(this);
	m_pBtNextScene->Create(L"../../export/ButtonNextScene.bmp");
	m_pBtNextScene->SetPos(0,0);
	AttachObject(m_pBtNextScene);

	m_pBtExit = new cGUIButton(this);
	m_pBtExit->Create(L"../../export/ButtonExit.bmp");
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
		/*
		if (Application::App::Instance()->GetSceneMng()->IsTopScene((Scene*)TestGameApp->m_pMenuScene))
		{
			Application::App::Instance()->GetSceneMng()->ChangeTopScene((Scene*)p->m_pTestScene);
		}
		else if (Application::App::Instance()->GetSceneMng()->IsTopScene((Scene*)p->m_pTestScene))
		{
			Application::App::Instance()->GetSceneMng()->ChangeTopScene((Scene*)p->m_pMenuScene);
		}
		*/
	}
	else if (pSource == m_pBtExit)
	{
		Application::App::Instance()->QuitLoop();
	}
}

void cGlobalScene::ProcessRender()
{
	Scene::ProcessRender();

}