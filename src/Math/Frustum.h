#pragma once
#include "Plane.h"



class Frustum
{
public:
	Frustum(void);
	~Frustum(void);
	// Plane Number
	enum PLANE_NUM { PN_LEFT,PN_RIGHT,PN_TOP,PN_BOTTOM,PN_NEAR,PN_FAR,PN_MAX};
	enum PLANE_BIT 
	{ 
		PB_LEFT =       0x0001,
		PB_RIGHT =		0x0002,
		PB_TOP =		0x0004,
		PB_BOTTOM =		0x0008,
		PB_NEAR	=		0x000F,
		PB_FAR =		0x0010
	};	
public:
	cPlane m_plane[PN_MAX];

public:
	void Make(D3DXMATRIX &mat);
	cPlane& GetPlane( int side );
};

