// texture.cpp: implementation of the texture class.
//
//////////////////////////////////////////////////////////////////////

#include "texture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

texture::texture()
{
	pt = NULL;
}

texture::~texture()
{
	if(pt!=NULL)pt->Release();
}

/* 
 char *filename, int repeat, int filter, int compression
 repeat : 0-clamp, 1-repeat
 filter :
 0 - nearest
 1 - linear
 2 - bilinear, linear_mipmap_nearest
 3 - trilinear, linear_mipmap_linear
*/
int texture::load( char* filename, int repeat, int filter, int compression )
{
/*	pd->SetTextureStageState(0, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState(0, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	
	switch(filter)
	{
	case 2:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_POINT); break;
	case 3:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_LINEAR); break;
	default:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_NONE);
	}*/
//	pd->SetTextureStageState(0, D3DTSS_MAXANISOTROPY,16);
	
	if( FAILED(
	D3DXCreateTextureFromFileEx( pd, filename, 0, 0, D3DX_DEFAULT, 0, D3DFMT_R8G8B8, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0xFF000000, NULL, NULL, &pt)
				))
	{
		pt = NULL; 
		MessageBox( hWnd, filename, "Nenájdený súbor", MB_OK );
		return 0;
	}
	return 1;
}

/* 
 repeat : 0-clamp, 1-repeat
 filter :
 0 - nearest
 1 - linear
 2 - bilinear, linear_mipmap_nearest
 3 - trilinear, linear_mipmap_linear
*/
int texture::load( Image &im, int repeat, int filter, int compression)
{
	if(im.data==NULL)return 0;
	int size;
	unsigned char* data;
	data = im.Get( &size);
	if(data==NULL)return 0;

	pd->SetTextureStageState(0, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState(0, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	
/*	switch(filter)
	{
	case 2:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_POINT); break;
	case 3:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_LINEAR); break;
	default:
		pd->SetTextureStageState(0, D3DTSS_MIPFILTER,D3DTEXF_NONE);
	}*/
//	pd->SetTextureStageState(0, D3DTSS_MAXANISOTROPY,16);
	if( FAILED(
	D3DXCreateTextureFromFileInMemoryEx( pd, data, size, 
						0, 0, D3DX_DEFAULT, 0, D3DFMT_R8G8B8, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0xFF000000, NULL, NULL, &pt)
				))
	{
		pt = NULL; 
		if(data!=NULL)delete [] data;
		return 0;
	}
	if(data!=NULL)delete [] data;
	return 1;
}