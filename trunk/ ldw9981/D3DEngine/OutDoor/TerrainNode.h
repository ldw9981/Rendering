#pragma once
#include "../SceneGraph/SceneGraphNode.h"
#include "../Geometrics/Sphere.h"
#include "../Framework/StaticD3DDevice9.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Common/Interface.h"
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
	public cSceneGraphNode,
	public cIRenderer,
	private cStaticD3DDEVICE9
{
public:
	cTerrainNode();
	virtual ~cTerrainNode();
protected:
private:
	cQuadSpatial*		m_pQuadSpatial;			//������ ������ ���´�.
	
	TERRAINVERTEX*		m_pTerrainVertex;

	cRscTexture*		m_pRscTexture;
	cRscVertexBuffer*	m_pRscVertexBuffer;		//���ؽ� ���۸� ���´�.
	cRscIndexBuffer*	m_pRscIndexBuffer;
	
	
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

	cRscVertexBuffer*	GetRscVertexBuffer() const { return m_pRscVertexBuffer; }
	TERRAINVERTEX*		GetTerrainVertex() const { return m_pTerrainVertex; }

	BOOL				IsValidHeightMap(UINT cx,UINT cz);	
	void				FillIndexBuffer();
};