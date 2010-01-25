#include "Line.h"

cLine::cLine(void)
{
	m_Start=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Direction=D3DXVECTOR3(0.0f,0.0f,0.0f);
}

cLine::cLine( D3DXVECTOR3& start,D3DXVECTOR3& end )
{
	Make(start,end);
}

cLine::~cLine(void)
{
}

void cLine::Make( D3DXVECTOR3& start,D3DXVECTOR3& end )
{
	m_Start=start;
	D3DXVECTOR3 temp;
	temp = end - start;
	D3DXVec3Normalize(&m_Direction,&temp);	
}

void cLine::GetPosition( const float t,D3DXVECTOR3& posOut ) 
{
	posOut = t * GetDirection();
	posOut += m_Start;
}