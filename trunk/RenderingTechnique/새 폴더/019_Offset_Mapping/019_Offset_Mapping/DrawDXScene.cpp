#include "Scene.h"

void Scene::DrawDXScene()
{
	if(pd->TestCooperativeLevel()!= D3D_OK){ RestoreLostDevice(); return;}

	ProcessKeyForMenu();
	SetAnisotropicFilter();

    // Clear the backbuffer
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();
	stat.Reset();

	if(set.wire_frame)pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	if(set.cull_face)pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW );

	if((int)set.normalizer_size!=normalizer_size){normalizer_size = (int)set.normalizer_size;	normalizer.MakeCubeMapNormalizer(normalizer_size);}

	if(set.model==1)
		DrawPlane();
	else
		DrawRoom();

	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );
	pd->SetRenderState( D3DRS_LIGHTING, 0 );
	pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	font0.Begin();
	// vypis FPS
	timeframe = timer0.GetTime();
	fps = 1.0f / timeframe;
	timer0.Reset();
	fps_counter.GetAvgFps(timeframe,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
	font0.Print( 0, 0, text, 0);
	
	font0.Print( 10, 0, set.ps14_selected?"ps14":"ps11", 0);
	if(set.model==1)
	{
		switch(set.mode)
		{
		case 1: font0.Print( 40, 0, "Base map", 0); break;
		case 2: font0.Print( 40, 0, "Offset Mapping", 0); break;
		case 3: font0.Print( 40, 0, "Displacement Mapping", 0); break;
		}
	}
	else
	{
		font0.Print_int(20,0,set.use_AsePplVso_class?room_vso.lights.size():room.lights.size(),"lights:");
		font0.Print( 40, 0, set.offset_mapping?"Offset Mapping":"Bump Mapping", 0);
	}
	if(keys[VK_F12])
	{
		font0.End();
		pd->EndScene();
		pd->Present( NULL, NULL, NULL, NULL );
		SaveScreen_BMP();
		return;
	}

	if(set.print_statistic)
		stat.Draw(0,1,&font0,fps_counter.avg_fps);
	
	if(set.print_variable)
	{
		font0.Print_vec(0, 4, c->vp, "vp");
		font0.Print_vec(0, 5, c->vr, "vr");
		font0.Print_vec(0, 6, c->vu, "vu");
		font0.Print_vec(0, 7, c->vd, "vd");
	}
	
	if(keys['H'])DrawHelp();

	font0.End();
	if(smenu)english ? menu[0].Render() : menu[1].Render();
	
	// End the scene
	pd->EndScene();

	// Present the backbuffer contents to the display
	pd->Present( NULL, NULL, NULL, NULL );
}

void Scene::DrawPlane()
{
	mat4 m;
	if(keys['1'])set.mode=1;
	else if(keys['2'])set.mode=2;
	else if(keys['3'])set.mode=3;
	menu[0].items[3].IntIndex = set.mode-1;
	menu[1].items[3].IntIndex = set.mode-1;
	
	c = &cplane;

	if(smenu)
		c->SetTransform();
	else 
	{
		english ? menu[0].ChangeSetting() : menu[1].ChangeSetting();
		c->MoveWorld(fps, ROTATE_WALK|ROTATE_ABOUT_POINT);
		c->SetTransform();
	}

	mat4 proj,mod;

	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );
	pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
	m = proj*mod;
	m = ~m;

	mod.Identity();
	mod.MakeMatrix( c->vr, c->vu, -c->vd);		// right-handed coordinate systems
	mod.Translate( -c->vp);

	mod = !mod;
	vec4 eye = mod*vec(0,0,0);
	static vec4 lp(0,0,5,0);
    if(set.set_light_position)
    {
        lp = eye;            // lp - light position
    }

	pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
	pd->SetVertexShaderConstant(  4, eye.v, 1 );	// c4 eye
	pd->SetVertexShaderConstant(  5, vec4(0.5f,0.5f,0.5f,0.5f).v, 1 );		// c5

	pd->SetVertexShaderConstant(  6, lp.v, 1 );		// lpos
