#include "StdAfx.h"
#include "TestStateB.h"
#include "ASEParser/ASEParser.h"
#include "ObjDragon.h"
#include "Framework/EnvironmentVariable.h"
#include "ObjTank.h"
#include "TestGameApp.h"

TestStateB::TestStateB(void)
	:m_pDragon(NULL),m_pTank(NULL),m_pAirPlaneBake(NULL)
{
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,-1000.0f));	
}


TestStateB::~TestStateB(void)
{
}

void TestStateB::Enter()
{
	SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());
	m_Camera.SetActive();


	m_bControlCamera=FALSE;
	
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	cASEParser parser;

	
	m_pDragon = new cObjDragon;
	parser.Load(std::string(strDataPath+"Dragon2.ase").c_str(),m_pDragon);
	parser.Close();
	m_pDragon->BuildComposite();
	m_pDragon->Init();
	AttachObject(m_pDragon);	

	m_pAirPlaneBake = new cObjTank;	
	parser.Load(std::string(strDataPath+"AirPlaneBake.ase").c_str(),m_pAirPlaneBake);
	parser.Close();
	m_pAirPlaneBake->BuildComposite();
	m_pAirPlaneBake->Init();
	AttachObject(m_pAirPlaneBake);	
	
}

void TestStateB::Leave()
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);

	DettachObject(m_pAirPlaneBake);
	SAFE_DELETE(m_pAirPlaneBake);
}

void TestStateB::Control()
{
	if (m_pWinInput->IsTurnDn(DIK_TAB))
	{
		if(m_bControlCamera)
		{
			m_bControlCamera=FALSE;
			if (m_pAirPlaneBake)
				m_pAirPlaneBake->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
			if (m_pAirPlaneBake)
				m_pAirPlaneBake->m_bControl=FALSE;			
		}
	}

	if (!m_bControlCamera)
	{
		return;
	}

	if (m_pWinInput->IsCurrDn(DIK_W))
	{
		m_Camera.SetVelocityPosition(0.0f,0.0f,200.0f);
	}
	else if (m_pWinInput->IsCurrDn(DIK_S))
	{
		m_Camera.SetVelocityPosition(0.0f,0.0f,-200.0f);
	}	


	if (m_pWinInput->IsCurrDn(DIK_A))
	{
		m_Camera.SetVelocityRotation(0.0f,-45.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn(DIK_D))
	{
		m_Camera.SetVelocityRotation(0.0f,45.0f,0.0f);
	}
	if (m_pWinInput->IsCurrDn(DIK_E))
	{
		m_Camera.SetVelocityRotation(-45.0f,0.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn(DIK_C))
	{
		m_Camera.SetVelocityRotation(45.0f,0.0f,0.0f);
	}	
}

void TestStateB::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void TestStateB::ProcessRender()
{
	m_Camera.Render();
	cView::ProcessRender();
}
