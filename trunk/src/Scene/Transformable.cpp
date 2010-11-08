#include "StdAfx.h"
#include "Transformable.h"
#include "Scene/SceneNode.h"

cTransformable::cTransformable( void )
{
	D3DXMatrixIdentity(&m_WorldTM);
	D3DXMatrixIdentity(&m_LocalTM);

	m_bLocalTMModified = TRUE;
	m_bWorldTMModified = TRUE;
}

cTransformable::~cTransformable( void )
{

}
/*
	������ ������ FALSE
	���ŵȰ��̸� TRUE
*/
void cTransformable::UpdateWorldTM(D3DXMATRIX* pRepLocalTM/*=NULL*/,cTransformable* pParent/*=NULL*/)
{	
	bool bWorldTMModified = false;
	
	if (pRepLocalTM!=NULL)
	{	
		 m_LocalTM = *pRepLocalTM;	// ��ü�� LocalTM�� ������ ��ü
		 m_bLocalTMModified = true;
	}	
	
	if (pParent==NULL)	
	{
		if (m_bLocalTMModified)
		{
			m_WorldTM = m_LocalTM;		// �θ� ������ LocalTM�� WorldTM�� �ȴ�.
			bWorldTMModified = true;
		}					
	}
	else
	{
		if (m_bLocalTMModified||pParent->m_bWorldTMModified)
		{	 
			m_WorldTM = m_LocalTM * pParent->m_WorldTM;	// �θ� ������ WorldTM�� LocalTM * GetWorldTM �� �ȴ�.
			bWorldTMModified = true;
		}		
	}	

	m_bWorldTMModified = bWorldTMModified;
	m_bLocalTMModified = false;
}


void cTransformable::SetLocalPos( D3DXVECTOR3& pos )
{
	m_LocalTM._41 =  pos.x; 
	m_LocalTM._42 =  pos.y; 
	m_LocalTM._43 =  pos.z; 
	m_bLocalTMModified=TRUE;
}

void cTransformable::GetLocalPos( D3DXVECTOR3& pos )
{
	pos.x = m_LocalTM._41;   
	pos.y = m_LocalTM._42;  
	pos.z = m_LocalTM._43;  
}


void cTransformable::GetWorldPos(D3DXVECTOR3& pos)
{
	pos.x = m_WorldTM._41;   
	pos.y = m_WorldTM._42;  
	pos.z = m_WorldTM._43;  
}


void cTransformable::MoveOnLocal( float deltaX,float deltaY,float deltaZ )
{
	D3DXMATRIX				m_TranslationTM;
	D3DXMatrixTranslation(&m_TranslationTM,deltaX,deltaY,deltaZ);
	m_LocalTM =  m_TranslationTM * m_LocalTM;	
	m_bLocalTMModified=TRUE;
}

void cTransformable::RotateOnLocal( float angleX,float angleY,float angleZ )
{
	D3DXMATRIX				m_RotationTM;
	D3DXMatrixRotationYawPitchRoll(&m_RotationTM,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_LocalTM =  m_RotationTM * m_LocalTM;
	m_bLocalTMModified=TRUE;
}


void cTransformable::SetWorldPos( D3DXVECTOR3& pos )
{
	m_WorldTM._41 =  pos.x; 
	m_WorldTM._42 =  pos.y; 
	m_WorldTM._43 =  pos.z; 

}