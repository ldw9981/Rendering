// Plane_VNTTB.h: interface for the Plane_VNTTB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANE_VNTTB_H__29B516E2_0806_11D9_8A6F_BE72EA573B38__INCLUDED_)
#define AFX_PLANE_VNTTB_H__29B516E2_0806_11D9_8A6F_BE72EA573B38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "Image.h"		// for Displacement_Mapping

// A structure for our custom vertex type
struct zPlane_VNTTB_CUSTOMVERTEX
{
	vec v;		// vertex
	vec	n;		// normal
	vec2 t0;	// texture coord 0
	vec s;		// texture axis s in object space, s = Tangent
	vec t;		// texture axis t in object space, t = Binormal
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX_PLANE_VNTTB (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX3| D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE3(1)|D3DFVF_TEXCOORDSIZE3(2))

class Plane_VNTTB  
{
public:
	int Init_Displacement_Mapping(vec pos, vec xs, vec ys, vec zs, vec2 tex_pos, vec2 texxy, int div_x, int div_y, char* filename, float scale, float bias);
	void Render(DWORD hVertexShader = D3DFVF_CUSTOMVERTEX_PLANE_VNTTB);
	int Init(vec pos, vec x, vec y, vec2 tex_pos, vec2 texxy, int div_x, int div_y);
	Plane_VNTTB();
	virtual ~Plane_VNTTB();

	LPDIRECT3DVERTEXBUFFER8 pvb;
	int count;						// count of primitive
};

#endif // !defined(AFX_PLANE_VNTTB_H__29B516E2_0806_11D9_8A6F_BE72EA573B38__INCLUDED_)
