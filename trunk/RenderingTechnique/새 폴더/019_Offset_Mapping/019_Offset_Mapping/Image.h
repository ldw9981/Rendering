// Image.h: interface for the Image class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__B2067222_405E_11D8_8A6F_896871ADD523__INCLUDED_)
#define AFX_IMAGE_H__B2067222_405E_11D8_8A6F_896871ADD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"

// jpeglibmm
int LoadJPG(const char *filename, int *planes, int *sizeX, int *sizeY, unsigned char **data);

class Image  
{
public:
	int Load(char *filename);
	int LoadNoMessage(char *filename);
	void Set( const Image &in);
	int SaveBMP(char *filename);
	int SaveTGA(char *filename);
	unsigned char* Get( int *size);
	unsigned char* Get_dds(int *size);

	int LoadBMP(char *filename);
private:
	int LoadBMP1bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	int LoadBMP4bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	int LoadBMP8bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
public:
	float getPointBilinear(float x, float y, int plane);
	int setPlanes(int count, int copy_from_plane=0);
	int MakeExponentTable2D_NL_NH(int size_x, int size_y, float exp, int channels);
	int MakeExponentTable2D(int size_x, int size_y, float min_exp);
	int MakeExponentTable1D(int size_x, float exp);
	int MakeAttenuationTable1D(int size_x, int equation);
	int Make(int size_x, int size_y, int planes);
	int LoadJPG(char *filename);
	int LoadTGA(char *filename);
	unsigned char getPoint( int x, int y, int plane){return data[(sizeX*y+x)*planes+plane];}
	void setPoint( int x, int y, int plane, unsigned char ch){data[(sizeX*y+x)*planes+plane]=ch;}

	void RGBtoBGR();
	void BGRtoRGB();
	void shift();
	void invShift();
	int resize(int newx, int newy);
	int resizeToHalfX();
	int resizeToHalfY();
	int resizeToHalfXY();
	void Flip();
	void FlipMirror();			// up - down, right - left
	unsigned char getSavePoint( int x, int y, int plane);
	float evaluateMask(  int x, int y,	int plane,	\
								float m11, float m12, float m13, \
								float m21, float m22, float m23, \
								float m31, float m32, float m33 );
	void Filter( int maska=0, int plane=0, float strong=1.0f);

	int planes;					// pocet rovin
	int sizeX;					// sirka obrazka
	int sizeY;					// vyska obrazka
	unsigned char *data;		// smernik na data
	char isBGR;					// nastavene na 1, ak nacitane data su bgr
	Image();
	Image(const Image &in);
	virtual ~Image();
};


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

#define DDSD_CAPS               0x00000001L
#define DDSD_HEIGHT             0x00000002L
#define DDSD_WIDTH              0x00000004L
#define DDSD_PITCH              0x00000008L
#define DDSD_PIXELFORMAT        0x00001000L
#define DDSD_MIPMAPCOUNT        0x00020000L
#define DDSD_LINEARSIZE         0x00080000L
#define DDSD_DEPTH              0x00800000L


#define DDPF_ALPHAPIXELS        0x00000001L
#define DDPF_FOURCC             0x00000004L
#define DDPF_RGB                0x00000040L

#define DDSCAPS_COMPLEX         0x00000008L
#define DDSCAPS_TEXTURE         0x00001000L
#define DDSCAPS_MIPMAP          0x00400000L

#define DDSCAPS2_CUBEMAP           0x00000200L
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400L
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800L
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000L
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000L
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000L
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000L
#define DDSCAPS2_VOLUME            0x00200000L


#endif // !defined(AFX_IMAGE_H__B2067222_405E_11D8_8A6F_896871ADD523__INCLUDED_)
