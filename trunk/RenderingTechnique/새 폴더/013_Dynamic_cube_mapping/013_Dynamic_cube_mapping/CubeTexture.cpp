// CubeTexture.cpp: implementation of the CubeTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "CubeTexture.h"
#include <ddraw.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CubeTexture::CubeTexture()
{
	pt = NULL;
}

CubeTexture::~CubeTexture()
{
	if(pt!=NULL)pt->Release();
}

int CubeTexture::load(char *filename)
{
	if( D3DXCreateCubeTextureFromFile( pd, filename, &pt) != D3D_OK )
	{
		pt = NULL; 
		MessageBox( hWnd, filename, "Nenájdený súbor", MB_OK );
		return 0;
	}
	return 1;
}
static char isPow2(int x)
{	
	for( int i=0; x; x>>=1)
		if(x&1)i++;
	return (i==1);
}

int CubeTexture::load(char *directory, char *extension, int filter, int compression)
{
	// right, left, top, bottom, back, front
	char filename[6][200];
	Image i[6];

	if( !(caps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP)) return 0;
	for(int j=0; j<6; j++)
	{
	//	filename[j] = new char[strlen(directory)+strlen(extension)+7];
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
			i[j].LoadNoMessage(filename[j]);
		else
			i[j].Load( filename[j]);
	}

	if(i[3].data==NULL)			// ak sa nieje bottom, mozno bude down
	{
		strcpy( filename[3], directory);
		strcat( filename[3], "down.");
		strcat( filename[3], extension);
		i[3].Load( filename[3]);
	}
	for( j=0; j<6; j++)
	{
		if(i[j].data==NULL)
		{
			MessageBox( hWnd, filename[j], "Nenájdený súbor", MB_OK );
			return 0;
		}
	}

	for( j=0; j<6; j++)
	{
		// uprava obrazku aby bol 2^x x 2^x
		if( !isPow2(i[j].sizeX) || !isPow2(i[j].sizeY || i[j].sizeX!=i[j].sizeY) )
		{
			int newx=1,newy=1;
			for( int newx2=i[j].sizeX; newx2 ;newx2>>=1)newx<<=1;
			for( int newy2=i[j].sizeY; newy2 ;newy2>>=1)newy<<=1;
			if( isPow2(i[j].sizeX) ) newx = i[j].sizeX;
			if( isPow2(i[j].sizeY) ) newy = i[j].sizeY;
			if(newx!=newy) newx<newy ? newx=newy : newy=newx;

			if(!i[j].resize(newx,newy))return 0;
		}

		// uprava obrazku aby velkost nebola vecsia ako maximalna velkost
		while(i[j].sizeX>(int)caps.MaxTextureWidth || i[j].sizeY>(int)caps.MaxTextureHeight)
		{
			if(!i[j].resize((int)caps.MaxTextureWidth,(int)caps.MaxTextureHeight))return 0;
		}
		if(j>0)
		{
			if(i[j].sizeX!=i[0].sizeX || i[j].sizeY!=i[0].sizeY)
				if(!i[j].resize(i[0].sizeX,i[0].sizeY))return 0;
		}
		i[j].RGBtoBGR();
		// right, left, top, bottom, back, front
		if( j==0 || j==1 || j==4 || j==5) i[j].FlipMirror();
	}

	unsigned char* data;

	data = new unsigned char[sizeof(z_dds)+6*i[0].sizeX*i[0].sizeY*i[0].planes];
	if(data==NULL)return 0;
	z_dds *dds;
	dds = (z_dds*)data;
	memset( dds, 0, sizeof(z_dds));
	((unsigned char*)&dds->dwMagic)[0] = 'D';
	((unsigned char*)&dds->dwMagic)[1] = 'D';
	((unsigned char*)&dds->dwMagic)[2] = 'S';
	((unsigned char*)&dds->dwMagic)[3] = ' ';
	dds->dwSize = sizeof(z_dds)-4;
	dds->dwFlags =  DDSD_CAPS|DDSD_PIXELFORMAT|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PITCH;
	dds->dwHeight = i[0].sizeY;
	dds->dwWidth = i[0].sizeX;
	dds->dwPitchOrLinearSize = i[0].sizeX*i[0].planes;
	dds->dwDepth = 0;
	dds->dwMipMapCount = 0;
	dds->ddpfPixelFormat.dwSize = sizeof(z_dds_DDPIXELFORMAT);
	if(i[0].planes==4)dds->ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
	else dds->ddpfPixelFormat.dwFlags = DDPF_RGB;
	dds->ddpfPixelFormat.dwRGBBitCount = 8*i[0].planes;
	dds->ddpfPixelFormat.dwRBitMask = 0x00ff0000;
	dds->ddpfPixelFormat.dwGBitMask = 0x0000ff00;
	dds->ddpfPixelFormat.dwBBitMask = 0x000000ff;
	if(i[0].planes==4)dds->ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
	dds->ddsCaps.dwCaps1 = DDSCAPS_COMPLEX;
	dds->ddsCaps.dwCaps2 = DDSCAPS2_CUBEMAP |	DDSCAPS2_CUBEMAP_POSITIVEX|DDSCAPS2_CUBEMAP_NEGATIVEX|
												DDSCAPS2_CUBEMAP_POSITIVEY|DDSCAPS2_CUBEMAP_NEGATIVEY|
												DDSCAPS2_CUBEMAP_POSITIVEZ|DDSCAPS2_CUBEMAP_NEGATIVEZ;
	for( j=0; j<6; j++)
	{
		memcpy( &data[sizeof(z_dds)+j*i[0].sizeX*i[0].sizeY*i[0].planes], i[j].data, i[0].sizeX*i[0].sizeY*i[0].planes);
	}

//	FILE* file;	file = fopen( "pokus.dds","wb");fwrite( data, 1, sizeof(z_dds)+6*i[0].sizeX*i[0].sizeY*i[0].planes, file);	fclose(file);
	
	pd->SetTextureStageState(0, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState(0, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	D3DFORMAT format;
	if(     i[0].planes==1)format = D3DFMT_A8;
	else if(i[0].planes==3)format = D3DFMT_R8G8B8;
	else if(i[0].planes==4)format = D3DFMT_A8R8G8B8;

	if( 
		D3DXCreateCubeTextureFromFileInMemoryEx( pd, data, sizeof(z_dds)+6*i[0].sizeX*i[0].sizeY*i[0].planes, 
						0, D3DX_DEFAULT, 0, format, D3DPOOL_DEFAULT, 
						D3DX_FILTER_BOX, D3DX_FILTER_BOX, 0xFF000000, NULL, NULL, &pt)
		!= D3D_OK )
	{
		pt = NULL; 
		if(data!=NULL)delete [] data;
		MessageBox( hWnd, directory, "Problem pri nacitavani cube mapy", MB_OK );
		return 0;
	}
	if(data!=NULL)delete [] data;
	return 1;
}

int CubeTexture::CreateCubeTextureAsRenderTarget(int size)
{
	int ok=0;
	
	if( pd->CreateCubeTexture( size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R8G8B8, D3DPOOL_DEFAULT, &pt )
	== D3D_OK )ok=1;
	if(!ok)
	{
		if( pd->CreateCubeTexture( size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pt )
			== D3D_OK )ok=1;
	}
	
	if(!ok)
	{

		pt = NULL; 
		MessageBox( hWnd, "Problem pri vytvarani cube mapy a pouzitim ako RENDER TARGET", "ERROR", MB_OK );
		return 0;
	}
	return 1;
}
