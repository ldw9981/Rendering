// Scene.cpp: implementation of the Scene class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene.h"

int Scene::sirka,Scene::vyska;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene::Scene()
{
	pvb_help_rectange = NULL;
	font0 = NULL;
	c = NULL;

	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	InitDX();
	if(!CreateVertexBufferForHelpRectange(10,screen_y_font-40,screen_x_font-10,200,0.5f)){	error = 1;	return;}


	font0 = new Font_bitmap("data/font.bmp");
	if(error)return;

/*	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
    pd->BeginScene();
	font0->Begin();
	font0->Print_xy_scale(270,400,"ASE loader",0,2.f,2);
	font0->Print_xy_scale(270,380,english ? "+ better collision detection" : "+ lepsia detekcia kolizie",0,0.8f,0.8f);
	font0->Print_xy_scale(270,360,english ? "+ gravitation" : "+ gravitacia",0,0.8f,0.8f);
	font0->Print_xy_scale(300,320,"Marek Mizanin",0,1.5f,1.5f);
	font0->Print_xy_scale(260,280,"www.mizanin.szm.sk",0,1.5f,1.5f);
	font0->End();
	pd->EndScene();
	pd->Present( NULL, NULL, NULL, NULL );*/
	c = new camera;

	if(!room.Init("data/027model/room.ase", 0, 1, 0.1f)){ error = 1; return;}

	menu[0].Init( font0 );
	menu[0].AddText( "Show/hide menu - key F1           ");
	menu[0].AddKey ( "Help", 'H', &set.help);
	menu[0].AddBool( "Base texture", 'I', &set.baset, 1);
	menu[0].AddBool( "Lightmap", 'O', &set.lmapt, 1);
	menu[0].AddBool( "Collision", 'K', &set.collision, 1);
	menu[0].AddBool( "Use simple collision", -1, &set.collision_old, 0);
	menu[0].AddFloatComp( "Radius of collision sphere", -1, &set.sphere_radius, 2.f*Z_NEAR_VIEW_PLANE, 1.f, 0.5f);
	menu[0].AddBool( "Gravitation", 'G', &set.gravitation, 1);
	menu[0].AddFloatComp( "Gravitation in m/s2", -1, &set.gravity_acceleration, 0.f, 100.f, 9.81f);
	menu[0].AddFloatComp( "Height of jump in m", -1, &set.jump_height, 0.f, 8.f, 1.5f);
	menu[0].AddBool( "Jump during flying", -1, &set.jump_durind_jump, 0);
	menu[0].AddBool( "Moving during flying", -1, &set.move_during_jump, 0);
	if(caps.MaxSimultaneousTextures>1)menu[0].AddBool( "Multitexturing", -1, &set.use_multitexturing, 1); else set.use_multitexturing=0;
	menu[0].AddBool( "Cull face", 'C', &set.cull_face, 1);
	menu[0].AddBool( "Wire frame", 'P', &set.line, 0);
	menu[0].AddBool( "Texturing", 'T', &set.texturing, 1);
	menu[0].AddBool( "Material", 'M', &set.material, 0);
	menu[0].AddBool( "Lighting", 'L', &set.lighting, 0);
	menu[0].AddKey ( "Set light position", 'U', &set.light_update);
	set.anisotropic_filter=0;
	anisotropic_filter_actual = 0;
	if(caps.RasterCaps&D3DPRASTERCAPS_ANISOTROPY)
	{
		int val;
		menu[0].AddInt("anisotropic_filter", -1, &set.anisotropic_filter);
		val = 0; menu[0].AddIntItem("anisotropic filter: OFF", &val);
		if(caps.MaxAnisotropy>=2){ val = 2; menu[0].AddIntItem("anisotropic filter: 2", &val);}
		if(caps.MaxAnisotropy>=4){ val = 4; menu[0].AddIntItem("anisotropic filter: 4", &val);}
		if(caps.MaxAnisotropy>=8){ val = 8; menu[0].AddIntItem("anisotropic filter: 8", &val);}
		if(caps.MaxAnisotropy>=16){ val = 16; menu[0].AddIntItem("anisotropic filter: 16", &val);}
		if(caps.MaxAnisotropy>=32){ val = 32; menu[0].AddIntItem("anisotropic filter: 32", &val);}
		menu[0].AddIntSetIndex(0);
	}
	
	menu[0].AddBool( "Print variables", -1, &set.print_variable, 0);
#ifdef _DEBUG
	menu[0].AddFloatComp( "Delay", -1, &set.time_delay, 0.f, 0.3f, 0.f);
#endif
	set.language_switch=0;
	menu[0].AddKeyChange( "Slovensky jazyk", -1, &set.language_switch);

	menu[1].Init(font0);
	menu[1].AddText( "Zobrazenie/skrytie menu klavesa F1");
	menu[1].AddKey ( "Help", 'H', &set.help);
	menu[1].AddBool( "Zakladna textura", 'I', &set.baset, 1);
	menu[1].AddBool( "Lightmap -a", 'O', &set.lmapt, 1);
	menu[1].AddBool( "Kolizia", 'K', &set.collision, 1);
	menu[1].AddBool( "Pouzit jednoduchu koliziu", -1, &set.collision_old, 0);
	menu[1].AddFloatComp( "Polomer kolidujucej gule", -1, &set.sphere_radius, 2.f*Z_NEAR_VIEW_PLANE, 1.f, 0.5f);
	menu[1].AddBool( "Gravitacia", 'G', &set.gravitation, 1);
	menu[1].AddFloatComp( "Gravitacia v m/s2", -1, &set.gravity_acceleration, 0.f, 100.f, 9.81f);
	menu[1].AddFloatComp( "Vyska skoku v m", -1, &set.jump_height, 0.f, 8.f, 1.5f);
	menu[1].AddBool( "Skok pocas letu", -1, &set.jump_durind_jump, 0);
	menu[1].AddBool( "Pohyb pocas letu", -1, &set.move_during_jump, 0);
	if(caps.MaxSimultaneousTextures>1)menu[1].AddBool( "Multitexturing", -1, &set.use_multitexturing, 1); else set.use_multitexturing=0;
	menu[1].AddBool( "Cull face", 'C', &set.cull_face, 1);
	menu[1].AddBool( "Ciarovy rezim", 'P', &set.line, 0);
	menu[1].AddBool( "Texturing", 'T', &set.texturing, 1);
	menu[1].AddBool( "Material", 'M', &set.material, 0);
	menu[1].AddBool( "Osvetlenie", 'L', &set.lighting, 0);
	menu[1].AddKey ( "Nastavenie polohy svetla", 'U', &set.light_update);
	if(caps.RasterCaps&D3DPRASTERCAPS_ANISOTROPY)
	{
		int val;
		menu[1].AddInt("anisotropic_filter", -1, &set.anisotropic_filter);
		val = 0; menu[1].AddIntItem("anizotropny filter: OFF", &val);
		if(caps.MaxAnisotropy>=2){ val = 2; menu[1].AddIntItem("anizotropny filter: 2", &val);}
		if(caps.MaxAnisotropy>=4){ val = 4; menu[1].AddIntItem("anizotropny filter: 4", &val);}
		if(caps.MaxAnisotropy>=8){ val = 8; menu[1].AddIntItem("anizotropny filter: 8", &val);}
		if(caps.MaxAnisotropy>=16){ val = 16; menu[1].AddIntItem("anizotropny filter: 16", &val);}
		if(caps.MaxAnisotropy>=32){ val = 32; menu[1].AddIntItem("anizotropny filter: 32", &val);}
		menu[1].AddIntSetIndex(0);
	}
	menu[1].AddBool( "Vypisovanie premennych", -1, &set.print_variable, 0);
#ifdef _DEBUG
	menu[1].AddFloatComp( "Delay", -1, &set.time_delay, 0.f, 0.3f, 0.f);
#endif
	menu[1].AddKeyChange( "English language", -1, &set.language_switch);
	smenu = 1;
	if(smenu)c->set_3ds_mouse_style();
}

