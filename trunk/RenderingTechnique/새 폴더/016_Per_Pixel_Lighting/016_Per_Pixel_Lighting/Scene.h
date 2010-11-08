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
#include "CubeTexture.h"
#include "Menu.h"
#include "GLizer.h"
#include "texture3D.h"

#define MODELS_COUNT 3
class Scene  
{
public:
	void DrawQuad(vec p, vec n, vec s, vec t);
	void RenderModel(DWORD hVertexShader, int texturing=1);
	void SaveScreen_BMP();
	void InitDX();
	void ReSizeDXScene(int width, int height);
	void DrawDXScene();
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha);
	Scene();
	virtual ~Scene();

	SphereVN sphere_light;
	camera	*c;
	camera	cam[2];
	C3ds sphere;
	C3ds room01;
	C3ds room02;
	vec	lp[MODELS_COUNT];
	uVertexShader vs10_Diffuse_Specular;
	uVertexShader vs10_Diffuse_Specular_clamp;
	uVertexShader vs10_Diffuse_Specular_with_cubemap_normalizer;
	uVertexShader vs10_attenuation;
	uVertexShader vs10_attenuation_texture_3D;
	uVertexShader vs10_Diffuse_Specular_attenuation;
	uVertexShader vs10_Diffuse_Specular_attenuation_texture_3D;
	uVertexShader vs10_Diffuse_Specular_attenuation_for_ps11;
	uVertexShader vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11;
	uVertexShader vs10_Diffuse_Specular_attenuation_for_ps11_cn;
	uVertexShader vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11_cn;

	uPixelShader ps11_1_AD;
	uPixelShader ps11_2_AD_cube_normalizer;
	uPixelShader ps11_3_ADS16;
	uPixelShader ps11_4_ADS16_cube_normalizer;
	uPixelShader ps11_5_ADSn;
	uPixelShader ps11_7_attenuation_1;
	uPixelShader ps11_7_attenuation_2;
	uPixelShader ps11_7_attenuation_lookup;
	uPixelShader ps11_8_ADS16_attenuation_1;
	uPixelShader ps11_8_ADS16_attenuation_2;
	uPixelShader ps12_8_ADS16_attenuation_lookup;
	uPixelShader ps11_8_ADS16_attenuation_texture_3D;
	uPixelShader ps11_9_ADS16_cn_attenuation_1;
	uPixelShader ps11_9_ADS16_cn_attenuation_2;
	uPixelShader ps11_9_ADS16_cn_attenuation_texture_3D;

	uPixelShader ps14_1_AD;
	uPixelShader ps14_2_AD_cube_normalizer;
	uPixelShader ps14_3_ADS16;
	uPixelShader ps14_4_ADS16_cube_normalizer;
	uPixelShader ps14_5_ADSn;
	uPixelShader ps14_6_ADSn_cube_normalizer;
	uPixelShader ps14_7_attenuation_1;
	uPixelShader ps14_7_attenuation_2;
	uPixelShader ps14_7_attenuation_lookup;
	uPixelShader ps14_8_ADS16_attenuation_1;
	uPixelShader ps14_8_ADS16_attenuation_2;
	uPixelShader ps14_8_ADS16_attenuation_lookup;
	uPixelShader ps14_8_ADS16_attenuation_texture_3D;
	uPixelShader ps14_9_ADS16_cn_attenuation_1;
	uPixelShader ps14_9_ADS16_cn_attenuation_2;
	uPixelShader ps14_9_ADS16_cn_attenuation_lookup;
	uPixelShader ps14_9_ADS16_cn_attenuation_texture_3D;

	CubeTexture normalizer;
	int normalizer_size;

	texture specTable_1D;
	float specTable_1D_exponet;
	texture specTable_NLNH_RGBA;
	float specTable_NLNH_RGBA_exponet;
	int table_size;

	texture attenuation1D[7];
	int table_size_attenuation_1D;

	texture3D tex3D_1to1;
	texture3D attenuation3D[7];
	int table_size_attenuation_3D;

	int ps11;
	int ps12;
	int ps14;
	
	int supp_texture3D;

	Menu menu[2];
	int smenu;
	struct setting
	{
		int language_switch;
		int model;
		int mode;
		int ps14_selected;
		float specExponent;
		int table_size;
		int normalizer_size;
		float light_range;
		int attenuation_equation;
		int attenuation_technique;
		int table_size_attenuation_1D;
		int table_size_attenuation_3D;
		int show_textures;
		int set_light_pos;
		int wire_frame;
		int light_rotate;
		float light_rotate_speed;
		vec4 ambient,diffuse,specular;
		int anisotropic_filter;
		int print_variable;
	}set;
	int anisotropic_filter_actual;

	POINT mouse_pos;
	D3DLIGHT8 light0;
	Timer	timer0;
	Font_bitmap	*font0;

	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int window_size_x,window_size_y;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
