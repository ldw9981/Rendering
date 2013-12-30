#include "stdafx.h"
#include "ObjDragon.h"
#include "Scene/SceneNode.h"

cObjDragon::cObjDragon()
{	
	m_pDummy=NULL;
	m_bControl = FALSE;
}

cObjDragon::~cObjDragon(void)
{
	
}

void cObjDragon::Init()
{


}

void cObjDragon::Uninit()
{
//	UninitChildren();
}

void cObjDragon::Update(DWORD elapseTime)
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix();
	UpdateChildren(elapseTime);
}

void cObjDragon::Control()
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
		SetVelocityRotation(0.0f,10.0f,0.0f);
	}
	else if (m_pWinInput->IsCurrDn('D'))
	{
		SetVelocityRotation(0.0f,-10.0f,0.0f);
	}
	
}

void cObjDragon::Render()
{

}

