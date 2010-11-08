// SkyBox.cpp: implementation of the SkyBox class.
//
//////////////////////////////////////////////////////////////////////

#include "SkyBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SkyBox::SkyBox()
{
	pvb = NULL;
}

SkyBox::~SkyBox()
{
	if(pvb!=NULL) pvb->Release();
}

int SkyBox::Init(float size)
{
	if(pvb!=NULL) pvb->Release();

	if( FAILED( pd->CreateVertexBuffer( 6*4*5*sizeof(float),D3DUSAGE_WRITEONLY, 
								D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0),D3DPOOL_DEFAULT, &pvb ) ) )return 0;
	void* b;
	if( FAILED( pvb->Lock( 0, 6*4*5*sizeof(float), (BYTE**)&b, 0 ) ) )return 0;

	vec p,n,s,t;
	// pozerame na +x
	n.set(-1,0,0);
	s.set(0,0,2*size);
	t.set(0,2*size,0);
	p = vec(size,0,0) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[0*4*5] );
	// pozerame na -x
	n.set(1,0,0);
	s.set(0,0,-2*size);
	p = vec(-size,0,0) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[1*4*5] );
	// pozerame na +y
	n.set(0,-1,0);
	s.set(2*size,0,0);
	t.set(0,0,2*size);
	p = vec(0,size,0) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[2*4*5] );
	// pozerame na -y
	n.set(0,1,0);
	s.set(2*size,0,0);
	t.set(0,0,-2*size);
	p = vec(0,-size,0) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[3*4*5] );
	// pozerame na +z
	n.set(0,0,1);
	s.set(-2*size,0,0);
	t.set(0,2*size,0);
	p = vec(0,0,size) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[4*4*5] );
	// pozerame na -z
	n.set(0,0,1);
	s.set(2*size,0,0);
	p = vec(0,0,-size) -0.5*s -0.5*t;
	ComputeQuad( p, n, s, t, &((float*)b)[5*4*5] );

	pvb->Unlock();
	return 1;
}

void SkyBox::Render(DWORD hVertexShader)
{
	pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
	pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	pd->SetStreamSource( 0, pvb, 5*sizeof(float) );
	pd->SetVertexShader( hVertexShader );
	for(int i=0; i<6; i++)
	{
		tex[i].SetTexture(0);
		pd->DrawPrimitive( D3DPT_TRIANGLESTRIP, i*4, 2);
	}
	pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
}

void SkyBox::ComputeQuad(vec p, vec n, vec s, vec t, float *b)
{
	b[0*5+0] = p.x;
	b[0*5+1] = p.y;
	b[0*5+2] = p.z;
	b[0*5+3] = 0;
	b[0*5+4] = 1;

	b[1*5+0] = p.x + s.x;
	b[1*5+1] = p.y + s.y;
	b[1*5+2] = p.z + s.z;
	b[1*5+3] = 1;
	b[1*5+4] = 1;

	b[2*5+0] = p.x + t.x;
	b[2*5+1] = p.y + t.y;
	b[2*5+2] = p.z + t.z;
	b[2*5+3] = 0;
	b[2*5+4] = 0;

	b[3*5+0] = p.x + s.x + t.x;
	b[3*5+1] = p.y + s.y + t.y;
	b[3*5+2] = p.z + s.z + t.z;
	b[3*5+3] = 1;
	b[3*5+4] = 0;
}

int SkyBox::LoadTextureSkyBox(char *directory, char *extension, int repeat, int filter, int compression)
{
	// right, left, top, bottom, back, front
	char* filename[6];
	int OK=1;

	for(int j=0; j<6; j++)
	{
		filename[j] = new char[strlen(directory)+strlen(extension)+7];
		strcpy( filename[j], directory);
	}

	strcat( filename[0], "right.");
	strcat( filename[1], "left.");
	strcat( filename[2], "top.");
	strcat( filename[3], "bottom.");
	strcat( filename[4], "back.");
	strcat( filename[5], "front.");
	for( j=0; j<6; j++) strcat( filename[j], extension);

	strlwr(extension);				// Convert a string to lowercase
	
	for( j=0; j<6; j++)
	{
		if(j==3)
		{
			FILE* f;
			f = fopen(filename[3],"rb");
			if(f==NULL)
			{
				strcpy( filename[3], directory);
				strcat( filename[3], "down.");
				strcat( filename[3], extension);
			}
			else fclose(f);
		}
		if(!tex[j].load(filename[j], repeat, filter, compression))OK=0;
	}
	return OK;
}
