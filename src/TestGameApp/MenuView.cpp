#include "StdAfx.h"
#include "MenuView.h"
#include "Framework/D3DFramework.h"
#include "TestGameApp.h"
#include "Framework/ViewMng.h"
#include "ObjTank.h"
#include "ObjDragon.h"
#include "GUI/GUIButton.h"
#include "Framework/DebugInfoView.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Graphics.h"
#include "Graphics/Entity.h"

using namespace Sophia;
#define PI           3.14159265f
#define FOV          (PI/4.0f)	


cMenuView::cMenuView(void)
{
	
	m_bControlCamera=FALSE;

	m_instancingNormal = true;
	m_instancingSkinned = false;
	m_pZTerrain=NULL;
	
	m_pTank=NULL;
	m_pAirPlaneBake=NULL;
	m_pDragon=NULL;

	for (int i=0;i<STRESS_NORMAL;i++)
	{
		m_pHouse[i] = NULL;
	}
	for (int i=0;i<STRESS_SKINNED;i++)
	{
		m_pSkinned[i]=NULL;
	}
	m_graphicWorld.SetViewPortInfo(0,0,1024,768);
	m_showNormal = true;
	m_showSkinned = true;
}

cMenuView::~cMenuView(void)
{
	
}

void cMenuView::Enter()
{
	cView::Enter();
	m_pInputTarget = &m_graphicWorld.m_camera;
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,100000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f,1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	
	m_graphicWorld.m_camera.TranslateLocal(0.0f, 300.0f, -2000.0f);

	m_graphicWorld.SetWorldLightDirection(D3DXVECTOR3(0.0f,-0.5f,1.0f));
	
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	
	
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	
	m_pTank = m_graphicWorld.CreateEntity();
	m_pTank->LoadScene(std::string(strDataPath + "TigerTank.scene").c_str());
	m_pTank->LoadAnimationSet(std::string(strDataPath + "TigerTank.aniset").c_str());
	m_pTank->LoadMaterial(std::string(strDataPath + "TigerTank.material").c_str());
	m_pTank->Build();
	m_pTank->SetLocalPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	m_pTank->RotateOnLocal(0, 180, 0);	
	

	m_pGirl = m_graphicWorld.CreateEntity();
	m_pGirl->LoadScene(std::string(strDataPath+"Beautiful Girl.scene").c_str());
	m_pGirl->LoadAnimationSet(std::string(strDataPath+"Beautiful Girl.aniset").c_str());
	m_pGirl->LoadMaterial(std::string(strDataPath+"Beautiful Girl.material").c_str());
	m_pGirl->Build();
	m_pGirl->SetLocalPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_pGirl->RotateOnLocal(0,180,0);
	

	
	m_pDragon = m_graphicWorld.CreateEntity();
	m_pDragon->LoadScene(std::string(strDataPath+"dragon.scene").c_str());
	m_pDragon->LoadAnimationSet(std::string(strDataPath+"dragon.aniset").c_str());
	m_pDragon->LoadMaterial(std::string(strDataPath+"dragon.material").c_str());
	m_pDragon->Build();
	m_pDragon->SetLocalPos(D3DXVECTOR3(0,0.0f,0));
	m_pDragon->RotateOnLocal(0,180,0);
	m_pDragon->PlayBaseAnimation(0,true);
	m_graphicWorld.DettachEntity(m_pTank);
	m_pDragon->AttachEntityAtBone("Bone-tail6", m_pTank);

	
	
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	/*
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"Dragon.ase").c_str());		
	*/
	m_pAirPlaneBake->LoadScene(std::string(strDataPath + "AirplaneBake.scene").c_str());
	m_pAirPlaneBake->LoadAnimationSet(std::string(strDataPath + "AirplaneBake.aniset").c_str());
	m_pAirPlaneBake->LoadMaterial(std::string(strDataPath + "AirplaneBake.material").c_str());
	m_pAirPlaneBake->Build();	
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));
	
	Graphics::m_pInstance->SetEntityInstancingMax("leaf",1024);
	Graphics::m_pInstance->SetEntityInstancingMax("Dragon",1024);

	
	for (int i=0;i<STRESS_NORMAL;i++)
	{
		m_pHouse[i] = m_graphicWorld.CreateEntity();
		m_pHouse[i]->LoadScene(std::string(strDataPath+"leaf.scene").c_str());
		m_pHouse[i]->LoadAnimationSet(std::string(strDataPath+"leaf.aniset").c_str());
		m_pHouse[i]->LoadMaterial(std::string(strDataPath+"leaf.material").c_str());
	

		m_pHouse[i]->Build();
		m_pHouse[i]->ChangeInstanceEnable(true);
//		m_pHouse[i]->PlayBaseAnimation(0);
		
		D3DXVECTOR3 pos;
		pos.x = i% 10 *200.0f ;		
		pos.z = i/10 *200.0f + 4000.0f;

		pos.y =  100;
		m_pHouse[i]->SetLocalPos(pos);		
	}
	
	
	for (int i=0;i<STRESS_SKINNED;i++)
	{
		m_pSkinned[i] = m_graphicWorld.CreateEntity();		
		m_pSkinned[i]->LoadScene(std::string(strDataPath+"Dragon.scene").c_str());
		m_pSkinned[i]->LoadAnimationSet(std::string(strDataPath+"Dragon.aniset").c_str());
		m_pSkinned[i]->LoadMaterial(std::string(strDataPath+"Dragon.material").c_str());
	
		m_pSkinned[i]->Build();
		m_pSkinned[i]->ChangeInstanceEnable(true);
		m_pSkinned[i]->PlayBaseAnimation(0,true,0,0,0, rand()%1000 );

		D3DXVECTOR3 pos;
		pos.x = i% 20 *250.0f - 1000.0f;		
		pos.z = i/20 *250.0f;

		pos.y =  100;
		m_pSkinned[i]->SetLocalPos(pos);		
		m_pSkinned[i]->RotateOnLocal(0, 180, 0);
	}
	
}

