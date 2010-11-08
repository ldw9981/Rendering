#pragma once
#include "GameObject.h"
#include "./Renderer/D3DGraphics.h"

class cBackWall :
	public cGameObject,
	public cStaticD3DDEVICE9
{
public:
	cBackWall(cWorld* pWorld);
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

	virtual void Update();
	virtual void Render();
};
