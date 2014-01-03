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



void cObjTank::Control()
{
	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (m_bControl)
	{
		if (m_pWinInput->IsCurrDn('W'))
		{
			vecPos.z = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn('S'))
		{
			vecPos.z = -200.0f;
		}	
		if (m_pWinInput->IsCurrDn('A'))
		{
			vecPos.x = -200.0f;
		}
		else if (m_pWinInput->IsCurrDn('D'))
		{
			vecPos.x = 200.0f;
		}	
		if (m_pWinInput->IsCurrDn('R'))
		{
			vecPos.y = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn('F'))
		{
			vecPos.y = -200.0f;
		}	


		if (m_pWinInput->IsCurrDn('Q'))
		{
			vecRot.y = -45.0f;
		}
		else if (m_pWinInput->IsCurrDn('E'))
		{
			vecRot.y = 45.0f;
		}


	}	

	SetVelocityPosition(vecPos);	
	SetVelocityRotation(vecRot);
}

void cObjTank::Render()
{

}