Scene::~Scene()
{
	if(font0!=NULL) delete font0;
	if(c!=NULL) delete c;
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
}

void Scene::DrawDXScene()
{
	mat4 m;
    if( NULL == pd )return;

	if(keys['L'] && keys[VK_CONTROL] || set.language_switch)
	{
		english = !english; 
		keys['L']=0;
		set.language_switch=0;
		for(int i=0; i<menu[0].items.size(); i++)
		{
			if(english)
			{
				menu[0].items[i].key = menu[1].items[i].key;
				menu[0].items[i].IntIndex = menu[1].items[i].IntIndex;
				menu[0].items[i].float_item.aktual = menu[1].items[i].float_item.aktual;
			}
			else
			{
				menu[1].items[i].key = menu[0].items[i].key;
				menu[1].items[i].IntIndex = menu[0].items[i].IntIndex;
				menu[1].items[i].float_item.aktual = menu[0].items[i].float_item.aktual;
			}
		}
	}
	if(keys[VK_F1])
	{
		smenu=!smenu; 
		keys[VK_F1]=0;
		if(smenu)
			c->set_3ds_mouse_style();
		else
			c->set_q3_mouse_style();
	}

	if(anisotropic_filter_actual!=set.anisotropic_filter)
	{
		anisotropic_filter_actual=set.anisotropic_filter;
		for(unsigned int i=0; i<caps.MaxSimultaneousTextures; i++)
		{
			if(anisotropic_filter_actual==0)
				pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
			else
			{
				pd->SetTextureStageState( i, D3DTSS_MAXANISOTROPY, anisotropic_filter_actual);
				pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_ANISOTROPIC);
			}
		}
	}

    // Clear the backbuffer to a black color
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();

	static vec speed(0,0,0);			// rychlost pre skakanie
	static int jump=0;					// nastavena na 1 ak sme v skoku
	static int ground=0;				// nastavena na 1 ak sme na zemi
	static int ground_return=0;
	if(smenu)
		c->SetTransform();
	else 
	{
		english ? menu[0].ChangeSetting() : menu[1].ChangeSetting();
		if(keys['R']){ speed.clear(); jump=0; ground=0; ground_return=0; }
		c->MoveWorld(fps, ROTATE_WALK);	// otoci svetom na zaklade vstupov od mysi a klavesnice
		if(set.gravitation && !set.collision_old)
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
			if(keys[VK_SPACE])
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
			}
			if(!ground)c->vp += speed*timeframe;	// aplikujeme vplyv rychlosti
		}
	}

	int ground_old = ground;
	if(set.collision)
	{
		if(set.collision_old)
		{
			c->vp = room.CheckSimpleCollision( c->vp, set.sphere_radius);
		}
		else
		{
			collision.clear();
			float radius=set.sphere_radius;
			// 0.7072f namiesto 0.5f preto, lebo klzanim po polygonoch mozme vyjst aj mimo gule s polomerom 0.5f*dlzka kroku
			float radius_big=1.1f*radius+0.7072f*Length(c->vp-c->vp_old);		// big sphere - contain sphere: (vp,radius), (vp_old, radius)
			vec center_big = 0.5f*(c->vp+c->vp_old);
			room.AddCollisionFaces( &collision, center_big, radius_big );
			c->vp = CheckCollision( collision, c->vp, c->vp_old, radius);
			if(set.gravitation)ground = CheckCollisionGround( collision, c->vp, radius, 60.f, 0.01f*radius);
		}
	}

	if(set.gravitation && !set.collision_old)
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
	c->SetTransform();

	static vec l(-1.1f,3,-4.9f);
	if(set.light_update)l = c->vp;//+c->vd;
	if(set.lighting)
	{
		pd->GetTransform( D3DTS_WORLD, (struct _D3DMATRIX *)(void*)&m );
		vec lpos1 = m*l;
		light0.Position.x = lpos1.x;
		light0.Position.y = lpos1.y;
		light0.Position.z = lpos1.z;
		pd->SetLight( 0, &light0 );
		pd->SetRenderState( D3DRS_LIGHTING, 1 );
	}

	if(set.cull_face)
		pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
	else
		pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);

	if(set.line)pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if(!set.lmapt&&set.baset)room.Render(set.texturing, set.material, 0);
	if(set.lmapt&&!set.baset)room.Render(set.texturing, set.material, 1);
	if(set.lmapt&&set.baset)
	{
		if(set.use_multitexturing )room.RenderMultitexture( set.texturing, set.material);
		else
		{
			room.Render(set.texturing, set.material, 0);
			pd->SetRenderState( D3DRS_LIGHTING, 0 );

			pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
			pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			pd->SetRenderState( D3DRS_ZBIAS, 2);
			room.Render(set.texturing, 0, 1);
			pd->SetRenderState( D3DRS_ZBIAS, 0);
			pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
		}
	}
	
	pd->SetRenderState( D3DRS_LIGHTING, 0 );
	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	font0->Begin();
	if(set.print_variable)
	{
		font0->Print_vec( 0, 1, c->vp, english ? "position :" : "pozicia :");
		font0->Print_vec( 0, 2, speed, english ? "velocity:" : "rychlost:");
		font0->Print_int( 0, 3, ground, "ground" );
		font0->Print_int( 0, 4, ground_return, "ground return" );
		font0->Print_int( 0, 5, ground||ground_return, english ? "ground or g return" : "ground alebo g return" );
		font0->Print_int( 0, 6, jump, english ? "jump" : "skok" );
	}
