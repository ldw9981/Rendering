#include "StdAfx.h"
#include "Transformable.h"
#include "Scene/SceneNode.h"

cTransformable::cTransformable( void )
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matVelocityPos);
	D3DXMatrixIdentity(&m_matVelocityRot);

	m_bModifiedMatLocal = TRUE;
	m_bModifiedMatWorld = TRUE;
	m_bModifiedVelocityPos = FALSE;
	m_bModifiedVelocityRot = FALSE;
}

cTransformable::~cTransformable( void )
{

}
/*
	������ ������ FALSE
	���ŵȰ��̸� TRUE
*/
void cTransformable::UpdateWorldMatrix(D3DXMATRIX* pRepMatLocal/*=NULL*/,cTransformable* pParent/*=NULL*/)
{	
	bool bModifiedMatWorld = false;
	
	if (pRepMatLocal!=NULL)
	{	
		 m_matLocal = *pRepMatLocal;	// ��ü�� LocalTM�� ������ ��ü
		 m_bModifiedMatLocal = true;
	}	
	
	if (pParent==NULL)	
	{
		if (m_bModifiedMatLocal)
		{
			m_matWorld = m_matLocal;		// �θ� ������ LocalTM�� WorldTM�� �ȴ�.
			bModifiedMatWorld = true;
		}					
	}
	else
	{
		if (m_bModifiedMatLocal || pParent->m_bModifiedMatWorld)
		{	 
			m_matWorld = m_matLocal * pParent->m_matWorld;	// �θ� ������ WorldTM�� LocalTM * GetWorldTM �� �ȴ�.
			bModifiedMatWorld = true;
		}		
	}	

	m_bModifiedMatWorld = bModifiedMatWorld;
	m_bModifiedMatLocal = false;
}


void cTransformable::SetLocalPos( D3DXVECTOR3& pos )
{
	m_matLocal._41 =  pos.x; 
	m_matLocal._42 =  pos.y; 
	m_matLocal._43 =  pos.z; 
	m_bModifiedMatLocal = TRUE;
}

void cTransformable::GetLocalPos( D3DXVECTOR3& pos )
{
	pos.x = m_matLocal._41;   
	pos.y = m_matLocal._42;  
	pos.z = m_matLocal._43;  
}


void cTransformable::GetWorldPos(D3DXVECTOR3& pos)
{
	pos.x = m_matWorld._41;   
	pos.y = m_matWorld._42;  
	pos.z = m_matWorld._43;  
}


void cTransformable::MoveOnLocal( float deltaX,float deltaY,float deltaZ )
{
	D3DXMATRIX	m_matTranslation;
	D3DXMatrixTranslation(&m_matTranslation,deltaX,deltaY,deltaZ);
	m_matLocal =  m_matTranslation * m_matLocal;	
	m_bModifiedMatLocal = TRUE;
}

void cTransformable::RotateOnLocal( float angleX,float angleY,float angleZ )
{
	D3DXMATRIX	m_matRotation;
	D3DXMatrixRotationYawPitchRoll(&m_matRotation,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_matLocal =  m_matRotation * m_matLocal;
	m_bModifiedMatLocal = TRUE;
}


void cTransformable::SetWorldPos( D3DXVECTOR3& pos )
{
	m_matWorld._41 =  pos.x; 
	m_matWorld._42 =  pos.y; 
	m_matWorld._43 =  pos.z; 

}

void cTransformable::Update( DWORD dwElapseMS )
{
	float fElapseSec = float(dwElapseMS) / 1000.0f;

	//D3DXMATRIX tmTemp,tmSCL,tmROT,tmPOS;		
	if (m_bModifiedVelocityPos)
	{
		D3DXMatrixTranslation( &m_matVelocityPos, m_velPosPerSec.x * fElapseSec, m_velPosPerSec.y * fElapseSec, m_velPosPerSec.z * fElapseSec );
		m_bModifiedVelocityPos = FALSE;
		
		m_matLocal = m_matVelocityPos * m_matLocal;	
		
		m_bModifiedMatLocal = TRUE;
	}	

	if (m_bModifiedVelocityRot)
	{
		D3DXMatrixRotationYawPitchRoll( &m_matVelocityRot, m_velRotPerSec.y * fElapseSec, m_velRotPerSec.x * fElapseSec, m_velRotPerSec.z * fElapseSec );
		m_bModifiedVelocityRot = FALSE;

		m_matLocal = m_matVelocityRot * m_matLocal;	

		m_bModifiedMatLocal = TRUE;
	}	
}

void cTransformable::SetVelocityPosition( float x,float y,float z )
{
	m_velPosPerSec.x = x;
	m_velPosPerSec.y = y;
	m_velPosPerSec.z = z;
	m_bModifiedVelocityPos = TRUE;
}

void cTransformable::SetVelocityRotation( float angleX,float angleY,float angleZ )
{
	m_velRotPerSec.x = D3DXToRadian(angleX);
	m_velRotPerSec.y = D3DXToRadian(angleY);
	m_velRotPerSec.z = D3DXToRadian(angleZ);	
	m_bModifiedVelocityRot = TRUE;
}
