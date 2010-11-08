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


int Scene::sirka,Scene::vyska;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene::Scene()
{
	pvb_help_rectange = NULL;
	hVertexShader = 0;
	font0 = NULL;
	c = NULL;

	if(caps.MaxSimultaneousTextures<3)
	{ 	MessageBox( hWnd, "MaxSimultaneousTextures must by > 2\nprogram need multitexture", "ERROR", MB_OK); error=1;	}

	if(!(caps.TextureOpCaps&D3DTEXOPCAPS_BUMPENVMAP))
	{ 	MessageBox( hWnd, "Program need BUMPENVMAP texture operator!\n\nEnvironment mapped bump mapping (EMBM) not supported.", "ERROR", MB_OK); error=1;	}

	if(error)return;
	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	InitDX();
	ShowCursor(0);

	font0 = new Font_bitmap("data/font.bmp");
	if(error)return;

	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
    pd->BeginScene();
	font0->Begin();
	font0->Print_xy_scale(60,450,"Environment mapped bump mapping",0,2,3);
	font0->Print_xy_scale(160,380,english? "Loading models and creating normal maps":"Nacitavam modely a vytvaram normal mapy",0,1.2f,1.2f);
	font0->Print_xy_scale(300,320,"Marek Mizanin",0,1.5f,1.5f);
	font0->Print_xy_scale(260,280,"www.mizanin.szm.sk",0,1.5f,1.5f);
    font0->Print_xy_scale(english ? 340:300,240,english ? "Key H - help" : "Klavesa H - Help",0,1.0f,1.0f);
	font0->End();
	pd->EndScene();
	pd->Present( NULL, NULL, NULL, NULL );

	if(!CreateVertexBufferForHelpRectange(80,screen_y_font-45,screen_x_font-80,280,0.6f)){	error = 1;	return;}
	if(!sphere_map.load("data/opengl/spheremap.JPG"))error=1;
	if(!EMBMbase.load("data/EMBMbase.JPG"))error=1;
	if(error)return;
	texture* ptex;
	Image* pim;
	float strong;
	{
		Image im;
		if(!im.Load("data/EMBMbump.JPG"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.3f);
		EMBMbump.load(im,1,3,D3DFMT_V8U8);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		strong = 0.3f;
		obj_filter_strong.push_back(strong);
	}
	if(error)return;
	strong = 0.1f;

	C3ds* p3ds;

	p3ds = new C3ds("data/sphere.3ds", 2, 0, 0, 1);
//	if(!p3ds->LoadTextures( "data/sphere.3ds", 2, 2))error=1;
	obj.push_back(p3ds);
	if(error)return;


	p3ds = new C3ds("data/quad_dot3/quad.3DS", 2, 0, 0, 1);
	if(!p3ds->LoadTextures( "data/quad_dot3/quad.3DS", 2, 2))error=1;
	obj.push_back(p3ds);
	{
		Image im;
		if(!im.Load("data/quad_dot3/16tile01.JPG"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.1f);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		obj_filter_strong.push_back(strong);
	}

	p3ds = new C3ds("data/quad_dot3/quad.3DS", 2, 0, 0, 1);
	strcpy(p3ds->materials[0].strFile, "Fieldstone.JPG");
	if(!p3ds->LoadTextures( "data/quad_dot3/quad.3DS", 2, 2))error=1;
	obj.push_back(p3ds);
	{
		Image im;
		if(!im.Load("data/quad_dot3/FieldstoneBump.JPG"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.1f);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		obj_filter_strong.push_back(strong);
	}

	p3ds = new C3ds("data/quad_dot3/quad.3DS", 2, 0, 0, 1);
	strcpy(p3ds->materials[0].strFile, "Oxidated.jpg");
	if(!p3ds->LoadTextures( "data/quad_dot3/quad.3DS", 2, 2))error=1;
	obj.push_back(p3ds);
	{
		Image im;
		if(!im.Load("data/quad_dot3/Oxidated.JPG"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.1f);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		obj_filter_strong.push_back(strong);
	}

	p3ds = new C3ds("data/quad_dot3/quad.3DS", 2, 0, 0, 1);
	strcpy(p3ds->materials[0].strFile, "stena.JPG");
	if(!p3ds->LoadTextures( "data/quad_dot3/quad.3DS", 2, 2))error=1;
	obj.push_back(p3ds);
	{
		Image im;
		if(!im.Load("data/quad_dot3/stenabump.jpg"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.1f);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		obj_filter_strong.push_back(strong);
	}

	p3ds = new C3ds("data/quad_dot3/quad.3DS", 2, 0, 0, 1);
	strcpy(p3ds->materials[0].strFile, "Tiles.JPG");
	if(!p3ds->LoadTextures( "data/quad_dot3/quad.3DS", 2, 2))error=1;
	obj.push_back(p3ds);
	{
		Image im;
		if(!im.Load("data/quad_dot3/Tilesbump.JPG"))error=1;
		pim = new Image;  pim->Set(im);	obj_bump_image.push_back(pim);
		im.Filter(0,0,0.1f);
		ptex = new texture;
		ptex->load(im,1,3);
		obj_bump_rgb.push_back(ptex);
		obj_filter_strong.push_back(strong);
	}

	if(error)return;

	if(!LoadVertexShader( "Shaders/011_environment_mapped_bump_mapping/vertex_shader.txt", dwDecl, &hVertexShader)){	error = 1;	return;}
	for(int i=0; i<obj.size(); i++)
		if(!obj[i]->CreateDXObjects()){	error = 1;	return;}

	c = new camera;
	c->vr.set( 0.00f, 0.00f,-1.00f);
	c->vu.set(-0.72f, 0.70f, 0.003f);
	c->vd.set(-0.70f,-0.72f, 0.003f);
	c->vpd = 2.68f;
}

Scene::~Scene()
{
	for(int i=0; i<obj_bump_rgb.size(); i++)
		if(obj_bump_rgb[i]!=NULL) delete obj_bump_rgb[i];
	for( i=0; i<obj_bump_image.size(); i++)
		if(obj_bump_image[i]!=NULL) delete obj_bump_image[i];
	if(hVertexShader!=NULL)pd->DeleteVertexShader( hVertexShader);

	for( i=0; i<obj.size(); i++)
		if(obj[i]!=NULL) delete obj[i];
	if(font0!=NULL) delete font0;
	if(c!=NULL)delete c;
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
	ShowCursor(1);
}

void Scene::DrawDXScene()
{
	mat4 m;
    if( NULL == pd )return;
	if(keys['L'] && keys[VK_CONTROL]){ english = !english; keys['L']=0; }

    // Clear the backbuffer to a blue color
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();

	c->MoveWorld(fps, ROTATE_WALK|ROTATE_ABOUT_POINT);	// otoci svetom na zaklade vstupov od mysi a klavesnice

	static int stage=4;
	
	if(keys['1'])stage=1;
	else if(keys['2'])stage=2;
	else if(keys['3'])stage=3;
	else if(keys['4'])stage=4;


	static int object=0;
	if(keys[VK_SPACE])
	{
		object++;
		if(object>=obj.size())object=0;
		keys[VK_SPACE]=0;
	}
	if(keys[VK_BACK])
	{
		object--;
		if(object<0)object=obj.size()-1;
		keys[VK_BACK]=0;
	}

	if(keys['P'])pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	int update=0;
	if(keys['M'])
	{
		obj_filter_strong[object]*=1.1f;
		update=1;
		keys['M']=0;
	}
	if(keys['N'])
	{
		obj_filter_strong[object]/=1.1f;
		update=1;
		keys['N']=0;
	}
	if(update)
	{
		Image im;
		im.Set(*obj_bump_image[object]);
		im.Filter(0,0,obj_filter_strong[object]);
		if(object==0)
			EMBMbump.load(im,1,3,D3DFMT_V8U8);
		else
			obj[object]->textures[obj[object]->materials.size()].load(im,1,3,D3DFMT_V8U8);
		obj_bump_rgb[object]->load(im,1,3);
	}

	vec4 eye;		// camera position
	mat4 proj,mod;

	pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );

	eye = (!mod)*vec(0,0,0);		// bod (0,0,0) v suradniciach camery = eye v suradniciach objektu

	m = proj*mod;
	m = ~m;

	vec4 c5(0.5f,-0.5f,0.5f,0.5f);
	vec4 c7(-2,-2,-2,-2);
	vec4 c8(0,0,1,0);
	pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
	pd->SetVertexShaderConstant(  5, c5.v, 1 );		// c5   0.5,-0.5,0.5,0.5
	pd->SetVertexShaderConstant(  6, eye.v, 1 );	// c6   pozicia kamery v suradniciach objektu
	pd->SetVertexShaderConstant(  7, c7.v, 1 );		// c7   -2,-2,-2,-2
	pd->SetVertexShaderConstant(  8, c8.v, 1 );		// c8   0,0,1,0

	if(stage==1)		// normalova mapa - obsahuje DUDV
	{
		obj_bump_rgb[object]->SetTexture(0);
		pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	}
	else if(stage==2)
	{
		pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_CURRENT );
		pd->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		sphere_map.SetTexture(2);
		pd->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_CURRENT );
	}
	else if(stage>2)
	{
		// the base texture.
		if(object!=0)
			obj[object]->textures[0].SetTexture(0);
		else
			EMBMbase.SetTexture(0);
		pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pd->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// the bump map
		if(object!=0)
			obj[object]->textures[obj[object]->materials.size()].SetTexture(1);
		else
			EMBMbump.SetTexture(1);
		pd->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP );
		pd->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );

		// a specular environment map.
		sphere_map.SetTexture(2);
		if(stage==3)
			pd->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		else
			pd->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_ADD );
		pd->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_CURRENT );
	}

	float one = 1.0f, zero = 0.0f;
    pd->SetTextureStageState( 1, D3DTSS_BUMPENVMAT00, *(DWORD*)&one );
    pd->SetTextureStageState( 1, D3DTSS_BUMPENVMAT01, *(DWORD*)&zero );
    pd->SetTextureStageState( 1, D3DTSS_BUMPENVMAT10, *(DWORD*)&zero );
    pd->SetTextureStageState( 1, D3DTSS_BUMPENVMAT11, *(DWORD*)&one );

	obj[object]->Render(2,0,hVertexShader);

	pd->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	pd->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );


	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	font0->Begin();
	// vypis FPS
	timeframe = timer0.GetTime();
	fps = 1.0f / timeframe;
	timer0.Reset();
	fps_counter.GetAvgFps(timeframe,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
	font0->Print( 0, 0, text, 0);
	
/*	this->Print_vec( 0, 5, c->vr, "vr");
	this->Print_vec( 0, 6, c->vu, "vu");
	this->Print_vec( 0, 7, c->vd, "vd");
	this->Print_vec( 0, 8, c->vp, "vp");
*/
	if(keys[VK_F12])
	{
		font0->End();
		pd->EndScene();
		pd->Present( NULL, NULL, NULL, NULL );
		SaveScreen_BMP();
		return;
	}

	switch(stage)
	{
	case 1: font0->Print(0,1,english ? "normal map - contain DUDV":"normalova mapa - obsahuje DUDV",0); break;
	case 2: font0->Print(0,1,"sphere enviromental mapping",0); break;
	case 3: font0->Print(0,1,"environment mapped bump mapping (EMBM)",0); break;
	case 4: font0->Print(0,1,english ? "EMBM + texture" : "EMBM + textura",0); break;
	}
	sprintf(text,english ? "bumpiness=%.3f":"hrbolatost=%.3f", obj_filter_strong[object]);
	font0->Print(0,2,text,0);

	if(keys['H'])				//vypise help
	{
		int i=3;

		// first draw rectange
		pd->SetRenderState( D3DRS_LIGHTING, 0 );
		m.Identity();
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m);
		pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

		pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		
		pd->SetStreamSource( 0, pvb_help_rectange, 3*sizeof(float)+sizeof(DWORD));
		pd->SetVertexShader( D3DFVF_XYZ|D3DFVF_DIFFUSE );
		pd->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		pd->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR);
		pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
		
		pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pd->SetRenderState( D3DRS_LIGHTING, 1 );

		font0->Print(21,i++,"Program: Environment mapped bump mapping", 0);
		font0->SetColor(0.9f,0.9f,1.0f,1.f);

		if(english)
		{
			font0->Print(8,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(15,i++,"Controls:", 1);
			font0->Print(20,i++,"space, backspace - change model",1);
			font0->Print(20,i++,"1,2,3,4 - change phase",1);
			font0->Print(20,i++,"M - increase, N - decrease bumpiness of normal map",1);
			font0->Print(20,i++,"P - wire frame",1);
			i++;
			font0->Print(20,i++,"F12 - take screenshot",1);
			font0->Print(20,i++,"H - help", 1);
			font0->Print(20,i++,"mouse, button on mouse - moving",1);
			font0->Print(20,i++,"arrow, Page Up,Page Down - moving",1);
			font0->Print(20,i++,"shift - slow moving",1);
			font0->Print(20,i++,"R - reset camera",1);
		}
		else
		{
			font0->Print(8,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(15,i++,"Pouzivane klavesy:", 1);
			font0->Print(20,i++,"medzernik, backspace - meni model",1);
			font0->Print(20,i++,"1,2,3,4 - meni fazu",1);
			font0->Print(20,i++,"M - zvysuje, N - znizuje hrbolatost normalovej mapy",1);
			font0->Print(20,i++,"P - zapne ciarovy rezim", 1);
			i++;
			font0->Print(20,i++,"F12 - ulozi obrazovku do subora",1);
			font0->Print(20,i++,"H - help", 1);
			font0->Print(20,i++,"mys, tlacitka na mysi - pohyb", 1);
			font0->Print(20,i++,"sipky, Page Up,Page Down - pohyb", 1);
			font0->Print(20,i++,"shift - pomaly alebo rychli pohyb", 1);
			font0->Print(20,i++,"R - reset kamery", 1);
		}
	}
	font0->End();
	
	// End the scene
	pd->EndScene();

	// Present the backbuffer contents to the display
	pd->Present( NULL, NULL, NULL, NULL );
}

