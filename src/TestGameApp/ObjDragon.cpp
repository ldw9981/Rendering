#include "stdafx.h"
#include "ObjDragon.h"
#include "Graphics/SceneNode.h"
#include "Input/Input.h"

using namespace Sophia;

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


void cObjDragon::Control()
{
	if (!m_bControl)
	{
		return;
	}

	if (g_pInput->IsCurrDn('W'))
	{
		SetVelocityPosition(0.0f,0.0f,100.0f);
	}
	else if (g_pInput->IsCurrDn('S'))
	{
		SetVelocityPosition(0.0f,0.0f,-100.0f);
	}	

	if (g_pInput->IsCurrDn('A'))
	{
		SetVelocityRotation(0.0f,10.0f,0.0f);
	}
	else if (g_pInput->IsCurrDn('D'))
	{
		SetVelocityRotation(0.0f,-10.0f,0.0f);
	}
	
}

void cObjDragon::Render(DWORD elapseTime)
{

}

