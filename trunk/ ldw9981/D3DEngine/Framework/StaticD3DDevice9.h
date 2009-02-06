#pragma once
class cStaticD3DDEVICE9
{
public:
	cStaticD3DDEVICE9(void) {};
	~cStaticD3DDEVICE9(void) {};
protected:
	static LPDIRECT3DDEVICE9	m_pD3DDevice;
};