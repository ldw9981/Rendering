#include "StdAfx.h"
#include "MenuView.h"
#include "ASEParser/ASEParser.h"
#include "Framework/D3DFramework.h"
#include "TestGameApp.h"
#include "Scene/ViewMng.h"
#include "ObjTank.h"
#include "ObjDragon.h"
#include "GUI/GUIButton.h"
#include "Scene/DebugInfoView.h"
#include "Framework/EnvironmentVariable.h"
#include "Graphics/Graphics.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)	

cMenuView::cMenuView(void)
{
	SetViewPortInfo(0,0,1024,768);
	m_bControlCamera=FALSE;

	m_pZTerrain=NULL;
	m_pHouse=NULL;
	m_pTank=NULL;
	m_pAirPlaneBake=NULL;
	m_pDragon=NULL;


}

cMenuView::~cMenuView(void)
{

}

void cMenuView::Enter()
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(FOV,1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 400.0f, -1500.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	m_pZTerrain = new ZTerrain;	

	m_pZTerrain->Create(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str()
		);
	AttachObject(m_pZTerrain);


	cASEParser parser;

	
	m_pTank = new cObjTank;
	//parser.Load(std::string(strDataPath+"03ik-joe.ASE").c_str(),m_pTank);
	parser.Load(std::string(strDataPath+"TigerTank.ase").c_str(),m_pTank);
	parser.Close();
	m_pTank->BuildComposite();
	m_pTank->Init();
	m_pTank->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));
	AttachObject(m_pTank);
	
	
	m_pDragon = new cObjDragon;
	parser.Load(std::string(strDataPath+"Dragon2.ase").c_str(),m_pDragon);
	parser.Close();
	m_pDragon->BuildComposite();
	m_pDragon->Init();
	//m_pDragon->GetVelRotPerSec().y = D3DXToRadian(-45);
	m_pDragon->SetLocalPos(D3DXVECTOR3(300.0f,200.0f,-100.0f));
	AttachObject(m_pDragon);	
	

	
	m_pAirPlaneBake = new cObjDragon;	
	parser.Load(std::string(strDataPath+"AirPlaneBake.ase").c_str(),m_pAirPlaneBake);
	parser.Close();
	m_pAirPlaneBake->BuildComposite();
	m_pAirPlaneBake->Init();
	m_pAirPlaneBake->GetVelRotPerSec().y = D3DXToRadian(-45);
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));
	AttachObject(m_pAirPlaneBake);	
	
	
	
	m_pHouse = new cObjDragon;	
	parser.Load(std::string(strDataPath+"Dragon2.ase").c_str(),m_pHouse);
	parser.Close();
	m_pHouse->BuildComposite();
	m_pHouse->Init();
	m_pHouse->GetVelRotPerSec().y = D3DXToRadian(-45);
	m_pHouse->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));
	AttachObject(m_pHouse);
	
	
	
}

void cMenuView::Leave()
{
	DettachObject(m_pZTerrain);
	SAFE_DELETE(m_pZTerrain);

	DettachObject(m_pHouse);
	SAFE_DELETE(m_pHouse);	

	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);	

	DettachObject(m_pAirPlaneBake);
	SAFE_DELETE(m_pAirPlaneBake);

	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);

}

void cMenuView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cMenuView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
	
	/*
	if (m_pTank)
	{
		D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
		m_pTank->GetWorldPos(pos);

		if(!m_pZTerrain->GetHeight(pos.x,pos.z,pos.y))
		{
			printf("d");
		}
		pos.y+=50.0f;
		m_pTank->SetLocalPos(pos);
	}
	*/
	
}

void cMenuView::ProcessRender()
{
	cCameraNode* pActiveCamera=cCameraNode::GetActiveCamera();
	pActiveCamera->Render();
	cView::ProcessRender();	
}

void cMenuView::Control()
{
	cView::Control();
	cCameraNode* pActiveCamera=cCameraNode::GetActiveCamera();
	pActiveCamera->Control();
	
	if (g_pInput->IsTurnDn(DIK_TAB))
	{
		if(m_Camera.GetProcessInput())
		{
			m_Camera.SetProcessInput(false);
			m_pTank->m_bControl=TRUE;
		}
		else
		{
			m_Camera.SetProcessInput(true);
			m_pTank->m_bControl=FALSE;			
		}
	}
	
	if (g_pInput->IsTurnDn(DIK_EQUALS))
	{
		Graphics::g_pGraphics->m_WorldLightPosition.y += 50;

	}

	if (g_pInput->IsTurnDn(DIK_MINUS))
	{
		Graphics::g_pGraphics->m_WorldLightPosition.y -= 50;
	}


	if (g_pInput->IsTurnDn(DIK_F12))
	{
		Graphics::g_pGraphics->m_bDebugBound = !Graphics::g_pGraphics->m_bDebugBound;
	}
			
}