#include "StdAfx.h"
#include "Triangle.h"

namespace Sophia
{

cTriangle::cTriangle( void )
{

}

cTriangle::cTriangle(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3)
{
	Make(p1,p2,p3);	
}
cTriangle::~cTriangle( void )
{

}


//           p1
//          /  |
//		   /    |
//vecNormal<-    |
//       /        |
//      p3--------p2
void cTriangle::Make(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3)
{
	m_p1=p1;
	m_p2=p2;
	m_p3=p3;
	cPlane::Make(p1,p2,p3);
}
}