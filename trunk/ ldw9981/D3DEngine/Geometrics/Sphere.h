#pragma once

class cSphere
{
public:
	cSphere(void);
	cSphere(const D3DXVECTOR3& center,const float radius);
	
	virtual ~cSphere(void);

private:
	D3DXVECTOR3		m_CenterPos;		//중점

	float			m_Radius;		//반지름
	float			m_RadiusSQ;		//반지름제곱
	
	
public:
	void			Make(const D3DXVECTOR3& center,const float radius);
	void			Make(const D3DXVECTOR3& min,const D3DXVECTOR3& max);

	float			GetRadiusSQ() const { return m_RadiusSQ; }
	D3DXVECTOR3&	GetCenterPos()  { return m_CenterPos; }
	void			SetCenterPos(const D3DXVECTOR3& val) { m_CenterPos = val; }
	float			GetRadius() const { return m_Radius; }
	void			SetRadius(float val) { m_Radius = val; m_RadiusSQ=m_Radius*m_Radius;}
	void			Merge(cSphere& other);

	cSphere&		operator=(cSphere& rhs);
};

