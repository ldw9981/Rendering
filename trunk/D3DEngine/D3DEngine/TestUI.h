#pragma once
#include "./SceneGraph/SceneGraphNode.h"
#include "./Graphics/D3DGraphics.h"

class cTestUI:
	public cSceneGraphNode,
	public cIRenderable,
	private cStaticD3DDEVICE9
{
public:
	cTestUI(void);
	virtual ~cTestUI(void);

private:
	LPDWORD		m_pArVertex;
	LPWORD		m_pArIndex;
	DWORD		m_dwByteVertex;
	DWORD		m_dwByteIndex;

	LPDIRECT3DVERTEXBUFFER9 m_pVB; 
	LPDIRECT3DINDEXBUFFER9  m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexture;


public:
	virtual void Init();
	virtual void Uninit();

	virtual void Update(DWORD elapseTime);
	virtual void Render();
	virtual void ProcessRender();
};