/*
Offset mapping
	
posunie texturove koordinacie:
kde je kopec posunie texeli od nas, 
kde je dolina posunie k nam

toto sa dosiahne pripocitanim posuvu k texturovim koordinaciam
pre x smer:
kladny posuv posunie texturu vlavo, pretoze po pripocitani sa na povodne miesto dostane texel z prava

posun spocitame podla vysky a polohy pozorovatela,
vyskova mapu najprv upravime aby hodnoty boli v rozsahu [-0.02,0.02]
potom vyuzijeme eye vektor transponovany do surface space
offset = eye*height
newTextureCoordinations = offset + baseTextureCoordinations
pretoze DX ma texturove koordinacie pre smer y opacne ako svetova suradnicova sustava,
musime offset.y negovat, mozme to urobit bud vo vertex shadere s eye vektorom, alebo pri 
zmensovani vyskovej mapy v pixel shadery

pixel shader 1.4
textures:
	r0 - base
	r1 - normal map
	r2 - height map
	r3 - normalization cube map
	r4 - normalization cube map
texture coordinations:
	t0 - base & normal & height
	t1 - normal
	t2 - L
	t3 - H

pixel shader 1.1
u' = (height*scale + bias)*Ex + u = height*scale*Ex + bias*Ex + u = (height, 1, 1) dot (scale*Ex, bias*Ex, u)
v' = (height*scale + bias)*Ey + v = height*scale*Ey + bias*Ey + v = (height, 1, 1) dot (scale*Ey, bias*Ey, v)

textures & texture coordinations:
	t0 - texture = (height, 1, 1), (u, v)
	t1 - no texture, (scale*Ex, bias*Ex, u)
	t2 - base map,   (scale*Ey, bias*Ey, v)
	
bump Offset mapping:
1 pass
	t2 - normal map
	t3 - Cubic Normalized L vector
2 pass
	t2 - base map
3 pass
	t2 - normal map
	t3 - Cubic Normalized H vector

pixel shader 1.1 and 1.4
	c0 - ambient
	c1 - diffuse
	c2 - specular

base
offset
dispacement

bump map
offset + bump
displacement + bump
*/
	
	pd->SetPixelShaderConstant( 0, set.ambient.v, 1);
	pd->SetPixelShaderConstant( 1, set.diffuse.v, 1);
	pd->SetPixelShaderConstant( 2, set.specular.v, 1);
	pd->SetPixelShaderConstant( 7, vec4(0.005f*set.height_scale,0.005f*set.height_scale,0.005f*set.height_scale).v, 1);		// for ps 14

	pd->SetVertexShaderConstant(  7, vec4(1,-1,1).v, 1);	// for vs
	pd->SetVertexShaderConstant(  8, vec4(0.01f*set.height_scale,-0.005f*set.height_scale,0).v, 1); // for vs for ps11
	
	if(set.bump_mapping)
	{
		switch(set.mode)
		{
		case 1: // base map
			tex[set.actual_texture].base.SetTexture(0);
			tex[set.actual_texture].normal_map.SetTexture(1);
			normalizer.SetTexture(2);		// L
			normalizer.SetTexture(3);		// H

			if(set.ps14_selected)
			{
				pd->SetPixelShader( ps14_bump.hPixelShader );
				plane.Render(vs11_for_ps14.hVertexShader );
			}
			else
			{
				pd->SetPixelShader( ps11_bump.hPixelShader );
				plane.Render(vs11_for_ps11_bump.hVertexShader );
			}
			break;
		case 2: // offset mapping
			if(set.ps14_selected)
			{
				tex[set.actual_texture].base.SetTexture(0);
				tex[set.actual_texture].normal_map.SetTexture(1);
				tex[set.actual_texture].height.SetTexture(2);
				normalizer.SetTexture(3);		// L
				normalizer.SetTexture(4);		// H

				pd->SetPixelShader( ps14_bump_OffsetMapping.hPixelShader );
				plane.Render(vs11_for_ps14.hVertexShader);
			}
			else // ps11
			{
				tex[set.actual_texture].height_for_ps11.SetTexture(0);
				// pass 1 - D*N.L + A
				tex[set.actual_texture].normal_map.SetTexture(2);
				normalizer.SetTexture(3);		// L
				
				pd->SetPixelShader( ps11_bump_OffsetMapping_1_pass.hPixelShader );
				plane.Render( vs11_for_ps11_bump_OffsetMapping_1_pass.hVertexShader);

				// pass 2 - (D*N.L + A)*base
				pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
				pd->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO);
				pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
				pd->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
				pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				
				tex[set.actual_texture].base.SetTexture(2);
				
				pd->SetPixelShader( ps11_bump_OffsetMapping_2_pass.hPixelShader );
				plane.Render( vs11_for_ps11_bump_OffsetMapping_2_pass.hVertexShader);

				// pass 3 - S*N.H^32 + (D*N.L + A)*Base
				pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
				pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
				
				tex[set.actual_texture].normal_map.SetTexture(2);
				normalizer.SetTexture(3);		// H
				
				pd->SetPixelShader( ps11_bump_OffsetMapping_3_pass.hPixelShader );
				plane.Render( vs11_for_ps11_bump_OffsetMapping_3_pass.hVertexShader);
				// end
				pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS);
				pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
				pd->SetRenderState( D3DRS_ZWRITEENABLE, 1 );
			}
			break;
		case 3: // Displacement Mapping
			tex[set.actual_texture].base.SetTexture(0);
			tex[set.actual_texture].normal_map.SetTexture(1);
			normalizer.SetTexture(2);		// L
			normalizer.SetTexture(3);		// H

			if(set.ps14_selected)
			{
				pd->SetPixelShader( ps14_bump.hPixelShader );
				tex[set.actual_texture].plane_DisplacementMapping.Render(vs11_for_ps14.hVertexShader);
			}
			else
			{
				pd->SetPixelShader( ps11_bump.hPixelShader );
				tex[set.actual_texture].plane_DisplacementMapping.Render(vs11_for_ps11_bump.hVertexShader);
			}
			break;
		}
	}
	else	// witout bump mapping
	{
		switch(set.mode)
		{
		case 1: // base map
			tex[set.actual_texture].base.SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			plane.Render();
			break;
		case 2: // offset mapping
			if(set.ps14_selected)
			{
				tex[set.actual_texture].base.SetTexture(0);
				tex[set.actual_texture].height.SetTexture(2);
				normalizer.SetTexture(4);

				pd->SetPixelShader( ps14_OffsetMapping.hPixelShader );
				plane.Render(vs11_for_ps14.hVertexShader);
			}
			else // ps11
			{
				tex[set.actual_texture].height_for_ps11.SetTexture(0);
				tex[set.actual_texture].base.SetTexture(2);
				pd->SetPixelShader( ps11_OffsetMapping.hPixelShader );
				plane.Render(vs11_for_ps11_OffsetMapping.hVertexShader);
			}
			break;
		case 3: // Displacement Mapping
			tex[set.actual_texture].base.SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			tex[set.actual_texture].plane_DisplacementMapping.Render();
			break;
		}
	}

	pd->SetPixelShader( NULL );
	pd->SetTexture( 0, NULL );
	pd->SetTexture( 1, NULL );
	pd->SetTexture( 2, NULL );
	pd->SetTexture( 3, NULL );
	pd->SetTexture( 4, NULL );
	pd->SetTexture( 5, NULL );
}

