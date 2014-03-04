#include "StdAfx.h"
#include "State.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Graphics.h"
#include "Foundation/StringUtil.h"
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
	m_pModel = NULL;
	m_bModifiedAnimation = false;
	m_bModifiedScene = false;
	m_bModifiedMaterial = false;
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

	/*
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	*/
	/*
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"AirPlaneBake.ase").c_str());
	m_pAirPlaneBake->Build();
	//m_pAirPlaneBake->Init();
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));	
	*/
}

void State::Leave()
{
	/*
	m_graphicWorld.DeleteTerrain(m_pZTerrain);
	//DettachObject(m_pZTerrain);
	//SAFE_DELETE(m_pZTerrain);
	*/
	//m_graphicWorld.DeleteEntity(m_pAirPlaneBake);
	
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
	}
	cView::Leave();
}


void State::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);	
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

void State::OpenASE( const char* path )
{
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
		m_pModel = NULL;
	}
	m_pModel = m_graphicWorld.CreateEntity();
	m_pModel->LoadASE(path);
	m_pModel->Build();
	//m_pAirPlaneBake->Init();
	m_pModel->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pModel->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));
}

void State::SaveAsset()
{
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	m_pModel->SaveScene(std::string(strDataPath+m_pModel->GetNodeName()+".scene").c_str());
	if (m_bModifiedAnimation)
	{
		m_pModel->SaveAnimationSet(std::string(strDataPath+m_pModel->GetNodeName()+".aniset").c_str());		
		m_bModifiedAnimation = false;
	}	
	m_pModel->SaveMaterial(std::string(strDataPath+m_pModel->GetNodeName()+".material").c_str(),0);
}



void State::OpenAsset( const char* path )
{
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
		m_pModel = NULL;
	}
	m_pModel = m_graphicWorld.CreateEntity();

	std::string dir,name;
	StringUtil::SplitPath(std::string(path),NULL,&dir,&name,NULL);
	m_pModel->LoadScene(std::string(dir+name+".scene").c_str());
	m_pModel->LoadAnimationSet(std::string(dir+name+".aniset").c_str());
	m_pModel->LoadMaterial(std::string(dir+name+".material").c_str());
	m_pModel->Build();
	//m_pAirPlaneBake->Init();
	m_pModel->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pModel->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));
}

Sophia::Entity* State::GetEntity()
{
	return m_pModel;
}



