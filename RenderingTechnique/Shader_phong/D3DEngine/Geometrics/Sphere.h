#pragma once

class cSphere
{
public:
	cSphere(void);
	cSphere(D3DXVECTOR3& center,float radius);
	virtual ~cSphere(void);

private:
	D3DXVECTOR3		m_Center;

	float			m_Radius;
	float			m_RadiusSQ;
	
	
public:
	void			Make(D3DXVECTOR3& center,float radius);
	void			Make(D3DXVECTOR3& min,D3DXVECTOR3& max);

	float			GetRadiusSQ() const { return m_RadiusSQ; }
	D3DXVECTOR3&	GetCenter()  { return m_Center; }
	void			SetCenter(D3DXVECTOR3 val) { m_Center = val; }
	float			GetRadius() const { return m_Radius; }
	void			SetRadius(float val) { m_Radius = val; m_RadiusSQ=m_Radius*m_Radius;}
};

