#pragma once
#include "Plane.h"
namespace Sophia
{

class cTriangle
	:public cPlane
{
public:
	cTriangle(void);
	cTriangle(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3);
	~cTriangle(void);

private:
	D3DXVECTOR3 m_p1;	
	D3DXVECTOR3 m_p2;
	D3DXVECTOR3	m_p3;
public:
	void		Make(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3);

	const D3DXVECTOR3	GetP1() const { return m_p1; }
	void				SetP1(const D3DXVECTOR3& val) { m_p1 = val; }
	const D3DXVECTOR3	GetP2() const { return m_p2; }
	void				SetP2(const D3DXVECTOR3& val) { m_p2 = val; }
	const D3DXVECTOR3	GetP3() const { return m_p3; }
	void				SetP3(const D3DXVECTOR3& val) { m_p3 = val; }
};
}