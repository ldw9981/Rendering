// texture3D.h: interface for the texture3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE3D_H__121AE361_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_)
#define AFX_TEXTURE3D_H__121AE361_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "Image3D.h"

class texture3D  
{
public:
	int load( Image3D &im);
	void SetTexture(int Stage=0){pd->SetTexture( Stage, pt );}
	texture3D();
	virtual ~texture3D();

	LPDIRECT3DVOLUMETEXTURE8 pt;
};

#endif // !defined(AFX_TEXTURE3D_H__121AE361_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_)
