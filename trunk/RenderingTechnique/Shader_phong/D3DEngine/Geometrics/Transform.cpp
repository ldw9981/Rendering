#include "StdAfx.h"
#include "Transform.h"

cTransform::cTransform( void )
{
	m_bLocalTMModified=TRUE;
	D3DXMatrixIdentity(&m_WorldTM);
	D3DXMatrixIdentity(&m_WorldTMOld);
	D3DXMatrixIdentity(&m_LocalTM);
	D3DXMatrixIdentity(&m_LocalTranslationTM);
	D3DXMatrixIdentity(&m_LocalRotationTM);
	D3DXMatrixIdentity(&m_LocalScaleTM);	
}

cTransform::~cTransform( void )
{

}
D3DXMATRIX& cTransform::Transform()
{
	if (m_bLocalTMModified)
	{
		m_WorldTMOld = m_WorldTM;
		m_LocalTM = m_LocalScaleTM * m_LocalRotationTM * m_LocalTranslationTM;
		m_WorldTM = m_LocalTM;
		m_bLocalTMModified=FALSE;
	}
	return m_WorldTM;
}