void Scene::ReSizeDXScene(int width, int height)
{
	sirka = width;
	vyska = height;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, FIELD_OF_VIEW*PI180, (float)sirka/(float)vyska, Z_NEAR_VIEW_PLANE, Z_FAR_VIEW_PLANE );
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

	pd->SetTextureStageState( 0, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 0, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 0, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);

	if(caps.MaxSimultaneousTextures>1)
	{
		pd->SetTextureStageState( 1, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( 1, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( 1, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);
	}
//	pd->SetTextureStageState(0, D3DTSS_MAXANISOTROPY,16);
//	pd->SetTextureStageState(1, D3DTSS_MAXANISOTROPY,16);
//	pd->SetTextureStageState(2, D3DTSS_MAXANISOTROPY,16);
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

int Scene::LoadVertexShader(char *filename, DWORD *pDeclaration, DWORD *phVertexShader, char *verprog)
{
	// Create the vertex shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(filename==NULL)
		D3DXAssembleShader( verprog, strlen(verprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError )  // assemble shader code
			!= D3D_OK)
			MessageBox(hWnd, filename, "Nenajdeny subor pre vertex program", MB_OK);
	}

	if(pError!=NULL)
	{
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;

		pError->Release();
		MessageBox(hWnd, error_text, "Chyba pri nacitavanie vertex programu", MB_OK);
		delete [] error_text;
		phVertexShader = NULL;
		return 0;
	}

	if(	pd->CreateVertexShader( pDeclaration, (DWORD*)pCode->GetBufferPointer(), phVertexShader, software_vertexprocessing )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, "Chyba pri nacitavanie vertex programu", MB_OK);
	return 0;
}

void Scene::Print_vec(int stlpec, int riadok, const vec &a, char *meno)
{
//	font0->Begin();
	if(meno!=NULL)
	{
		font0->Print(stlpec, riadok, meno,0);
		stlpec += strlen(meno)+1;
	}
	
	gcvt( (double)a.x, 2, text);
	font0->Print(stlpec,riadok,text,0);
	stlpec+=10;
	gcvt( (double)a.y, 2, text);
	font0->Print(stlpec,riadok,text,0);
	stlpec+=10;
	gcvt( (double)a.z, 2, text);
	font0->Print(stlpec,riadok,text,0);

//	font0->End();
}

void Scene::Print_mat4(int stlpec, int riadok, const mat4 &m, char *nadpis)
{
	font0->Begin();
	if(nadpis!=NULL)
	{
		font0->Print(stlpec, riadok++, nadpis,0);
	}
	for(int i=0;i<16;i++)
	{
		gcvt( (double)m.m[i], 2, text);			//prevod realneho cisla na text
		//	a00  a04  a08  a12 
		//  a01  a05  a09  a13
		//  a02  a06  a10  a14
		//  a03  a07  a11  a15
		font0->Print(stlpec+(i/4)*10,riadok+(i%4),text,0);
	}
	font0->End();
}

int Scene::CreateVertexBufferForHelpRectange(float left, float top, float right, float bottom, float alpha)
{
	int size_of_custom_vertex = 3*sizeof(float)+sizeof(DWORD);
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

