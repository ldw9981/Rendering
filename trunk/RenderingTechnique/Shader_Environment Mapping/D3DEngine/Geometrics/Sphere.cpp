#include "StdAfx.h"
#include "Sphere.h"

cSphere::cSphere(void)
{
	m_Center=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Radius=0.0f;
	m_RadiusSQ=0.0f;
}

cSphere::cSphere( D3DXVECTOR3& center,float radius )
{
	Make(center,radius);
}
cSphere::~cSphere(void)
{
}

void cSphere::Make( D3DXVECTOR3& center,float radius )
{
	m_Center=center;
	m_Radius=radius;
	m_RadiusSQ=m_Radius*m_Radius;
}

void cSphere::Make( D3DXVECTOR3& min,D3DXVECTOR3& max )
{
	m_Center.x= min.x + (max.x - min.x)/2.0f;
	m_Center.y= min.y + (max.y - min.y)/2.0f;
	m_Center.z= min.z + (max.z - min.z)/2.0f;

	D3DXVECTOR3 temp;
	temp = max - m_Center;
	m_RadiusSQ = D3DXVec3LengthSq(&temp);
	m_Radius = sqrt(m_RadiusSQ); 
}