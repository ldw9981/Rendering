#pragma once
#include "Scene/SceneNode.h"

class cGround:
	public Sophia::cSceneNode
{
public:
	cGround();
	virtual ~cGround(void);
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
	virtual void Init(void* arg);
	virtual void Uninit();

	virtual void Update(DWORD elapseTime);
	virtual void Render();
	
};
