// Scene.cpp: implementation of the Scene class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene.h"

// Create the shader declaration.
DWORD dwDecl[] =
{
    D3DVSD_STREAM(0),
    D3DVSD_REG(0, D3DVSDT_FLOAT3),      // the position data
    D3DVSD_REG(1, D3DVSDT_FLOAT3),		// normal
    D3DVSD_END()
};

int Scene::sirka,Scene::vyska;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene::Scene()
{
	pvb_help_rectange = NULL;
	font0 = NULL;
	hVertexShader8 = NULL;
	hVertexShader17 = NULL;
	obj = 0;
	c.angle_x=0;
	c.angle_y=0;
	c.pos_x=0;
	c.pos_y=0;
	c.pos_z=5.f;
	c.MouseBod.x = screen_x/2;
	c.MouseBod.y = screen_y/2;
	SetCursorPos(screen_x/2,screen_y/2);

	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	InitDX();
	ShowCursor(0);
	if(!CreateVertexBufferForHelpRectange(80,screen_y_font-45,screen_x_font-80,290,0.4f)){	error = 1;	return;}

	font0 = new Font_bitmap("data/font.bmp");
	if(error)return;

	obj = new C3ds("data/angel.3ds");
	
	if(error)return;
	obj->Rescale(0.025f);
	if(!obj->CreateDXObjects()){	error = 1;	return;}

	if(!sphere.Init(100,200,0.8f)){	error = 1;	return;}
	if(!spherelight.Init(5,10,0.05f)){	error = 1;	return;}
	if(!obj->CreateDXObjects()){	error = 1;	return;}

	if(!LoadVertexShader( "Shaders/008_Many_Point_Lights/vertex_shader8.txt", dwDecl, &hVertexShader8)){	error = 1;	return;}
	if(!LoadVertexShader( "Shaders/008_Many_Point_Lights/vertex_shader17.txt", dwDecl, &hVertexShader17)){	error = 1;	return;}

	color[ 0].set( 1.0f, 0.0f, 0.0f);
	color[ 1].set( 0.0f, 1.0f, 0.0f);
	color[ 2].set( 0.0f, 0.0f, 1.0f);
	color[ 3].set( 0.5f, 0.5f, 0.0f);
	color[ 4].set( 0.5f, 0.0f, 0.5f);
	color[ 5].set( 0.0f, 0.5f, 0.5f);
	color[ 6].set( 0.3f, 0.3f, 0.3f);
	color[ 7].set( 0.6f, 0.4f, 0.0f);
	color[ 8].set( 0.6f, 0.0f, 0.4f);
	color[ 9].set( 0.4f, 0.6f, 0.0f);
	color[10].set( 0.0f, 0.6f, 0.4f);
	color[11].set( 0.4f, 0.0f, 0.6f);
	color[12].set( 0.0f, 0.4f, 0.6f);
	color[13].set( 0.6f, 0.2f, 0.2f);
	color[14].set( 0.2f, 0.6f, 0.2f);
	color[15].set( 0.2f, 0.2f, 0.6f);
	color[16].set( 0.8f, 0.1f, 0.1f);
}

Scene::~Scene()
{
	if(obj!=NULL) delete obj;
	if(font0!=NULL) delete font0;
	if(hVertexShader8!=NULL)pd->DeleteVertexShader( hVertexShader8);
	if(hVertexShader17!=NULL)pd->DeleteVertexShader( hVertexShader17);
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
	ShowCursor(1);
}

