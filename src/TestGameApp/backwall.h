#pragma once

class cBackWall 
{
public:
	cBackWall();
	virtual ~cBackWall(void);

private:
	LPDWORD		m_pArVertex;
	LPWORD		m_pArIndex;
	DWORD		m_dwByteVertex;
	DWORD		m_dwByteIndex;

	LPDIRECT3DVERTEXBUFFER9 m_pVB; 
	LPDIRECT3DINDEXBUFFER9  m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexture;

	D3DXMATRIX m_matWorld;
public:
	virtual void Init();
	virtual void Uninit();

	virtual void Update(DWORD elapseTime);
	virtual void Render();
};
