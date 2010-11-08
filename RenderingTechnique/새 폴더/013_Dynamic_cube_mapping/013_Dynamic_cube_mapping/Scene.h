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
#include "CubeTexture.h"
#include "camera.h"
#include "Util.h"

class Scene  
{
public:
	int GetVariable(char *temp, FILE *sub, char *varname, char *varvalue, int size_strings);
	int ReadInit(char *filename);
	void RenderCubeMaps(vec &vp, mat4 &lm, vec &lpos);
	void SaveScreen_BMP();
	void InitDX();
	void ReSizeDXScene(int width, int height);
	void DrawDXScene();
	int LoadVertexShader( char* filename, DWORD* pDeclaration, DWORD* phVertexShader, char* verprog=NULL);
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha); 
	Scene();
	virtual ~Scene();

	CubeTexture cube;
	CubeTexture cube1;
	C3ds *teapot,*torus;
	C3ds *lietadlo;		// airplane
	SphereVN sphere;
	SphereVN spherelight;
	SkyBox skybox;
	DWORD hVertexShader;
	camera *c;

	D3DLIGHT8 light0;
	Timer	timer0;
	Font_bitmap	*font0;

	int		renderableCubeTexture;
	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int sirka,vyska;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