void Scene::DrawDXScene()
{
	mat4 m;
	vec4 lp[17];
    if( NULL == pd )return;
	if(keys['L'] && keys[VK_CONTROL]){ english = !english; keys['L']=0; }

    // Clear the backbuffer to a blue color
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();

	ChangeCameraPosition();
	m.Scale(1,1,-1);
	m.Translate( -c.pos_x, -c.pos_y, -c.pos_z);
	m.RotX( c.angle_x);
	m.RotY( c.angle_y);
	pd->SetTransform( D3DTS_WORLD, (struct _D3DMATRIX *)(void*)&m );
	
	if(keys['P'])pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	
	static int stage=3;
	static int model=1;
	static int stop=0;

	if(keys['1'])stage=1;
	else if(keys['2'])stage=2;
	else if(keys['3'])stage=3;
	else if(keys['4'] && caps.MaxActiveLights>=17)stage=4;

	if(keys[VK_SPACE]){	model=!model;keys[VK_SPACE]=0;}
	if(keys['B']){	stop=!stop;keys['B']=0;}

	static float rotz=0.f;
	static float roty=0.f;
	if(keys['R'])
	{
		rotz=0;
		roty=0;
	}
	if(!stop)
	{
		rotz+=90.f*timeframe;
		roty+=10.f*timeframe;
	}

	if(stage==1 || stage==2)		// 8 lights
	{
		vec a;
		for(int i=0; i<8; i++)
		{
			if(i<4)
			{
				a.set(2.0f,0,0);
				a.RotZ(rotz+(float)i*360.f/(float)4.f);
				a.y *= 0.5;
				a.RotZ(45.f);
				a.RotY(roty);
			}
			else
			{
				a.set(2.0f,0,0);
				a.RotZ(rotz+(float)i*360.f/(float)4.f+45.f);
				a.y *= 0.5f;
				a.RotZ(-45.f);
				a.RotY(roty+90.f);
			}
			lp[i] = a;
	// pretoze pozicia svetla v DX je chapana v suradniciach kamery a ja chcem aby bola 
	// v suradniciach objektu musim previest suradnice zo suradnic objektu na suradnice kamery cez modelovaciu maticu
			if(stage==1)
			{
				a = m*a;
				light0.Position.x = a.x;
				light0.Position.y = a.y;
				light0.Position.z = a.z;
				light0.Diffuse.r = color[i].x;
				light0.Diffuse.g = color[i].y;
				light0.Diffuse.b = color[i].z;
				pd->SetLight( i, &light0 );
				pd->LightEnable( i, TRUE);
				pd->SetRenderState( D3DRS_LIGHTING, 1 );
			}
		}
	}
	else		// 17 lights
	{
		vec a;
		for(int i=0; i<17; i++)
		{
			if(i<8)
			{
				a.set(2.0f,0,0);
				a.RotZ(rotz+(float)i*360.f/(float)8.f);
				a.y *= 0.5f;
				a.RotZ(45.f);
				a.RotY(roty);
			}
			else
			{
				a.set(2.0f,0,0);
				a.RotZ(rotz+(float)i*360.f/(float)9.f+45.f);
				a.y *= 0.5f;
				a.RotZ(-45.f);
				a.RotY(roty+90.f);
			}
			lp[i] = a;
			if(stage==4)
			{
				a = m*a;
				light0.Position.x = a.x;
				light0.Position.y = a.y;
				light0.Position.z = a.z;
				light0.Diffuse.r = color[i].x;
				light0.Diffuse.g = color[i].y;
				light0.Diffuse.b = color[i].z;
				pd->SetLight( i, &light0 );
				pd->LightEnable( i, TRUE);
				pd->SetRenderState( D3DRS_LIGHTING, 1 );
			}
		}
	}

	mat4 proj,mod;

	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );

	if(stage==2 || stage==3)
	{
		pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
		m = proj*mod;
		m = ~m;

		vec4 zero(0,0,0,0);
		pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
		pd->SetVertexShaderConstant(  5, zero.v, 1 );	// c5   0,0,0,0

		for(int i=0; i<17; i++)
		{
			pd->SetVertexShaderConstant( i+10, lp[i].v, 1 );
			pd->SetVertexShaderConstant( i+40, color[i].v,  1 );
		}
	}

	switch(stage)
	{
	case 1:
		if(model==0)
			sphere.Render();
		else
			obj->Render(0,0);
		break;
	case 2:
		if(model==0)
			sphere.Render(hVertexShader8);
		else
			obj->Render(0,0,hVertexShader8);
		break;
	case 3:
		if(model==0)
			sphere.Render(hVertexShader17);
		else
			obj->Render(0,0,hVertexShader17);
		break;
	case 4:
		if(model==0)
			sphere.Render();
		else
			obj->Render(0,0);
		break;
	}

	D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

	for(int i=0; i<17; i++) 
		pd->LightEnable( i, 0);
	pd->SetRenderState( D3DRS_LIGHTING, 1 );
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1) );
	int count_of_light = (stage==1 || stage==2) ? 8:17;
	for( i=0; i<count_of_light; i++)
	{
		m = mod;
		m.Translate(lp[i]);
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m );
		mtrl.Ambient.r = color[i].x;
		mtrl.Ambient.g = color[i].y;
		mtrl.Ambient.b = color[i].z;
		pd->SetMaterial( &mtrl );
		spherelight.Render();
	}
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pd->SetMaterial( &mtrl );
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );

	pd->SetRenderState( D3DRS_LIGHTING, 0 );
	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	font0->Begin();
	// vypis FPS
	timeframe = timer0.GetTime();
	fps = 1.0f / timeframe;
	timer0.Reset();
	fps_counter.GetAvgFps(timeframe,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
	font0->Print( 0, 0, text, 0);
	
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
	case 1:
		font0->Print(1,1,"8 point lights, fixed function pipeline", 0);
		break;
	case 2:
		font0->Print(1,1,"8 point lights, vertex shader 1.0", 0);
		break;
	case 3:
		font0->Print(0,1,"17 point lights, vertex shader 1.0", 0);
		break;
	case 4:
		font0->Print(0,1,"17 point lights, fixed function pipeline", 0);
	}	

	if(keys['H'])				//vypise help
	{
		int i=3;

		// first draw rectange
		m.Identity();
		pd->SetRenderState( D3DRS_LIGHTING, 0 );
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

		if(english)
		{
			font0->Print(10,i++,"Program: Many Point Lights - vertex shader 1.0", 0);
			font0->SetColor(0.9f,0.9f,1.0f,1.f);
			font0->Print(8,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(15,i++,"Controls:", 1);
			font0->Print(20,i++,"1,2,3,4 - change count of light", 1);
			font0->Print(20,i++,"space - cahnge model", 1);
			font0->Print(20,i++,"B - off/on rotation lights", 1);
			font0->Print(20,i++,"P - wire frame", 1);
			font0->Print(20,i++,"R - reset light positions", 1);
			i++;
			font0->Print(20,i++,"F12 - take screenshot",1);
			font0->Print(20,i++,"H - help", 1);
			font0->Print(20,i++,"mouse, button on mouse - moving",1);
			font0->Print(20,i++,"arrow, Page Up,Page Down - moving",1);
			font0->Print(20,i++,"shift - slow moving",1);
			font0->Print(20,i++,"R - reset camera",1);
			font0->Print(20,i++,"F4 - toggle fullscreen", 1);
		}
		else
		{
			font0->Print(10,i++,"Program: Many Point Lights - vertex shader 1.0", 0);
			font0->SetColor(0.9f,0.9f,1.0f,1.f);
			font0->Print(8,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(15,i++,"Pouzivane klavesy:", 1);
			font0->Print(20,i++,"1,2,3,4 - meni pocet svetiel", 1);
			font0->Print(20,i++,"medzernik - meni meni model", 1);
			font0->Print(20,i++,"B - zastavy/spusti rotaciu svetiel", 1);
			font0->Print(20,i++,"P - zapne ciarovy rezim", 1);
			font0->Print(20,i++,"R - reset pozicie svetiel", 1);
			i++;
			font0->Print(20,i++,"F12 - ulozi obrazovku do subora",1);
			font0->Print(20,i++,"H - help", 1);
			font0->Print(20,i++,"mys, tlacitka na mysi - pohyb", 1);
			font0->Print(20,i++,"sipky, Page Up,Page Down - pohyb", 1);
			font0->Print(20,i++,"shift - pomaly alebo rychli pohyb", 1);
			font0->Print(20,i++,"R - reset kamery", 1);
			font0->Print(20,i++,"F4 - prepina do/z fullscreen", 1);
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
//	light0.Type = D3DLIGHT_DIRECTIONAL;
	light0.Type = D3DLIGHT_POINT;
	light0.Diffuse.r = 1.0f;
	light0.Diffuse.g = 1.0f;
	light0.Diffuse.b = 1.0f;
	light0.Diffuse.a = 1.0f;
	light0.Ambient.r = 0.0f;
	light0.Ambient.g = 0.0f;
	light0.Ambient.b = 0.0f;
	light0.Ambient.a = 1.0f;

	light0.Direction.x = 0;	light0.Direction.y = 0;	light0.Direction.z = 1;
	
	light0.Position.x = 0;
	light0.Position.y = 0;
	light0.Position.z = -3.f;
	light0.Attenuation0 = 1.f;

	light0.Range = 1000;

	pd->SetLight( 0, &light0 );
	pd->LightEnable( 0, TRUE);
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
}

void Scene::ChangeCameraPosition()
{
	float mys_x,mys_y,step;
	
	if(keys['R'])
	{
		c.angle_x=0;
		c.angle_y=0;
		c.pos_x=0;
		c.pos_y=0;
		c.pos_z=5.f;
	//	keys['R']=0;
	}
	if(keys[VK_SHIFT])step=5.0f*timeframe;else step=25.0f*timeframe;	//ak je stlaceny shift mensi krok
	step *= 0.2f;

	if (keys[VK_UP] || keys[VK_LBUTTON])		// sipka hore alebo lave tlacidlo mysi
	{	
	//	c.pos_x += step * (float)sin(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
	//	c.pos_y -= step * (float)sin(c.angle_x*PI180);
	//	c.pos_z -= step * (float)cos(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
		c.pos_z -= step;
	}
	if (keys[VK_DOWN] || keys[VK_RBUTTON])		// sipka dole alebo prave tlacidlo mysi
	{
	//	c.pos_x -= step * (float)sin(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
	//	c.pos_y += step * (float)sin(c.angle_x*PI180);
	//	c.pos_z += step * (float)cos(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
		c.pos_z += step;
	}
	if(c.pos_z<0.f)c.pos_z=0.f;

	GetCursorPos(&c.MouseBod);						//nacitanie pozicie mysi
	SetCursorPos(screen_x/2,screen_y/2);				//nastavenie pozicie mysi na stred obrazovky
	mys_x= ((float)c.MouseBod.x - screen_x/2)/10;		//vypocet zmeny pozicie mysi v smere x
	mys_y= ((float)c.MouseBod.y - screen_y/2)/10;		//vypocet zmeny pozicie mysi v smere y
	if     (keys[VK_NEXT]) mys_y =  3*step;		//Page Down
	else if(keys[VK_PRIOR])mys_y = -3*step;		//Page Up
	if     (keys[VK_RIGHT])mys_x =  3*step;		//sipka vpravo
	else if(keys[VK_LEFT]) mys_x = -3*step;		//sipka vlavo
	c.angle_x+= mys_y;
	c.angle_y+= mys_x;

	if(c.angle_x>90.f) c.angle_x = 90.f;
	if(c.angle_x<-90.f) c.angle_x = -90.f;
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


int Scene::LoadVertexShader(char *filename, DWORD *pDeclaration, DWORD* phVertexShader, char* verprog)
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

int Scene::CreateVertexBufferForHelpRectange(float left, float top, float right, float bottom, float alpha)
{
	int size_of_custom_vertex = 3*sizeof(float)+sizeof(DWORD);
	if( FAILED( pd->CreateVertexBuffer( 4*size_of_custom_vertex,0, 
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
