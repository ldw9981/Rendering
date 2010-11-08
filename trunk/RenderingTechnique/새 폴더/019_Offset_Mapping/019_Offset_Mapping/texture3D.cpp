// texture3D.cpp: implementation of the texture3D class.
//
//////////////////////////////////////////////////////////////////////

#include "texture3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//D3DXCreateVolumeTextureFromFileInMemoryEx
texture3D::texture3D()
{
	pt = NULL;
}

texture3D::~texture3D()
{
	if(pt!=NULL)pt->Release();
}

int texture3D::load(Image3D &im)
{
	int format;

	switch(im.colorPlanes)
	{
//	case 1: format = D3DFMT_A8; break;
	case 1: format = D3DFMT_L8; break;
	case 2: format = D3DFMT_A8P8; break;
	case 3: format = D3DFMT_R8G8B8; break;
	case 4: format = D3DFMT_A8R8G8B8; break;
	}

	if(im.data==NULL)return 0;
	if(pt!=NULL)pt->Release();

	if( FAILED(
	D3DXCreateVolumeTextureFromFileInMemoryEx( pd, im.data, im.size,
						0, 0, 0, D3DX_DEFAULT, 0, (D3DFORMAT)format, D3DPOOL_DEFAULT,
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0xFF000000, NULL, NULL, &pt)
					//	D3DX_FILTER_LINEAR, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &pt)
				))
	{
		pt = NULL;
		return 0;
	}
	return 1;
}
