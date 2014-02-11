#include "StdAfx.h"
#include "GlobalView.h"
#include "GUI/GUIButton.h"
#include "Framework/ViewMng.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "TestGameApp.h"



cGlobalView::cGlobalView(void)
{

}

cGlobalView::~cGlobalView(void)
{

}

void cGlobalView::Enter()
{
	assert(m_pParentView!=NULL);
	m_graphicWorld.SetViewPortInfo(m_pParentView->m_graphicWorld.m_ViewPortInfo.X+128,m_pParentView->m_graphicWorld.m_ViewPortInfo.Y+0
		,128*3,128);


	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	m_pBtNextScene = new cGUIButton((INotifiable*) g_pApp->GetView());
	m_pBtNextScene->Create(std::string(strDataPath+"ButtonNextScene.bmp").c_str());
	m_pBtNextScene->SetPos(256,0);
	AttachObject(m_pBtNextScene);

	m_pBtExit = new cGUIButton((INotifiable*)g_pApp->GetView());
	m_pBtExit->Create(std::string(strDataPath+"ButtonExit.bmp").c_str());
	m_pBtExit->SetPos(128,0+64);
	AttachObject(m_pBtExit);

	//	m_pFont = new cGUIFont;
	//	m_pFont->Create();
	//	m_pFont->SetColor(D3DXCOLOR(0.0,0.0,0.0,0.0));
}

void cGlobalView::Leave()
{

	DettachObject(m_pBtNextScene);
	SAFE_DELETE(m_pBtNextScene);

	DettachObject(m_pBtExit);
	SAFE_DELETE(m_pBtExit);
}



void cGlobalView::ProcessRender()
{	
	cView::ProcessRender();
}