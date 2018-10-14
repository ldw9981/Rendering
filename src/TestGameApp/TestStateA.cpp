
#include "StdAfx.h"
#include "TestStateA.h"

#include "Framework/D3DFramework.h"
#include "TestGameApp.h"
#include "Framework/ViewMng.h"
#include "ObjTank.h"
#include "GUI/GUIButton.h"
#include "Framework/DebugInfoView.h"
#include "Foundation/EnvironmentVariable.h"
using namespace Sophia;
TestStateA::TestStateA(void)
	:m_pTank(NULL)
{

	m_bControlCamera=FALSE;

	m_graphicWorld.m_camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 100.0f, -950.0f),
		&D3DXVECTOR3(0.0f, -10.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	
	m_graphicWorld.m_camera.SetLocalPos(D3DXVECTOR3(0.0f,200.0f,-1000.0f));	
}

TestStateA::~TestStateA(void)
{

}

void TestStateA::Enter()
{
	cView::Enter();
	m_graphicWorld.SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());


	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str()
		);	

	m_pTank = m_graphicWorld.CreateEntity();
	m_pTank->LoadASE(std::string(strDataPath+"TigerTank.ase").c_str());
	m_pTank->Build();
}

void TestStateA::Leave()
{
	m_graphicWorld.DeleteTerrain(m_pZTerrain);
	m_graphicWorld.DeleteEntity(m_pTank);
	cView::Leave();
}

void TestStateA::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void TestStateA::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);

	D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
	m_pTank->GetWorldPosition(pos);

	if(!m_pZTerrain->GetHeight(pos.x,pos.z,pos.y))
	{
		printf("d");
	}


	pos.y+=50.0f;
	if (pos.y > 150.0f)
	{
		printf("d");
	}

	m_pTank->SetLocalPos(pos);


}

void TestStateA::ProcessRender(DWORD elapseTime)
{	
	cView::ProcessRender(elapseTime);		
}

void TestStateA::Control(DWORD elapseTime)
{
	if (g_pInput->IsTurnDn(DIK_TAB))
	{
		if(m_bControlCamera)
		{
			m_bControlCamera=FALSE;
//			if (m_pTank)
	//			m_pTank->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
//			if (m_pTank)
	//			m_pTank->m_bControl=FALSE;			
		}
	}

	if (!m_bControlCamera)
	{
		return;
	}

	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	if (g_pInput->IsCurrDn(DIK_W))
	{
		vecPos.z = 200.0f;
	}
	else if (g_pInput->IsCurrDn(DIK_S))
	{
		vecPos.z = -200.0f;
	}	
	if (g_pInput->IsCurrDn(DIK_A))
	{
		vecPos.x = -200.0f;
	}
	else if (g_pInput->IsCurrDn(DIK_D))
	{
		vecPos.x = 200.0f;
	}	
	//m_graphicWorld.m_camera.SetVelocityPosition(vecPos);	

	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (g_pInput->IsCurrDn(DIK_Q))
	{
		vecRot.y = -45.0f;
	}
	else if (g_pInput->IsCurrDn(DIK_E))
	{
		vecRot.y = 45.0f;
	}

	if (g_pInput->IsCurrDn(DIK_R))
	{
		vecRot.x = -45.0f;
	}
	else if (g_pInput->IsCurrDn(DIK_F))
	{
		vecRot.x = 45.0f;
	}	
	//m_graphicWorld.m_camera.SetVelocityRotation(vecRot);

}



