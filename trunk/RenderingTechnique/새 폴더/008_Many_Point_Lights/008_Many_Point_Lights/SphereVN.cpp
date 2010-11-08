// SphereVN.cpp: implementation of the SphereVN class.
//
//////////////////////////////////////////////////////////////////////

#include "SphereVN.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SphereVN::SphereVN()
{
	pvb = NULL;
}

SphereVN::~SphereVN()
{
	if(pvb!=NULL) pvb->Release();
}

int SphereVN::Init(int del_uhol_x, int del_uhol_y, float size)
{
	if(pvb!=NULL) pvb->Release();

	del_x = del_uhol_x;
	del_y = del_uhol_y;
	if( FAILED( pd->CreateVertexBuffer( del_x*2*(del_y+1)*sizeof(zSphereVN_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								zSphereVN_D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &pvb ) ) )return 0;
	zSphereVN_CUSTOMVERTEX* p;
	if( FAILED( pvb->Lock( 0, 0, (BYTE**)&p, 0 ) ) )return 0;
	ComputeSphere( del_x, del_y, p, size);
	pvb->Unlock();
	return 1;
}

void SphereVN::Render( DWORD hVertexShader)
{
	pd->SetStreamSource( 0, pvb, sizeof(zSphereVN_CUSTOMVERTEX) );
	pd->SetVertexShader( hVertexShader );
	pd->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, del_x*2*(del_y+1)-2 );
}

void SphereVN::ComputeSphere(int del_uhol_x, int del_uhol_y, zSphereVN_CUSTOMVERTEX *v, float size)
{
	// del_uhol_x krat 2*(del_uhol_y+1) bodov

	vec a,b,ay,by;
	float dy=360.f/del_uhol_y;
	float dx=180.f*PI180/del_uhol_x;
	for(int x=0; x<del_uhol_x; x++)
	{
		float tx = (float)x/(float)del_uhol_x;
		float ang = (tx*180.f-90.f)*PI180;		// <-90, 90)
		a.set( 0, (float)sin(ang), (float)cos(ang));
		b.set( 0, (float)sin(ang+dx), (float)cos(ang+dx));
		
	//	v[x*2*(del_uhol_y+1)+0].t0.set( 0, 1.f-tx);
		v[x*2*(del_uhol_y+1)+0].n = a;
		v[x*2*(del_uhol_y+1)+0].v = size*a;

	//	v[x*2*(del_uhol_y+1)+1].t0.set( 0, 1.f-(tx+1.f/del_uhol_x));
		v[x*2*(del_uhol_y+1)+1].n = b;
		v[x*2*(del_uhol_y+1)+1].v = size*b;

		ay = a;	by = b;
		for(int y=1; y<del_uhol_y+1; y++)
		{
			float ty = (float)y/(float)del_uhol_y;
			ay = a;	ay.RotY( ty*360.f);
			by = b;	by.RotY( ty*360.f);

		//	v[x*2*(del_uhol_y+1)+2*y].t0.set( ty, 1.f-tx);
			v[x*2*(del_uhol_y+1)+2*y].n = ay;
			v[x*2*(del_uhol_y+1)+2*y].v = size*ay;

		//	v[x*2*(del_uhol_y+1)+2*y+1].t0.set( ty, 1.f-(tx+1.f/del_uhol_x));
			v[x*2*(del_uhol_y+1)+2*y+1].n = by;
			v[x*2*(del_uhol_y+1)+2*y+1].v = size*by;
		}
	}
}