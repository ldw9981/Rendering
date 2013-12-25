
#include "StdAfx.h"
#include "TestStateA.h"
#include "ASEParser/ASEParser.h"
#include "Framework/D3DFramework.h"
#include "TestGameApp.h"
#include "Scene/ViewMng.h"
#include "ObjTank.h"
#include "GUI/GUIButton.h"
#include "Scene/DebugInfoView.h"
#include "Framework/EnvironmentVariable.h"

TestStateA::TestStateA(void)
	:m_pTank(NULL)
{

	m_bControlCamera=FALSE;

	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 100.0f, -950.0f),
		&D3DXVECTOR3(0.0f, -10.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	
	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,200.0f,-1000.0f));	
}

TestStateA::~TestStateA(void)
{

}

void TestStateA::Enter()
{
	SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());
	m_Camera.SetActive();


	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	m_pZTerrain = new ZTerrain;	

	m_pZTerrain->Create(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str()
		);
	AttachObject(m_pZTerrain);



	m_pTank = new cObjTank;
	cASEParser parser;
	//parser.Load(std::string(strDataPath+"03ik-joe.ASE").c_str(),m_pTank);
	parser.Load(std::string(strDataPath+"TigerTank.ase").c_str(),m_pTank);


	m_pTank->BuildComposite();
	m_pTank->Init();
	AttachObject(m_pTank);
}

void TestStateA::Leave()
{
	DettachObject(m_pZTerrain);
	SAFE_DELETE(m_pZTerrain);


	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);	
}

void TestStateA::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void TestStateA::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);

	D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
	m_pTank->GetWorldPos(pos);

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

void TestStateA::ProcessRender()
{
	cCameraNode* pActiveCamera=cCameraNode::GetActiveCamera();
	pActiveCamera->Render();

	
	cView::ProcessRender();	
	
}

void TestStateA::Control()
{
	if (m_pWinInput->IsTurnDn(VK_TAB))
	{
		if(m_bControlCamera)
		{
			m_bControlCamera=FALSE;
			if (m_pTank)
				m_pTank->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
			if (m_pTank)
				m_pTank->m_bControl=FALSE;			
		}
	}

	if (!m_bControlCamera)
	{
		return;
	}

	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	if (m_pWinInput->IsCurrDn('W'))
	{
		vecPos.z = 200.0f;
	}
	else if (m_pWinInput->IsCurrDn('S'))
	{
		vecPos.z = -200.0f;
	}	
	if (m_pWinInput->IsCurrDn('A'))
	{
		vecPos.x = -200.0f;
	}
	else if (m_pWinInput->IsCurrDn('D'))
	{
		vecPos.x = 200.0f;
	}	
	m_Camera.SetVelocityPosition(vecPos);	

	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (m_pWinInput->IsCurrDn('Q'))
	{
		vecRot.y = -45.0f;
	}
	else if (m_pWinInput->IsCurrDn('E'))
	{
		vecRot.y = 45.0f;
	}

	if (m_pWinInput->IsCurrDn('R'))
	{
		vecRot.x = -45.0f;
	}
	else if (m_pWinInput->IsCurrDn('F'))
	{
		vecRot.x = 45.0f;
	}	
	m_Camera.SetVelocityRotation(vecRot);

}



