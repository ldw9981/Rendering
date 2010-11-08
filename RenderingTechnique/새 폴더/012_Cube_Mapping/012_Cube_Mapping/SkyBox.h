// SkyBox.h: interface for the SkyBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKYBOX_H__1026B326_5B07_11D8_8A6F_CA5DEC104F1E__INCLUDED_)
#define AFX_SKYBOX_H__1026B326_5B07_11D8_8A6F_CA5DEC104F1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "texture.h"

class SkyBox  
{
public:
	int LoadTextureSkyBox(char *directory, char *extension, int repeat=0, int filter=3, int compression=0 );
	void ComputeQuad(vec p, vec n, vec s, vec t, float* b);
	void Render( DWORD hVertexShader = D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0));
	int Init(float size=1.0f);
	LPDIRECT3DVERTEXBUFFER8 pvb;		// Buffers to hold vertices
	texture tex[6];

	SkyBox();
	virtual ~SkyBox();
};

#endif // !defined(AFX_SKYBOX_H__1026B326_5B07_11D8_8A6F_CA5DEC104F1E__INCLUDED_)
