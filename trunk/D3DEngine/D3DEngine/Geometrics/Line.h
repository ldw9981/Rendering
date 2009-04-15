#pragma once

class cLine
{
public:
	cLine(void);
	cLine(const D3DXVECTOR3& start,const D3DXVECTOR3& end);

	virtual ~cLine(void);

private:
	D3DXVECTOR3 m_Start;

	D3DXVECTOR3	m_Direction;

public:
	void			Make(const D3DXVECTOR3& start,const D3DXVECTOR3& end);
	D3DXVECTOR3&	GetStart()  { return m_Start; }
	void			SetStart(const D3DXVECTOR3& val) { m_Start = val; }

	const D3DXVECTOR3&	GetDirection()  { return m_Direction; }
	void			SetDirection(const D3DXVECTOR3& val) { m_Direction = val; }

	void			GetPosition(const float t,D3DXVECTOR3& posOut);
};


