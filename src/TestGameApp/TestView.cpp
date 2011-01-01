#include "StdAfx.h"
#include "TestView.h"
#include "Scene/ViewMng.h"
#include "ASEParser/ASEParser.h"
#include "Framework/D3DFramework.h"
#include "GUI/GUIButton.h"

#include "TestGameApp.h"
#include "Ground.h"
#include "ObjDragon.h"
#include "Ground.h"
#include "BackWall.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/EnvironmentVariable.h"

cTestView::cTestView(void)
:m_pDragon(NULL)
{
	SetViewPortInfo(0,0,1024,768);

	m_pDragon = new cObjDragon;
	cASEParser parser;

	string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	parser.Load(string(strDataPath+"Dragon2.ase").c_str(),m_pDragon);
	m_pDragon->BuildComposite();
	m_pDragon->Init();
	AttachObject(m_pDragon);
}

cTestView::~cTestView(void)
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);



}


void cTestView::Open(void* arg)
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pD3DFramework->GetRequestRectWidth(),(float)g_pD3DFramework->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		
}

void cTestView::Close()
{
	
}

void cTestView::Control()
{	
	if (m_pWinInput->IsTurnDn(VK_SPACE))
	{
		TestGameApp* p=(TestGameApp*)g_pD3DFramework;
			
		p->m_pGlobalScene->SetHide(!p->m_pGlobalScene->GetHide());
		
	}
	cView::Control();
}

void cTestView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cTestView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void cTestView::ProcessRender()
{
	cCameraNode::GetActiveCamera()->Render();
	cView::ProcessRender();
}