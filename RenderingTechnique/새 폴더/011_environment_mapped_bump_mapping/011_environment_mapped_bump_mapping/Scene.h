// Scene.h: interface for the Scene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
#define AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"
#include "Matrix.h"
#include "Font_bitmap.h"
#include "timer.h"
#include "texture.h"
#include "3ds.h"
#include "camera.h"
#include "SphereVN.h"
#include "Util.h"

class Scene  
{
public:
	void Print_mat4(int stlpec, int riadok, const mat4 &m, char* nadpis=NULL);
	void Print_vec(int stlpec, int riadok, const vec &a, char* meno=NULL);
	int LoadVertexShader( char* filename, DWORD* pDeclaration, DWORD* phVertexShader, char* verprog=NULL);
	void SaveScreen_BMP();
	void InitDX();
	void ReSizeDXScene(int width, int height);
	void DrawDXScene();
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha);
	Scene();
	virtual ~Scene();

	texture sphere_map;
	texture EMBMbase,EMBMbump;
	camera	*c;
	vector<C3ds*> obj;

	vector<float> obj_filter_strong;		// pre nastavovanie filtra
	vector<texture*> obj_bump_rgb;			// pre vykreslenie normalovej mapy
	vector<Image*> obj_bump_image;			// pre vytvorenie normalovej mapy pocas behu programu

	DWORD hVertexShader;

	D3DLIGHT8 light0;
	Timer	timer0;
	Font_bitmap	*font0;

	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int sirka,vyska;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
