#include "StdAfx.h"
#include "TestScene.h"
#include "./Scene/SceneMng.h"
#include "./ASEParser/ASEParser.h"
#include "./Framework/D3DFramework.h"
#include "./GUI/GUIButton.h"

#include "TestGameApp.h"
#include "Ground.h"
#include "ObjDragon.h"
#include "Ground.h"
#include "BackWall.h"
#include "TestScene.h"
#include "MenuScene.h"
#include "GlobalScene.h"

cTestScene::cTestScene(void)
:m_pDragon(NULL)
{
	SetViewPortInfo(0,0,1024,768);

	m_pDragon = new cObjDragon;
	cASEParser parser;
	parser.Load(L"Dragon2.ase",m_pDragon);
	m_pDragon->Init();
	AttachObject(m_pDragon);
}

cTestScene::~cTestScene(void)
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);



}


void cTestScene::Open(void* arg)
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pD3DFramework->GetRequestRectWidth(),(float)g_pD3DFramework->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 100.0f, -950.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		
}

void cTestScene::Close()
{
	
}

void cTestScene::Control()
{	
	if (m_pWinInput->IsTurnDn(VK_SPACE))
	{
		cTestGameApp* p=(cTestGameApp*)g_pD3DFramework;
			
		p->m_pGlobalScene->SetHide(!p->m_pGlobalScene->GetHide());
		
	}
	cScene::Control();
}

void cTestScene::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cTestScene::Update( DWORD elapseTime )
{
	cScene::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void cTestScene::ProcessRender()
{
	cCameraNode::GetActiveCamera()->Render();
	cScene::ProcessRender();
}