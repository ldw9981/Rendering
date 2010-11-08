#ifndef _SCENE_H
#define _SCENE_H

#include "init.h"
#include "Font_bitmap.h"
#include "timer.h"
#include "texture.h"
//#include "3ds.h"
#include "AsePpl.h"
#include "AsePplVso.h"
#include "camera.h"
#include "SphereVN.h"
#include "Util.h"
#include "CubeTexture.h"
#include "Menu.h"
#include "GLizer.h"
#include "texture3D.h"
#include <stdlib.h>		// for rand
#include <time.h>		// for rand
#include "Plane_VNTTB.h"


#define	WindowName "019_Offset_Mapping"

struct z_Light_fly
{
	vec lpos;			// light position in object space
	vec color;
	float light_range;	// = 1.f/light_range
	vec dir;
	float time;
};

class Scene  
{
public:
	void DrawRoom();
	void DrawPlane();
	Scene();
	int Init();
	int InitMenu();
	int InitLoadModels();
	int InitLoadTextures();
	virtual ~Scene();

	void ReSizeDXScene(int width, int height);
	void InitDX();
	void SaveScreen_BMP();
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha);
	void DrawQuad(vec p, vec n, vec s, vec t);
	void RestoreLostDevice();

	void DrawDXScene();
	void CheckColosion();
	inline void ProcessKeyForMenu();
	inline void SetAnisotropicFilter();
	inline void DrawHelp();

	struct textures
	{
		texture base, normal_map, height, height_for_ps11;
		Plane_VNTTB plane_DisplacementMapping;
	}tex[2];
	Plane_VNTTB plane;

	SphereVN sphere_light;
	camera	*c;
	camera  cplane,croom;
	AsePpl room;
	AsePplVso room_vso;
	AseFile *pAseForCollision;
	vector<z_Light_fly> shot_lights;

	vector<z_face> collision;

	// plane
	uVertexShader vs11_for_ps11_bump;
	uVertexShader vs11_for_ps11_OffsetMapping;
	uVertexShader vs11_for_ps11_bump_OffsetMapping_1_pass;
	uVertexShader vs11_for_ps11_bump_OffsetMapping_2_pass;
	uVertexShader vs11_for_ps11_bump_OffsetMapping_3_pass;
	uVertexShader vs11_for_ps14;

	uPixelShader ps11_bump;
	uPixelShader ps11_bump_OffsetMapping_1_pass;
	uPixelShader ps11_bump_OffsetMapping_2_pass;
	uPixelShader ps11_bump_OffsetMapping_3_pass;
	uPixelShader ps11_OffsetMapping;
	
	uPixelShader ps14_bump;
	uPixelShader ps14_bump_OffsetMapping;
	uPixelShader ps14_OffsetMapping;

	// room
	uVertexShader vs11_room_ps11;
	uVertexShader vs11_room_ps11_with_optimalization;
	uVertexShader vs11_room_ps14;
	uVertexShader vs11_room_ps14_with_optimalization;
	uVertexShader vs11_room_for_AsePpl_class;
	
	uPixelShader ps11_room;
	uPixelShader ps11_room_with_ambient;

	uPixelShader ps14_room;
	uPixelShader ps14_room_with_ambient;
	uPixelShader ps14_room_OffsetMapping;
	uPixelShader ps14_room_OffsetMapping_with_ambient;
	uPixelShader ps14_room_OffsetMapping_for_AsePpl_class;
	uPixelShader ps14_room_OffsetMapping_for_AsePpl_class_with_ambient;

	CubeTexture normalizer;
	int normalizer_size;

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
		int bump_mapping;
		int offset_mapping;		// only for room
		int ps14_selected;
		float height_scale;
		int actual_texture;
		int set_light_position;	// only for plane
		int use_AsePplVso_class;  // only for room
		int max_count_of_lights; // only for room
		int static_lights;
		vec4 ambient,diffuse,specular;
		int normalizer_size;
		int show_lights;
		int wire_frame;
		int cull_face;
		int lights_moving;
		float shot_light_speed;
		float shot_light_range;
		float shot_per_second;
		float time_since_last_shot;
		int anisotropic_filter;
		int print_statistic;
		int print_variable;
		// collision
		int		collision, gravitation, move_during_jump, jump_durind_jump;
		float	gravity_acceleration, jump_height, sphere_radius;
	}set;
	int anisotropic_filter_actual;

	POINT mouse_pos;
	D3DLIGHT8 light0;
	Timer	timer0;
	Font_bitmap	font0;

	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int window_size_x,window_size_y;
};

#endif
