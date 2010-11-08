// Frustum.cpp: implementation of the Frustum class.
//
//////////////////////////////////////////////////////////////////////

#include "Frustum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum FrustumSide
{
	RIGHT	= 0,		// The RIGHT side of the frustum
	LEFT	= 1,		// The LEFT	 side of the frustum
	BOTTOM	= 2,		// The BOTTOM side of the frustum
	TOP		= 3,		// The TOP side of the frustum
	BACK	= 4,		// The BACK	side of the frustum
	FRONT	= 5			// The FRONT side of the frustum
}; 

Frustum::Frustum()
{

}

Frustum::~Frustum()
{

}

void Frustum::Init(const mat4 &m)
{
	n[RIGHT].x = m.m41 - m.m11;
	n[RIGHT].y = m.m42 - m.m12;
	n[RIGHT].z = m.m43 - m.m13;
	d[RIGHT]   = m.m44 - m.m14;

	n[LEFT ].x = m.m41 + m.m11;
	n[LEFT ].y = m.m42 + m.m12;
	n[LEFT ].z = m.m43 + m.m13;
	d[LEFT ]   = m.m44 + m.m14;

	n[BOTTOM].x = m.m41 + m.m21;
	n[BOTTOM].y = m.m42 + m.m22;
	n[BOTTOM].z = m.m43 + m.m23;
	d[BOTTOM]   = m.m44 + m.m24;

	n[TOP   ].x = m.m41 - m.m21;
	n[TOP   ].y = m.m42 - m.m22;
	n[TOP   ].z = m.m43 - m.m23;
	d[TOP   ]   = m.m44 - m.m24;

	n[BACK ].x = m.m41 - m.m31;
	n[BACK ].y = m.m42 - m.m32;
	n[BACK ].z = m.m43 - m.m33;
	d[BACK ]   = m.m44 - m.m34;

	n[FRONT].x = m.m41 + m.m31;
	n[FRONT].y = m.m42 + m.m32;
	n[FRONT].z = m.m43 + m.m33;
	d[FRONT]   = m.m44 + m.m34;

	float length;
	for(int i=0; i<6; i++)
	{
		length = n[i].Length();
		n[i] /= length;
		d[i] /= length;
	}
}

int Frustum::PointInFrustum(const vec &p)
{
	for(int i = 0; i < 6; i++ )				// Go through all the sides of the frustum
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if(PlanePointDelta(n[i],d[i],p)<=0)
			return 0;			// The point was behind a side, so it ISN'T in the frustum
	}
	return 1;	// The point was inside of the frustum (In front of ALL the sides of the frustum)
}

void Frustum::Init()
{
	mat4 proj,mod;
	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );
	pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
	Init(proj*mod);
}

int Frustum::SphereInFrustum(const vec &p, float radius)
{
	for(int i = 0; i < 6; i++ )				// Go through all the sides of the frustum
	{
		// If the center of the sphere is farther away from the plane than the radius
		if(PlanePointDelta(n[i],d[i],p)<=-radius)
			return 0;		// The distance was greater than the radius so the sphere is outside of the frustum
	}
	return 1;	// The sphere was inside of the frustum!
}

int Frustum::BoxInFrustum(const vec &min, const vec &max)
{
	for(int i=0; i<6; i++)
	{
		if( n[i].x*min.x + n[i].y*min.y + n[i].z*min.z + d[i] > 0)continue;
		if( n[i].x*max.x + n[i].y*min.y + n[i].z*min.z + d[i] > 0)continue;
		if( n[i].x*min.x + n[i].y*max.y + n[i].z*min.z + d[i] > 0)continue;
		if( n[i].x*max.x + n[i].y*max.y + n[i].z*min.z + d[i] > 0)continue;
		if( n[i].x*min.x + n[i].y*min.y + n[i].z*max.z + d[i] > 0)continue;
		if( n[i].x*max.x + n[i].y*min.y + n[i].z*max.z + d[i] > 0)continue;
		if( n[i].x*min.x + n[i].y*max.y + n[i].z*max.z + d[i] > 0)continue;
		if( n[i].x*max.x + n[i].y*max.y + n[i].z*max.z + d[i] > 0)continue;

		return 0;
	}
	return 1;
}

int Frustum::CubeInFrustum(const vec &c, float size)
{
	for(int i=0; i<6; i++)
	{
		if( n[i].x*(c.x - size) + n[i].y*(c.y - size) + n[i].z*(c.z - size) + d[i] > 0)continue;
		if( n[i].x*(c.x + size) + n[i].y*(c.y - size) + n[i].z*(c.z - size) + d[i] > 0)continue;
		if( n[i].x*(c.x - size) + n[i].y*(c.y + size) + n[i].z*(c.z - size) + d[i] > 0)continue;
		if( n[i].x*(c.x + size) + n[i].y*(c.y + size) + n[i].z*(c.z - size) + d[i] > 0)continue;
		if( n[i].x*(c.x - size) + n[i].y*(c.y - size) + n[i].z*(c.z + size) + d[i] > 0)continue;
		if( n[i].x*(c.x + size) + n[i].y*(c.y - size) + n[i].z*(c.z + size) + d[i] > 0)continue;
		if( n[i].x*(c.x - size) + n[i].y*(c.y + size) + n[i].z*(c.z + size) + d[i] > 0)continue;
		if( n[i].x*(c.x + size) + n[i].y*(c.y + size) + n[i].z*(c.z + size) + d[i] > 0)continue;
		return 0;
	}
	return 1;
}
