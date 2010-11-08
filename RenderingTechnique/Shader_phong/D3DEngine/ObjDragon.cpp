#include "stdafx.h"
#include "ObjDragon.h"
#include "./ASEImporter/Scene.h"


cObjDragon::cObjDragon(cWorld* pWorld)
:cGameObject(pWorld)
{
	m_pDummy=NULL;
}

cObjDragon::~cObjDragon(void)
{
}

void cObjDragon::Init()
{
	assert(GetModel()!=NULL);
//	GetModel()->SetMeshRenderType(cScene::ONLYSKINEDMESH);
//	GetModel()->SetActiveCamera(FALSE);
	GetModel()->Init();

	
//	m_pDummy=GetModel()->FindSceneNode("Dummy01");
//	assert(m_pDummy!=NULL);
//	m_pDummy->SetIsActiveAnimation(FALSE);
//GetModel()->MovePositionByLocalAxis(0.0f,250.0f,0.0f);	
	
}

void cObjDragon::Uninit()
{
	assert(GetModel()!=NULL);
	GetModel()->Uninit();
	

}

void cObjDragon::Update()
{
	GetModel()->Update();
}

void cObjDragon::Render()
{
	GetModel()->Render();
}

void cObjDragon::OnCollide( cGameObject* pObj,cMeshNode* pMyMesh )
{
	
}

void cObjDragon::Control()
{
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
		cz= -10.0f;	
	}
	if (m_pWinInput->IsCurrDn('Q'))
	{
		cx= -10.0f;
	}
	if (m_pWinInput->IsCurrDn('E'))
	{
		cx= 10.0f;	
	}
	if (m_pWinInput->IsCurrDn('R'))
	{
		cy= 10.0f;
	}
	if (m_pWinInput->IsCurrDn('F'))
	{
		cy= -10.0f;	
	}

	if (m_pWinInput->IsCurrDn('A'))
	{
		cay= -10.0f;
	}
	if (m_pWinInput->IsCurrDn('D'))
	{
		cay= 10.0f;
	}
	GetModel()->MovePositionByLocalAxis(cx,cy,cz);
	GetModel()->RotateByLocalAxis(cax,cay,0.0f);
	
}