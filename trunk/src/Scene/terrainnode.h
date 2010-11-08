#pragma once
#include "SceneNode.h"
#include "Math/Sphere.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "Foundation/Interface.h"
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
	public cSceneNode,	
	private StaticD3DDEVICE9
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
	BOOL				Create(const char* strHeightBitmapFile,const char* strTextureFile);
	
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