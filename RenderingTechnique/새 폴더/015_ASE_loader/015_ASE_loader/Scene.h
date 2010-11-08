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
#include "camera.h"
#include "Util.h"
#include "ase.h"
#include "Menu.h"

class Scene  
{
public:
	int ReadInit(char *filename);
	int GetVariable(char *temp, FILE *sub, char *varname, char *varvalue, int size_strings);
	void SaveScreen_BMP();
	void InitDX();
	void ReSizeDXScene(int width, int height);
	void DrawDXScene();
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha); 
	Scene();
	virtual ~Scene();

	Case	room;
	vector<z_face> collision;
	Menu	menu[2];
	int smenu;
	struct setting
	{
		int		help, baset, lmapt, light_update, cull_face, print_variable, use_multitexturing;
		int		texturing, material, line, lighting, rendernormals;
		int		collision, collision_old, gravitation, move_during_jump, jump_durind_jump, language_switch;
		float	gravity_acceleration, jump_height, sphere_radius, time_delay;
		int		anisotropic_filter;
	}set;
	int anisotropic_filter_actual;

	camera *c;

	Timer	timer0;
	Font_bitmap	*font0;
	D3DLIGHT8 light0;

	int		renderableCubeTexture;
	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int sirka,vyska;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
