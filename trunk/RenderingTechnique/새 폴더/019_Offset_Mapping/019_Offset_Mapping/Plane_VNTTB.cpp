// Plane_VNTTB.cpp: implementation of the Plane_VNTTB class.
//
//////////////////////////////////////////////////////////////////////

#include "Plane_VNTTB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Plane_VNTTB::Plane_VNTTB()
{
	pvb = NULL;
}

Plane_VNTTB::~Plane_VNTTB()
{
	if(pvb!=NULL) pvb->Release();
}

// 12 triangles with TRIANGLESTRIP = 8 commonly + 4 empty
// 1   3   5
// 2   4   6+7
// 8+9 11  13
// 10  12  14
int Plane_VNTTB::Init(vec pos, vec x, vec y, vec2 tex_pos, vec2 texxy, int div_x, int div_y)
{
	int count_vertex;
	if(pvb!=NULL) pvb->Release();
	if(div_x<0 || div_y<0)return 0;

	count = (2*div_x+4)*div_y-4;
	count_vertex = count+2;
	
	if( FAILED( pd->CreateVertexBuffer( count_vertex*sizeof(zPlane_VNTTB_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								D3DFVF_CUSTOMVERTEX_PLANE_VNTTB,D3DPOOL_DEFAULT, &pvb ) ) )return 0;
	zPlane_VNTTB_CUSTOMVERTEX* p;
	if( FAILED( pvb->Lock( 0, count_vertex*sizeof(zPlane_VNTTB_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )return 0;
	int i_vec=0;
	float xf,yf1,yf2;
	vec s,t,n;
	n = Normalize(CROSS( x, y));
	tangent( pos, pos+x, pos+y, 
			 tex_pos, vec2(tex_pos.x+texxy.x,tex_pos.y), vec2(tex_pos.x,tex_pos.y+texxy.y), s);
	s.Normalize();
	t = Normalize(CROSS( n, s));
	
	for(int yi=0; yi<div_y; yi++)
	{
		yf1 = 1.f - (float)yi/(float)div_y;
		yf2 = 1.f - (float)(yi+1)/(float)div_y;
		for(int xi=0; xi<div_x+1; xi++)
		{
			xf = (float)xi/(float)div_x;
			p[i_vec].v = pos + x*xf + y*yf1;
			p[i_vec].t0.set( tex_pos.x + texxy.x*xf, tex_pos.y + texxy.y*yf1 );
			p[i_vec].s = s;
			p[i_vec].t = t;
			p[i_vec].n = n;
			i_vec++;
			if(xi==0 && yi!=0 )
			{
				p[i_vec].v = pos + x*xf + y*yf1;
				p[i_vec].t0.set( tex_pos.x + texxy.x*xf, tex_pos.y + texxy.y*yf1 );
				p[i_vec].s = s;
				p[i_vec].t = t;
				p[i_vec].n = n;
				i_vec++;
			}	
			
			p[i_vec].v = pos + x*xf + y*yf2;
			p[i_vec].t0.set( tex_pos.x + texxy.x*xf, tex_pos.y + texxy.y*yf2 );
			p[i_vec].s = s;
			p[i_vec].t = t;
			p[i_vec].n = n;
			i_vec++;
			if(xi==div_x && yi!=div_y-1)
			{
				p[i_vec].v = pos + x*xf + y*yf2;
				p[i_vec].t0.set( tex_pos.x + texxy.x*xf, tex_pos.y + texxy.y*yf2 );
				p[i_vec].s = s;
				p[i_vec].t = t;
				p[i_vec].n = n;
				i_vec++;
			}
		}
	}
	pvb->Unlock();
	return 1;
}

void Plane_VNTTB::Render(DWORD hVertexShader)
{
	pd->SetStreamSource( 0, pvb, sizeof(zPlane_VNTTB_CUSTOMVERTEX) );
	pd->SetVertexShader( hVertexShader );
//	pd__DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, count );

	for(int j=0; j<count-21000; j+=21000)
		pd__DrawPrimitive( D3DPT_TRIANGLESTRIP, j, 21000 );
	pd__DrawPrimitive( D3DPT_TRIANGLESTRIP, j, count-j );
}

int Plane_VNTTB::Init_Displacement_Mapping(vec pos, vec xs, vec ys, vec zs, vec2 tex_pos, vec2 texxy, int div_x, int div_y, char *filename, float scale, float bias)
{
	Image im;
	int count_vertex;
	if(pvb!=NULL) pvb->Release();
	if(div_x<0 || div_y<0)return 0;
	if(!im.Load(filename))return 0;

	int x,y;
	vec **ver,**nor,**tan,**bin;
	vec2 **tex;
	ver = new vec*[div_y+1];		if(ver==NULL)return 0;
	nor = new vec*[div_y+1];		if(nor==NULL)return 0;
	tan = new vec*[div_y+1];		if(tan==NULL)return 0;
	bin = new vec*[div_y+1];		if(bin==NULL)return 0;
	tex = new vec2*[div_y+1];		if(tex==NULL)return 0;
	for( y=0; y<div_y+1; y++)
	{
		ver[y] = new vec[div_x+1];
		nor[y] = new vec[div_x+1];
		tan[y] = new vec[div_x+1];
		bin[y] = new vec[div_x+1];
		tex[y] = new vec2[div_x+1];
	}

	float yf,xf;

	for( y=0; y<div_y+1; y++)
	{
		yf = 1.f - (float)y/(float)div_y;
		for( x=0; x<div_x+1; x++)
		{
			xf = (float)x/(float)div_x;
			ver[y][x] = pos + xs*xf + ys*yf + zs*(scale*im.getPointBilinear(xf,yf,0)+bias);
			tex[y][x].set(tex_pos.x + texxy.x*xf, tex_pos.y + texxy.y*yf);
		}
	}

	vec* facenormal = new vec[div_x*div_y];
	if(facenormal==NULL)return 0;

	for( y=0; y<div_y; y++)
		for( x=0; x<div_x; x++)
			facenormal[y*div_x+x] = MakeNormal( ver[y][x], ver[y+1][x], ver[y][x+1]);

	// compute normal
	for( y=1; y<div_y; y++)
	{
		x = 0;
		nor[y][x]  = facenormal[y*div_x+x];
		nor[y][x] += facenormal[(y-1)*div_x+x];
		nor[y][x].Normalize();
		for( x=1; x<div_x; x++)
		{
			nor[y][x]  = facenormal[y*div_x+x];
			nor[y][x] += facenormal[y*div_x+x-1];
			nor[y][x] += facenormal[(y-1)*div_x+x];
			nor[y][x] += facenormal[(y-1)*div_x+x-1];
			nor[y][x].Normalize();
		}
		x = div_x;
		nor[y][x]  = facenormal[y*div_x+x-1];
		nor[y][x] += facenormal[(y-1)*div_x+x-1];
		nor[y][x].Normalize();
	}

	y = 0; x = 0;
	nor[y][x] = facenormal[y*div_x+x];
	y = 0;
	for( x=1; x<div_x; x++)
	{
		nor[y][x]  = facenormal[y*div_x+x];
		nor[y][x] += facenormal[y*div_x+x-1];
		nor[y][x].Normalize();
	}
	x = div_x;
	nor[y][x] = facenormal[y*div_x+x-1];
	
	y = div_y; x = 0;
	nor[y][x] = facenormal[(y-1)*div_x+x];
	for( x=1; x<div_x; x++)
	{
		nor[y][x]  = facenormal[(y-1)*div_x+x];
		nor[y][x] += facenormal[(y-1)*div_x+x-1];
		nor[y][x].Normalize();
	}
	x = div_x;
	nor[y][x] = facenormal[(y-1)*div_x+x-1];


	// compute face tangent
	vec* faceTangent = facenormal;
	
	for( y=0; y<div_y; y++)
		for( x=0; x<div_x; x++)
		{
			tangent( ver[y][x],
					 ver[y+1][x],
					 ver[y][x+1],
					 tex[y][x],
					 tex[y+1][x],
					 tex[y][x+1],
					 faceTangent[y*div_x+x] );
		//	faceTangent[y*div_x+x].y = - faceTangent[y*div_x+x].y;

		}
	// compute tangent
	for( y=1; y<div_y; y++)
	{
		x = 0;
		tan[y][x]  = faceTangent[y*div_x+x];
		tan[y][x] += faceTangent[(y-1)*div_x+x];
		tan[y][x].Normalize();
		for( x=1; x<div_x; x++)
		{
			tan[y][x]  = faceTangent[y*div_x+x];
			tan[y][x] += faceTangent[y*div_x+x-1];
			tan[y][x] += faceTangent[(y-1)*div_x+x];
			tan[y][x] += faceTangent[(y-1)*div_x+x-1];
			tan[y][x].Normalize();
		}
		x = div_x;
		tan[y][x]  = faceTangent[y*div_x+x-1];
		tan[y][x] += faceTangent[(y-1)*div_x+x-1];
		tan[y][x].Normalize();
	}

	y = 0; x = 0;
	tan[y][x] = faceTangent[y*div_x+x];
	y = 0;
	for( x=1; x<div_x; x++)
	{
		tan[y][x]  = faceTangent[y*div_x+x];
		tan[y][x] += faceTangent[y*div_x+x-1];
		tan[y][x].Normalize();
	}
	x = div_x;
	tan[y][x] = faceTangent[y*div_x+x-1];
	
	y = div_y; x = 0;
	tan[y][x] = faceTangent[(y-1)*div_x+x];
	for( x=1; x<div_x; x++)
	{
		tan[y][x]  = faceTangent[(y-1)*div_x+x];
		tan[y][x] += faceTangent[(y-1)*div_x+x-1];
		tan[y][x].Normalize();
	}
	x = div_x;
	tan[y][x] = faceTangent[(y-1)*div_x+x-1];

	// compute binormal
	for( y=0; y<div_y+1; y++)
		for( x=0; x<div_x+1; x++)
			bin[y][x] = Normalize( CROSS( nor[y][x], tan[y][x] ));

	delete [] facenormal;

	count = (2*div_x+4)*div_y-4;
	count_vertex = count+2;
	
	if( FAILED( pd->CreateVertexBuffer( count_vertex*sizeof(zPlane_VNTTB_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								D3DFVF_CUSTOMVERTEX_PLANE_VNTTB,D3DPOOL_DEFAULT, &pvb ) ) )return 0;
	zPlane_VNTTB_CUSTOMVERTEX* p;
	if( FAILED( pvb->Lock( 0, count_vertex*sizeof(zPlane_VNTTB_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )return 0;
	int i_vec=0;
	
	for(int yi=0; yi<div_y; yi++)
	{
		for(int xi=0; xi<div_x+1; xi++)
		{
			p[i_vec].v = ver[yi][xi];
			p[i_vec].t0 = tex[yi][xi];
			p[i_vec].s = tan[yi][xi];
			p[i_vec].t = bin[yi][xi];
			p[i_vec].n = nor[yi][xi];
			i_vec++;
			if(xi==0 && yi!=0 )
			{
				p[i_vec].v = ver[yi][xi];
				p[i_vec].t0 = tex[yi][xi];
				p[i_vec].s = tan[yi][xi];
				p[i_vec].t = bin[yi][xi];
				p[i_vec].n = nor[yi][xi];
				i_vec++;
			}	
			
			p[i_vec].v = ver[yi+1][xi];
			p[i_vec].t0 = tex[yi+1][xi];
			p[i_vec].s = tan[yi+1][xi];
			p[i_vec].t = bin[yi+1][xi];
			p[i_vec].n = nor[yi+1][xi];
			i_vec++;
			if(xi==div_x && yi!=div_y-1)
			{
				p[i_vec].v = ver[yi+1][xi];
				p[i_vec].t0 = tex[yi+1][xi];
				p[i_vec].s = tan[yi+1][xi];
				p[i_vec].t = bin[yi+1][xi];
				p[i_vec].n = nor[yi+1][xi];
				i_vec++;
			}
		}
	}
	pvb->Unlock();
	
	for( y=0; y<div_y+1; y++)
	{
		delete [] ver[y];
		delete [] nor[y];
		delete [] tan[y];
		delete [] bin[y];
		delete [] tex[y];
	}
	delete [] ver;
	delete [] nor;
	delete [] tan;
	delete [] bin;
	delete [] tex;
	return 1;
}
