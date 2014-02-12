#include "StdAfx.h"
#include "TestView.h"
#include "Framework/ViewMng.h"

#include "GUI/GUIButton.h"
#include "TestGameApp.h"
#include "Ground.h"

#include "Ground.h"
#include "BackWall.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/D3DFramework.h"

#include "Scene/ZTerrain.h"
#include "TestView.h"
#include "TestStateA.h"
#include "TestStateB.h"
#include "GlobalView.h"
#include "ObjTank.h"
#include "Graphics/Graphics.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)	

cTestView::cTestView(void)
{
	m_pTestStateA=NULL;
	m_pTestStateB=NULL;
	m_pGlobalButtonScene=NULL;
	m_pP38=NULL;
}

cTestView::~cTestView(void)
{

}


void cTestView::Enter()
{
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 400.0f, -1500.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	m_pP38 = m_graphicWorld.CreateEntity();	
	m_pP38->LoadASE(std::string(strDataPath+"Light Map.ase").c_str());	
	m_pP38->Build();
	m_pP38->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pP38->SetLocalPos(D3DXVECTOR3(0.0f,300.0f,-100.0f));

	m_pTestStateA = new TestStateA;
	m_pTestStateA->SetParentView(this);	

	m_pTestStateB = new TestStateB;
	m_pTestStateB->SetParentView(this);
	
	
	m_pGlobalButtonScene = new cGlobalView;
	m_pGlobalButtonScene->SetParentView(this);
	m_pGlobalButtonScene->Enter();
	AttachObject(m_pGlobalButtonScene);		
}

void cTestView::Leave()
{
	m_pGlobalButtonScene->Leave();
	DettachObject(m_pGlobalButtonScene);
	SAFE_DELETE(m_pGlobalButtonScene);		

	cView* pView = GetState().GetCurr();
	if (pView)
	{
		pView->Leave();
	}
	SAFE_DELETE(m_pTestStateB);	
	SAFE_DELETE(m_pTestStateA);	

	if (m_pP38)
	{
		m_graphicWorld.DeleteEntity(m_pP38);
	}
}

void cTestView::Control()
{	
	cView::Control();

	if (g_pInput->IsTurnDn(DIK_SPACE))
	{
		TestGameApp* p=(TestGameApp*)g_pApp;
			
		m_pGlobalButtonScene->SetHide(m_pGlobalButtonScene->GetHide());
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
		Graphics::g_pGraphics->m_bDebugBound = !Graphics::g_pGraphics->m_bDebugBound;
	}
	
	m_graphicWorld.m_camera.Control();
}

void cTestView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{
	if (pSource== m_pGlobalButtonScene->m_pBtNextScene)
	{
		m_graphicWorld.m_camera.SetProcessInput(false);

		TestGameApp* p = (TestGameApp*)g_pApp;
		
		if (GetState().IsCurr(m_pTestStateB))
		{
			GetState().Transite(m_pTestStateA);
		}
		else 
		{
			if (m_pP38)
			{
				m_graphicWorld.DeleteEntity(m_pP38);
				m_pP38 = NULL;
			}			

			GetState().Transite(m_pTestStateB);
		}		
	}
	else if (pSource == m_pGlobalButtonScene->m_pBtExit)
	{
		g_pApp->QuitLoop();
	}
}

void cTestView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
}

void cTestView::ProcessRender()
{
	cView::ProcessRender();
}