#include "StdAfx.h"
#include "Transformable.h"
#include "Graphics/SceneNode.h"

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
}

cTransformable::~cTransformable( void )
{

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

void cTransformable::GetWorldPosition( D3DXVECTOR4& pos )
{
	pos.x = m_matWorld._41;   
	pos.y = m_matWorld._42;  
	pos.z = m_matWorld._43;  
	pos.w = m_matWorld._44;  
}

D3DXVECTOR3* cTransformable::GetWorldPositionPtr()
{
	return (D3DXVECTOR3*)&m_matWorld._41;
}


void cTransformable::TranslateLocal( float deltaX,float deltaY,float deltaZ )
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

void cTransformable::SetLocalTM( const D3DXMATRIX& val )
{
	m_matLocal = val; 
	//m_bModifiedMatLocal=TRUE;
}

void cTransformable::GetForward( D3DXVECTOR3& out )
{
	out.x = m_matLocal._31;
	out.y = m_matLocal._32;
	out.z = m_matLocal._33;
}

const D3DXVECTOR3* cTransformable::GetForward()
{
	return (D3DXVECTOR3*)&m_matWorld._31;
}

void cTransformable::GetUp( D3DXVECTOR3& out )
{
	out.x = m_matLocal._21;
	out.y = m_matLocal._22;
	out.z = m_matLocal._23;
}

const D3DXVECTOR3* cTransformable::GetUp()
{
	return (D3DXVECTOR3*)&m_matWorld._21;
}

}