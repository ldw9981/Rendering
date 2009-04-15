#include "StdAfx.h"
#include "MenuScene.h"
#include "./ASEParser/ASEParser.h"
#include "./Framework/D3DFramework.h"
#include "./Scene/SceneMng.h"
#include "./GUI/GUIButton.h"
#include "./Log/DebugInfoScene.h"
#include "TestGameApp.h"
#include "ObjTank.h"

cMenuScene::cMenuScene(void)
:m_pTank(NULL),m_pcTerrainNode(NULL)
{
	SetViewPortInfo(0,0,1024,768);
	m_bControlCamera=FALSE;


	LPCTSTR test=_T("DDDD");
	TCHAR sss[100]={0,};
	wcscat_s(sss,100,test);
 
 	m_pcTerrainNode = new cTerrainNode;
	
	D3DXVECTOR3 scale(20.0f,0.8f,20.0f);
 	m_pcTerrainNode->Create(&scale,_T("map129.bmp"),_T("ground.bmp"));
 	AttachObject(m_pcTerrainNode);


 	m_pTank = new cObjTank;
 	cASEParser parser;
 	parser.Load(L"tigertank.ase",m_pTank);
 	m_pTank->Init();
 	AttachObject(m_pTank);

}

cMenuScene::~cMenuScene(void)
{
 	DettachObject(m_pcTerrainNode);
 	SAFE_DELETE(m_pcTerrainNode);

	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);	


}

void cMenuScene::Open( void* arg )
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pD3DFramework->GetRequestRectWidth(),(float)g_pD3DFramework->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 400.0f, -2000.0f),
		&D3DXVECTOR3(0.0f, 200.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

}

void cMenuScene::Close()
{

}

void cMenuScene::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cMenuScene::Update( DWORD elapseTime )
{
	cScene::Update(elapseTime);
	m_Camera.Update(elapseTime);

 	D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
 	pos=m_pTank->GetWorldPos();
 	if(m_pcTerrainNode->GetHeight(pos.x,pos.z,pos.y))
 	{
		pos.y+=50.0f;
		if (pos.y > 150.0f)
		{
			printf("d");
		}

		m_pTank->SetLocalPos(pos);
 	}	
}

void cMenuScene::ProcessRender()
{
	cCameraNode* pActiveCamera=cCameraNode::GetActiveCamera();
	pActiveCamera->Render();

	cScene::ProcessRender();

	g_pD3DFramework->GetDebugInfoScene()->AddDebugString(L"이동 wasd qe rf  카메라<->탱크 tab");
}

void cMenuScene::Control()
{
	cScene::Control();
	
	if (m_pWinInput->IsTurnDn(VK_TAB))
	{
		if(m_bControlCamera)
		{
			m_bControlCamera=FALSE;
			m_pTank->m_bControl=TRUE;
		}
		else
		{
			m_bControlCamera=TRUE;
			m_pTank->m_bControl=FALSE;			
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
		cz= 5.0f;
	}
	if (m_pWinInput->IsCurrDn('S'))
	{
		cz= -5.0f;	
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
		cay= -2.0f;
	}
	if (m_pWinInput->IsCurrDn('D'))
	{
		cay= 2.0f;
	}
	cCameraNode::GetActiveCamera()->MoveOnLocal(cx,cy,cz);
	cCameraNode::GetActiveCamera()->RotateOnLocal(cax,cay,0.0f);

}