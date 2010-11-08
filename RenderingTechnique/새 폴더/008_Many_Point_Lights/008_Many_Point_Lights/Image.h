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
	void Set( const Image &in);
	int SaveBMP(char *filename);
	unsigned char* Get( int *size);
	
	int LoadBMP(char *filename);
private:
	int LoadBMP1bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	int LoadBMP4bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	int LoadBMP8bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
public:
	int LoadJPG(char *filename);
	int LoadTGA(char *filename);

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

#endif // !defined(AFX_IMAGE_H__B2067222_405E_11D8_8A6F_896871ADD523__INCLUDED_)
