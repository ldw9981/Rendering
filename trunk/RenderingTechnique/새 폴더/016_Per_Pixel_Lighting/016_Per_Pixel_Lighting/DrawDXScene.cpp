#include "Scene.h"

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

    // Clear the backbuffer
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();

	if(set.model==0)c = &cam[0]; else c = &cam[1];

	if(smenu)
		c->SetTransform();
	else 
	{
		english ? menu[0].ChangeSetting() : menu[1].ChangeSetting();
		if(set.model==0)c = &cam[0]; else c = &cam[1];
		if(set.model==0)
			c->MoveWorld(fps, ROTATE_WALK|ROTATE_ABOUT_POINT);	// otoci svetom na zaklade vstupov od mysi a klavesnice
		else
			c->MoveWorld(fps, ROTATE_WALK);

	}

	if(keys['1'])set.mode=1;
	else if(keys['2'])set.mode=2;
	else if(keys['3'])set.mode=3;
	else if(keys['4'])set.mode=4;
	else if(keys['5'])set.mode=5;
	else if(keys['6'])set.mode=6;
	else if(keys['7'])set.mode=7;
	else if(keys['8'])set.mode=8;
	else if(keys['9'])set.mode=9;
//	else if(keys['0'])set.mode=10;
	menu[0].items[2].IntIndex = set.mode-1;
	menu[1].items[2].IntIndex = set.mode-1;

	if(set.wire_frame)pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	if(set.mode==2 || set.mode==4 || set.mode==6 || set.mode==9)
	{
		if((int)set.normalizer_size!=normalizer_size)
		{
			normalizer_size = (int)set.normalizer_size;
			normalizer.MakeCubeMapNormalizer(normalizer_size);
		}
	}
	
	if(set.table_size!=table_size)
	{
		Image im;
		table_size = set.table_size;

		if(im.MakeExponentTable1D(table_size,specTable_1D_exponet))
			specTable_1D.load(im);

		if(im.MakeExponentTable2D_NL_NH(table_size,table_size,specTable_NLNH_RGBA_exponet,4))
			specTable_NLNH_RGBA.load(im,0,3,D3DFMT_A8R8G8B8);
	}

	if(set.mode>4 && set.specExponent != specTable_1D_exponet)
	{
		Image im;
		specTable_1D_exponet = set.specExponent;
		if(im.MakeExponentTable1D(table_size,specTable_1D_exponet))
			specTable_1D.load(im);
	}

	if(set.mode==5 && set.specExponent != specTable_NLNH_RGBA_exponet)
	{
		Image im;
		specTable_NLNH_RGBA_exponet = set.specExponent;
		if(im.MakeExponentTable2D_NL_NH(table_size,table_size,specTable_NLNH_RGBA_exponet,4))
			specTable_NLNH_RGBA.load(im,0,3,D3DFMT_A8R8G8B8);
	}

	if(set.mode>=7 && set.table_size_attenuation_1D != table_size_attenuation_1D)
	{
		Image im;
		table_size_attenuation_1D = set.table_size_attenuation_1D;
		for( int equation=0; equation<7; equation++)
		{
			im.MakeAttenuationTable1D( table_size_attenuation_1D, equation);
			attenuation1D[equation].load(im);
		}
	}

	if(set.mode>=7 && set.table_size_attenuation_3D != table_size_attenuation_3D && supp_texture3D)
	{
		Image3D im3;
		table_size_attenuation_3D = set.table_size_attenuation_3D;
		for( int equation=0; equation<7; equation++)
		{
			if(im3.MakeAttenuationTable3D( table_size_attenuation_3D, equation))
				attenuation3D[equation].load(im3);
		}
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
	if(set.set_light_pos)
	{
		lp[set.model] = eye;			// lp - light position
		set.light_rotate=0;
	}
	static int light_down[MODELS_COUNT-1];
	if(set.light_rotate)
	{
		if(set.model>0)
		{
			lp[set.model].RotY(60.f*timeframe*set.light_rotate_speed);
			if(light_down[set.model-1])
				lp[set.model].y -= 0.25f*timeframe*set.light_rotate_speed;
			else
				lp[set.model].y += 0.25f*timeframe*set.light_rotate_speed;

			if(lp[set.model].y > 8.5f)light_down[set.model-1]=1;
			if(lp[set.model].y < 0.5f)light_down[set.model-1]=0;
		}
		else
			lp[set.model].RotY(60.f*timeframe*set.light_rotate_speed);
	}

	vec4 lpos( lp[set.model].x, lp[set.model].y, lp[set.model].z, 0.f);
	vec4 c5(0.5f,0.5f,0.5f,0.5f);
	vec4 light_range;
	light_range.set(1/set.light_range,0,0,0);
	pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
	pd->SetVertexShaderConstant(  4, lpos.v, 1 );	// c4 lpos
	pd->SetVertexShaderConstant(  5, c5.v, 1 );		// c5
	pd->SetVertexShaderConstant(  6, eye.v, 1 );	// c6 eye
	pd->SetVertexShaderConstant(  7, light_range.v, 1 );	// c7 light_range

/*
1 - (D*N.L + A)*BASE
2 - (D*N.L + A)*BASE - with cube normalizer
3 - (D*N.L + A)*BASE + S*(N.H)^16
4 - (D*N.L + A)*BASE + S*(N.H)^16 - with cube normalizer
5 - (D*N.L + A)*BASE + S*(N.H)^n
6 - (D*N.L + A)*BASE + S*(N.H)^n - with cube normalizer
7 - attenuation
8 - A*BASE + att*( D*N.L*base + S*(N.H)^16
9 - A*BASE + att*( D*N.L*base + S*(N.H)^16 - with cube normalizer

distance   = (x*x + y*y + z*z)^0.5
distance^2 =  x*x + y*y + z*z
normalized_distance = distance/light_range
normalized_distance^2 = distance^2/light_range^2 = (x*x + y*y + z*z) / R*R = x*x/R*R + y*y/R*R + z*z/R*R
d2 = normalized_distance^2		[0,oo]
d2_saturate = min( d2, 1)		[0,1]

att:                 out range  0 .5  1    2   5  10    a. ps | b. ps 1D lookup | c. 3D tex
0. att = 1 - d2_sat     [0,1]   1 .5  0    0   0   0    yes   | yes             | yes
1. att =(1 - d2_sat)^2   [0,1]   1 .25 0    0   0   0   yes   | yes             | yes
2. att = 1 - d2_sat^0.5  [0,1]   0 .29 0    0   0   0   no    | yes             | yes
3. att = 1 / (10*d2+0.25)[0,4]   4 .19 .1   .05 .02 0   no    | yes             | yes
4. att = e ^(-2*d2^0.5)  [0,1]   1 .24 .14  .05 .01 0   no    | yes             | yes
5. att = e ^(-4*d2)      [0,1]   1 .13 .02  0   0   0   no    | yes             | yes
6. att = e ^(-4*d2^2)    [0,1]   1 .36 .02  0   0   0   no    | yes             | yes

a. pixel shader
b. pixel shader with 1D texture lookup
c. 3D texture
d. 1D and 2D texture

// base texture 0
// normal map 1
// cube normalizer 2
// cube normalizer 3
// attenuation lookup 4
Phong:  R = 2(N.L)N - L,   V = eye - vertex,   (D*N.L + A)*BASE + S*(R.V)^16
*/

	pd->SetPixelShaderConstant( 0, set.ambient.v, 1);
	pd->SetPixelShaderConstant( 1, set.diffuse.v, 1);
	pd->SetPixelShaderConstant( 2, set.specular.v, 1);

	if(set.ps14_selected)
	{
		switch(set.mode)
		{
		case 1: 
			pd->SetPixelShader( ps14_1_AD.hPixelShader ); 
			RenderModel( vs10_Diffuse_Specular.hVertexShader );
			break;
		case 2: 
			pd->SetPixelShader( ps14_2_AD_cube_normalizer.hPixelShader );
			normalizer.SetTexture(2);
			RenderModel( vs10_Diffuse_Specular_with_cubemap_normalizer.hVertexShader );
			break;
		case 3: 
			pd->SetPixelShader( ps14_3_ADS16.hPixelShader );
			RenderModel( vs10_Diffuse_Specular.hVertexShader );
			break;
		case 4: 
			pd->SetPixelShader( ps14_4_ADS16_cube_normalizer.hPixelShader ); 
			normalizer.SetTexture(2);
			normalizer.SetTexture(3);
			RenderModel( vs10_Diffuse_Specular_with_cubemap_normalizer.hVertexShader );
			break;
		case 5:
			pd->SetPixelShader( ps14_5_ADSn.hPixelShader );
			specTable_1D.SetTexture(4);
			pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			RenderModel( vs10_Diffuse_Specular.hVertexShader );
			break;
		case 6:
			pd->SetPixelShader( ps14_6_ADSn_cube_normalizer.hPixelShader );
			normalizer.SetTexture(2);
			normalizer.SetTexture(3);
			specTable_1D.SetTexture(4);
			pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			RenderModel( vs10_Diffuse_Specular_with_cubemap_normalizer.hVertexShader );
			break;
		case 7:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					pd->SetPixelShader( ps14_7_attenuation_1.hPixelShader );
					RenderModel( vs10_attenuation.hVertexShader, 0 );
					break;
				case 1:
					pd->SetPixelShader( ps14_7_attenuation_2.hPixelShader );
					RenderModel( vs10_attenuation.hVertexShader, 0 );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				attenuation1D[set.attenuation_equation].SetTexture(0);
				pd->SetPixelShader( ps14_7_attenuation_lookup.hPixelShader );
				RenderModel( vs10_attenuation.hVertexShader, 0);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				break;
			case 2:	// 3D texture
				if(supp_texture3D)
				{
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					attenuation3D[set.attenuation_equation].SetTexture(0);
					pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
					RenderModel( vs10_attenuation_texture_3D.hVertexShader, 0);
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
					pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_WRAP);
					pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
				}
				break;
			}
			break;
		case 8:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					pd->SetPixelShader( ps14_8_ADS16_attenuation_1.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader );
					break;
				case 1:
					pd->SetPixelShader( ps14_8_ADS16_attenuation_2.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				attenuation1D[set.attenuation_equation].SetTexture(4);
				pd->SetPixelShader( ps14_8_ADS16_attenuation_lookup.hPixelShader );
				RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader);
				break;
			case 2:	// 3D texture
				if(supp_texture3D)
				{
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					attenuation3D[set.attenuation_equation].SetTexture(4);
					pd->SetPixelShader( ps14_8_ADS16_attenuation_texture_3D.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_texture_3D.hVertexShader);
				}
				break;
			}
			break;
		case 9:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					pd->SetPixelShader( ps14_9_ADS16_cn_attenuation_1.hPixelShader );
					normalizer.SetTexture(2);
					normalizer.SetTexture(3);
					RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader );
					break;
				case 1:
					pd->SetPixelShader( ps14_9_ADS16_cn_attenuation_2.hPixelShader );
					normalizer.SetTexture(2);
					normalizer.SetTexture(3);
					RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				normalizer.SetTexture(2);
				normalizer.SetTexture(3);
				pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				attenuation1D[set.attenuation_equation].SetTexture(4);
				pd->SetPixelShader( ps14_9_ADS16_cn_attenuation_lookup.hPixelShader );
				RenderModel( vs10_Diffuse_Specular_attenuation.hVertexShader);
				break;
			case 2:	// 3D texture
				if(supp_texture3D)
				{
					normalizer.SetTexture(2);
					normalizer.SetTexture(3);
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 4, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					attenuation3D[set.attenuation_equation].SetTexture(4);
					pd->SetPixelShader( ps14_9_ADS16_cn_attenuation_texture_3D.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_texture_3D.hVertexShader);
				}
				break;
			}
			break;
		}
	}
	else	// ps 1.1
	{
		switch(set.mode)
		{
		case 1: 
			pd->SetPixelShader( ps11_1_AD.hPixelShader );
			RenderModel( vs10_Diffuse_Specular_clamp.hVertexShader );
			break;
		case 2:
			pd->SetPixelShader( ps11_2_AD_cube_normalizer.hPixelShader ); 
			normalizer.SetTexture(2);
			RenderModel( vs10_Diffuse_Specular_with_cubemap_normalizer.hVertexShader );
			break;
		case 3: 
			pd->SetPixelShader( ps11_3_ADS16.hPixelShader );
			RenderModel( vs10_Diffuse_Specular_clamp.hVertexShader );
			break;
		case 4: 
			pd->SetPixelShader( ps11_4_ADS16_cube_normalizer.hPixelShader ); 
			normalizer.SetTexture(2);
			normalizer.SetTexture(3);
			RenderModel( vs10_Diffuse_Specular_with_cubemap_normalizer.hVertexShader );
			break;
		case 5:
			pd->SetPixelShader( ps11_5_ADSn.hPixelShader );
			pd->SetTextureStageState( 3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			specTable_NLNH_RGBA.SetTexture(3);
			RenderModel( vs10_Diffuse_Specular.hVertexShader );
			break;
		case 6:
			font0->Begin();
			font0->Print_xy_scale(10,300,english ? "impossible with ps 1.1 in 1 pass" : "nemozne s ps 1.1 v jednom prechode",0,2,4);
			font0->End();
			break;
		case 7:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					pd->SetPixelShader( ps11_7_attenuation_1.hPixelShader );
					RenderModel( vs10_attenuation.hVertexShader, 0 );
					break;
				case 1:
					pd->SetPixelShader( ps11_7_attenuation_2.hPixelShader );
					RenderModel( vs10_attenuation.hVertexShader, 0 );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
				if(supp_texture3D)
					tex3D_1to1.SetTexture(0);
				pd->SetTextureStageState( 2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				attenuation1D[set.attenuation_equation].SetTexture(2);
				pd->SetPixelShader( ps11_7_attenuation_lookup.hPixelShader );
				RenderModel( vs10_attenuation.hVertexShader, 0);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_WRAP);
				break;
			case 2:	// 3D texture
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
				if(supp_texture3D)
					attenuation3D[set.attenuation_equation].SetTexture(0);
				pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				RenderModel( vs10_attenuation_texture_3D.hVertexShader, 0);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_WRAP);
				pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
				break;
			}
			break;
		case 8:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					pd->SetPixelShader( ps11_8_ADS16_attenuation_1.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_for_ps11.hVertexShader );
					break;
				case 1:
					pd->SetPixelShader( ps11_8_ADS16_attenuation_2.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_for_ps11.hVertexShader );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				if(ps12)
				{
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					if(supp_texture3D)
						tex3D_1to1.SetTexture(2);
					pd->SetTextureStageState( 3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					attenuation1D[set.attenuation_equation].SetTexture(3);
					pd->SetPixelShader( ps12_8_ADS16_attenuation_lookup.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_for_ps11.hVertexShader);
				}
				else
				{
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "request ps 1.2" : "vyzaduje ps 1.2",0,2,4);
					font0->End();
				}
				break;
			case 2:	// 3D texture
				if(supp_texture3D)
				{
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 2, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					attenuation3D[set.attenuation_equation].SetTexture(2);
					pd->SetPixelShader( ps11_8_ADS16_attenuation_texture_3D.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11.hVertexShader);
				}
				break;
			}
			break;
		case 9:
			switch(set.attenuation_technique)
			{
			case 0: // pixel shader without depended read
				switch(set.attenuation_equation)
				{
				case 0:
					normalizer.SetTexture(2);
					pd->SetPixelShader( ps11_9_ADS16_cn_attenuation_1.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_for_ps11_cn.hVertexShader );
					break;
				case 1:
					normalizer.SetTexture(2);
					pd->SetPixelShader( ps11_9_ADS16_cn_attenuation_2.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_for_ps11_cn.hVertexShader );
					break;
				default:
					font0->Begin();
					font0->Print_xy_scale(10,300,english ? "impossible with ps without depended read" : "nemozne s ps bez citania z textury",0,2,4);
					font0->End();
				}
				break;
			case 1: // pixel shader with 1D texture lookup
				font0->Begin();
				font0->Print_xy_scale(10,300,english ? "impossible with ps 1.2 in 1 pass" : "nemozne s ps 1.2 v 1 passe",0,2,4);
				font0->End();
				break;
			case 2:	// 3D texture
				if(supp_texture3D)
				{
					normalizer.SetTexture(2);
					pd->SetTextureStageState( 3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
					pd->SetTextureStageState( 3, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
					attenuation3D[set.attenuation_equation].SetTexture(3);
					pd->SetPixelShader( ps11_9_ADS16_cn_attenuation_texture_3D.hPixelShader );
					RenderModel( vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11_cn.hVertexShader);
				}
				break;
			}
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

	// render light == white sphere
	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m );
	m.Translate(lp[set.model]);
	pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m );
	sphere_light.Render();
	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	if(set.show_textures)
	{
		float scale_x = screen_x_font/(float)window_size_x;
		float scale_y = screen_y_font/(float)window_size_y;
		D3DXMATRIX matProj;
		D3DXMatrixOrthoOffCenterLH( &matProj, 0, window_size_x, 0, window_size_y, -1, 1);
		pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m);
		pd->SetTransform( D3DTS_PROJECTION, &matProj);
		mod.Identity();
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );

		if(set.mode==5 && !set.ps14_selected)
		{
			specTable_NLNH_RGBA.SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			pd->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			DrawQuad( vec(10-0.5f,10-0.5f,0), vec(0,0,1), vec(table_size>256 ? 256:table_size,0,0), vec(0,table_size>256 ? 256:table_size,0));

			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE );
			DrawQuad( vec(256+20-0.5f,10-0.5f,0), vec(0,0,1), vec(table_size>256 ? 256:table_size,0,0), vec(0,table_size>256 ? 256:table_size,0));

			font0->Begin();
			font0->Print_xy_scale(scale_x*10, scale_y*((table_size>256 ? 256:table_size)+20), "NL - RGB", 0, 1, 1);
			font0->Print_xy_scale(scale_x*(256+20), scale_y*((table_size>256 ? 256:table_size)+20), "NH^exp - A", 0, 1, 1);
			font0->End();
		}
		if( (set.mode==5 || set.mode==6) && set.ps14_selected)
		{
			specTable_1D.SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			DrawQuad( vec(520-0.5f,10-0.5f,0), vec(0,0,1), vec(table_size>256 ? 480:table_size,0,0), vec(0,16,0));
			font0->Begin();
			font0->Print_xy_scale(scale_x*520, scale_y*(16+20), "1D exponet table NH^exp", 0, 1, 1);
			font0->End();
		}
		
		if(set.mode%2==0 && set.mode < 7 || set.mode==9)
		{
			GLizer gl;
			// cube normalizer map
			vec p0,p,s,t;
			if(set.normalizer_size>=256)
				p0.set(-0.5f, 0.5f,0);
			else 
				p0.set(10-0.5f,10-0.5f,0);
			s.set(set.normalizer_size>256 ? 256 : set.normalizer_size,0,0);
			t.set(0,set.normalizer_size>256 ? 256 : set.normalizer_size,0);
			normalizer.SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			
			// 3 4 vertex
			// 1 2
			//   1
			// 2 3 4 5
			//   6
			// 1 +y
			p = p0 + s + 2.f*t;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord(-1, 1,-1); gl.glVertex( p );
				gl.glTexCoord( 1, 1,-1); gl.glVertex( p+s );
				gl.glTexCoord(-1, 1, 1); gl.glVertex( p+t );
				gl.glTexCoord( 1, 1, 1); gl.glVertex( p+s+t );
			gl.glEnd();

			// 2 -x
			p = p0 + t;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord(-1,-1, 1); gl.glVertex( p );
				gl.glTexCoord(-1,-1,-1); gl.glVertex( p+s );
				gl.glTexCoord(-1, 1, 1); gl.glVertex( p+t );
				gl.glTexCoord(-1, 1,-1); gl.glVertex( p+s+t );
			gl.glEnd();

			// 3 -z
			p = p0 + s + t;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord(-1,-1,-1); gl.glVertex( p );
				gl.glTexCoord( 1,-1,-1); gl.glVertex( p+s );
				gl.glTexCoord(-1, 1,-1); gl.glVertex( p+t );
				gl.glTexCoord( 1, 1,-1); gl.glVertex( p+s+t );
			gl.glEnd();

			// 4 +x
			p = p0 + 2.f*s + t;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord( 1,-1,-1); gl.glVertex( p );
				gl.glTexCoord( 1,-1, 1); gl.glVertex( p+s );
				gl.glTexCoord( 1, 1,-1); gl.glVertex( p+t );
				gl.glTexCoord( 1,-1, 1); gl.glVertex( p+s+t );
			gl.glEnd();

			// 5 +z
			p = p0 + 3.f*s + t;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord( 1,-1, 1); gl.glVertex( p );
				gl.glTexCoord(-1,-1, 1); gl.glVertex( p+s );
				gl.glTexCoord( 1,-1, 1); gl.glVertex( p+t );
				gl.glTexCoord(-1, 1, 1); gl.glVertex( p+s+t );
			gl.glEnd();
			
			// 6 -y
			p = p0 + s;
			gl.glBegin(D3DPT_TRIANGLESTRIP, 0, 0, 1);
				gl.glTexCoord(-1,-1, 1); gl.glVertex( p );
				gl.glTexCoord( 1,-1, 1); gl.glVertex( p+s );
				gl.glTexCoord(-1,-1,-1); gl.glVertex( p+t );
				gl.glTexCoord( 1,-1,-1); gl.glVertex( p+s+t );
			gl.glEnd();
		}

		if( set.mode>=7 && set.attenuation_technique==1)
		{
			attenuation1D[set.attenuation_equation].SetTexture(0);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			DrawQuad( vec(520-0.5f,60-0.5f,0), vec(0,0,1), vec(table_size_attenuation_1D>256 ? 480:table_size_attenuation_1D,0,0), vec(0,16,0));
			font0->Begin();
			font0->Print_xy_scale(scale_x*520, scale_y*(16+70), "1D attenuation table", 0, 1, 1);
			font0->End();
		}

		pd->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		pd->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		pd->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m);
	}

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

	if(set.mode==8 && set.attenuation_technique==1)
		 font0->Print( 10, 0, set.ps14_selected?"ps14":"ps12", 0);
	else font0->Print( 10, 0, set.ps14_selected?"ps14":"ps11", 0);

	switch(set.mode)
	{
	case 1: font0->Print(0,1,"(D*N.L + A)*BASE",0); break;
	case 2: font0->Print(0,1,"(D*N.L + A)*BASE - with cube normalizer",0); break;
	case 3: font0->Print(0,1,"(D*N.L + A)*BASE + S*(N.H)^16",0); break;
	case 4: font0->Print(0,1,"(D*N.L + A)*BASE + S*(N.H)^16 - with cube normalizer",0); break;
	case 5: font0->Print(0,1,"(D*N.L + A)*BASE + S*(N.H)^n",0); break;
	case 6: font0->Print(0,1,"(D*N.L + A)*BASE + S*(N.H)^n - with cube normalizer",0); break;
	case 7: font0->Print(0,1,"attenuation",0); break;
	case 8: font0->Print(0,1,"A*BASE + att*( D*N.L*base + S*(N.H)^16",0); break;
	case 9: font0->Print(0,1,"A*BASE + att*( D*N.L*base + S*(N.H)^16 - with cube normalizer",0); break;
	case 10: font0->Print(0,1,"diffuse + specular + ambient",0); break;
	}

	if(set.mode>=7)
	{
		switch(set.attenuation_technique)
		{
		case 0: font0->Print(0,2, "attenuation technique: pixel shader", 0); break;
		case 1: font0->Print(0,2, "attenuation technique: pixel shader with 1D texture lookup", 0); break;
		case 2: font0->Print(0,2, "attenuation technique: 3D texture", 0); break;
		}
		switch(set.attenuation_equation)
		{
		case 0: font0->Print(0,3, "attenuation = 1 - d2sat", 0); break;
		case 1: font0->Print(0,3, "attenuation =(1 - d2sat)^2", 0); break;
		case 2: font0->Print(0,3, "attenuation = 1 - d2sat^0.5", 0); break;
		case 3: font0->Print(0,3, "attenuation = 1 / (10*d2+0.25)", 0); break;
		case 4: font0->Print(0,3, "attenuation = e ^ (-3*d2^0.5)", 0); break;
		case 5: font0->Print(0,3, "attenuation = e ^ (-4*d2)", 0); break;
		case 6: font0->Print(0,3, "attenuation = e ^ (-4*d2^2)", 0); break;
		}
	}

	if(set.print_variable)
	{
		font0->Print_vec(0, 4, c->vp, "vp");
		font0->Print_vec(0, 5, c->vr, "vr");
		font0->Print_vec(0, 6, c->vu, "vu");
		font0->Print_vec(0, 7, c->vd, "vd");
		font0->Print_vec(0, 8, lp[set.model], "lp");
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

		font0->Print(21,i++,"Program: Per Pixel Lighting", 0);
		font0->SetColor(0.9f,0.9f,1.0f,1.f);

		if(english)
		{
			font0->Print(8,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(10,i++,"Controls:", 1);
			font0->Print(2,i++,"space - change model",1);
			font0->Print(2,i++,"1,2,3,4,5,6,7,8,9,0 - change mode",1);
			font0->Print(2,i++,"F1 - show/hide menu",1);
			font0->Print(2,i++,"Menu is to control arrow and mouse.",1);
			font0->Print(2,i++,"Character on right is to use to control item out of menu.",1);
			font0->Print(2,i++,"Setting press relevant key, cancellation press backspace",1);
			font0->Print(2,i++,"Setting of numeric value, arrow right, left, shift, mouse",1);
			font0->Print(2,i++,"In program, press key increase, shift+key. decrease",1);
			i++;
			font0->Print(2,i++,"F4 - switch to window",1);
			font0->Print(2,i++,"F12 - take screenshot",1);
			font0->Print(2,i++,"H - help", 1);
			font0->Print(2,i++,"mouse, button on mouse - moving",1);
			font0->Print(2,i++,"arrow, Page Up,Page Down - moving",1);
			font0->Print(2,i++,"shift - slow moving",1);
			font0->Print(2,i++,"R - reset camera",1);
		}
		else
		{
			font0->Print(8,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			font0->SetColor();
			font0->Print(10,i++,"Pouzivane klavesy:", 1);
			font0->Print(2,i++,"medzernik - meni model",1);
			font0->Print(2,i++,"1,2,3,4,5,6,7,8,9,0 - meni mod",1);
			font0->Print(2,i++,"F1 - zobrazy/skryje menu",1);
			font0->Print(2,i++,"Menu sa ovlada sipkamy a mysov.",1);
			font0->Print(2,i++,"Znak na pravo sa pouziva na ovladanie danej polozky mimo menu",1);
			font0->Print(2,i++,"Nastavenie stlacenim prislusnej klavesy, zrusenie backspace",1);
			font0->Print(2,i++,"Nastavenie ciselnej hodnoty, sipky vpravo vlavo, shift, mysov",1);
			font0->Print(2,i++,"V programe, klavesov zvysovanie, shift+klav. znizovanie",1);
			i++;
			font0->Print(2,i++,"F4 - prepne do okna",1);
			font0->Print(2,i++,"F12 - ulozi obrazovku do subora",1);
			font0->Print(2,i++,"H - help", 1);
			font0->Print(2,i++,"mys, tlacitka na mysi - pohyb", 1);
			font0->Print(2,i++,"sipky, Page Up,Page Down - pohyb", 1);
			font0->Print(2,i++,"shift - pomaly alebo rychli pohyb", 1);
			font0->Print(2,i++,"R - reset kamery", 1);

		}
	}
	font0->End();
	if(smenu)english ? menu[0].Render() : menu[1].Render();
	
	// End the scene
	pd->EndScene();

	// Present the backbuffer contents to the display
	pd->Present( NULL, NULL, NULL, NULL );
}

void Scene::RenderModel(DWORD hVertexShader, int texturing)
{
	pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW );
	if(texturing)
	{
		switch(set.model)
		{
		case 0: sphere.Render_pixel_shader( hVertexShader ); break;
		case 1: room01.Render_pixel_shader( hVertexShader ); break;
		case 2: room02.Render_pixel_shader( hVertexShader ); break;
		}
	}
	else
	{
		switch(set.model)
		{
		case 0: sphere.Render(2, 0, hVertexShader ); break;
		case 1: room01.Render(2, 0, hVertexShader ); break;
		case 2: room02.Render(2, 0, hVertexShader ); break;
		}
	}
	pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );
}
