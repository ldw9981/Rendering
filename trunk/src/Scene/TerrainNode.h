#pragma once
#include "../Scene/SceneGraphNode.h"
#include "../MAth/Sphere.h"
#include "../D3D9/SharedDevice.h"
#include "../D3D9/VertexBuffer.h"
#include "../Framework/Interface.h"
/*

	-  Render수행시 카메라 컬링에의한 실시간 인덱스 생성 후 렌더한다.
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
	cQuadSpatial*		m_pQuadSpatial;			//지형은 공간을 갖는다.
	
	TERRAINVERTEX*		m_pTerrainVertex;

	Texture*		m_pTexture;
	VertexBuffer*	m_pVertexBuffer;		//버텍스 버퍼를 갖는다.
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