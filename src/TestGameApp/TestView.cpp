#include "StdAfx.h"
#include "TestView.h"
#include "Scene/ViewMng.h"
#include "ASEParser/ASEParser.h"
#include "Framework/D3DFramework.h"
#include "GUI/GUIButton.h"

#include "TestGameApp.h"
#include "Ground.h"
#include "ObjDragon.h"
#include "Ground.h"
#include "BackWall.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/EnvironmentVariable.h"
#include "ObjTank.h"

cTestView::cTestView(void)
:m_pDragon(NULL)
{
	SetViewPortInfo(0,0,1024,768);

	m_pDragon = new cObjDragon;
	cASEParser parser;

	string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	parser.Load(string(strDataPath+"Dragon2.ase").c_str(),m_pDragon);
	parser.Close();
	m_pDragon->BuildComposite();
	m_pDragon->Init();
	AttachObject(m_pDragon);
	


	m_pTank = new cObjTank;	
	parser.Load(string(strDataPath+"TigerTank.ase").c_str(),m_pTank);
	parser.Close();
	m_pTank->BuildComposite();
	m_pTank->Init();
	AttachObject(m_pTank);

	m_pP38 = new cObjTank;	
	parser.Load(string(strDataPath+"p38.ase").c_str(),m_pP38);
	parser.Close();
	m_pP38->BuildComposite();
	m_pP38->Init();
	AttachObject(m_pP38);
}

cTestView::~cTestView(void)
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);

	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);

	DettachObject(m_pP38);
	SAFE_DELETE(m_pP38);

}


void cTestView::Open(void* arg)
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pD3DFramework->GetRequestRectWidth(),(float)g_pD3DFramework->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,100.0f));

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
			m_pDragon->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
			m_pDragon->m_bControl=FALSE;			
		}
	}

	if (!m_bControlCamera)
	{
		return;
	}
	float x=0.0f,y=0.0f,z=0.0f;
	float ax=0.0f,ay=0.0f,az=0.0f;
	float cx=0.0f,cy=0.0f,cz=0.0f;
	float cax=0.0f,cay=0.0f;
	float cbax=0.0f,cbay=0.0f,cbaz=0.0f;
	float apax=0.0f,apay=0.0f,apaz=0.0f;
	D3DXMATRIX tempTM,tempRM,tempViewTM;


	// 자신의 축벡터에 크기만큼 자신의 위치를 변경한다.

	if (m_pWinInput->IsCurrDn('W'))
	{
		cz= 10.0f;
	}
	if (m_pWinInput->IsCurrDn('S'))
	{
		cz= -10.1f;	
	}
	if (m_pWinInput->IsCurrDn('Q'))
	{
		cx= -5.0f;
	}
	if (m_pWinInput->IsCurrDn('E'))
	{
		cx= 5.0f;	
	}
	if (m_pWinInput->IsCurrDn('R'))
	{
		cy= 5.0f;
	}
	if (m_pWinInput->IsCurrDn('F'))
	{
		cy= -5.0f;	
	}

	if (m_pWinInput->IsCurrDn('A'))
	{
		cay= -0.1f;
	}
	if (m_pWinInput->IsCurrDn('D'))
	{
		cay= 0.1f;
	}

	if ((cx != 0.0f)||(cy != 0.0f)||(cz != 0.0f))
	{
		cCameraNode::GetActiveCamera()->MoveOnLocal(cx,cy,cz);
	}

	if ((cax != 0.0f)||(cay != 0.0f))
	{	
		cCameraNode::GetActiveCamera()->RotateOnLocal(cax,cay,0.0f);
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