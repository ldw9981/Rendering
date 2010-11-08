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
#include "SphereVN.h"
#include "SkyBox.h"
#include "Util.h"

class Scene  
{
public:
	void SaveScreen_BMP();
	void ChangeCameraPosition();
	void InitDX();
	void ReSizeDXScene(int width, int height);
	void DrawDXScene();
	int LoadVertexShader( char* filename, DWORD* pDeclaration, DWORD* phVertexShader, char* verprog=NULL);
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha); 
	Scene();
	virtual ~Scene();

	C3ds *teapot,*torus;
	SphereVN sphere;
	SkyBox skybox;
	DWORD hVertexShader;

	D3DLIGHT8 light0;
	Timer	timer0;
	Font_bitmap	*font0;
	texture sphere_map;

	struct camera
	{
		float angle_x,angle_y;
		float pos_x,pos_y,pos_z;
		POINT MouseBod;
	}c;

	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int sirka,vyska;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
