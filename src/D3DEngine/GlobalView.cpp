#include "StdAfx.h"
#include "GlobalView.h"
#include "GUI/GUIButton.h"
#include "Scene/ViewMng.h"
#include "Framework/D3DFramework.h"
#include "Framework/EnvironmentVariable.h"
#include "TestGameApp.h"


cGlobalView::cGlobalView(void)
{
	SetViewPortInfo(0,0,128,128);


	string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	m_pBtNextScene = new cGUIButton((INotifiable*)this);
	m_pBtNextScene->Create(string(strDataPath+"ButtonNextScene.bmp").c_str());
	m_pBtNextScene->SetPos(0,0);
	AttachObject(m_pBtNextScene);

	m_pBtExit = new cGUIButton((INotifiable*)this);
	m_pBtExit->Create(string(strDataPath+"ButtonExit.bmp").c_str());
	m_pBtExit->SetPos(0,0+64);
	AttachObject(m_pBtExit);

//	m_pFont = new cGUIFont;
//	m_pFont->Create();
//	m_pFont->SetColor(D3DXCOLOR(0.0,0.0,0.0,0.0));

}

cGlobalView::~cGlobalView(void)
{
	DettachObject(m_pBtNextScene);
	SAFE_DELETE(m_pBtNextScene);

	DettachObject(m_pBtExit);
	SAFE_DELETE(m_pBtExit);
}

void cGlobalView::Open( void* arg )
{

}

void cGlobalView::Close()
{

}

void cGlobalView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{
	if (pSource== m_pBtNextScene)
	{
		/*
		cTestGameApp* p = (cTestGameApp*)g_pD3DFramework;

		if (g_pD3DFramework->GetSceneMng()->IsTopScene((cView*)p->m_pMenuScene))
		{
			g_pD3DFramework->GetSceneMng()->ChangeTopScene((cView*)p->m_pTestScene);
		}
		else if (g_pD3DFramework->GetSceneMng()->IsTopScene((cView*)p->m_pTestScene))
		{
			g_pD3DFramework->GetSceneMng()->ChangeTopScene((cView*)p->m_pMenuScene);
		}
		*/
	}
	else if (pSource == m_pBtExit)
	{
		g_pD3DFramework->QuitLoop();
	}
}

void cGlobalView::ProcessRender()
{
	cView::ProcessRender();

}