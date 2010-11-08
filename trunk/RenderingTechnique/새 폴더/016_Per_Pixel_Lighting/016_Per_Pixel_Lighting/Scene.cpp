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
	font0 = NULL;
	c = NULL;
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
	{ 	MessageBox( hWnd, "Program need pixel shader 1.1", "ERROR", MB_OK); error=1; return; }

	if(caps.TextureCaps&D3DPTEXTURECAPS_VOLUMEMAP )supp_texture3D = 1;
	else supp_texture3D=0;

	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	InitDX();

	font0 = new Font_bitmap("data/font.bmp");
	if(error)return;

	FILE* fi;
	fi = fopen("data/Earthbump.JPG","rb");
	if(fi==NULL)
	{
		english ? MessageBox(hWnd,"Copy file Earthbump.JPG from DX program 10 to folder data","Missing file: data/Earthbump.JPG",MB_OK):
		          MessageBox(hWnd,"Skopiruj subor Earthbump.JPG z DX programu 10 do adresara data","Chyba subor: data/Earthbump.JPG",MB_OK);
	}
	else fclose(fi);

	// load vertex shaders
	if(!vs10_Diffuse_Specular.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_clamp.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_clamp.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_with_cubemap_normalizer.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_with_cubemap_normalizer.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_attenuation.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_attenuation.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_attenuation_texture_3D.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_attenuation_texture_3D.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation_texture_3D.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation_texture_3D.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation_for_ps11.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation_for_ps11.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation_for_ps11_cn.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation_for_ps11_cn.txt", dwDecl)){	error = 1;	return;}
	if(!vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11_cn.LoadVS( "Shaders/016_Per_Pixel_Lighting/vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11_cn.txt", dwDecl)){	error = 1;	return;}

	// load pixel shaders
	if(!ps11_1_AD.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_1_AD.txt")){	error = 1;	return;}
	if(!ps11_2_AD_cube_normalizer.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_2_AD_cube_normalizer.txt")){	error = 1;	return;}
	if(!ps11_3_ADS16.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_3_ADS16.txt")){	error = 1;	return;}
	if(!ps11_4_ADS16_cube_normalizer.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_4_ADS16_cube_normalizer.txt")){	error = 1;	return;}
	if(!ps11_5_ADSn.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_5_ADSn.txt")){	error = 1;	return;}
	if(!ps11_7_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_7_attenuation_1.txt")){	error = 1;	return;}
	if(!ps11_7_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_7_attenuation_2.txt")){	error = 1;	return;}
	if(!ps11_7_attenuation_lookup.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_7_attenuation_lookup.txt")){	error = 1;	return;}
	if(!ps11_8_ADS16_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_8_ADS16_attenuation_1.txt")){	error = 1;	return;}
	if(!ps11_8_ADS16_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_8_ADS16_attenuation_2.txt")){	error = 1;	return;}
	if(!ps11_8_ADS16_attenuation_texture_3D.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_8_ADS16_attenuation_texture_3D.txt")){	error = 1;	return;}
	if(!ps11_9_ADS16_cn_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_9_ADS16_cn_attenuation_1.txt")){	error = 1;	return;}
	if(!ps11_9_ADS16_cn_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_9_ADS16_cn_attenuation_2.txt")){	error = 1;	return;}
	if(!ps11_9_ADS16_cn_attenuation_texture_3D.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps11_9_ADS16_cn_attenuation_texture_3D.txt")){	error = 1;	return;}
	if(ps12)
		if(!ps12_8_ADS16_attenuation_lookup.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps12_8_ADS16_attenuation_lookup.txt")){	error = 1;	return;}

	if(ps14)
	{
		if(!ps14_1_AD.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_1_AD.txt")){	error = 1;	return;}
		if(!ps14_2_AD_cube_normalizer.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_2_AD_cube_normalizer.txt")){	error = 1;	return;}
		if(!ps14_3_ADS16.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_3_ADS16.txt")){	error = 1;	return;}
		if(!ps14_4_ADS16_cube_normalizer.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_4_ADS16_cube_normalizer.txt")){	error = 1;	return;}
		if(!ps14_5_ADSn.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_5_ADSn.txt")){	error = 1;	return;}
		if(!ps14_6_ADSn_cube_normalizer.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_6_ADSn_cube_normalizer.txt")){	error = 1;	return;}
		if(!ps14_7_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_7_attenuation_1.txt")){	error = 1;	return;}
		if(!ps14_7_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_7_attenuation_2.txt")){	error = 1;	return;}
		if(!ps14_7_attenuation_lookup.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_7_attenuation_lookup.txt")){	error = 1;	return;}
		if(!ps14_8_ADS16_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_8_ADS16_attenuation_1.txt")){	error = 1;	return;}
		if(!ps14_8_ADS16_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_8_ADS16_attenuation_2.txt")){	error = 1;	return;}
		if(!ps14_8_ADS16_attenuation_lookup.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_8_ADS16_attenuation_lookup.txt")){	error = 1;	return;}
		if(!ps14_8_ADS16_attenuation_texture_3D.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_8_ADS16_attenuation_texture_3D.txt")){	error = 1;	return;}
		if(!ps14_9_ADS16_cn_attenuation_1.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_9_ADS16_cn_attenuation_1.txt")){	error = 1;	return;}
		if(!ps14_9_ADS16_cn_attenuation_2.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_9_ADS16_cn_attenuation_2.txt")){	error = 1;	return;}
		if(!ps14_9_ADS16_cn_attenuation_lookup.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_9_ADS16_cn_attenuation_lookup.txt")){	error = 1;	return;}
		if(!ps14_9_ADS16_cn_attenuation_texture_3D.LoadPS( "Shaders/016_Per_Pixel_Lighting/ps14_9_ADS16_cn_attenuation_texture_3D.txt")){	error = 1;	return;}
	}

	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
    pd->BeginScene();
	font0->Begin();
	font0->Print_xy_scale(120,400,"Per Pixel Lighting",0,3,3);
	font0->Print_xy_scale(160,380,english? "Loading models and creating normal maps":"Nacitavam modely a vytvaram normal mapy",0,1.2f,1.2f);
	font0->Print_xy_scale(300,320,"Marek Mizanin",0,1.5f,1.5f);
	font0->Print_xy_scale(260,280,"www.mizanin.szm.sk",0,1.5f,1.5f);
	font0->Print_xy_scale(english ? 340:300,240,english ? "Key H - help" : "Klavesa H - Help",0,1.0f,1.0f);
	font0->End();
	pd->EndScene();
	pd->Present( NULL, NULL, NULL, NULL );
	if(!CreateVertexBufferForHelpRectange(10,screen_y_font-45,screen_x_font-80,230,0.6f)){	error = 1;	return;}

	sphere.Init("data/sphere.3ds",3,0,0,2);
	sphere.ComputeTangentBinormal();
	if(!sphere.Init_CreateDXObjects())error = 1;
	if(error)return;

	room01.Init("data/016dxmodel/room01.3ds",3,0,0,2);
	room01.ComputeTangentBinormal();
	if(!room01.Init_CreateDXObjects())error = 1;
	if(error)return;

	room02.Init("data/016dxmodel/room02.3ds",3,0,0,2);
	room02.ComputeTangentBinormal();
	if(!room02.Init_CreateDXObjects())error = 1;
	if(error)return;

	sphere_light.Init(5,10,0.03f);
	
	normalizer_size = 64;
	if(!normalizer.MakeCubeMapNormalizer(normalizer_size))error=1;

	Image im;

	table_size=256;
	specTable_1D_exponet = 32;
	if(!im.MakeExponentTable1D(table_size,specTable_1D_exponet)){error = 1;return;}
	if(!specTable_1D.load(im)){error = 1;return;}

	specTable_NLNH_RGBA_exponet = 32;
	if(!im.MakeExponentTable2D_NL_NH(table_size,table_size,specTable_NLNH_RGBA_exponet,4)){error = 1;return;}
//	im.SaveTGA("NL_NH.tga");
	if(!specTable_NLNH_RGBA.load(im,0,3,D3DFMT_A8R8G8B8)){error = 1;return;}

	table_size_attenuation_1D = 256;

	for( int equation=0; equation<7; equation++)
	{
		im.MakeAttenuationTable1D( table_size_attenuation_1D, equation);
		attenuation1D[equation].load(im);
	}

	table_size_attenuation_3D = 64;
	if(supp_texture3D)
	{
		Image3D im3;
		im3.Make1to1(32,32,32);
		if(!tex3D_1to1.load(im3))MessageBox(hWnd,"Nenacital 3D texturu","ERROR",MB_OK);

		for( int equation=0; equation<7; equation++)
		{
			if(im3.MakeAttenuationTable3D( table_size_attenuation_3D, equation))
				attenuation3D[equation].load(im3);
		}
	}
	
	// menu 0 - english
	// menu 1 - slovak

	menu[0].Init( font0 );
	menu[1].Init(font0);

	menu[0].AddText( "Show/hide menu - key F1           ");
	menu[1].AddText( "Zobrazenie/skrytie menu klavesa F1");
	menu[0].AddText(""); menu[1].AddText("");

	int num;
	for(int lang=0; lang<2; lang++)
	{
		set.mode=1;
		menu[lang].AddInt("mode",'B', &set.mode);
		num=1; menu[lang].AddIntItem("1. (D*N.L + A)*BASE",&num);
		num++; menu[lang].AddIntItem("2. (D*N.L + A)*BASE - cube nor.", &num);
		num++; menu[lang].AddIntItem("3. (D*N.L + A)*BASE + S*(N.H)^16", &num);
		num++; menu[lang].AddIntItem("4. (D*N.L + A)*BASE + S*(N.H)^16 - cube nor.", &num);
		num++; menu[lang].AddIntItem("5. (D*N.L + A)*BASE + S*(N.H)^n", &num);
		num++; menu[lang].AddIntItem("6. (D*N.L + A)*BASE + S*(N.H)^n - cube nor.", &num);
		num++; menu[lang].AddIntItem(english ? "7. attenuation" : "7. utlm - attenuation", &num);
		num++; menu[lang].AddIntItem("8. A*BASE + att*( D*N.L*base + S*(N.H)^16", &num);
		num++; menu[lang].AddIntItem("9. A*BASE + att*( D*N.L*base + S*(N.H)^16 - cn", &num);
		menu[lang].AddIntSetIndex(8);
	}
	menu[0].AddInt("model",' ', &set.model);
	num = 0; menu[0].AddIntItem("model: sphere",&num);
	num = 1; menu[0].AddIntItem("model: box", &num);
	num = 2; menu[0].AddIntItem("model: box and sphere", &num);
	menu[0].AddIntSetIndex(1);

	menu[1].AddInt("model",' ', &set.model);
	num = 0; menu[1].AddIntItem("model: gula", &num);
	num = 1; menu[1].AddIntItem("model: kocka", &num);
	num = 2; menu[1].AddIntItem("model: kocka s gulov", &num);
	menu[1].AddIntSetIndex(1);

	if(ps14)
	{
		menu[0].AddBool( "pixel shader 1.4", 'V', &set.ps14_selected, 1);
		menu[1].AddBool( "pixel shader 1.4", 'V', &set.ps14_selected, 1);
	}
	else
		set.ps14_selected = 0;
	menu[0].AddFloatComp("Specular exponent", 'E', &set.specExponent, 1.f, 256.f, 32.f);
	menu[1].AddFloatComp("Specular exponent", 'E', &set.specExponent, 1.f, 256.f, 32.f);

	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddInt("exponent table size",-1,&set.table_size);
		num =  2; menu[lang].AddIntItem("exponent table size = 2", &num);
		num =  4; menu[lang].AddIntItem("exponent table size = 4", &num);
		num =  8; menu[lang].AddIntItem("exponent table size = 8", &num);
		num = 16; menu[lang].AddIntItem("exponent table size = 16", &num);
		num = 32; menu[lang].AddIntItem("exponent table size = 32", &num);
		num = 64; menu[lang].AddIntItem("exponent table size = 64", &num);
		num =128; menu[lang].AddIntItem("exponent table size = 128", &num);
		num =256; menu[lang].AddIntItem("exponent table size = 256", &num);
		num =512; menu[lang].AddIntItem("exponent table size = 512", &num);
		num =1024;menu[lang].AddIntItem("exponent table size = 1024", &num);
		menu[lang].AddIntSetIndex(7);
	}

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

	menu[0].AddText(""); menu[1].AddText("");
	menu[0].AddText("   Light Attenuation");
	menu[1].AddText("   Utlm svetla - Attenuation");

	menu[0].AddFloatComp( "light range", -1, &set.light_range, 1.f, 30.f, 10.f);
	menu[1].AddFloatComp( "dosah svetla", -1, &set.light_range, 1.f, 30.f, 10.f);

	menu[0].AddInt("attenuation_equation", 'U', &set.attenuation_equation);
	num=2; menu[0].AddIntItem( "attenuation = 1 - d2sat^0.5", &num);
	num=0; menu[0].AddIntItem( "attenuation = 1 - d2sat", &num);
	num=6; menu[0].AddIntItem( "attenuation = e ^ (-4*d2^2)", &num);
	num=1; menu[0].AddIntItem( "attenuation =(1 - d2sat)^2", &num);
	num=3; menu[0].AddIntItem( "attenuation = 1 / (10*d2+0.25)", &num);
	num=5; menu[0].AddIntItem( "attenuation = e ^ (-4*d2)", &num);
	num=4; menu[0].AddIntItem( "attenuation = e ^ (-3*d2^0.5)", &num);
	menu[0].AddIntSetIndex(1);

	menu[1].AddInt("vzorec pre utlm", 'U', &set.attenuation_equation);
	num=2; menu[1].AddIntItem( "utlm = att = 1 - d2sat^0.5", &num);
	num=0; menu[1].AddIntItem( "utlm = att = 1 - d2sat", &num);
	num=6; menu[1].AddIntItem( "utlm = att = e ^ (-4*d2^2)", &num);
	num=1; menu[1].AddIntItem( "utlm = att =(1 - d2sat)^2", &num);
	num=3; menu[1].AddIntItem( "utlm = att = 1 / (10*d2+0.25)", &num);
	num=5; menu[1].AddIntItem( "utlm = att = e ^ (-4*d2)", &num);
	num=4; menu[1].AddIntItem( "utlm = att = e ^ (-4*d2^0.5)", &num);
	menu[1].AddIntSetIndex(1);

	menu[0].AddInt("attenuation_technique", 'T', &set.attenuation_technique);
	num=0; menu[0].AddIntItem( "technique: pixel shader without depended read", &num);
	num++; menu[0].AddIntItem( "technique: pixel shader with 1D texture lookup", &num);
	num++; menu[0].AddIntItem( "technique: 3D texture", &num);
	menu[0].AddIntSetIndex(1);

	menu[1].AddInt("attenuation_technique", 'T', &set.attenuation_technique);
	num=0; menu[1].AddIntItem( "technika: pixel shader bez citania s textury", &num);
	num++; menu[1].AddIntItem( "technika: pixel shader s citanim z 1D textury", &num);
	num++; menu[1].AddIntItem( "technika: 3D textura", &num);
	menu[1].AddIntSetIndex(1);

	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddInt("table size attenuation 1D",-1,&set.table_size_attenuation_1D);
		num =  2; menu[lang].AddIntItem("table size attenuation 1D = 2", &num);
		num =  4; menu[lang].AddIntItem("table size attenuation 1D = 4", &num);
		num =  8; menu[lang].AddIntItem("table size attenuation 1D = 8", &num);
		num = 16; menu[lang].AddIntItem("table size attenuation 1D = 16", &num);
		num = 32; menu[lang].AddIntItem("table size attenuation 1D = 32", &num);
		num = 64; menu[lang].AddIntItem("table size attenuation 1D = 64", &num);
		num =128; menu[lang].AddIntItem("table size attenuation 1D = 128", &num);
		num =256; menu[lang].AddIntItem("table size attenuation 1D = 256", &num);
		num =512; menu[lang].AddIntItem("table size attenuation 1D = 512", &num);
		num =1024;menu[lang].AddIntItem("table size attenuation 1D = 1024", &num);
		menu[lang].AddIntSetIndex(7);
	}

	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddInt("table size attenuation 3D",-1,&set.table_size_attenuation_3D);
		num =  2; menu[lang].AddIntItem("table size attenuation 3D = 2", &num);
		num =  4; menu[lang].AddIntItem("table size attenuation 3D = 4", &num);
		num =  8; menu[lang].AddIntItem("table size attenuation 3D = 8", &num);
		num = 16; menu[lang].AddIntItem("table size attenuation 3D = 16", &num);
		num = 32; menu[lang].AddIntItem("table size attenuation 3D = 32", &num);
		num = 64; menu[lang].AddIntItem("table size attenuation 3D = 64", &num);
		num =128; menu[lang].AddIntItem("table size attenuation 3D = 128", &num);
		menu[lang].AddIntSetIndex(5);
	}
	menu[0].AddText(""); menu[1].AddText("");

	for( lang=0; lang<2; lang++)
	{
		menu[lang].AddFloatComp( "ambient R", 'Z', &set.ambient.x, 0.f, 1.0f, 0.3f);
		menu[lang].AddFloatComp( "ambient G", 'Z', &set.ambient.y, 0.f, 1.0f, 0.3f);
		menu[lang].AddFloatComp( "ambient B", 'Z', &set.ambient.z, 0.f, 1.0f, 0.3f);
		
		menu[lang].AddFloatComp( "diffuse R", 'X', &set.diffuse.x, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "diffuse G", 'X', &set.diffuse.y, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "diffuse B", 'X', &set.diffuse.z, 0.f, 1.0f, 1.f);
		
		menu[lang].AddFloatComp( "specular R", 'C', &set.specular.x, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "specular G", 'C', &set.specular.y, 0.f, 1.0f, 1.f);
		menu[lang].AddFloatComp( "specular B", 'C', &set.specular.z, 0.f, 1.0f, 1.f);
	}

	menu[0].AddText(""); menu[1].AddText("");
	menu[0].AddKey( "set light position", 'L', &set.set_light_pos);
	menu[1].AddKey( "nastavy poziciu svetla", 'L', &set.set_light_pos);
	
	menu[0].AddBool( "light rotate", 'K', &set.light_rotate, 1);
	menu[1].AddBool( "rotacia svetla", 'K', &set.light_rotate, 1);

	menu[0].AddFloatComp( "light rotate speed", -1, &set.light_rotate_speed, 0.2f, 30.f, 1.f);
	menu[1].AddFloatComp( "rychlost rotacie svetla", -1, &set.light_rotate_speed, 0.2f, 30.f, 1.f);

	menu[0].AddBool( "show textures", -1, &set.show_textures, 0);
	menu[1].AddBool( "ukaz textury", -1, &set.show_textures, 0);

	menu[0].AddBool( "wire frame", 'P', &set.wire_frame, 0);
	menu[1].AddBool( "kresli ciary", 'P', &set.wire_frame, 0);

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

/*	for( lang=0; lang<2; lang++)
	{
		for(int i=0; i<11; i++)
		menu[lang].AddText("dalsi");
		menu[lang].AddText("123456_10 123456_20 123456_30 123456_40 123456_50 123456_60 123456_70 123456_80_123456_90_123456_100");
	}*/


	cam[0].reset.vd.set( 0.18f,-0.59f, 0.79f);
	cam[0].reset.vu.set( 0.13f, 0.81f, 0.57f);
	cam[0].reset.vr.set(-0.97f, 0.00f, 0.22f);
	
	cam[0].reset.vd.RotY(180.f);
	cam[0].reset.vu.RotY(180.f);
	cam[0].reset.vr.RotY(180.f);
	cam[0].reset.vpd = 3;
	cam[0].reset.vp = -3.f*cam[0].reset.vd;
	cam[0].Reset();
	lp[0].set(1.1f,.1f,-1.8f);
	lp[0].RotY(180.f);

	cam[1].set.k_remap_mouse_button_vd_vpd=0;
	cam[1].set_q3_mouse_style();
	cam[1].setKeys();
	cam[1].reset.vp.set(0,15,11);
	cam[1].reset.vd.set(0,0,-1); cam[1].reset.vd.RotX(-45.f);
	cam[1].reset.vr.set(1,0,0);
	cam[1].reset.vu.set(0,1,0); cam[1].reset.vu.RotX(-45.f);
	cam[1].Reset();
	lp[1].set(4.7f,5.0f,0.0f);
	lp[2].set(4.7f,5.0f,0.0f);

	c = &(cam[0]);
	mouse_pos = c->last;
	c->Reset();
}

Scene::~Scene()
{
	if(font0!=NULL) delete font0;
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
        gl.glTexCoord(0,0); gl.glVertex( p );
        gl.glTexCoord(1,0); gl.glVertex( p+s );
        gl.glTexCoord(0,1); gl.glVertex( p+t );
        gl.glTexCoord(1,1); gl.glVertex( p+s+t );
    gl.glEnd();
}
