#include "StdAfx.h"
#include "Transformable.h"
#include "Scene/SceneNode.h"

cTransformable::cTransformable( void )
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);

	m_bModifiedMatLocal = TRUE;
	m_bModifiedMatWorld = TRUE;
}

cTransformable::~cTransformable( void )
{

}
/*
	������ ������ FALSE
	���ŵȰ��̸� TRUE
*/
void cTransformable::UpdateMatrix(D3DXMATRIX* pRepMatLocal/*=NULL*/,cTransformable* pParent/*=NULL*/)
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
	m_bModifiedMatLocal=TRUE;
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
	m_bModifiedMatLocal=TRUE;
}

void cTransformable::RotateOnLocal( float angleX,float angleY,float angleZ )
{
	D3DXMATRIX	m_matRotation;
	D3DXMatrixRotationYawPitchRoll(&m_matRotation,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_matLocal =  m_matRotation * m_matLocal;
	m_bModifiedMatLocal=TRUE;
}


void cTransformable::SetWorldPos( D3DXVECTOR3& pos )
{
	m_matWorld._41 =  pos.x; 
	m_matWorld._42 =  pos.y; 
	m_matWorld._43 =  pos.z; 

}