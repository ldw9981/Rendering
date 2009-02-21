#pragma once
#include "Plane.h"

class cAABB
{
public:
	cAABB(void);
	cAABB(D3DXVECTOR3& min,D3DXVECTOR3& max);
	virtual ~cAABB(void);

	enum { PLANE_LEFT,PLANE_RIGHT,PLANE_TOP,PLANE_BOTTOM,PLANE_FRONT,PLANE_BACK};
private:
	D3DXVECTOR3 m_min;
	D3DXVECTOR3 m_max;
	D3DXVECTOR3 m_pos[8];

	cPlane	m_Plane[6];

public:
	void Make(D3DXVECTOR3& min,D3DXVECTOR3& max);
	D3DXVECTOR3& GetPos(int idx);
	cPlane& GetPlane(int side);
};
