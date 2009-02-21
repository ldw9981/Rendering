#include "StdAfx.h"
#include "Transformable.h"
#include "../SceneGraph/SceneGraphNode.h"

cTransformable::cTransformable( void )
{
	D3DXMatrixIdentity(&m_WorldTM);
	D3DXMatrixIdentity(&m_LocalTM);
	D3DXMatrixIdentity(&m_TranslationTM);
	D3DXMatrixIdentity(&m_RotationTM);
	D3DXMatrixIdentity(&m_ScaleTM);	
	m_WorldPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_bLocalTMModified = TRUE;
	m_bWorldTMModified = TRUE;
}

cTransformable::~cTransformable( void )
{

}
/*
	이전과 같으면 FALSE
	갱신된것이면 TRUE
*/
BOOL cTransformable::UpdateWorldTM(D3DXMATRIX* pRepLocalTM/*=NULL*/,cTransformable* pParent/*=NULL*/)
{	
	if (pRepLocalTM!=NULL)
	{	
		SetLocalTM(*pRepLocalTM);	// 교체할 LocalTM이 있으면 교체
	}	
	
	if (pParent==NULL)	
	{
		if (m_bLocalTMModified)
		{
			m_WorldTM = m_LocalTM;		// 부모가 없으면 LocalTM이 WorldTM이 된다.
			m_bWorldTMModified = TRUE;
		}		
		else 
			m_bWorldTMModified = FALSE;
	}
	else
	{
		if (m_bLocalTMModified||pParent->GetWorldTMModified())
		{	 
			m_WorldTM = m_LocalTM * pParent->GetWorldTM();	// 부모가 있으면 WorldTM은 LocalTM * GetWorldTM 이 된다.
			m_bWorldTMModified = TRUE;
		}
		else
			m_bWorldTMModified = FALSE;		
	}	
	m_bLocalTMModified = FALSE;
	return m_bWorldTMModified;
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


const D3DXVECTOR3& cTransformable::GetWorldPos()
{
	m_WorldPos.x = m_WorldTM._41;   
	m_WorldPos.y = m_WorldTM._42;  
	m_WorldPos.z = m_WorldTM._43;  
	return m_WorldPos;
}


void cTransformable::MoveOnLocal( float deltaX,float deltaY,float deltaZ )
{
	D3DXMatrixTranslation(&m_TranslationTM,deltaX,deltaY,deltaZ);
	m_LocalTM =  m_TranslationTM * m_LocalTM;	
	m_bLocalTMModified=TRUE;
}

void cTransformable::RotateOnLocal( float angleX,float angleY,float angleZ )
{
	D3DXMatrixRotationYawPitchRoll(&m_RotationTM,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_LocalTM =  m_RotationTM * m_LocalTM;
	m_bLocalTMModified=TRUE;
}

const D3DXMATRIX& cTransformable::GetWorldInverseTM()
{
	D3DXMatrixInverse(&m_WorldInverseTM,NULL,&m_WorldTM);
	return m_WorldInverseTM;
}

void cTransformable::SetWorldPos( D3DXVECTOR3& pos )
{
	m_WorldTM._41 =  pos.x; 
	m_WorldTM._42 =  pos.y; 
	m_WorldTM._43 =  pos.z; 

}