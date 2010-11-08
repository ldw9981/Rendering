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

// MipLevels :
// 0 - complete mipmap chain is created
// 1 - only base level = mipmapping off
int texture::load( char* filename, int MipLevels, unsigned int format)
{
	if(pt!=NULL)pt->Release();
	pt = NULL;

	if( FAILED(
	D3DXCreateTextureFromFileEx( pd, filename, 0, 0, MipLevels, 0, (D3DFORMAT)format, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0, NULL, NULL, &pt)
				))
	{
		pt = NULL; 
		MessageBox( hWnd, filename, "Nenájdený súbor", MB_OK );
		return 0;
	}
	return 1;
}

int texture::loadNoMessage(char *filename, int MipLevels, unsigned int format)
{
	if(pt!=NULL)pt->Release();
	pt = NULL;
	if( FAILED(
	D3DXCreateTextureFromFileEx( pd, filename, 0, 0, MipLevels, 0, (D3DFORMAT)format, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0, NULL, NULL, &pt)
				))
	{
		pt = NULL; 
		return 0;
	}
	return 1;
}

// MipLevels :
// 0 - complete mipmap chain is created
// 1 - only base level = mipmapping off
int texture::load( Image &im, int MipLevels, unsigned int format)
{
	if(im.data==NULL)return 0;
	int size;
	unsigned char* data;
	data = im.Get( &size);
	if(data==NULL)return 0;
	if(pt!=NULL)pt->Release();
	pt = NULL;

	if( FAILED(
	D3DXCreateTextureFromFileInMemoryEx( pd, data, size, 
						0, 0, MipLevels, 0, (D3DFORMAT)format, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0, NULL, NULL, &pt)
				))
	{
		pt = NULL; 
		if(data!=NULL)delete [] data;
		return 0;
	}
	if(data!=NULL)delete [] data;
	return 1;
}
