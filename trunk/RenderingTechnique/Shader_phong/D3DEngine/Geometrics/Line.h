#pragma once

class cLine
{
public:
	cLine(void);
	cLine(D3DXVECTOR3& start,D3DXVECTOR3& end);

	virtual ~cLine(void);

private:
	D3DXVECTOR3 m_Start;

	D3DXVECTOR3	m_Direction;

public:
	void			Make(D3DXVECTOR3& start,D3DXVECTOR3& end);
	D3DXVECTOR3&	GetStart()  { return m_Start; }
	void			SetStart(D3DXVECTOR3 val) { m_Start = val; }

	D3DXVECTOR3&	GetDirection()  { return m_Direction; }
	void			SetDirection(D3DXVECTOR3 val) { m_Direction = val; }

	void			GetPosition(const float t,D3DXVECTOR3& posOut);
};