void Scene::DrawRoom()
{
	mat4 m;
	int i;
	c = &croom;
	AseFile *ase;

	if(set.use_AsePplVso_class)
		pAseForCollision = &room_vso;
	else
		pAseForCollision = &room;
	ase = pAseForCollision;

	if(smenu)
		c->SetTransform();
	else 
	{
		english ? menu[0].ChangeSetting() : menu[1].ChangeSetting();
		c->MoveWorld(fps, ROTATE_WALK);
		CheckColosion();
		c->SetTransform();
	}

	mat4 proj,mod;

	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );
	pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
	m = proj*mod;
	ase->CheckVisibility(m);
	m = ~m;

	mod.Identity();
	mod.MakeMatrix( c->vr, c->vu, -c->vd);		// right-handed coordinate systems
	mod.Translate( -c->vp);

	mod = !mod;
	vec4 eye = mod*vec(0,0,0);
	vec4 lp(0,2.0f,0,1.f/6.f);

	pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
	pd->SetVertexShaderConstant(  4, eye.v, 1 );	// c4 eye
	pd->SetVertexShaderConstant(  5, vec4(0.5f,0.5f,0.5f,0.0f).v, 1 );		// c5

//	pd->SetVertexShaderConstant(  6, lp.v, 1 );		// lpos
/*
Offset mapping
	
posunie texturove koordinacie:
kde je kopec posunie texeli od nas, 
kde je dolina posunie k nam

toto sa dosiahne pripocitanim posuvu k texturovim koordinaciam
pre x smer:
kladny posuv posunie texturu vlavo, pretoze po pripocitani sa na povodne miesto dostane texel z prava

posun spocitame podla vysky a polohy pozorovatela,
vyskova mapu najprv upravime aby hodnoty boli v rozsahu [-0.02,0.02]
potom vyuzijeme eye vektor transponovany do surface space
offset = eye*height
newTextureCoordinations = offset + baseTextureCoordinations
pretoze DX ma texturove koordinacie pre smer y opacne ako svetova suradnicova sustava,
musime offset.y negovat, mozme to urobit bud vo vertex shadere s eye vektorom, alebo pri 
zmensovani vyskovej mapy v pixel shadery

pixel shader 1.4
textures:
	r0 - base
	r1 - normal map
	r2 - height map
	r3 - normalization cube map
	r4 - normalization cube map
texture coordinations:
	t0 - base & normal & height
	2 - L for attenuation
	t3 - L
	t4 - H
	t5 - eye

pixel shader 1.1

textures & texture coordinations:
; t0 - base
; t1 - normal map
; t2 - H with cube map normalizer
; t3 - L for attenuation
; v0 - L
	
pixel shader 1.1 and 1.4
	c0 - ambient
	c1 - diffuse
	c2 - specular
*/
	if(keys[VK_LBUTTON] && !smenu && set.time_since_last_shot>1.f/set.shot_per_second)
	{
		z_Light_fly a;

		a.color.set( rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX );
		a.lpos = eye+0.3f*c->vd;
		a.light_range = set.shot_light_range;
		a.dir = c->vd;
		a.time = 1.f;
		shot_lights.push_back(a);
		set.time_since_last_shot = 0;
		if(keys[VK_SHIFT])
			keys[VK_LBUTTON] = 0;
	}
	set.time_since_last_shot += timeframe;

	if(set.lights_moving)
	{
		float max2 = MAX(ase->max.Length2(),ase->min.Length2());
		max2 = MAX(max2,c->vp.Length2());
		for( i=0; i<shot_lights.size(); i++)
		{
		//	uPalette pal;
		//	pal.set_fire();
		//	shot_lights[i].color = pal.get(i/((float)shot_lights.size()-1));
			shot_lights[i].lpos += timeframe*set.shot_light_speed*shot_lights[i].dir;
			vec ret = ase->CheckSimpleCollision( shot_lights[i].lpos, 0.2f);
			if(set.collision)
			{
				if(ret!=shot_lights[i].lpos || !PointInBox(shot_lights[i].lpos, ase->min, ase->max))
					shot_lights[i].dir.clear();
			}
			else
			{
				if(shot_lights[i].lpos.Length2()>max2)
					shot_lights[i].dir.clear();
			}

			if(shot_lights[i].dir.Length2()<0.01f)
			{
				shot_lights[i].time -= timeframe;
			}
			
			if(shot_lights[i].time < 0)
			{
				shot_lights.erase(shot_lights.begin()+i);
				i--;
			}
		}
	}

	ase->lights.clear();		// clear lights
	// add static lights
	if(set.static_lights)
	{
		for( i=0; i<ase->static_lights.size(); i++)
			ase->lights.push_back(ase->static_lights[i]);
	}

	// add shot lights
	for( i=0; i<shot_lights.size(); i++)
	{
		z_Light a;
		a.color = shot_lights[i].color;
		if(shot_lights[i].dir.Length2()<0.01f)
			a.light_range = set.shot_light_range*shot_lights[i].time;
		else
			a.light_range = set.shot_light_range;
		a.lpos = shot_lights[i].lpos;
		a.modification = 1;
		ase->lights.push_back(a);
	}
	
	pd->SetPixelShaderConstant( 0, set.ambient.v, 1);
	pd->SetPixelShaderConstant( 1, set.diffuse.v, 1);
	pd->SetPixelShaderConstant( 2, set.specular.v, 1);
	pd->SetPixelShaderConstant( 7, vec4(0.005f*set.height_scale,0.005f*set.height_scale,0.005f*set.height_scale).v, 1);		// for ps 14

	pd->SetVertexShaderConstant(  7, vec4(1,-1,1).v, 1);			// for vs
	
	if(set.use_AsePplVso_class)
	{
		if(set.ps14_selected)
		{
			normalizer.SetTexture(3);		// L
			normalizer.SetTexture(4);		// H
			if(set.offset_mapping)
			{
				normalizer.SetTexture(5);	// eye
				room_vso.ppl_Render( vs11_room_ps14.hVertexShader, ps14_room_OffsetMapping_with_ambient.hPixelShader, 
								  vs11_room_ps14_with_optimalization.hVertexShader, ps14_room_OffsetMapping.hPixelShader,
								  1, c->vp+2.f*c->vd, set.max_count_of_lights);
			}
			else
			{
				room_vso.ppl_Render( vs11_room_ps14.hVertexShader, ps14_room_with_ambient.hPixelShader, 
								  vs11_room_ps14_with_optimalization.hVertexShader, ps14_room.hPixelShader, 
								  0, c->vp+2.f*c->vd, set.max_count_of_lights);
			}
		}
		else	//ps11
		{
			if(set.offset_mapping)
			{
				room_vso.Render(1,0);
			}
			else
			{
				normalizer.SetTexture(2);		// H
				room_vso.ppl_Render( vs11_room_ps11.hVertexShader, ps11_room_with_ambient.hPixelShader, 
								  vs11_room_ps11_with_optimalization.hVertexShader, ps11_room.hPixelShader, 
								  0, c->vp+2.f*c->vd, set.max_count_of_lights);
			}
		}
	}
	else // use AsePpl class
	{
	//	for(int i=0; i<100; i++)
		{
			room.ppl_compute_light_intenzity();
			room.ppl_MakeVertexBuffers_ps11(set.max_count_of_lights);
	//		for(int j=0; j<room.lights.size(); j++)room.lights[j].modification=1;
		}
		if(set.ps14_selected)
		{
			if(set.offset_mapping)
			{
				normalizer.SetTexture(3);	// L
				normalizer.SetTexture(4);	// H
				normalizer.SetTexture(5);	// eye
				vs11_room_for_AsePpl_class.SetVertexShader();
				room.ppl_Render_( set.max_count_of_lights, 
								  ps14_room_OffsetMapping_for_AsePpl_class_with_ambient.hPixelShader, 
								  ps14_room_OffsetMapping_for_AsePpl_class.hPixelShader, 1);
			}
			else
			{
				normalizer.SetTexture(2);		// L
				normalizer.SetTexture(4);		// H
				room.ppl_Render_ps14(set.max_count_of_lights);
			}
		}
		else	//ps11
		{
			if(set.offset_mapping)
			{
				room.Render(1,0);
			}
			else
			{
				normalizer.SetTexture(2);		// H
				room.ppl_Render_ps11(set.max_count_of_lights);
			}
		}
	}

	pd->SetPixelShader( NULL );
	pd->SetTexture( 0, NULL );
	pd->SetTexture( 1, NULL );
	pd->SetTexture( 2, NULL );
	pd->SetTexture( 3, NULL );
	pd->SetTexture( 4, NULL );
	pd->SetTexture( 5, NULL );

	// render light == white sphere

	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m );
	pd->SetRenderState( D3DRS_LIGHTING, 1 );

	if(set.show_lights)
	{
		if(set.static_lights)
		{
			for( i=0; i<ase->static_lights.size(); i++)
			{
				mat4 m1(m);
				m1.Translate(ase->static_lights[i].lpos);
				pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m1.m );
				pd->SetRenderState( D3DRS_AMBIENT, vec4(ase->static_lights[i].color).getBGRA() );
				sphere_light.Render();
			}
		}
	}

	for( i=0; i<shot_lights.size(); i++)
	{
		mat4 m1(m);
		m1.Translate(shot_lights[i].lpos);
		m1.Scale(0.2f,0.2f,0.2f);
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m1.m );
		pd->SetRenderState( D3DRS_AMBIENT, vec4(shot_lights[i].color).getBGRA() );
		sphere_light.Render();
	}
}


