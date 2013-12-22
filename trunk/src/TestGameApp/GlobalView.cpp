#include "StdAfx.h"
#include "GlobalView.h"
#include "GUI/GUIButton.h"
#include "Scene/ViewMng.h"
#include "Framework/D3DFramework.h"
#include "Framework/EnvironmentVariable.h"
#include "TestGameApp.h"

#include "ASEParser/ASEParser.h"


cGlobalView::cGlobalView(void)
{

}

cGlobalView::~cGlobalView(void)
{

}

void cGlobalView::Enter()
{
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
	SetViewPortInfo(128,0,128*3,128);


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