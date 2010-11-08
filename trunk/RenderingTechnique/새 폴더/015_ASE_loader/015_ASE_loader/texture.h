#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "init.h"
#include "Image.h"

class texture  
{
public:
	int loadNoMessage( char* filename, int MipLevels=0, unsigned int format=D3DFMT_R8G8B8);
	int load( char* filename, int MipLevels=0, unsigned int format=D3DFMT_R8G8B8);
	int load( Image &im, int MipLevels=0, unsigned int format=D3DFMT_R8G8B8);
	void SetTexture(int Stage=0){pd->SetTexture( Stage, pt );}

	LPDIRECT3DTEXTURE8 pt;

	texture();
	virtual ~texture();
};

#endif // _TEXTURE_H