#pragma once
class StaticD3DDEVICE9
{
public:
	StaticD3DDEVICE9(void) {};
	~StaticD3DDEVICE9(void) {};
protected:
	static LPDIRECT3DDEVICE9	m_pD3DDevice;
};