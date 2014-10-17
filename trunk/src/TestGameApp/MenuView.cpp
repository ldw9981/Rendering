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

using namespace Sophia;
#define PI           3.14159265f
#define FOV          (PI/4.0f)	


cMenuView::cMenuView(void)
{
	
	m_bControlCamera=FALSE;
	m_instancing = true;
	m_pZTerrain=NULL;
	
	m_pTank=NULL;
	m_pAirPlaneBake=NULL;
	m_pDragon=NULL;

	for (int i=0;i<STRESS;i++)
	{
		m_pHouse[i] = NULL;
	}
	m_graphicWorld.SetViewPortInfo(0,0,1024,768);
}

cMenuView::~cMenuView(void)
{
	
}

void cMenuView::Enter()
{
	cView::Enter();
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 1500.0f, -2000.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	
	/*
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	*/
	
	/*
	m_pTank = m_graphicWorld.CreateEntity();
	m_pTank->LoadScene(std::string(strDataPath+"Beautiful Girl.scene").c_str());
	m_pTank->LoadAnimationSet(std::string(strDataPath+"Beautiful Girl.aniset").c_str());
	m_pTank->LoadMaterial(std::string(strDataPath+"Beautiful Girl.material").c_str());
	m_pTank->Build();
	m_pTank->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));
	m_pTank->RotateOnLocal(0,180,0);
	*/

	/*
	m_pDragon = m_graphicWorld.CreateEntity();
	m_pDragon->LoadScene(std::string(strDataPath+"dragon.scene").c_str());
	m_pDragon->LoadAnimationSet(std::string(strDataPath+"dragon.aniset").c_str());
	m_pDragon->LoadMaterial(std::string(strDataPath+"dragon.material").c_str());
	m_pDragon->Build();
	m_pDragon->SetLocalPos(D3DXVECTOR3(600,200.0f,0));
	m_pDragon->RotateOnLocal(0,180,0);
	m_pDragon->PlayBaseAnimation(0,true);
	*/

	/*
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"AirPlaneBake.ase").c_str());
	m_pAirPlaneBake->Build();
	
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));
	*/
	
	
	for (int i=0;i<STRESS;i++)
	{
		m_pHouse[i] = m_graphicWorld.CreateEntity();
		if (i%2 == 0)
		{
			m_pHouse[i]->LoadScene(std::string(strDataPath+"dragon.scene").c_str());
			m_pHouse[i]->LoadAnimationSet(std::string(strDataPath+"dragon.aniset").c_str());
			m_pHouse[i]->LoadMaterial(std::string(strDataPath+"dragon.material").c_str());
		}
		else
		{
			m_pHouse[i]->LoadScene(std::string(strDataPath+"leaf.scene").c_str());
			m_pHouse[i]->LoadAnimationSet(std::string(strDataPath+"leaf.aniset").c_str());
			m_pHouse[i]->LoadMaterial(std::string(strDataPath+"leaf.material").c_str());
		}		

		m_pHouse[i]->Build();
		m_pHouse[i]->SetInstanceEnable(m_instancing);
		m_pHouse[i]->PlayBaseAnimation(0,true,0,0,0,rand()%1000);
		
		D3DXVECTOR3 pos;
		pos.x = i% 10 *200.0f - 1000.0f;		
		pos.z = i/10 *200.0f - 400.0f;

		pos.y =  100;
		m_pHouse[i]->SetLocalPos(pos);
		
	}
	
	
	

}

void cMenuView::Leave()
{
	//m_graphicWorld.DeleteTerrain(m_pZTerrain);



	for (int i=0;i<STRESS;i++)
	{
		if (m_pHouse[i])
			m_graphicWorld.DeleteEntity(m_pHouse[i]);
	}
	
	if (m_pTank)
		m_graphicWorld.DeleteEntity(m_pTank);
	
	if (m_pDragon)
		m_graphicWorld.DeleteEntity(m_pDragon);
	//m_graphicWorld.DeleteEntity(m_pAirPlaneBake);

	
	
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
	
	if (g_pInput->IsTurnDn(DIK_EQUALS))
	{
		m_graphicWorld.m_WorldLightPosition.y += 50;

	}

	if (g_pInput->IsTurnDn(DIK_MINUS))
	{
		m_graphicWorld.m_WorldLightPosition.y -= 50;
	}

	

	if (g_pInput->IsTurnDn(DIK_F11))
	{
		m_instancing = !m_instancing;
		for (int i=0;i<STRESS;i++)
		{
			
			m_pHouse[i]->SetInstanceEnable(m_instancing);
			m_pHouse[i]->ResetRenderQueue();

		}
	}	
	if (g_pInput->IsTurnDn(DIK_F5))
	{
		//Graphics::m_pInstance->m_bDebugBound = !Graphics::m_pInstance->m_bDebugBound;
		m_graphicWorld.m_bEnableShadow = !m_graphicWorld.m_bEnableShadow;
	}
	
	
}