// Font_bitmap.h: interface for the Font_bitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_BITMAP_H__98417DE1_180D_11D8_8A6F_B34FF1F9B003__INCLUDED_)
#define AFX_FONT_BITMAP_H__98417DE1_180D_11D8_8A6F_B34FF1F9B003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "texture.h"
#include "Matrix.h"
// A structure for our custom vertex type
struct zFont_bitmap_CUSTOMVERTEX
{
	vec v;		// vertex
	vec2 t0;	// texture coord 0
};

// Our custom FVF, which describes our custom vertex structure
//#define D3DFVF_CUSTOMVERTEX_Font_bitmap (D3DFVF_XYZ | D3DFVF_TEX1| D3DFVF_TEXCOORDSIZE2(0))
#define D3DFVF_CUSTOMVERTEX_Font_bitmap (D3DFVF_XYZ | D3DFVF_TEX1| D3DFVF_TEXCOORDSIZE2(0))

class Font_bitmap  
{
public:
	void Print_int(int stlpec, int riadok, int a, char *meno=NULL);
	void Print_xy_scale(int x, int y, char *string, int set, float scale_x, float scale_y);
	void Print_float(int stlpec, int riadok, float x, char *meno=NULL);
	void Print_mat4(int stlpec, int riadok, const mat4 &m, char *nadpis);
	void Print_vec(int stlpec, int riadok, const vec &a, char *meno);
	void SetColor(float r=1.f, float g=1.f, float b=1.f, float a=1.f){color.set(r,g,b,a);}
	void Print(int x, int y, char *string, int set);
	void End();
	void Begin();
	Font_bitmap();
	Font_bitmap(char *file_name);
	virtual ~Font_bitmap();
	texture tex;
	mat4	proj;
	mat4	model;
	vec4  color;

	void *p;
	char text[100];

	LPDIRECT3DVERTEXBUFFER8 pvb;	// Buffer to hold vertices

/*
	void Print_scale(GLint x, GLint y, char * string, int set,float scale_x,float scale_y);
	void Print_xy_scale(GLint x, GLint y, char *string, int set,float scale_x,float scale_y);
	void Print_xy_rot(GLint x, GLint y, char *string, int set,float uhol,float scale);
	void Print_xy(GLint x, GLint y, char * string, int set);
	Font(char *file_name, int *error);
	~Font(void);
	GLvoid glPrint_xy(GLint x, GLint y, char *string, int set);		// pise na OpenGL suradnice x,y
	GLvoid glPrint(GLint x, GLint y, char *string, int set);		// pise na y riadok a x stlpec
	
	// funkcie Begin a End treba pouzivat v paroch !!!!
	void Begin(void);			// nastavi vsetko potrebne pre pisenie pomocou Print
	void Print(GLint x, GLint y, char *string, int set);	// pisanie
	void End(void);				// vypne vsetko co sa zaplo pre pisenie pomocou Print
*/

};

#endif // !defined(AFX_FONT_BITMAP_H__98417DE1_180D_11D8_8A6F_B34FF1F9B003__INCLUDED_)