void cMenuView::Leave()
{	
	cView::Leave();
}

void cMenuView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cMenuView::ControlInputTarget(DWORD elapseTime)
{
}

void cMenuView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	
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

void cMenuView::ProcessRender(DWORD elapseTime)
{
	cView::ProcessRender(elapseTime);	
}

void cMenuView::Control(DWORD elapseTime)
{
	cView::Control(elapseTime);	
	
	if (g_pInput->IsTurnDn(DIK_TAB))
	{
		if (m_pInputTarget != m_pDragon)
		{
			m_pInputTarget = m_pDragon;
		}
		else
		{
			m_pInputTarget = &m_graphicWorld.m_camera;
		}
	}

	if (g_pInput->IsTurnDn(DIK_F4))
	{
		m_graphicWorld.m_bEnableShadow = !m_graphicWorld.m_bEnableShadow;
	}

	if (g_pInput->IsTurnDn(DIK_F5))
	{
		m_instancingNormal = !m_instancingNormal;
		for (int i=0;i<STRESS_NORMAL;i++)
		{
			if (m_pHouse[i])
			{
				m_pHouse[i]->ChangeInstanceEnable(m_instancingNormal);				
			}
		}
	}	

	if (g_pInput->IsTurnDn(DIK_F6))
	{
		m_instancingSkinned = !m_instancingSkinned;
		for (int i=0;i<STRESS_SKINNED;i++)
		{
			if (m_pSkinned[i])
			{
				m_pSkinned[i]->ChangeInstanceEnable(m_instancingSkinned);				
			}
		}
	}	

	if (g_pInput->IsTurnDn(DIK_F7))
	{
		m_showNormal = !m_showNormal;
		for (int i=0;i<STRESS_NORMAL;i++)
		{
			if (m_pHouse[i])
			{
				m_pHouse[i]->SetShow(m_showNormal);
			}
		}
	}
	if (g_pInput->IsTurnDn(DIK_F8))
	{
		m_showSkinned = !m_showSkinned;
		for (int i=0;i<STRESS_SKINNED;i++)
		{
			if (m_pSkinned[i])
			{
				m_pSkinned[i]->SetShow(m_showSkinned);
			}
		}		
	}
	
	float speed = 1.0f;
	D3DXVECTOR3 translation(0.0f,0.0f,0.0f);
	if (g_pInput->IsCurrDn(DIK_W) || g_pInput->IsCurrDn(DIK_UP))
	{
		translation.z = speed * elapseTime;
	}
	else if (g_pInput->IsCurrDn(DIK_S) || g_pInput->IsCurrDn(DIK_DOWN))
	{
		translation.z = -speed * elapseTime;
	}
	if (g_pInput->IsCurrDn(DIK_A) || g_pInput->IsCurrDn(DIK_LEFT))
	{
		translation.x = -speed * elapseTime;
	}
	else if (g_pInput->IsCurrDn(DIK_D) || g_pInput->IsCurrDn(DIK_RIGHT))
	{
		translation.x = speed * elapseTime;
	}
	if (g_pInput->IsCurrDn(DIK_Q))
	{
		translation.y = speed * elapseTime;
	}
	else if (g_pInput->IsCurrDn(DIK_E))
	{
		translation.y = -speed * elapseTime;
	}
	if (translation.x != 0.0f || translation.y != 0.0f || translation.z != 0.0f)
	{
		m_pInputTarget->TranslateLocal(translation.x, translation.y, translation.z);
	}

	D3DXVECTOR3 rotation(0.0f, 0.0f, 0.0f);
	if (g_pInput->GetMouseState().lY != 0)
	{
		rotation.x = (float)g_pInput->GetMouseState().lY  * (float)elapseTime ;
	}
	else if (g_pInput->GetMouseState().lX != 0)
	{
		rotation.y = (float)g_pInput->GetMouseState().lX * (float)elapseTime ;
	}
	if (rotation.x != 0.0f || rotation.y != 0.0f )
	{
		m_pInputTarget->RotateOnLocal(rotation.x*0.001f, rotation.y*0.001f, rotation.z);
	}	
}