// CubeTexture.h: interface for the CubeTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBETEXTURE_H__9889D4E3_6174_11D8_8A6F_B7C6FC994E0A__INCLUDED_)
#define AFX_CUBETEXTURE_H__9889D4E3_6174_11D8_8A6F_B7C6FC994E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "Image.h"

struct z_dds_DDPIXELFORMAT
{
	DWORD dwSize;		// size of structure This member must be set to 32.
	DWORD dwFlags;		// Uncompressed formats will usually use DDPF_RGB to indicate an RGB format
	DWORD dwFourCC;		// This is the four-character code for compressed formats
    DWORD dwRGBBitCount;	// For RGB formats, this is the total number of bits in the format. 
							// This value is usually 16, 24, or 32. For A8R8G8B8, this value would be 32.
	DWORD dwRBitMask;			// For A8R8G8B8, this value would be 0x00ff0000
	DWORD dwGBitMask;			// For A8R8G8B8, this value would be 0x0000ff00
	DWORD dwBBitMask;			// For A8R8G8B8, this value would be 0x000000ff
	DWORD dwRGBAlphaBitMask;	// For A8R8G8B8, this value would be 0xff000000
};

struct z_dds_DDCAPS2
{
	DWORD dwCaps1;	// DDS files should always include DDSCAPS_TEXTURE. If the file contains 
					// mipmaps, DDSCAPS_MIPMAP should be set. For any DDS file with more than 
					// one main surface, such as a mipmaps, cubic environment map, or volume texture, 
					// DDSCAPS_COMPLEX should also be set. 
	DWORD dwCaps2;	// For cubic environment maps, DDSCAPS2_CUBEMAP should be included as well 
					// as one or more faces of the map 
					// (DDSCAPS2_CUBEMAP_POSITIVEX, DDSCAPS2_CUBEMAP_NEGATIVEX, 
					//  DDSCAPS2_CUBEMAP_POSITIVEY, DDSCAPS2_CUBEMAP_NEGATIVEY, 
					//  DDSCAPS2_CUBEMAP_POSITIVEZ, DDSCAPS2_CUBEMAP_NEGATIVEZ). 
					// For volume textures, DDSCAPS2_VOLUME should be included. 
	DWORD Reserved[2];
};

struct z_dds
{
	DWORD dwMagic;
//struct z_dds_Surface_Format_Header{
	DWORD dwSize;		// Size of structure. This member must be set to 124.
	DWORD dwFlags;		// Flags to indicate valid fields. Always include DDSD_CAPS, DDSD_PIXELFORMAT, 
						// DDSD_WIDTH, DDSD_HEIGHT and either DDSD_PITCH or DDSD_LINEARSIZE.
	DWORD dwHeight;		// Height of the main image in pixels 
	DWORD dwWidth;		// Width of the main image in pixels
	DWORD dwPitchOrLinearSize;	// For uncompressed formats, this is the number of bytes per scan line 
						// (DWORD aligned) for the main image. dwFlags should include DDSD_PITCH in this case
	DWORD dwDepth;		// For volume textures, this is the depth of the volume. dwFlags should include DDSD_DEPTH in this case
	DWORD dwMipMapCount;	// For items with mipmap levels, this is the total number of levels in the mipmap chain 
							// of the main image. dwFlags should include DDSD_MIPMAPCOUNT in this case 
	DWORD dwReserved1[11];
	z_dds_DDPIXELFORMAT ddpfPixelFormat;	// A 32-byte value that specifies the pixel format structure. 
	z_dds_DDCAPS2 ddsCaps;					// A 16-byte value that specifies the capabilities structure. 
	DWORD dwReserved2;
};

class CubeTexture  
{
public:
	int load(char *directory, char *extension, int filter=3, int compression=0);
	int load( char* filename);
	void SetTexture(int Stage=0){pd->SetTexture( Stage, pt );}
	LPDIRECT3DCUBETEXTURE8 pt;

	CubeTexture();
	virtual ~CubeTexture();
};

#endif // !defined(AFX_CUBETEXTURE_H__9889D4E3_6174_11D8_8A6F_B7C6FC994E0A__INCLUDED_)
