#include "StdAfx.h"
#include "ObjTank.h"

cObjTank::cObjTank(void)
{	
	m_bControl=false;
}

cObjTank::~cObjTank(void)
{

}


void cObjTank::Init()
{

}

void cObjTank::Uninit()
{

}


void cObjTank::Update( DWORD elapseTime )
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix();
	UpdateChildren(elapseTime);
}

void cObjTank::Control()
{
	if (!m_bControl)
	{
		return;
	}
	if (m_pWinInput->IsCurrDn('W'))
	{
		SetVelocityPosition(0.0f,0.0f,100.0f);
	}
	else if (m_pWinInput->IsCurrDn('S'))
	{
		SetVelocityPosition(0.0f,0.0f,-100.0f);
	}
	if (m_pWinInput->IsCurrDn('A'))
	{
		SetVelocityRotation(0.0f,-45.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn('D'))
	{
		SetVelocityRotation(0.0f,45.0f,0.0f);
	}
	if (m_pWinInput->IsCurrDn('E'))
	{
		SetVelocityRotation(-45.0f,0.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn('C'))
	{
		SetVelocityRotation(45.0f,0.0f,0.0f);
	}
}

void cObjTank::Render()
{

}