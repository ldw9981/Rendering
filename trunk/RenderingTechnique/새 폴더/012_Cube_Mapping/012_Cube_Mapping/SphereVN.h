// SphereVN.h: interface for the SphereVN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHEREVN_H__79D3B881_58AB_11D8_8A6F_A657300BC31C__INCLUDED_)
#define AFX_SPHEREVN_H__79D3B881_58AB_11D8_8A6F_A657300BC31C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"

// A structure for our custom vertex type
struct zSphereVN_CUSTOMVERTEX
{
	vec		v;		// vertex
	vec		n;		// normal
};

// Our custom FVF, which describes our custom vertex structure
#define zSphereVN_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL )


class SphereVN  
{
public:
	void Render( DWORD hVertexShader = zSphereVN_D3DFVF_CUSTOMVERTEX);
	void ComputeSphere(int del_uhol_x, int del_uhol_y, zSphereVN_CUSTOMVERTEX *v, float size=1.0f);
	int Init( int del_uhol_x, int del_uhol_y, float size=1.0f);

	LPDIRECT3DVERTEXBUFFER8 pvb;		// Buffers to hold vertices
	int del_x, del_y;

	SphereVN();
	virtual ~SphereVN();

};

#endif // !defined(AFX_SPHEREVN_H__79D3B881_58AB_11D8_8A6F_A657300BC31C__INCLUDED_)
