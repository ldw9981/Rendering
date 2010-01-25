#include "ObjDragon.h"
#include "../Scene/SceneGraphNode.h"
#include <assert.h>

cObjDragon::cObjDragon()
{
	CreateCullingSphere();
	m_pDummy=NULL;
}

cObjDragon::~cObjDragon(void)
{
	
}

void cObjDragon::Init()
{

//	GetModel()->SetMeshRenderType(cScene::ONLYSKINEDMESH);
//	GetModel()->SetActiveCamera(FALSE);
	InitChildren();

	
	m_pDummy=FindNode(wstring(L"Dummy01"));
	assert(m_pDummy!=NULL);
	m_pDummy->SetIsActiveAnimation(FALSE);
//	MoveOnLocal(0.0f,250.0f,0.0f);	
	
}

void cObjDragon::Uninit()
{
	UninitChildren();
}

void cObjDragon::Update(DWORD elapseTime)
{
	UpdateWorldTM();
	UpdateChildren(elapseTime);
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


	// �ڽ��� �຤�Ϳ� ũ�⸸ŭ �ڽ��� ��ġ�� �����Ѵ�.

	if (m_SharedWinInput.IsCurrDn('W'))
	{
		cz= 10.0f;
	}
	if (m_SharedWinInput.IsCurrDn('S'))
	{
		cz= -10.0f;	
	}
	if (m_SharedWinInput.IsCurrDn('Q'))
	{
		cx= -10.0f;
	}
	if (m_SharedWinInput.IsCurrDn('E'))
	{
		cx= 10.0f;	
	}
	if (m_SharedWinInput.IsCurrDn('R'))
	{
		cy= 10.0f;
	}
	if (m_SharedWinInput.IsCurrDn('F'))
	{
		cy= -10.0f;	
	}

	if (m_SharedWinInput.IsCurrDn('A'))
	{
		cay= -10.0f;
	}
	if (m_SharedWinInput.IsCurrDn('D'))
	{
		cay= 10.0f;
	}
	MoveOnLocal(cx,cy,cz);
	RotateOnLocal(cax,cay,0.0f);
	
}


