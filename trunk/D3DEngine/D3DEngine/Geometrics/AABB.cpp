#include "StdAfx.h"
#include "AABB.h"

cAABB::cAABB(void)
{
}

cAABB::cAABB( D3DXVECTOR3& min,D3DXVECTOR3& max )
{
	m_min=min;
	m_max=max;
	Make(min,max);
}
cAABB::~cAABB(void)
{
}

/*
		3      0

    2      1
                
		7	   4
    6      5
*/
void cAABB::Make( D3DXVECTOR3& min,D3DXVECTOR3& max )
{
	// 윗면의4점 시계방향
	m_pos[0] = max;
	m_pos[1] = D3DXVECTOR3(max.x,max.y,min.z);
	m_pos[2] = D3DXVECTOR3(min.x,max.y,min.z); 
	m_pos[3] = D3DXVECTOR3(min.x,max.y,max.z);

	// 밑면의 4점 시계방향
	m_pos[4] = D3DXVECTOR3(max.x,min.y,max.z); 
	m_pos[5] = D3DXVECTOR3(max.x,min.y,min.z);
	m_pos[6] = min;
	m_pos[7] = D3DXVECTOR3(min.x,min.y,max.z);	

	m_Plane[PLANE_LEFT].Make(	m_pos[3],m_pos[2],m_pos[7]);
	m_Plane[PLANE_RIGHT].Make(	m_pos[0],m_pos[4],m_pos[1]);
	m_Plane[PLANE_TOP].Make(	m_pos[0],m_pos[1],m_pos[2]);
	m_Plane[PLANE_BOTTOM].Make(	m_pos[4],m_pos[7],m_pos[5]);
	m_Plane[PLANE_FRONT].Make(	m_pos[2],m_pos[1],m_pos[6]);
	m_Plane[PLANE_BACK].Make(	m_pos[7],m_pos[0],m_pos[3]);
}

cPlane& cAABB::GetPlane( int side )
{
	return m_Plane[side];
}

D3DXVECTOR3& cAABB::GetPos( int idx )
{
	return m_pos[idx];
}