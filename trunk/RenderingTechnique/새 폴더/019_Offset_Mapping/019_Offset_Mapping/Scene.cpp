// Scene.cpp: implementation of the Scene class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene.h"

DWORD dwDecl[] =
{
    D3DVSD_STREAM(0),
    D3DVSD_REG(0, D3DVSDT_FLOAT3),      // the position data
    D3DVSD_REG(1, D3DVSDT_FLOAT3),		// normal
	D3DVSD_REG(2, D3DVSDT_FLOAT2),		// texture coord 0
	D3DVSD_REG(3, D3DVSDT_FLOAT3),		// Tangent
	D3DVSD_REG(4, D3DVSDT_FLOAT3),		// Binormal
    D3DVSD_END()
};

int Scene::window_size_x,Scene::window_size_y;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene::Scene()
{
	pvb_help_rectange = NULL;
}

int Scene::Init()
{
	smenu = 1;
	if(!smenu)ShowCursor(0);

	if( HIBYTE(LOWORD(caps.PixelShaderVersion))==0 ||
	   (HIBYTE(LOWORD(caps.PixelShaderVersion))==1 && LOBYTE(LOWORD(caps.PixelShaderVersion))==0) )
		ps11 = 0;
	else
		ps11 = 1;
	
	if( HIBYTE(LOWORD(caps.PixelShaderVersion))==0 ||
	   (HIBYTE(LOWORD(caps.PixelShaderVersion))==1 && LOBYTE(LOWORD(caps.PixelShaderVersion))<2) )
		ps12 = 0;
	else
		ps12 = 1;

	if( HIBYTE(LOWORD(caps.PixelShaderVersion))==0 ||
	   (HIBYTE(LOWORD(caps.PixelShaderVersion))==1 && LOBYTE(LOWORD(caps.PixelShaderVersion))<4) )
		ps14 = 0;
	else
		ps14 = 1;

	if(!ps11)
	{ 	MessageBox( hWnd, "Program need pixel shader 1.1", "ERROR", MB_OK); return 0; }

	if(caps.TextureCaps&D3DPTEXTURECAPS_VOLUMEMAP )supp_texture3D = 1;
	else supp_texture3D=0;

	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	srand( (unsigned)time( NULL ) );

	InitDX();

	if(!font0.Init("data/font.bmp"))return 0;

	// load vertex shaders
	if(!vs11_for_ps11_bump.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps11_bump.txt", dwDecl))return 0;
	if(!vs11_for_ps11_bump_OffsetMapping_1_pass.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps11_bump_OffsetMapping_1_pass.txt", dwDecl))return 0;
	if(!vs11_for_ps11_bump_OffsetMapping_2_pass.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps11_bump_OffsetMapping_2_pass.txt", dwDecl))return 0;
	if(!vs11_for_ps11_bump_OffsetMapping_3_pass.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps11_bump_OffsetMapping_3_pass.txt", dwDecl))return 0;
	if(!vs11_for_ps11_OffsetMapping.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps11_OffsetMapping.txt", dwDecl))return 0;
	if(!vs11_for_ps14.LoadVS( "Shaders/019_Offset_Mapping/plane/vs11_for_ps14.txt", dwDecl))return 0;
	
	if(!vs11_room_ps11.LoadVS( "Shaders/019_Offset_Mapping/room/vs11_room_ps11.txt", AsePplVso::dwDecl))return 0;
	if(!vs11_room_ps11_with_optimalization.LoadVS( "Shaders/019_Offset_Mapping/room/vs11_room_ps11_with_optimalization.txt", AsePplVso::dwDecl))return 0;
	if(!vs11_room_ps14.LoadVS( "Shaders/019_Offset_Mapping/room/vs11_room_ps14.txt", AsePplVso::dwDecl))return 0;
	if(!vs11_room_ps14_with_optimalization.LoadVS( "Shaders/019_Offset_Mapping/room/vs11_room_ps14_with_optimalization.txt", AsePplVso::dwDecl))return 0;
	if(!vs11_room_for_AsePpl_class.LoadVS( "Shaders/019_Offset_Mapping/room/vs11_room_for_AsePpl_class.txt", AsePpl::dwDecl))return 0;

	// load pixel shaders
	if(!ps11_bump.LoadPS( "Shaders/019_Offset_Mapping/plane/ps11_bump.txt"))return 0;
	if(!ps11_bump_OffsetMapping_1_pass.LoadPS( "Shaders/019_Offset_Mapping/plane/ps11_bump_OffsetMapping_1_pass.txt"))return 0;
	if(!ps11_bump_OffsetMapping_2_pass.LoadPS( "Shaders/019_Offset_Mapping/plane/ps11_bump_OffsetMapping_2_pass.txt"))return 0;
	if(!ps11_bump_OffsetMapping_3_pass.LoadPS( "Shaders/019_Offset_Mapping/plane/ps11_bump_OffsetMapping_3_pass.txt"))return 0;
	if(!ps11_OffsetMapping.LoadPS( "Shaders/019_Offset_Mapping/plane/ps11_OffsetMapping.txt"))return 0;
	if(!ps11_room.LoadPS( "Shaders/019_Offset_Mapping/room/ps11_room.txt"))return 0;
	if(!ps11_room_with_ambient.LoadPS( "Shaders/019_Offset_Mapping/room/ps11_room_with_ambient.txt"))return 0;

	if(ps14)
	{
		if(!ps14_bump.LoadPS( "Shaders/019_Offset_Mapping/plane/ps14_bump.txt"))return 0;
		if(!ps14_bump_OffsetMapping.LoadPS( "Shaders/019_Offset_Mapping/plane/ps14_bump_OffsetMapping.txt"))return 0;
		if(!ps14_OffsetMapping.LoadPS( "Shaders/019_Offset_Mapping/plane/ps14_OffsetMapping.txt"))return 0;
		if(!ps14_room.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room.txt"))return 0;
		if(!ps14_room_with_ambient.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room_with_ambient.txt"))return 0;
		if(!ps14_room_OffsetMapping.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room_OffsetMapping.txt"))return 0;
		if(!ps14_room_OffsetMapping_with_ambient.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room_OffsetMapping_with_ambient.txt"))return 0;
		if(!ps14_room_OffsetMapping_for_AsePpl_class.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room_OffsetMapping_for_AsePpl_class.txt"))return 0;
		if(!ps14_room_OffsetMapping_for_AsePpl_class_with_ambient.LoadPS( "Shaders/019_Offset_Mapping/room/ps14_room_OffsetMapping_for_AsePpl_class_with_ambient.txt"))return 0;
	}

	if(!InitLoadTextures())return 0;
	if(!InitLoadModels())return 0;
	if(!InitMenu())return 0;

	c = &cplane;
	c->set.k_remap_mouse_button_vd_vpd=1;
	c->set_q3_mouse_style();
	c->setKeys();
//	c->reset.vp.set(0,15,11);
//	c->reset.vd.set(0,0,-1); c->reset.vd.RotX(-45.f);
//	c->reset.vr.set(1,0,0);
//	c->reset.vu.set(0,1,0); c->reset.vu.RotX(-45.f);
	c->reset.vp.set(0,0,0);
	c->reset.vcp.set(0,0,0);
	c->reset.vpd = 10.f;
	mouse_pos = c->last;
	c->Reset();
	c->MoveWorld(fps, ROTATE_WALK|ROTATE_ABOUT_POINT);

	c = &croom;
	c->set.k_remap_mouse_button_vd_vpd=0;
	c->set.use_mouse_button = 0;
	c->set_q3_mouse_style();
	c->setKeys();
//	c->reset.vp.set(0,15,11);
//	c->reset.vd.set(0,0,-1); c->reset.vd.RotX(-45.f);
//	c->reset.vr.set(1,0,0);
//	c->reset.vu.set(0,1,0); c->reset.vu.RotX(-45.f);
	c->reset.vp.set(0,1.2f,1);
//	c->reset.vp.set(0,0.5f,0);
	c->reset.vcp.set(0,0,0);
	c->reset.vpd = 10.f;
	mouse_pos = c->last;
	c->Reset();
	c->MoveWorld(fps, ROTATE_WALK);
	return 1;
}

int Scene::InitLoadTextures()
{
	Image im;
//	char* base_name = "data/rockwall.tga";
//	char* height_name = "data/rockwall_height.bmp";
//	char* base_name = "data/brick.tga";
//	char* height_name = "data/brick_height.bmp";

	char* base_name;
	char* height_name;

	for( int index=0; index<2; index++)
	{
		switch(index)
		{
		case 0:
				base_name = "data/ZanirBrick.JPG";
				height_name = "data/ZanirBrickBump.JPG";
				break;
		case 1:
				base_name = "data/019dxmodel/Fieldstone.JPG";
				height_name = "data/019dxmodel/FieldstoneBump.JPG";
				break;
		}
		if(!tex[index].base.load(base_name))return 0;
		if(!tex[index].height.load(height_name))return 0;
		if(!im.Load(height_name))return 0;
		im.Filter();
		if(!tex[index].normal_map.load(im))return 0;

		if(!im.Load(height_name))return 0;
		im.BGRtoRGB();
		im.setPlanes(3,0);		// set count of planes to 3
		for(int y=0; y<im.sizeY; y++)
		{
			for(int x=0; x<im.sizeX; x++)
			{
				im.setPoint(x,y,1,255);
				im.setPoint(x,y,2,255);
			}
		}
		if(!tex[index].height_for_ps11.load(im))return 0;
	}

	normalizer_size = 64;
	if(!normalizer.MakeCubeMapNormalizer(normalizer_size))return 0;
	return 1;
}

int Scene::InitLoadModels()
{
	if(!CreateVertexBufferForHelpRectange(10,screen_y_font-45,screen_x_font-80,230,0.6f))return 0;

	if(!room.Init("data/019dxmodel/room1.ase",0,1,0.1f,1))
	{
		MessageBox(hWnd,"data/019dxmodel/room1.ase","ERROR",MB_OK);
		return 0;
	}
	if(!room_vso.Init("data/019dxmodel/room1.ase",0,1,0.1f,1))
	{
		MessageBox(hWnd,"data/019dxmodel/room1.ase","ERROR",MB_OK);
		return 0;
	}

	char* height_name;
	for( int index=0; index<2; index++)
	{
		switch(index)
		{
		case 0:	height_name = "data/ZanirBrickBump.JPG"; break;
		case 1:	height_name = "data/019dxmodel/FieldstoneBump.JPG"; break;
		}
		if(!tex[index].plane_DisplacementMapping.Init_Displacement_Mapping( 
			  vec(-5,-5,0), vec(10,0,0), vec(0,10,0), vec(0,0,1), 
			  vec2(0,1), vec2(1,-1), 200, 200, height_name, 0.2f, -0.1f))return 0;
	}
	if(!plane.Init( vec(-5,-5,0), vec(10,0,0), vec(0,10,0),vec2(0,1), vec2(1,-1), 4, 4))return 0;


	if(!sphere_light.Init(10,20,0.06f))return 0;
	return 1;
}

int Scene::InitMenu()
{
	int num,lang;
	// menu 0 - english
	// menu 1 - slovak

	menu[0].Init(&font0);
	menu[1].Init(&font0);
	menu[0].SetShowCondition( 3, &set.model);		// 3 show if model is 1 or 2, it is always
	menu[1].SetShowCondition( 3, &set.model);

	menu[0].AddText( "F1 - Show/hide menu            H - help");
	menu[1].AddText( "F1 - Zobrazenie/skrytie menu   H - help");
	menu[0].AddText(""); menu[1].AddText("");

	set.model = 1;
//	menu[0].AddText(""); menu[1].AddText("");
	menu[0].AddInt("model",'M', &set.model);
	num = 1; menu[0].AddIntItem("model: plane", &num);
	num = 2; menu[0].AddIntItem("model: room", &num);
	menu[0].AddIntSetIndex(1);

	menu[1].AddInt("model",'M', &set.model);
	num = 1; menu[1].AddIntItem("model: rovina", &num);
	num = 2; menu[1].AddIntItem("model: miestnost", &num);
	menu[1].AddIntSetIndex(1);

	menu[0].SetShowCondition(1);	// if model is plane
	menu[1].SetShowCondition(1);
	set.mode=1;
	for( lang=0; lang<2; lang++)
	{
		set.mode=1;
		menu[lang].AddInt("mode",-1, &set.mode);
		num=1; menu[lang].AddIntItem("1. Base map",&num);
		num++; menu[lang].AddIntItem("2. Offset Mapping", &num);
		num++; menu[lang].AddIntItem("3. Displacement Mapping", &num);
		menu[lang].AddIntSetIndex(1);
	}
	menu[0].AddBool( "bump mapping", 'B', &set.bump_mapping, 1);
	menu[1].AddBool( "bump mapping", 'B', &set.bump_mapping, 1);

	menu[0].SetShowCondition(2);	// if model is room
	menu[1].SetShowCondition(2);
	menu[0].AddBool( "Offset Mapping", 'O', &set.offset_mapping, 1);
	menu[1].AddBool( "Offset Mapping", 'O', &set.offset_mapping, 1);
	if(!ps14)set.offset_mapping=0;

	menu[0].SetShowCondition(3);	// always
	menu[1].SetShowCondition(3);
	if(ps14)
	{
		menu[0].AddBool( "pixel shader 1.4", 'V', &set.ps14_selected, 1);
		menu[1].AddBool( "pixel shader 1.4", 'V', &set.ps14_selected, 1);
	}
	else
		set.ps14_selected = 0;

	menu[0].AddFloatComp( "height scale in %", -1, &set.height_scale, 0.f, 20.f, 4.f);
	menu[1].AddFloatComp( "zmensenie pre vyskovu mapu v %", -1, &set.height_scale, 0.f, 20.f, 4.f);

	menu[0].SetShowCondition(1);	// if model is plane
	menu[1].SetShowCondition(1);
	for( lang=0; lang<2; lang++)
	{
		set.actual_texture=1;
		menu[lang].AddInt("mode",'Y', &set.actual_texture);
		num=0; menu[lang].AddIntItem(lang?"texture 1":"textura 1", &num);
		num++; menu[lang].AddIntItem(lang?"texture 2":"textura 2", &num);
		menu[lang].AddIntSetIndex(1);
	}

	menu[0].AddKey("set light position",'U',&set.set_light_position);
	menu[1].AddKey("nastav poziciu svetla",'U',&set.set_light_position);
	
	menu[0].SetShowCondition(2);	// if model is room
	menu[1].SetShowCondition(2);
	
	menu[0].AddInt("technique",-1,&set.use_AsePplVso_class);
	num=0; menu[0].AddIntItem("use AsePpl class",&num);
	num=1; menu[0].AddIntItem("use AsePplVso class",&num);
	menu[0].AddIntSetIndex(0);
	menu[1].AddInt("technique",-1,&set.use_AsePplVso_class);
	num=0; menu[1].AddIntItem("pouzivam AsePpl triedu",&num);
	num=1; menu[1].AddIntItem("pouzivam AsePplVso triedu",&num);
	menu[1].AddIntSetIndex(0);

	menu[0].AddRangeInt("max count of lights",'L',&set.max_count_of_lights,1,100,1,40);
	menu[1].AddRangeInt("maximalny pocet svetiel",'L',&set.max_count_of_lights,1,100,1,40);

	menu[0].AddBool( "static lights", 'I', &set.static_lights, 1);
	menu[1].AddBool( "staticke svetla", 'I', &set.static_lights, 1);

	menu[0].SetShowCondition(3);	// always
	menu[1].SetShowCondition(3);
	menu[0].AddText(""); menu[1].AddText("");
	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddFloatComp( "ambient R", 'Z', &set.ambient.x, 0.f, 1.0f, 0.3f);
		menu[lang].AddFloatComp( "ambient G", 'Z', &set.ambient.y, 0.f, 1.0f, 0.3f);
		menu[lang].AddFloatComp( "ambient B", 'Z', &set.ambient.z, 0.f, 1.0f, 0.3f);
	}
	
	menu[0].SetShowCondition(1);	// if model is plane
	menu[1].SetShowCondition(1);
	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddFloatComp( "diffuse R", 'X', &set.diffuse.x, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "diffuse G", 'X', &set.diffuse.y, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "diffuse B", 'X', &set.diffuse.z, 0.f, 1.0f, 1.f);

		menu[lang].AddFloatComp( "specular R", 'F', &set.specular.x, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "specular G", 'F', &set.specular.y, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "specular B", 'F', &set.specular.z, 0.f, 1.0f, 1.f);
	}
	menu[0].SetShowCondition(3);	// always
	menu[1].SetShowCondition(3);
	menu[0].AddText(""); menu[1].AddText("");
	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddInt("cube normalizer size",-1,&set.normalizer_size);
		num =  2; menu[lang].AddIntItem("cube map normalizer size = 2", &num);
		num =  4; menu[lang].AddIntItem("cube map normalizer size = 4", &num);
		num =  8; menu[lang].AddIntItem("cube map normalizer size = 8", &num);
		num = 16; menu[lang].AddIntItem("cube map normalizer size = 16", &num);
		num = 32; menu[lang].AddIntItem("cube map normalizer size = 32", &num);
		num = 64; menu[lang].AddIntItem("cube map normalizer size = 64", &num);
		num =128; menu[lang].AddIntItem("cube map normalizer size = 128", &num);
		num =256; menu[lang].AddIntItem("cube map normalizer size = 256", &num);
		num =512; menu[lang].AddIntItem("cube map normalizer size = 512", &num);
		menu[lang].AddIntSetIndex(5);
	}
	
	menu[0].SetShowCondition(2);	// if model is room
	menu[1].SetShowCondition(2);
	menu[0].AddBool( "Collision", 'K', &set.collision, 1);
	menu[1].AddBool( "Kolizia", 'K', &set.collision, 1);

	menu[0].AddBool( "Gravitation", 'G', &set.gravitation, 0);
	menu[1].AddBool( "Gravitacia", 'G', &set.gravitation,  0);
	set.move_during_jump = 1;
	set.jump_durind_jump = 0;
	set.gravity_acceleration = 9.81f;
	set.jump_height = 1.5f;
	set.sphere_radius = 0.5f;

	menu[0].AddBool( "lights moving", 'J', &set.lights_moving, 1);
	menu[1].AddBool( "pohyb svetiel", 'J', &set.lights_moving, 1);

	menu[0].AddFloatComp( "shot light speed", -1, &set.shot_light_speed, 0.1f, 50.f, 5.f);
	menu[1].AddFloatComp( "rychlost vystrelenych svetiel", -1, &set.shot_light_speed, 0.1f, 50.f, 5.f);
	
	menu[0].AddFloatComp( "shot light range", -1, &set.shot_light_range, 0.5f, 20.f, 3.f);
	menu[1].AddFloatComp( "dosah vystrelenych svetiel", -1, &set.shot_light_range, 0.5f, 20.f, 3.f);
	
	menu[0].AddFloatComp( "shots per second", -1, &set.shot_per_second, 0.5f, 100.f, 10.f);
	menu[1].AddFloatComp( "pocet vystrelenych svetiel za sekundu", -1, &set.shot_per_second, 0.5f, 100.f, 10.f);

	set.time_since_last_shot = 0;
	menu[0].AddBool( "show lights", -1, &set.show_lights, 1);
	menu[1].AddBool( "kresli svetla", -1, &set.show_lights, 1);

	menu[0].SetShowCondition(3);	// always
	menu[1].SetShowCondition(3);
	menu[0].AddBool( "wire frame", 'P', &set.wire_frame, 0);
	menu[1].AddBool( "kresli ciary", 'P', &set.wire_frame, 0);

	menu[0].AddBool( "Cull face", 'C', &set.cull_face, 1);
	menu[1].AddBool( "Cull face", 'C', &set.cull_face, 1);

	menu[0].AddBool( "print statistic", 'T', &set.print_statistic, 0);
	menu[1].AddBool( "vypisuj statistiku", 'T', &set.print_statistic, 0);

	menu[0].AddBool( "print variable", -1, &set.print_variable, 0);
	menu[1].AddBool( "vypisuj premenne", -1, &set.print_variable, 0);

	set.anisotropic_filter=0;
	anisotropic_filter_actual = 0;
	if(caps.RasterCaps&D3DPRASTERCAPS_ANISOTROPY)
	{
		for( lang=0; lang<2; lang++)
		{
			int val;
			menu[lang].AddInt("anisotropic_filter", -1, &set.anisotropic_filter);
			val = 0; menu[lang].AddIntItem("anisotropic filter: OFF", &val);
			if(caps.MaxAnisotropy>=2){ val = 2; menu[lang].AddIntItem("anisotropic filter: 2", &val);}
			if(caps.MaxAnisotropy>=4){ val = 4; menu[lang].AddIntItem("anisotropic filter: 4", &val);}
			if(caps.MaxAnisotropy>=8){ val = 8; menu[lang].AddIntItem("anisotropic filter: 8", &val);}
			if(caps.MaxAnisotropy>=16){ val = 16; menu[lang].AddIntItem("anisotropic filter: 16", &val);}
			if(caps.MaxAnisotropy>=32){ val = 32; menu[lang].AddIntItem("anisotropic filter: 32", &val);}
			menu[lang].AddIntSetIndex(0);
		}
	}

	set.language_switch=0;
	menu[0].AddKeyChange( "Slovensky jazyk", -1, &set.language_switch);
	menu[1].AddKeyChange( "English language", -1, &set.language_switch);
	return 1;
}

Scene::~Scene()
{
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
	if(!smenu)ShowCursor(1);
	GetCursorPos(&mouse_pos);
}

void Scene::ReSizeDXScene(int width, int height)
{
	window_size_x = width;
	window_size_y = height;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, FIELD_OF_VIEW*PI180, (float)window_size_x/(float)window_size_y, Z_NEAR_VIEW_PLANE, Z_FAR_VIEW_PLANE );
//	D3DXMatrixOrthoOffCenterLH( &matProj, 0, 800, 0, 600, -1, 1 );
	pd->SetTransform( D3DTS_PROJECTION, &matProj );
}

void Scene::InitDX()
{
	if(!fullscreen)
	{
		RECT		WindowRect;					// miesto pre velkost okna
		WindowRect.left=(long)0;
		WindowRect.top=(long)0;
		GetClientRect(hWnd,&WindowRect);				//zistenie rozmerov uzivatelskej casti okna
		ReSizeDXScene(WindowRect.right,WindowRect.bottom);	//nastavenie perspektivy
	}
	else ReSizeDXScene(screen_x, screen_y);	//definovana v tomto subore

	pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );
	
	// Enable z-buffering.
	pd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	

	mat4 m;	
	pd->SetTransform( D3DTS_VIEW, (struct _D3DMATRIX *)(void*)&m );
	pd->SetTransform( D3DTS_WORLD, (struct _D3DMATRIX *)(void*)&m );

	D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pd->SetMaterial( &mtrl );

	ZeroMemory( &light0, sizeof(D3DLIGHT8) );
	light0.Type = D3DLIGHT_DIRECTIONAL;
//	light0.Type = D3DLIGHT_POINT;
	light0.Diffuse.r = 1.0f;
	light0.Diffuse.g = 1.0f;
	light0.Diffuse.b = 1.0f;
	light0.Diffuse.a = 1.0f;
	light0.Ambient.r = 0.3f;
	light0.Ambient.g = 0.3f;
	light0.Ambient.b = 0.3f;
	light0.Ambient.a = 1.0f;

	light0.Direction.x = 0;	light0.Direction.y = 0;	light0.Direction.z = 1;
	
	light0.Position.x = 0;
	light0.Position.y = 0;
	light0.Position.z = -10;
	light0.Attenuation0 = 1.f;

	light0.Range = 1000;

	pd->SetLight( 0, &light0 );
//	pd->LightEnable( 0, TRUE);
//	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f,0.3f,0.3f,1) );
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );

	for(unsigned int i=0; i<caps.MaxSimultaneousTextures; i++)
	{
		pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( i, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( i, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);
		if(i>2)
		{
			pd->SetTextureStageState( i, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( i, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
		}
		pd->SetTextureStageState( i, D3DTSS_MAXANISOTROPY,16);
	//	pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_ANISOTROPIC);
	}

	pd->SetRenderState( D3DRS_LIGHTING, 0 );
}

void Scene::SaveScreen_BMP()
{
	int scr_size_x; int scr_size_y;
	IDirect3DSurface8 *sur;
	D3DDISPLAYMODE Mode;
	
	pd->GetDisplayMode( &Mode);
	Mode.Format = D3DFMT_A8R8G8B8;
	if(pd->CreateImageSurface(Mode.Width, Mode.Height, Mode.Format, &sur)!=D3D_OK )return;
	
	D3DSURFACE_DESC surd;
	if(pd->GetFrontBuffer( sur)!=D3D_OK){sur->Release(); return;}
	sur->GetDesc( &surd);

	D3DLOCKED_RECT LockedRect;
	if(sur->LockRect( &LockedRect, NULL, D3DLOCK_READONLY)!=D3D_OK)
	{
		sur->UnlockRect();
		sur->Release();
		return;
	}

	scr_size_x = surd.Width;
	scr_size_y = surd.Height;

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	FILE *sub;
	int line_size = 3*scr_size_x;
	if(line_size%4)line_size += 4 - line_size%4;

	unsigned char* buf = new unsigned char[3*line_size*scr_size_y];
	char file[30];

	if(buf==NULL )
	{
		sur->UnlockRect();
		sur->Release();
		return;
	}

	int Pitch = LockedRect.Pitch;
	unsigned char* p = (unsigned char*) LockedRect.pBits;
	for(unsigned int y=0; y<surd.Height; y++)
	{
		for(unsigned int x=0; x<surd.Width; x++)
		{
			buf[line_size*y+3*x+0] = p[Pitch*(surd.Height-y-1)+4*x+0];
			buf[line_size*y+3*x+1] = p[Pitch*(surd.Height-y-1)+4*x+1];
			buf[line_size*y+3*x+2] = p[Pitch*(surd.Height-y-1)+4*x+2];
		}
	}
	sur->UnlockRect();
	sur->Release();

	for(int c=0;c<999;c++)
	{
		char pom[20];
		strcpy(file,"screen");
		if(c<100)strcat(file,"0");
		if(c<10)strcat(file,"0");
		strcat(file,itoa(c,pom,10));
		strcat(file,".bmp");
		sub = fopen(file,"r");
		if(sub==NULL)break;
		if(sub!=NULL)fclose(sub);
	}
	
	fileheader.bfType = 0x4D42; // Magic identifier   - "BM"	| identifikacia BMP suboru musi byt "BM"
	fileheader.bfSize = 3*scr_size_x*scr_size_y+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// File size in bytes			| velkos suboru v byte
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// Offset to image data, bytes	| posun na zaciatok dat
	
	infoheader.biSize = sizeof(BITMAPINFOHEADER);	// Header size in bytes			| velkost hlavicky BITMAPINFOHEADER
	infoheader.biWidth = scr_size_x;	// Width of image			| sirka obrazka - sizeX
	infoheader.biHeight = scr_size_y;	// Height of image			| vyska obrazka - sizeY
	infoheader.biPlanes = 1;		// Number of colour planes	| pocet farebnych rovin musi bit 1
	infoheader.biBitCount = 24;		// Bits per pixel			| bitov na pixel moze bit 1,4,8,24
	infoheader.biCompression = 0;	// Compression type			| typ compresie , 0 - bez kompresie
	infoheader.biSizeImage = line_size*infoheader.biHeight ;	// Image size in bytes		| velkost obrazka v byte
	infoheader.biXPelsPerMeter = 0;	// Pixels per meter X		| pixelov na meter v smere x
	infoheader.biYPelsPerMeter = 0;	// Pixels per meter Y		| pixelov na meter v smere y
	infoheader.biClrUsed = 0;		// Number of colours		| pocet  farieb v palete, ak 0 vsetky su pouzivane
	infoheader.biClrImportant = 0;	// Important colours		| dolezite farby v palete, ak 0 vsetky su dolezite
	
	sub = fopen(file,"wb");
	fwrite( &fileheader, sizeof(BITMAPFILEHEADER), 1, sub);
	fwrite( &infoheader, sizeof(BITMAPINFOHEADER), 1, sub);
	fwrite( buf, line_size*scr_size_y, 1, sub);

	delete[] buf;
	fclose(sub);
}

int Scene::CreateVertexBufferForHelpRectange(float left, float top, float right, float bottom, float alpha)
{
	int size_of_custom_vertex = 3*sizeof(float)+sizeof(DWORD);
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
	if( FAILED( pd->CreateVertexBuffer( 4*size_of_custom_vertex,D3DUSAGE_WRITEONLY, 
								D3DFVF_XYZ|D3DFVF_DIFFUSE,D3DPOOL_DEFAULT, &pvb_help_rectange ) ) )return 0;
	void* p1;
	if( FAILED( pvb_help_rectange->Lock( 0, 4*size_of_custom_vertex, (BYTE**)&p1, 0 ) ) )return 0;
	((float*)p1)[0*4+0] = left;
	((float*)p1)[0*4+1] = bottom;
	((float*)p1)[0*4+2] = 0;
	((DWORD*)p1)[0*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[1*4+0] = right;
	((float*)p1)[1*4+1] = bottom;
	((float*)p1)[1*4+2] = 0;
	((DWORD*)p1)[1*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[2*4+0] = left;
	((float*)p1)[2*4+1] = top;
	((float*)p1)[2*4+2] = 0;
	((DWORD*)p1)[2*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[3*4+0] = right;
	((float*)p1)[3*4+1] = top;
	((float*)p1)[3*4+2] = 0;
	((DWORD*)p1)[3*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	pvb_help_rectange->Unlock();
	return 1;
}


void Scene::DrawQuad(vec p, vec n, vec s, vec t)
{
	GLizer gl;
	gl.glBegin(D3DPT_TRIANGLESTRIP, 1, 0, 1);	// 3  4  poradie bodov
        gl.glNormal(n);							// 1  2
        gl.glTexCoord(0,1); gl.glVertex( p );
        gl.glTexCoord(1,1); gl.glVertex( p+s );
        gl.glTexCoord(0,0); gl.glVertex( p+t );
        gl.glTexCoord(1,0); gl.glVertex( p+s+t );
    gl.glEnd();
}

void Scene::RestoreLostDevice()
{
	MessageBox(hWnd,"Device Lost","ERROR",MB_OK|MB_ICONERROR);
	keys[VK_ESCAPE]=1;
	return;

	// free all
//	font0.tex.pt->Release();
//	font0.tex.pt = NULL;
//	if(font0.pvb!=NULL)font0.pvb->Release();
//	font0.pvb = NULL;

	// test
	if(pd->TestCooperativeLevel()==D3DERR_DEVICELOST)
	{
		MessageBox(hWnd,"D3DERR_DEVICELOST - Neuvolnene vsetky zdroje","Device lost",MB_OK);
		keys[VK_ESCAPE]=1;
		return;
	}

	// reset
	D3DPRESENT_PARAMETERS d3dpp;

	// Set up the presentation parameters
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferCount        = 1;
	*(int*)&d3dpp.MultiSampleType= MultiSample;					// D3DMULTISAMPLE_NONE
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;		// D3DSWAPEFFECT_FLIP
	d3dpp.hDeviceWindow          = hWnd;
	d3dpp.Windowed               = !fullscreen;
	d3dpp.EnableAutoDepthStencil = 1;					// 1 if use z-buffer
		
	if     (screen_bit==16)d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16 bits
	else if(screen_bit==24)d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16 bits
	else if(screen_bit==32)d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;	// 24 bits
	else             d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;	// 24 bits
	
	if(fullscreen)
	{
		if(synchronization)
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		else
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Get the current desktop display mode
	D3DDISPLAYMODE d3ddm;
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{		MessageBox(NULL,"Error in GetAdapterDisplayMode.","ERROR",MB_OK|MB_ICONEXCLAMATION);return;}
	if( !fullscreen )
	{

		RECT		rcWindowClient;
		GetClientRect( hWnd, &rcWindowClient );
		d3dpp.BackBufferWidth  = rcWindowClient.right - rcWindowClient.left;
		d3dpp.BackBufferHeight = rcWindowClient.bottom - rcWindowClient.top;
		d3dpp.BackBufferFormat = d3ddm.Format;
	}
	else
	{
		d3dpp.BackBufferWidth  = screen_x;
		d3dpp.BackBufferHeight = screen_y;
		if(screen_bit==16)d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else if(screen_bit==24)d3dpp.BackBufferFormat = D3DFMT_R8G8B8;
		else if(screen_bit==32)d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else d3dpp.BackBufferFormat = d3ddm.Format;
	}

	if(pd->Reset(&d3dpp) != D3D_OK)
	{
		MessageBox(hWnd,"Reset fail","Device lost",MB_OK);
		keys[VK_ESCAPE]=1;
		return;
	}

	ShowWindow(hWnd,SW_SHOW);		//zvyditelnie okna, posiela spravu WM_SHOWWINDOW
	SetForegroundWindow(hWnd);		//nastavuje okno na to s ktorym pracuje uzivatel
	SetFocus(hWnd);					//nastavy vstup klavesnice na toto okno

	// load all again
//	if(!InitLoadTextures())return;
//	if(!InitLoadModels())return;
//	menu[0].Init(&font0);
//	menu[1].Init(&font0);

//	if(!font0.Init("data/font.bmp"))return;
//	InitDX();
}


void Scene::CheckColosion()
{
	static vec speed(0,0,0);			// rychlost pre skakanie
	static int jump=0;					// nastavena na 1 ak sme v skoku
	static int ground=0;				// nastavena na 1 ak sme na zemi
	static int ground_return=0;
	if(!smenu)
	{
		if(keys['R']){ speed.clear(); jump=0; ground=0; ground_return=0; }
		if(set.gravitation)
		{
			if(!ground)
			{
				if( jump )
				{
					if( set.move_during_jump )
						c->vp = 0.5f*(c->vp-c->vp_old)+c->vp_old;	// 2 krat pomalsi pohyb vo vzduchu
					else
						c->vp = c->vp_old;		// ziadny pohyb mimo zeme
				}
				else
					c->vp.y = c->vp_old.y;		// ziadna zmena vysky
				speed += set.gravity_acceleration*timeframe*vec(0,-1,0);		// v = a*t, rychlost = zrychlenie*cas
			}
			if(keys[VK_SPACE] || keys[VK_RBUTTON])
			{
				if( set.jump_durind_jump || ground || ground_return )
				{
					speed = c->vp - c->vp_old;	// posledny pohyb
					speed *= fps;				// urobime ho nezavisli od fps
					speed.y = sqrt(2.f*set.gravity_acceleration*set.jump_height);
					ground = 0;
					jump = 1;
				}
				keys[VK_SPACE]=0;
				keys[VK_RBUTTON]=0;
			}
			if(!ground)c->vp += speed*timeframe;	// aplikujeme vplyv rychlosti
		}
	}

	int ground_old = ground;
	if(set.collision)
	{
		collision.clear();
		float radius=set.sphere_radius;
		// 0.7072f namiesto 0.5f preto, lebo klzanim po polygonoch mozme vyjst aj mimo gule s polomerom 0.5f*dlzka kroku
		float radius_big=1.1f*radius+0.7072f*Length(c->vp-c->vp_old);		// big sphere - contain sphere: (vp,radius), (vp_old, radius)
		vec center_big = 0.5f*(c->vp+c->vp_old);
		pAseForCollision->AddCollisionFaces( &collision, center_big, radius_big );
//		switch(set.model)
		{
//		case 0: room1.AddCollisionFaces( &collision, center_big, radius_big ); break;
	//	case 1: room2.AddCollisionFaces( &collision, center_big, radius_big ); break;
		}
		c->vp = CheckCollision( collision, c->vp, c->vp_old, radius);
		if(set.gravitation)ground = CheckCollisionGround( collision, c->vp, radius, 60.f, 0.01f*radius);
	}

	if(set.gravitation)
	{
		if( ground)
		{
			jump=0;								// ak sme na zem tak neletime
			speed.clear();						// sme na zemy, nasa rychlost pre let je nulova
			ground_return=0;					// ak sme sa nastavili na navrat na zem a na zem sme sa aj vratili vynulujeme
		}
		else if(!ground_old)ground_return=0;
		if( ground_old && !ground && !jump)		// ak bola zem a teraz nie je a zaroven nie je skok
		{
			c->vp.y = c->vp_old.y;				// vratime sa na zem
			if(!ground_return)					// ak este nebol navrat na zem
			{
				ground=1;						// vratili sme sa na zem
				ground_return=1;				// priznak, aby sme sa nevracali na zem viackrat
			}
		}
	}
	// ak sme na zemy a pohybujeme sa smerom hore:
	// 1. ground=1; ground_old=1; ground_return=0; jump=0; 
	// 2. ground=1; ground_old=1; ground_return=1; jump=0;		// vratime sa naspat a nastavime ground_return na 1 a ground na 0
	// ak sa vratime na zem
	// 3a ground=1; ground_old=1; ground_return=0; jump=0;		// sme na zemy, ground_return sa nastavy na 0
	// ak nie sme na zemy
	// 3b ground=0; ground_old=1; ground_return=1; jump=0;		// vratime sa naspat ale kedze uz sme sa raz vracali, ground nechame na 0
	// 4. ground=0; ground_old=0; ground_return=0; jump=0;		// nie sme na zemy pouzije sa gravitacia, ground_return sa nuluje aby sme sa pocas padu nemohli odrazit
	// 5. ground=1; ground_old=0; ground_return=0; jump=0;		// vratili sme sa na zem ground_return sa vynuluje
}

