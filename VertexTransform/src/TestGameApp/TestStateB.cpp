#include "StdAfx.h"
#include "TestStateB.h"

#include "ObjDragon.h"
#include "Foundation/EnvironmentVariable.h"
#include "ObjTank.h"
#include "TestGameApp.h"
#include "Input/Input.h"
using namespace Sophia;
TestStateB::TestStateB(void)
	:m_pDragon(NULL),m_pTank(NULL),m_pAirPlaneBake(NULL)
{
	m_graphicWorld.m_camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_graphicWorld.m_camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,-1000.0f));	
}


TestStateB::~TestStateB(void)
{
}

void TestStateB::Enter()
{
	cView::Enter();
	m_graphicWorld.SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());
	
	m_bControlCamera=FALSE;
	
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	
	m_pDragon = m_graphicWorld.CreateEntity();
	m_pDragon->LoadASE(std::string(strDataPath+"Dragon2.ase").c_str());
	m_pDragon->Build();	
	m_pDragon->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));

	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"AirPlaneBake.ase").c_str());
	m_pAirPlaneBake->Build();
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));

	
	
}

void TestStateB::Leave()
{
	m_graphicWorld.DeleteEntity(m_pDragon);	
	m_graphicWorld.DeleteEntity(m_pAirPlaneBake);
	cView::Leave();
}

void TestStateB::Control()
{
	if (g_pInput->IsTurnDn(DIK_TAB))
	{
		if(m_bControlCamera)
		{
			m_bControlCamera=FALSE;
	//		if (m_pAirPlaneBake)
//				m_pAirPlaneBake->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
	//		if (m_pAirPlaneBake)
		//		m_pAirPlaneBake->m_bControl=FALSE;			
		}
	}

	if (!m_bControlCamera)
	{
		return;
	}

	if (g_pInput->IsCurrDn(DIK_W))
	{
		m_graphicWorld.m_camera.SetVelocityPosition(0.0f,0.0f,200.0f);
	}
	else if (g_pInput->IsCurrDn(DIK_S))
	{
		m_graphicWorld.m_camera.SetVelocityPosition(0.0f,0.0f,-200.0f);
	}	


	if (g_pInput->IsCurrDn(DIK_A))
	{
		m_graphicWorld.m_camera.SetVelocityRotation(0.0f,-45.0f,0.0f);
	}
	else if (g_pInput->IsCurrDn(DIK_D))
	{
		m_graphicWorld.m_camera.SetVelocityRotation(0.0f,45.0f,0.0f);
	}
	if (g_pInput->IsCurrDn(DIK_E))
	{
		m_graphicWorld.m_camera.SetVelocityRotation(-45.0f,0.0f,0.0f);
	}
	else if (g_pInput->IsCurrDn(DIK_C))
	{
		m_graphicWorld.m_camera.SetVelocityRotation(45.0f,0.0f,0.0f);
	}	
}

void TestStateB::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
}

void TestStateB::ProcessRender()
{
	cView::ProcessRender();
}
