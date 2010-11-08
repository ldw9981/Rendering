#pragma once

class cPlane:
	public D3DXPLANE
{
public:
	cPlane(void);

	cPlane(float a,float b,float c,float d);
	cPlane(const D3DXVECTOR3& Normal,float d);
	~cPlane(void);
private:	
public:
	D3DXVECTOR3&	GetNormal();
	const float		GetDistance();
	const float		GetDistance(const D3DXVECTOR3& pos);
	void			Make(const D3DXVECTOR3& Normal,float d );	
	void			Make(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3 );
	void			Normalize();
};

