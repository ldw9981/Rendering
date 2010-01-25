#include "Sphere.h"

cSphere::cSphere(void)
{
	m_CenterPos=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Radius=0.0f;
	m_RadiusSQ=0.0f;
}

cSphere::cSphere(const D3DXVECTOR3& center,const float radius )
{
	Make(center,radius);
}


cSphere::~cSphere(void)
{
}

void cSphere::Make(const D3DXVECTOR3& center,const float radius )
{
	m_CenterPos=center;
	m_Radius=radius;
	m_RadiusSQ=m_Radius*m_Radius;
}

void cSphere::Make(const D3DXVECTOR3& min,const D3DXVECTOR3& max )
{
// 	m_Center.x= min.x + (max.x - min.x)/2.0f;
// 	m_Center.y= min.y + (max.y - min.y)/2.0f;
// 	m_Center.z= min.z + (max.z - min.z)/2.0f;
	D3DXVECTOR3 temp;
	temp = (max - min)/2.0f;
	m_CenterPos = min + temp;
	m_RadiusSQ = D3DXVec3LengthSq(&temp);
	m_Radius = sqrt(m_RadiusSQ); 
}

void cSphere::Merge(cSphere& other)
{
	
	D3DXVECTOR3 temp;	
	temp = (other.GetCenterPos() - m_CenterPos)/2.0f ;
	m_CenterPos = m_CenterPos + temp;

	float addRadius=0.0f,addRadiusSQ=0.0f;
	addRadius = max (m_Radius, other.GetRadius());
	addRadiusSQ = max (m_RadiusSQ, other.GetRadiusSQ());
	m_RadiusSQ = D3DXVec3LengthSq(&temp) + addRadiusSQ;
	m_Radius = sqrt(m_RadiusSQ);	
}

cSphere& cSphere::operator =(cSphere &rhs)
{
	m_RadiusSQ = rhs.m_RadiusSQ;
	m_Radius = rhs.m_Radius;
	m_CenterPos = rhs.m_CenterPos;
	return *this;
}