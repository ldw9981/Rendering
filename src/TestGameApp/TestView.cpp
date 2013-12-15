#include "StdAfx.h"
#include "TestView.h"
#include "Scene/ViewMng.h"
#include "ASEParser/ASEParser.h"
#include "GUI/GUIButton.h"
#include "TestGameApp.h"
#include "Ground.h"
#include "ObjDragon.h"
#include "Ground.h"
#include "BackWall.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/D3DFramework.h"
#include "Framework/EnvironmentVariable.h"
#include "ObjTank.h"

cTestView::cTestView(void)
:m_pDragon(NULL),m_pTank(NULL) ,m_pP38(NULL) ,m_pAirPlaneBake(NULL)
{
	SetViewPortInfo(0,0,1024,768);

	
	cASEParser parser;

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	/*
	m_pDragon = new cObjDragon;
	parser.Load(std::string(strDataPath+"Dragon2.ase").c_str(),m_pDragon);
	parser.Close();
	m_pDragon->BuildComposite();
	m_pDragon->Init();
	AttachObject(m_pDragon);	
	
	
	m_pTank = new cObjTank;	
	parser.Load(std::string(strDataPath+"TigerTank.ase").c_str(),m_pTank);
	parser.Close();
	m_pTank->BuildComposite();
	m_pTank->Init();
	AttachObject(m_pTank);
	*/
	m_pP38 = new cObjTank;	
	parser.Load(std::string(strDataPath+"Light Map.ase").c_str(),m_pP38);
	parser.Close();
	m_pP38->BuildComposite();
	m_pP38->Init();
	AttachObject(m_pP38);
		
	/*
	m_pAirPlaneBake = new cObjTank;	
	parser.Load(std::string(strDataPath+"AirPlaneBake.ase").c_str(),m_pAirPlaneBake);
	parser.Close();
	m_pAirPlaneBake->BuildComposite();
	m_pAirPlaneBake->Init();
	AttachObject(m_pAirPlaneBake);	
	*/
}

cTestView::~cTestView(void)
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);
	
	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);

	DettachObject(m_pP38);
	SAFE_DELETE(m_pP38);

	DettachObject(m_pAirPlaneBake);
	SAFE_DELETE(m_pAirPlaneBake);
}


void cTestView::Open(void* arg)
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pD3DFramework->GetRequestRectWidth(),(float)g_pD3DFramework->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,-1000.0f));

	m_bControlCamera=FALSE;
}

void cTestView::Close()
{
	
}

void cTestView::Control()
{	
	cView::Control();

	if (m_pWinInput->IsTurnDn(VK_SPACE))
	{
		TestGameApp* p=(TestGameApp*)g_pD3DFramework;
			
		p->m_pGlobalScene->SetHide(!p->m_pGlobalScene->GetHide());
		
	}
	
	

	if (m_pWinInput->IsTurnDn(VK_TAB))
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

	if (m_pWinInput->IsCurrDn('W'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityPosition(0.0f,0.0f,100.0f);
	}
	else if (m_pWinInput->IsCurrDn('S'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityPosition(0.0f,0.0f,-100.0f);
	}	


	if (m_pWinInput->IsCurrDn('A'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityRotation(0.0f,-45.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn('D'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityRotation(0.0f,45.0f,0.0f);
	}
	if (m_pWinInput->IsCurrDn('E'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityRotation(-45.0f,0.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn('C'))
	{
		cCameraNode::GetActiveCamera()->SetVelocityRotation(45.0f,0.0f,0.0f);
	}	
}

void cTestView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cTestView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void cTestView::ProcessRender()
{
	cCameraNode::GetActiveCamera()->Render();
	cView::ProcessRender();
}