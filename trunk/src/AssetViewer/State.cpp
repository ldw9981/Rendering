#include "StdAfx.h"
#include "State.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)	

using namespace Sophia;

State::State(void)
{
	m_bControlCamera=FALSE;

	m_pZTerrain=NULL;

	m_pTank=NULL;
	m_pAirPlaneBake=NULL;
	m_pDragon=NULL;

	for (int i=0;i<10;i++)
	{
		m_pHouse[i] = NULL;
	}
	m_graphicWorld.SetViewPortInfo(0,0,1024,768);
}

State::~State( void )
{

}
void State::Enter()
{
	cView::Enter();
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 400.0f, -1500.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	/*
	m_pZTerrain->Create(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str()
		);
	AttachObject(m_pZTerrain);
	*/
		
	m_pTank = m_graphicWorld.CreateEntity();
	m_pTank->LoadASE(std::string(strDataPath+"TigerTank.ase").c_str());
	m_pTank->Build();
	m_pTank->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));
	
	
	m_pDragon = m_graphicWorld.CreateEntity();
	m_pDragon->LoadASE(std::string(strDataPath+"Dragon2.ase").c_str());
	m_pDragon->Build();
	//m_pDragon->GetVelRotPerSec().y = D3DXToRadian(-45);
	m_pDragon->SetLocalPos(D3DXVECTOR3(300.0f,200.0f,-100.0f));

	
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"AirPlaneBake.ase").c_str());
	m_pAirPlaneBake->Build();
	//m_pAirPlaneBake->Init();
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));

	
	
	for (int i=0;i<10;i++)
	{
		m_pHouse[i] = m_graphicWorld.CreateEntity();
		m_pHouse[i]->LoadScene(std::string(strDataPath+"dragon.scene").c_str());
		m_pHouse[i]->LoadAnimation(std::string(strDataPath+"dragon.animation").c_str());
		m_pHouse[i]->LoadMaterial(std::string(strDataPath+"dragon.material").c_str());
		m_pHouse[i]->Build();
		
		m_pHouse[i]->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
		m_pHouse[i]->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f+ i*300));
	}
	
	
	

}

void State::Leave()
{
	m_graphicWorld.DeleteTerrain(m_pZTerrain);
	//DettachObject(m_pZTerrain);
	//SAFE_DELETE(m_pZTerrain);


	for (int i=0;i<10;i++)
	{
		//DettachObject(m_pHouse[i]);
		//SAFE_DELETE(m_pHouse[i]);	
		m_graphicWorld.DeleteEntity(m_pHouse[i]);
	}



	//DettachObject(m_pTank);
	//SAFE_DELETE(m_pTank);	
	m_graphicWorld.DeleteEntity(m_pTank);

	//DettachObject(m_pAirPlaneBake);
	//SAFE_DELETE(m_pAirPlaneBake);
	m_graphicWorld.DeleteEntity(m_pAirPlaneBake);

	//DettachObject(m_pDragon);
	//SAFE_DELETE(m_pDragon);
	m_graphicWorld.DeleteEntity(m_pDragon);
	cView::Leave();
}


void State::Update( DWORD elapseTime )
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

void State::ProcessRender()
{
	cView::ProcessRender();	
}

void State::Control()
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


	if (g_pInput->IsTurnDn(DIK_F12))
	{
		Graphics::m_pInstance->m_bDebugBound = !Graphics::m_pInstance->m_bDebugBound;
	}

	if (g_pInput->IsTurnDn(DIK_F11))
	{
		m_pDragon->m_animationIndex ++;		
		if (m_pDragon->m_animationIndex >= (int)m_pDragon->m_vecAnimation.size() )
		{
			m_pDragon->m_animationIndex =0;			
		}
		m_pDragon->PlayAnimation(m_pDragon->m_animationIndex,true);
	}	

	if (g_pInput->IsTurnDn(DIK_F10))
	{
		std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
		m_pDragon->SaveScene(std::string(strDataPath+"dragon.scene").c_str());
		m_pDragon->SaveAnimation(std::string(strDataPath+"dragon.animation").c_str(),0);		
		m_pDragon->SaveMaterial(std::string(strDataPath+"dragon.material").c_str(),0);
	}	

	if (g_pInput->IsTurnDn(DIK_F9))
	{
		m_pDragon->CutAndPushEntityAnimation(0,0,1000,"Cut");
	}	

	
}