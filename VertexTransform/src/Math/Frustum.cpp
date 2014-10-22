#include "StdAfx.h"
#include "Frustum.h"

namespace Sophia
{

Frustum::Frustum(void)
{
}


Frustum::~Frustum(void)
{
}

void Frustum::Make( D3DXMATRIX &mat )
{
	D3DXVECTOR3 Normal;
	float dist;	

	Normal.x=mat._14 + mat._11;
	Normal.y=mat._24 + mat._21;
	Normal.z=mat._34 + mat._31;
	dist=	 mat._44 + mat._41;
	m_plane[PN_LEFT].Make(Normal,dist);

	Normal.x=mat._14 - mat._11;
	Normal.y=mat._24 - mat._21;
	Normal.z=mat._34 - mat._31;
	dist=	 mat._44 - mat._41;
	m_plane[PN_RIGHT].Make(Normal,dist);

	Normal.x= mat._14 - mat._12;
	Normal.y= mat._24 - mat._22;
	Normal.z= mat._34 - mat._32;
	dist=	  mat._44 - mat._42;
	m_plane[PN_TOP].Make(Normal,dist);

	Normal.x= mat._14 + mat._12;
	Normal.y= mat._24 + mat._22;
	Normal.z= mat._34 + mat._32;
	dist=	  mat._44 + mat._42;
	m_plane[PN_BOTTOM].Make(Normal,dist);

	Normal.x= mat._14 + mat._13;
	Normal.y= mat._24 + mat._23;
	Normal.z= mat._34 + mat._33;
	dist=	  mat._44 + mat._43;
	m_plane[PN_NEAR].Make(Normal,dist);


	Normal.x= mat._14 - mat._13;
	Normal.y= mat._24 - mat._23;
	Normal.z= mat._34 - mat._33;
	dist=	  mat._44 - mat._43;
	m_plane[PN_FAR].Make(Normal,dist);

	for( int i = PN_LEFT; i < PN_MAX; ++i )
		m_plane[i].Normalize();
}

cPlane& Frustum::GetPlane( int side )
{
	return m_plane[side];
}

}