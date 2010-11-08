// Font_bitmap.cpp: implementation of the Font_bitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "Font_bitmap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Font_bitmap::Font_bitmap()
{
	pvb = NULL;
	color.set(1,1,1,1);
}

Font_bitmap::~Font_bitmap()
{
	if(pvb!=NULL)pvb->Release();
}

Font_bitmap::Init(char *file_name)
{
	pvb = NULL;
	color.set(1,1,1,1);
	if(!tex.load( file_name,0,3))
		return 0;
	if( FAILED( pd->CreateVertexBuffer( 256*4*sizeof(zFont_bitmap_CUSTOMVERTEX),D3DUSAGE_WRITEONLY,
						D3DFVF_CUSTOMVERTEX_Font_bitmap,D3DPOOL_DEFAULT, &pvb ) ) )
	{
		return 0;
	}

	void* p;
	if( FAILED( pvb->Lock( 0, 256*4*sizeof(zFont_bitmap_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )
	{
		return 0;
	}

	//	1	3  vertex:  A      texture   +->
	//	0	2           +->              V
	vec v[4];
	v[0].set(0,0,0);
	v[1].set(0,16,0);
	v[2].set(16,0,0);
	v[3].set(16,16,0);
	vec2 t[4];
	for(int i=0; i<256; i++)
	{
		t[0].x = (float) (i%16)/16.f;
		t[0].y = (float) (i/16)/16.f + 0.0625f;

		t[1].x = t[0].x;
		t[1].y = t[0].y - 0.0625f;
		
		t[2].x = t[0].x + 0.0625f;
		t[2].y = t[0].y;
		
		t[3].x = t[2].x;
		t[3].y = t[1].y;
		
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+0].t0 = t[0];
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+0].v  = v[0];
		
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+1].t0 = t[1];
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+1].v  = v[1];

		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+2].t0 = t[2];
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+2].v  = v[2];

		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+3].t0 = t[3];
		((zFont_bitmap_CUSTOMVERTEX*)p)[4*i+3].v  = v[3];
	}
	pvb->Unlock();
	return 1;
}

void Font_bitmap::Begin()
{
	tex.SetTexture();

	pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
//	pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	pd->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_CURRENT );

	// texel*(1-pixel)+alpha*pixel
	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
//	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
//	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR);
	
//	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
//	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
//	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );

	pd->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

	pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*)(void*) &proj);
	pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*)(void*) &model);

	D3DXMATRIX matProj;
	D3DXMatrixOrthoOffCenterLH( &matProj, 0, screen_x_font, 0, screen_y_font, -1, 1);
	pd->SetTransform( D3DTS_PROJECTION, &matProj);
	pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );
	pd->SetRenderState( D3DRS_LIGHTING, 1 );
}

void Font_bitmap::End()
{
	pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
	pd->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	pd->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)(void*) &proj);
	pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*)(void*) &model);
	pd->SetRenderState( D3DRS_LIGHTING, 0 );
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );
}


void Font_bitmap::Print(int x, int y, char *string, int set)
{
	mat4 m;
	int len = strlen(string);

	m.m24 = (float)(screen_y_font - (y+1)*16);

	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(color.x,color.y,color.z,0*color.w) );
	
	pd->SetStreamSource( 0, pvb, sizeof(zFont_bitmap_CUSTOMVERTEX) );
	pd->SetVertexShader( D3DFVF_CUSTOMVERTEX_Font_bitmap );

	for(int i=0; i<len; i++)
	{
		m.m14 = (float)(x+i)*11;
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*)(void*) &m);
		pd->DrawPrimitive( D3DPT_TRIANGLESTRIP, 4*(string[i]+set*128-32), 2 );
	}
}

void Font_bitmap::Print_vec(int x, int y, const vec &a, char *meno)
{
	if(meno!=NULL)
	{
		Print(x, y, meno,0);
		x += strlen(meno)+1;
	}
	
	gcvt( (double)a.x, 2, text);
	Print(x,y,text,0);
	x+=10;
	gcvt( (double)a.y, 2, text);
	Print(x,y,text,0);
	x+=10;
	gcvt( (double)a.z, 2, text);
	Print(x,y,text,0);
}

void Font_bitmap::Print_mat4(int x, int y, const mat4 &m, char *nadpis)
{
	if(nadpis!=NULL)
	{
		Print(x, y++, nadpis,0);
	}
	for(int i=0;i<16;i++)
	{
		gcvt( (double)m.m[i], 2, text);			//prevod realneho cisla na text
		//	a00  a04  a08  a12 
		//  a01  a05  a09  a13
		//  a02  a06  a10  a14
		//  a03  a07  a11  a15
		Print(x+(i/4)*10,y+(i%4),text,0);
	}
}

void Font_bitmap::Print_float(int x, int y, float f, char *meno, int digits)
{
	if(meno!=NULL)
	{
		Print(x, y, meno,0);
		x += strlen(meno)+1;
	}
	
	gcvt( (double)f, digits, text);
	Print(x,y,text,0);
}

void Font_bitmap::Print_xy_scale(int x, int y, char *string, int set, float scale_x, float scale_y)
{
	mat4 m;
	int len = strlen(string);

	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(color.x,color.y,color.z,0*color.w) );

	m.Translate( (float)x, (float)y,0);
	m.Scale( scale_x, scale_y, 1.0f);

	pd->SetStreamSource( 0, pvb, sizeof(zFont_bitmap_CUSTOMVERTEX) );
	pd->SetVertexShader( D3DFVF_CUSTOMVERTEX_Font_bitmap );

	for(int i=0; i<len; i++)
	{
		m.Identity();
		m.Translate( (float)(x+i*11.f*scale_x), (float)y,0);
		m.Scale( scale_x, scale_y, 1.0f);
		pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*) m.m);
		pd->DrawPrimitive( D3DPT_TRIANGLESTRIP, 4*(string[i]+set*128-32), 2 );
	}
}

void Font_bitmap::Print_int(int x, int y, int a, char *meno)
{
	if(meno!=NULL)
	{
		Print(x, y, meno,0);
		x += strlen(meno)+1;
	}
	
	itoa( a, text, 10);
	Print(x,y,text,0);
}
