// Image3D.h: interface for the Image3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE3D_H__121AE362_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_)
#define AFX_IMAGE3D_H__121AE362_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "Image.h"

class Image3D  
{
public:
	int MakeAttenuationTable3D(int size_, int equation);
	int Load_dds(char* filename);
	int Save_dds(char *filename);
	void Set_dds();
	int Make1to1(int size_x, int size_y, int size_z);
	int Make(int size_x, int size_y, int size_z, int color_planes=3);
	inline unsigned int getAdr(int x, int y, int z, int color)
	{	
		if(color==0)color=2;			// BGR format in data, variable color use RGB format
		else if(color==2)color=0;
		return delta + color + x*colorPlanes + y*sizeLine + z*sizePlane;
	}
	inline unsigned char get(int x, int y, int z, int color)
	{	return data[getAdr(x,y,z,color)];}
	inline void set(int x, int y, int z, int color, unsigned char val)
	{	data[getAdr(x,y,z,color)] = val;}
	Image3D();
	virtual ~Image3D();

	int colorPlanes;		// I use 3, it is BGR
	int sizeX;
	int sizeY;
	int sizeZ;
	int sizeLine;			// = colorPlanes*sizeX, must be: sizeLine%4==0
	int sizePlane;			// = sizeLine*sizeY
	int size;				// = delta + sizeZ*sizePlane
	int delta;				// delta from begin data buffer to color data
	unsigned char *data;	// BGR format
};

#endif // !defined(AFX_IMAGE3D_H__121AE362_F515_11D8_8A6F_E5A094FDBB04__INCLUDED_)
