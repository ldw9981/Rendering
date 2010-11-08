#pragma once

class cPlane:
	public D3DXPLANE
{
public:
	cPlane(void);
	cPlane(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3 );
	cPlane(float a,float b,float c,float d);
	cPlane(const D3DXVECTOR3& Normal,float d);
	~cPlane(void);
private:	
public:
	void			SetNormal(D3DXVECTOR3& normal);
	void			SetD(float d);
	D3DXVECTOR3&	GetNormal();
	const float		GetDistance();
	const float		GetDistance(const D3DXVECTOR3& pos);
	void			Make(const D3DXVECTOR3& Normal,float d );	
	void			Make(const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3 );
	void			Normalize();
};

