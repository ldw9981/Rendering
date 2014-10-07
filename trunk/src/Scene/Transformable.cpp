#include "StdAfx.h"
#include "Transformable.h"
#include "Scene/SceneNode.h"

namespace Sophia
{


cTransformable::cTransformable( void )
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matVelocityPos);
	D3DXMatrixIdentity(&m_matVelocityRot);

//	m_bModifiedMatLocal = TRUE;
//	m_bModifiedMatWorld = TRUE;
	m_velPosPerSec = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_velRotPerSec = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_bNoVelocityPos = true;
	m_bNoVelocityRot = true;
}

cTransformable::~cTransformable( void )
{

}
/*
	이전과 같으면 FALSE
	갱신된것이면 TRUE
*/
void cTransformable::UpdateWorldMatrix(cTransformable* pParent/*=NULL*/)
{		
	if (pParent==NULL)	
	{
		m_matWorld = m_matLocal;		// 부모가 없으면 LocalTM이 WorldTM이 된다.					
	}
	else
	{
		m_matWorld = m_matLocal * pParent->m_matWorld;	// 부모가 있으면 WorldTM은 LocalTM * GetWorldTM 이 된다.
	}
}


void cTransformable::SetLocalPos( D3DXVECTOR3& pos )
{
	m_matLocal._41 =  pos.x; 
	m_matLocal._42 =  pos.y; 
	m_matLocal._43 =  pos.z; 
//	m_bModifiedMatLocal = TRUE;
}

void cTransformable::GetLocalPos( D3DXVECTOR3& pos )
{
	pos.x = m_matLocal._41;   
	pos.y = m_matLocal._42;  
	pos.z = m_matLocal._43;  
}

const D3DXVECTOR3* cTransformable::GetLocalPos()
{
	return (D3DXVECTOR3*)&m_matLocal._41;
}


void cTransformable::GetWorldPosition(D3DXVECTOR3& pos)
{
	pos.x = m_matWorld._41;   
	pos.y = m_matWorld._42;  
	pos.z = m_matWorld._43;  
}

D3DXVECTOR3* cTransformable::GetWorldPositionPtr()
{
	return (D3DXVECTOR3*)&m_matWorld._41;
}


void cTransformable::MoveOnLocal( float deltaX,float deltaY,float deltaZ )
{
	D3DXMATRIX	m_matTranslation;
	D3DXMatrixTranslation(&m_matTranslation,deltaX,deltaY,deltaZ);
	m_matLocal =  m_matTranslation * m_matLocal;	
//	m_bModifiedMatLocal = TRUE;
}

void cTransformable::RotateOnLocal( float angleX,float angleY,float angleZ )
{
	D3DXMATRIX	m_matRotation;
	D3DXMatrixRotationYawPitchRoll(&m_matRotation,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_matLocal =  m_matRotation * m_matLocal;
//	m_bModifiedMatLocal = TRUE;
}


void cTransformable::SetWorldPos( D3DXVECTOR3& pos )
{
	m_matWorld._41 =  pos.x; 
	m_matWorld._42 =  pos.y; 
	m_matWorld._43 =  pos.z; 

}

/*
	속도 계산은 게임용루트에서만 작동하게한다.
*/
/*
void cTransformable::Update( DWORD dwElapseMS )
{
	float fElapseSec = float(dwElapseMS) / 1000.0f;

	if (!m_bNoVelocityPos)
	{
		D3DXMatrixIdentity(&m_matVelocityPos);
		D3DXMatrixTranslation( &m_matVelocityPos, m_velPosPerSec.x * fElapseSec, m_velPosPerSec.y * fElapseSec, m_velPosPerSec.z * fElapseSec );			
		m_matLocal = m_matVelocityPos * m_matLocal;			
	}

	if (!m_bNoVelocityRot)
	{
		D3DXMatrixIdentity(&m_matVelocityRot);
		D3DXMatrixRotationYawPitchRoll( &m_matVelocityRot, m_velRotPerSec.y * fElapseSec, m_velRotPerSec.x * fElapseSec, m_velRotPerSec.z * fElapseSec );	
		m_matLocal = m_matVelocityRot * m_matLocal;	
	}	
}
*/

void cTransformable::SetVelocityPosition( float x,float y,float z )
{
	m_velPosPerSec.x = x;
	m_velPosPerSec.y = y;
	m_velPosPerSec.z = z;

	if ((x == 0.0f)&&(y == 0.0f)&&(z == 0.0f))
	{
		m_bNoVelocityPos = true;
	}
	else
	{
		m_bNoVelocityPos = false;
	}
}

void cTransformable::SetVelocityPosition( D3DXVECTOR3& pos )
{
	m_velPosPerSec = pos;

	if ((pos.x == 0.0f)&&(pos.y == 0.0f)&&(pos.z == 0.0f))
	{
		m_bNoVelocityPos = true;
	}
	else
	{
		m_bNoVelocityPos = false;
	}
}

void cTransformable::SetVelocityRotation( float angleX,float angleY,float angleZ )
{
	m_velRotPerSec.x = D3DXToRadian(angleX);
	m_velRotPerSec.y = D3DXToRadian(angleY);
	m_velRotPerSec.z = D3DXToRadian(angleZ);	

	if ((angleX == 0.0f)&&(angleY == 0.0f)&&(angleZ == 0.0f))
	{
		m_bNoVelocityRot = true;
	}
	else
	{
		m_bNoVelocityRot = false;
	}
}

void cTransformable::SetVelocityRotation( D3DXVECTOR3& rot )
{
	m_velRotPerSec.x = D3DXToRadian(rot.x);
	m_velRotPerSec.y = D3DXToRadian(rot.y);
	m_velRotPerSec.z = D3DXToRadian(rot.z);	
	
	if ((rot.x == 0.0f)&&(rot.y == 0.0f)&&(rot.z == 0.0f))
	{
		m_bNoVelocityRot = true;
	}
	else
	{
		m_bNoVelocityRot = false;
	}
}

void cTransformable::SetLocalTM( const D3DXMATRIX& val )
{
	m_matLocal = val; 
	//m_bModifiedMatLocal=TRUE;
}

}