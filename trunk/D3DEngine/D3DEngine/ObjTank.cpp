#include "StdAfx.h"
#include "ObjTank.h"

cObjTank::cObjTank(void)
{
	CreateCullingSphere();
	m_bControl=TRUE;
}

cObjTank::~cObjTank(void)
{
}

void cObjTank::Init()
{
	InitChildren();
}

void cObjTank::Uninit()
{
	UninitChildren();
}

void cObjTank::Update( DWORD elapseTime )
{
	UpdateWorldTM();
	UpdateChildren(elapseTime);
}

void cObjTank::Control()
{
	float x=0.0f,y=0.0f,z=0.0f;
	float ax=0.0f,ay=0.0f,az=0.0f;
	float cx=0.0f,cy=0.0f,cz=0.0f;
	float cax=0.0f,cay=0.0f;
	float cbax=0.0f,cbay=0.0f,cbaz=0.0f;
	float apax=0.0f,apay=0.0f,apaz=0.0f;
	D3DXMATRIX tempTM,tempRM,tempViewTM;


	if (!m_bControl)
	{
		return;
	}

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
	MoveOnLocal(cx,cy,cz);
	RotateOnLocal(cax,cay,0.0f);
}