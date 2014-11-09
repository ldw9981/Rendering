#include "StdAfx.h"
#include "GlobalView.h"
#include "GUI/GUIButton.h"
#include "Framework/ViewMng.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "TestGameApp.h"


using namespace Sophia;

cGlobalView::cGlobalView(void)
{

}

cGlobalView::~cGlobalView(void)
{

}

void cGlobalView::Enter()
{
	cView::Enter();
	assert(m_pParentView!=NULL);
	m_graphicWorld.SetViewPortInfo(m_pParentView->m_graphicWorld.m_ViewPortInfo.X+128,m_pParentView->m_graphicWorld.m_ViewPortInfo.Y+0
		,128*3,128);


	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	m_pBtNextScene = m_graphicWorld.CreateButton(std::string(strDataPath+"ButtonNextScene.bmp").c_str());
	m_pBtNextScene->SetMediator((INotifiable*) g_pApp->GetView());
	m_pBtNextScene->SetPos(256,0);

	m_pBtExit = m_graphicWorld.CreateButton(std::string(strDataPath+"ButtonExit.bmp").c_str());
	m_pBtExit->SetMediator((INotifiable*) g_pApp->GetView());
	m_pBtExit->SetPos(128,0+64);
}

void cGlobalView::Leave()
{
	m_graphicWorld.DeleteButton(m_pBtNextScene);
	m_graphicWorld.DeleteButton(m_pBtExit);
	cView::Leave();
}



void cGlobalView::ProcessRender()
{	
	cView::ProcessRender();
}