/*	this->Print_vec( 0, 1, c->vp, "vp");
	this->Print_vec( 0, 2, c->vr, "vr");
	this->Print_vec( 0, 3, c->vu, "vu");
	this->Print_vec( 0, 4, c->vd, "vd");
*/
	if(keys[VK_F12]){	SaveScreen_BMP(); keys[VK_F12] = 0;}

#ifdef _DEBUG
	Timer t1;
	t1.Reset();
	while(t1.GetTime()<set.time_delay);
#endif

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

		font0->Print(15,i++,"Program: ASE loader",0);
		font0->SetColor(0.9f,0.9f,1.0f,1.f);

		if(english)
		{
			font0->Print(8,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(10,i++,"Controls:",0);
			font0->Print(2,i++,"F1 - show/hide menu",1);
			font0->Print(2,i++,"Menu is to control arrow and mouse.",1);
			font0->Print(2,i++,"Character on right is to use to control item out of menu.",1);
			font0->Print(2,i++,"Setting press relevant key, cancellation press backspace",1);
			font0->Print(2,i++,"Setting of numeric value, arrow right, left, shift, mouse",1);
			font0->Print(2,i++,"In program, press key increase, shift+key. decrease",1);
			font0->Print(2,i++,"F4 - switch to window",1);
			font0->Print(2,i++,"F12 - take screenshot",1);
			font0->Print(2,i++,"H - help",1);
			i++;
			font0->Print(2,i++,"Controls of camera:",0);
			font0->Print(2,i++,"W,S,arrow up,down,left and right button - move forward and backward",1);
			font0->Print(2,i++,"A,D,Delete,Insert - move left and right",1);
			font0->Print(2,i++,"Home,End - move up and down",1);
			font0->Print(2,i++,"arrow right,left and moving mouse <-> - rotation about y axis",1);
			font0->Print(2,i++,"Page Up, Page Down a moving mouse | - rotation about x axis",1);
			font0->Print(2,i++,"Shift - slow moving",1);
			font0->Print(2,i++,"R - reset camera",1);
		}
		else
		{
			font0->Print(8,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(10,i++,"Pouzivane klavesy:",0);
			font0->Print(2,i++,"F1 - zobrazy/skryje menu",1);
			font0->Print(2,i++,"Menu sa ovlada sipkamy a mysov.",1);
			font0->Print(2,i++,"Znak na pravo sa pouziva na ovladanie danej polozky mimo menu",1);
			font0->Print(2,i++,"Nastavenie stlacenim prislusnej klavesy, zrusenie backspace",1);
			font0->Print(2,i++,"Nastavenie ciselnej hodnoty, sipky vpravo vlavo, shift, mysov",1);
			font0->Print(2,i++,"V programe, klavesov zvysovanie, shift+klav. znizovanie",1);
			font0->Print(2,i++,"F4 - prepne do okna",1);
			font0->Print(2,i++,"F12 - urobi screenshot",1);
			font0->Print(2,i++,"medzernik - skok",1);
			i++;
			font0->Print(2,i++,"Ovladanie kamery:",0);
			font0->Print(2,i++,"W,S,sipka hore,dole,lave a prave tlacitko - pohyb dopredu a dozadu",1);
			font0->Print(2,i++,"A,D,Delete,Insert - pohyb do boka vlavo vpravo",1);
			font0->Print(2,i++,"Home,End - pohyb hore, dole",1);
			font0->Print(2,i++,"sipka vpravo,vlavo a pohyb mysov <->  - rotacia okolo osi y",1);
			font0->Print(2,i++,"Page Up, Page Down a pohyb mysov | - rotacia okolo osi x",1);
			font0->Print(2,i++,"Shift - pomalsi pohyb",1);
			font0->Print(2,i++,"R - reset kamery",1);
		}
	}
	font0->End();
	if(smenu)english ? menu[0].Render() : menu[1].Render();
	
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
//	light0.Diffuse.r = 1.0f;	light0.Diffuse.g = 1.0f;	light0.Diffuse.b = 1.0f;
	light0.Diffuse.r = 0.8f;	light0.Diffuse.g = 0.8f;	light0.Diffuse.b = 0.8f;
	light0.Diffuse.a = 1.0f;

	light0.Ambient.r = 0.0f;
	light0.Ambient.g = 0.0f;
	light0.Ambient.b = 0.0f;
	light0.Ambient.a = 1.0f;

	light0.Direction.x = 0;	light0.Direction.y = 0;	light0.Direction.z = 1;
	
	light0.Position.x = 0;
	light0.Position.y = 0;
	light0.Position.z = -10;
	light0.Attenuation0 = 1.f;

	light0.Range = 1000;

	pd->SetLight( 0, &light0 );
	pd->LightEnable( 0, TRUE);
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.2f,0.2f,0.2f,1) );
//	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );

	for(unsigned int i=0; i<caps.MaxSimultaneousTextures; i++)
	{
		pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( i, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
		pd->SetTextureStageState( i, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);
	//	pd->SetTextureStageState( i, D3DTSS_MINFILTER,D3DTEXF_POINT );
	//	pd->SetTextureStageState( i, D3DTSS_MAGFILTER,D3DTEXF_POINT );
	//	pd->SetTextureStageState( i, D3DTSS_MIPFILTER,D3DTEXF_POINT );
	//	pd->SetTextureStageState( i, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	//	pd->SetTextureStageState( i, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
	//	pd->SetTextureStageState( i, D3DTSS_MAXANISOTROPY, 1);
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

int Scene::ReadInit(char *filename)
{
	FILE* sub=NULL;
	char temp[100];
	char varname[100],varvalue[100];
	int ret=0;

	sub = fopen( filename, "rb");
	if(sub == NULL)return 0;

	renderableCubeTexture=0;
	while(GetVariable( temp, sub, varname, varvalue, 100))
	{
		float fl=(float)atof(varvalue);
		int in=atoi(varvalue);
	//	strlwr(varname);				// Convert a string to lowercase
		if(!strcmp("renderableCubeTexture",varname))renderableCubeTexture=in;
	}
	fclose(sub);
	if(renderableCubeTexture==0)renderableCubeTexture=128;
	return 1;
}

int Scene::GetVariable(char *temp, FILE *sub, char *varname, char *varvalue, int size_strings)
{
	if(fgets(temp, size_strings, sub) == NULL)
	{
		temp[0] = NULL;
		return 0;
	}
	varname[0]=NULL;
	varvalue[0]=NULL;
	char rovnasa=0;
	for(int i=0,j=0,k=0; temp[i]!=NULL&&temp[i]!='\n'&&temp[i]!='/'; i++)		// prejdeme vsetky znaky
	{
		if(temp[i]==' ' || temp[i]=='\t')continue;
		if(temp[i]=='='){ rovnasa=1;continue;}
		if(rovnasa)
		{
			varvalue[k]=temp[i]; k++;
		}
		else
		{
			varname[j]=temp[i]; j++;
		}
	}
	varname[j]=NULL;
	varvalue[k]=NULL;
	return 1;
}