void Scene::ProcessKeyForMenu()
{
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
		{
			SetCursorPos(mouse_pos.x, mouse_pos.y);
			ShowCursor(1);
		}
		else
		{
			GetCursorPos(&mouse_pos);
			SetCursorPos(screen_x/2,screen_y/2);
			ShowCursor(0);
		}
	}
}

void Scene::SetAnisotropicFilter()
{
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
}

void Scene::DrawHelp()
{
	mat4 m;
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

	font0.Print(21,i++,"Program: Offset Mapping", 0);
	font0.SetColor(0.9f,0.9f,1.0f,1.f);

	if(english)
	{
		font0.Print(8,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
		font0.SetColor();
		font0.Print(10,i++,"Controls:", 1);
	//	font0.Print(2,i++,"space - change model",1);
		font0.Print(2,i++,"space and right button - jump",1);
		font0.Print(2,i++,"left button - shot, with shift one light",1);
		font0.Print(2,i++,"1,2,3 - change mode",1);
		font0.Print(2,i++,"F1 - show/hide menu",1);
		font0.Print(2,i++,"Menu is to control arrow and mouse.",1);
		font0.Print(2,i++,"Character on right is to use to control item out of menu.",1);
		font0.Print(2,i++,"Setting press relevant key, cancellation press backspace",1);
		font0.Print(2,i++,"Setting of numeric value, arrow right, left, shift, mouse",1);
		font0.Print(2,i++,"In program, press key increase, shift+key. decrease",1);
		i++;
		font0.Print(2,i++,"F4 - switch to window",1);
		font0.Print(2,i++,"F12 - take screenshot",1);
		font0.Print(2,i++,"H - help", 1);
		font0.Print(2,i++,"mouse, button on mouse - moving",1);
		font0.Print(2,i++,"W, S, A, D, arrows, Page Up,Page Down, mouse - moving",1);
		font0.Print(2,i++,"shift - slow moving",1);
		font0.Print(2,i++,"R - reset camera",1);
	}
	else
	{
		font0.Print(8,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
		font0.SetColor();
		font0.Print(10,i++,"Pouzivane klavesy:", 1);
	//	font0.Print(2,i++,"medzernik - meni model",1);
		font0.Print(2,i++,"medzernik a prave tlacitko - skok",1);
		font0.Print(2,i++,"lave tlacitko - vystreli svetlo, s shift iba jedno svetlo",1);
		font0.Print(2,i++,"1,2,3 - meni mod",1);
		font0.Print(2,i++,"F1 - zobrazy/skryje menu",1);
		font0.Print(2,i++,"Menu sa ovlada sipkamy a mysov.",1);
		font0.Print(2,i++,"Znak na pravo sa pouziva na ovladanie danej polozky mimo menu",1);
		font0.Print(2,i++,"Nastavenie stlacenim prislusnej klavesy, zrusenie backspace",1);
		font0.Print(2,i++,"Nastavenie ciselnej hodnoty, sipky vpravo vlavo, shift, mysov",1);
		font0.Print(2,i++,"V programe, klavesov zvysovanie, shift+klav. znizovanie",1);
		i++;
		font0.Print(2,i++,"F4 - prepne do okna",1);
		font0.Print(2,i++,"F12 - ulozi obrazovku do subora",1);
		font0.Print(2,i++,"H - help", 1);
		font0.Print(2,i++,"mys, tlacitka na mysi - pohyb", 1);
		font0.Print(2,i++,"W, S, A, D, sipky, Page Up,Page Down, mys - pohyb", 1);
		font0.Print(2,i++,"shift - pomaly alebo rychli pohyb", 1);
		font0.Print(2,i++,"R - reset kamery", 1);
	}
}
