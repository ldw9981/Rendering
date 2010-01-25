#include "Plane.h"

cPlane::cPlane(void)
{
	a=0.0f;
	b=0.0f;
	c=0.0f;
	d=0.0f;
}

cPlane::cPlane(const D3DXVECTOR3& Normal,float d )
{
	Make(Normal,d);
}

cPlane::cPlane( float a,float b,float c,float d )
{
	Make(D3DXVECTOR3(a,b,c),d);
}

cPlane::cPlane(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3 )
{
	Make(p1,p2,p3);
}

cPlane::~cPlane(void)
{
}

const float cPlane::GetDistance()
{
	return d;
}

const float cPlane::GetDistance(const D3DXVECTOR3& pos )
{
	return (a*pos.x + b*pos.y + c*pos.z + d);
}
void cPlane::Make(const D3DXVECTOR3& Normal,float d )
{
	D3DXPLANE::a=Normal.x;
	D3DXPLANE::b=Normal.y;
	D3DXPLANE::c=Normal.z;
	D3DXPLANE::d=d;
}

//           p1
//          /  |
//		   /    |
//vecNormal<-    |
//       /        |
//      p3--------p2
void cPlane::Make(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3 )
{

	D3DXVECTOR3 vecNormal;
	D3DXVec3Cross(&vecNormal,&D3DXVECTOR3(p2-p1),&D3DXVECTOR3(p3-p1));
	D3DXVec3Normalize(&vecNormal,&vecNormal);
	Make(vecNormal,-D3DXVec3Dot(&vecNormal,&p1));

//	D3DXPlaneFromPoints((D3DXPLANE*)this,&p1,&p2,&p3);
}
D3DXVECTOR3& cPlane::GetNormal()
{
	static D3DXVECTOR3 temp;
	temp.x=a;
	temp.y=b;
	temp.z=c;
	return temp;
}

void cPlane::Normalize()
{
	D3DXPlaneNormalize(this,this);
}

void cPlane::SetNormal( D3DXVECTOR3& normal )
{
	D3DXPLANE::a = normal.x;
	D3DXPLANE::b = normal.y;
	D3DXPLANE::c = normal.z;
}

void cPlane::SetD( float d )
{
	D3DXPLANE::d = d;
}