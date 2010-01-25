#pragma once
#include "../Scene/SceneGraphNode.h"
#include "../MAth/Sphere.h"
#include "../D3D9/SharedDevice.h"
#include "../D3D9/VertexBuffer.h"
#include "../Framework/Interface.h"
/*

	-  Render����� ī�޶� �ø������� �ǽð� �ε��� ���� �� �����Ѵ�.
*/





struct TERRAININDEX
{
	WORD _0,_1,_2;
};

struct TERRAINVERTEX;
class cQuadSpatial;
class cRscTexture;
class cTerrainNode:
	public SceneGraphNode,
	public Framework::IRenderer,
	public SharedLPDIRECT3DDEVICE9,
	public SharedResourceMng
{
public:
	cTerrainNode();
	virtual ~cTerrainNode();
protected:
private:
	cQuadSpatial*		m_pQuadSpatial;			//������ ������ ���´�.
	
	TERRAINVERTEX*		m_pTerrainVertex;

	Texture*		m_pTexture;
	VertexBuffer*	m_pVertexBuffer;		//���ؽ� ���۸� ���´�.
	IndexBuffer*	m_pIndexBuffer;
	
	
	D3DXVECTOR3			m_VertexScale;
	int					m_cxHeightMap;	
	int					m_czHeightMap;	
	UINT				m_TotalVertex;
	UINT				m_nTriangles;
public:
	virtual void		Init();
	virtual void		Uninit();
	BOOL				Create(LPCWSTR strHeightBitmapFile,LPCWSTR strTextureFile);
	
	virtual void		Update(DWORD elapseTime);
	virtual void		Render();

	D3DXVECTOR3&		GetVertexScale() { return m_VertexScale; }
	void				SetVertexScale(D3DXVECTOR3& val) { m_VertexScale = val; }

	UINT 				GetCxHeightMap() const { return m_cxHeightMap; }
	UINT 				GetCzHeightMap() const { return m_czHeightMap; }

	VertexBuffer*		GetVertexBuffer() const { return m_pVertexBuffer; }
	TERRAINVERTEX*		GetTerrainVertex() const { return m_pTerrainVertex; }

	BOOL				IsValidHeightMap(UINT cx,UINT cz);	
	void				FillIndexBuffer();
};