#include "StdAfx.h"
#include "ObjTank.h"
#include "Input/Input.h"

using namespace Sophia;

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



void cObjTank::Control(DWORD elapseTime)
{
	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (m_bControl)
	{
		if (g_pInput->IsCurrDn('W'))
		{
			vecPos.z = 200.0f;
		}
		else if (g_pInput->IsCurrDn('S'))
		{
			vecPos.z = -200.0f;
		}	
		if (g_pInput->IsCurrDn('A'))
		{
			vecPos.x = -200.0f;
		}
		else if (g_pInput->IsCurrDn('D'))
		{
			vecPos.x = 200.0f;
		}	
		if (g_pInput->IsCurrDn('R'))
		{
			vecPos.y = 200.0f;
		}
		else if (g_pInput->IsCurrDn('F'))
		{
			vecPos.y = -200.0f;
		}	


		if (g_pInput->IsCurrDn('Q'))
		{
			vecRot.y = -45.0f;
		}
		else if (g_pInput->IsCurrDn('E'))
		{
			vecRot.y = 45.0f;
		}


	}	

	//SetVelocityPosition(vecPos);	
	//SetVelocityRotation(vecRot);
}

void cObjTank::Render(DWORD elapseTime)
{

}