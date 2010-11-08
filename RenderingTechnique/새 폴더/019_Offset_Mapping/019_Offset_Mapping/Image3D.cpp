// Image3D.cpp: implementation of the Image3D class.
//
//////////////////////////////////////////////////////////////////////

#include "Image3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Image3D::Image3D()
{
	data = NULL;
}

Image3D::~Image3D()
{
	if(data!=NULL) delete [] data;
}

int Image3D::Make(int size_x, int size_y, int size_z, int color_planes)
{
	if(size_x<1 || size_y<1 || size_z<1 || color_planes<3)return 0;
	if(data!=NULL) delete [] data;
	data = NULL;
	sizeX = size_x;
	sizeY = size_y;
	sizeZ = size_z;
	colorPlanes = color_planes;
	sizeLine = colorPlanes*sizeX;
	if( sizeLine%4 ) sizeLine += 4 - sizeLine%4;
	sizePlane = sizeLine*sizeY;
	delta = sizeof(z_dds);
	size = delta + sizeZ*sizePlane;
	data = new unsigned char[size];
	if(data==NULL)return 0;
	Set_dds();
	return 1;
}

int Image3D::Make1to1(int size_x, int size_y, int size_z)
{
	if(!Make( size_x, size_y, size_z, 3))return 0;
	for(int z=0; z<sizeZ; z++)
	{
		float zf = (float)z/(float)(sizeZ-1);
		unsigned char zc = (unsigned char)(zf*255.f);
		for(int y=0; y<sizeY; y++)
		{
			float yf = (float)y/(float)(sizeY-1);
			unsigned char yc = (unsigned char)(yf*255.f);
			for(int x=0; x<sizeX; x++)
			{
				float xf = (float)x/(float)(sizeX-1);
				unsigned char xc = (unsigned char)(xf*255.f);
				set(x,y,z,0,xc);
				set(x,y,z,1,yc);
				set(x,y,z,2,zc);
			}
		}
	}
	return 1;
}


void Image3D::Set_dds()
{
	z_dds *dds;
	dds = (z_dds*)data;
	memset( dds, 0, sizeof(z_dds));
	((unsigned char*)&dds->dwMagic)[0] = 'D';
	((unsigned char*)&dds->dwMagic)[1] = 'D';
	((unsigned char*)&dds->dwMagic)[2] = 'S';
	((unsigned char*)&dds->dwMagic)[3] = ' ';
	dds->dwSize = sizeof(z_dds)-4;
	dds->dwFlags =  DDSD_CAPS|DDSD_PIXELFORMAT|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PITCH|DDSD_DEPTH;
	dds->dwHeight = sizeY;
	dds->dwWidth = sizeX;
	dds->dwPitchOrLinearSize = sizeLine;
	dds->dwDepth = sizeZ;
	dds->dwMipMapCount = 0;

	dds->ddpfPixelFormat.dwSize = sizeof(z_dds_DDPIXELFORMAT);
	if(colorPlanes==4)dds->ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
	else dds->ddpfPixelFormat.dwFlags = DDPF_RGB;
	dds->ddpfPixelFormat.dwRGBBitCount = 8*colorPlanes;
	dds->ddpfPixelFormat.dwRBitMask = 0x00ff0000;
	dds->ddpfPixelFormat.dwGBitMask = 0x0000ff00;
	dds->ddpfPixelFormat.dwBBitMask = 0x000000ff;
	if(colorPlanes==4)dds->ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
	dds->ddsCaps.dwCaps1 = DDSCAPS_TEXTURE|DDSCAPS_COMPLEX;
	dds->ddsCaps.dwCaps2 = DDSCAPS2_VOLUME;
}

int Image3D::Save_dds(char *filename)
{
	FILE* file;
	file = fopen( filename,"wb");
	if(file==NULL)return 0;
	fwrite( data, 1, size, file);
	fclose(file);
	return 1;
}

int Image3D::Load_dds(char *filename)
{
	FILE* file;
	file = fopen( filename,"rb");
	if(file==NULL)return 0;
	if(data!=NULL)delete [] data;
	data = NULL;

	fseek(file, 0, SEEK_END);
	size = (int)ftell(file);
	rewind(file);
	if(size>0)data = new unsigned char[size];
	fread( data, 1, size, file);
	fclose(file);
	z_dds *dds;
	dds = (z_dds*)data;
	delta = sizeof(z_dds);

	sizeX = dds->dwWidth;
	sizeY = dds->dwHeight;
	sizeZ = dds->dwDepth;
	if(sizeZ==0)sizeZ=1;
	colorPlanes = dds->ddpfPixelFormat.dwRGBBitCount/8;
	sizeLine = colorPlanes*sizeX;
	if( sizeLine%4 ) sizeLine += 4 - sizeLine%4;
	sizePlane = sizeLine*sizeY;
	delta = sizeof(z_dds);
	size = delta + sizeZ*sizePlane;

	return 1;
}

// att:                  out range  0 .5  1    2   5  10
// 0. att = 1 - d2_sat      [0,1]   1 .5  0    0   0   0
// 1. att =(1 - d2_sat)^2   [0,1]   1 .25 0    0   0   0
// 2. att = 1 - d2_sat^0.5  [0,1]   0 .29 0    0   0   0
// 3. att = 1 / (10*d2+0.25)[0,4]   4 .19 .1   .05 .02 0
// 4. att = e ^(-2*d2^0.5)  [0,1]   1 .24 .14  .05 .01 0
// 5. att = e ^(-4*d2)      [0,1]   1 .13 .02  0   0   0
// 6. att = e ^(-4*d2^2)    [0,1]   1 .36 .02  0   0   0
int Image3D::MakeAttenuationTable3D(int size_, int equation)
{
	if(!Make( size_, size_, size_, 3))return 0;
	for(int z=0; z<sizeZ; z++)
	{
		float zf = (float)z/(float)(sizeZ-1);
		for(int y=0; y<sizeY; y++)
		{
			float yf = (float)y/(float)(sizeY-1);
			for(int x=0; x<sizeX; x++)
			{
				float xf = (float)x/(float)(sizeX-1);

				float out;
				float d2 = xf*xf + yf*yf + zf*zf;
				switch(equation)
				{
				case 0: out = 1.f - d2; if(out<0.f)out=0.f; break;
				case 1: out = 1.f - d2; if(out<0.f)out=0.f; out*=out; break;
				case 2: out = 1.f - d2;	if(out<0.f)out=0.f;	out = (float)sqrt(out); break;
				case 3: out = 1.f / (10.f*d2+0.25f); if(out>1.f)out=1.f; break;
				case 4: out = (float)exp(-3.f*sqrt(d2)); break;
				case 5: out = (float)exp(-4.f*d2); break;
				case 6: out = (float)exp(-4.f*d2*d2); break;
				default: out = 1.f - d2;
				}
				unsigned char ch = (unsigned char)(out*255.f);
				set(x,y,z,0,ch);
				set(x,y,z,1,ch);
				set(x,y,z,2,ch);
			}
		}
	}
	return 1;
}
