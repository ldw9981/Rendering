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
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,30000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 1500.0f, -2500.0f),
		&D3DXVECTOR3(0.0f, 0.0f,2000.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	
	m_graphicWorld.SetWorldLightDirection(D3DXVECTOR3(0.0f,-0.5f,1.0f));

	
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	
	
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	
	
	

	m_pTank = m_graphicWorld.CreateEntity();
	m_pTank->LoadScene(std::string(strDataPath+"Beautiful Girl.scene").c_str());
	m_pTank->LoadAnimationSet(std::string(strDataPath+"Beautiful Girl.aniset").c_str());
	m_pTank->LoadMaterial(std::string(strDataPath+"Beautiful Girl.material").c_str());
	m_pTank->Build();
	m_pTank->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));
	m_pTank->RotateOnLocal(0,180,0);
	

	
	m_pDragon = m_graphicWorld.CreateEntity();
	m_pDragon->LoadScene(std::string(strDataPath+"dragon.scene").c_str());
	m_pDragon->LoadAnimationSet(std::string(strDataPath+"dragon.aniset").c_str());
	m_pDragon->LoadMaterial(std::string(strDataPath+"dragon.material").c_str());
	m_pDragon->Build();
	m_pDragon->SetLocalPos(D3DXVECTOR3(600,200.0f,0));
	m_pDragon->RotateOnLocal(0,180,0);
	m_pDragon->PlayBaseAnimation(0,true);
	

	
	
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	/*
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"Dragon.ase").c_str());		
	*/
	m_pAirPlaneBake->LoadScene(std::string(strDataPath + "dragon.scene").c_str());
	m_pAirPlaneBake->LoadAnimationSet(std::string(strDataPath + "dragon.aniset").c_str());
	m_pAirPlaneBake->LoadMaterial(std::string(strDataPath + "dragon.material").c_str());
	m_pAirPlaneBake->Build();
	m_pAirPlaneBake->ChangeInstanceEnable(true);

	
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
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
		pos.x = i% 10 *200.0f - 1000.0f;		
		pos.z = i/10 *200.0f; 

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
		m_pSkinned[i]->PlayBaseAnimation(0,true);

		D3DXVECTOR3 pos;
		pos.x = i% 10 *200.0f - 1000.0f;		
		pos.z = i/10 *200.0f;

		pos.y =  100;
		m_pSkinned[i]->SetLocalPos(pos);		
	}
	
}

void cMenuView::Leave()
{
	m_graphicWorld.DeleteTerrain(m_pZTerrain);
	
	for (int i=0;i<STRESS_NORMAL;i++)
	{
		if (m_pHouse[i])
			m_graphicWorld.DeleteEntity(m_pHouse[i]);
	}

	for (int i=0;i<STRESS_SKINNED;i++)
	{
		if (m_pSkinned[i])
			m_graphicWorld.DeleteEntity(m_pSkinned[i]);
	}
	
	if (m_pTank)
		m_graphicWorld.DeleteEntity(m_pTank);
	
	if (m_pDragon)
		m_graphicWorld.DeleteEntity(m_pDragon);
	
	if (m_pAirPlaneBake)
		m_graphicWorld.DeleteEntity(m_pAirPlaneBake);

	
	
	cView::Leave();
}

void cMenuView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
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

void cMenuView::ProcessRender()
{
	cView::ProcessRender();	
}

void cMenuView::Control()
{
	cView::Control();	
	
	if (g_pInput->IsTurnDn(DIK_TAB))
	{
		if(m_graphicWorld.m_camera.GetProcessInput())
		{
			m_graphicWorld.m_camera.SetProcessInput(false);
		//	if(m_pTank)
		//		m_pTank->m_bControl=TRUE;
		}
		else
		{
			m_graphicWorld.m_camera.SetProcessInput(true);
		//	if(m_pTank)
		//		m_pTank->m_bControl=FALSE;			
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
	
	if (g_pInput->GetMouseState().lZ != 0)
	{

		int iz = *(int*)&g_pInput->GetMouseState().lZ;
		iz /= abs(iz);
		D3DXVECTOR3 temp;
		m_graphicWorld.m_camera.GetLocalPos(temp);
		temp.z += (float)(iz) * 100;		
		m_graphicWorld.m_camera.SetLocalPos(temp);
	}
}