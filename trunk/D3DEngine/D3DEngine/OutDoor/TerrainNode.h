#ifndef _cTerrainNode_H_
#define _cTerrainNode_H_

#include "../SceneGraph/SceneGraphNode.h"
#include "../Geometrics/Sphere.h"
#include "../Framework/StaticD3DDevice9.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Common/Interface.h"
#include "QuadTreeNode.h"

struct TERRAINVERTEX
{
	enum { FVF=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	D3DXVECTOR3	p;
	D3DXVECTOR3 n;
	float		u,v;
};

#define MAX_TERRAIN_TEX	4
#define _USE_INDEX16

class cCameraNode;
class cRscTexture;
class cRscVertexBuffer;
class cRscIndexBuffer;





/*
	오브젝트 노드의 삽입루틴을 만들어야한다. 오브젝트의 컬링스피어의 
	반지름크기와 위치에따라 쿼트트리 노드에 추가한다.
*/
class cTerrainNode:
	public cSceneGraphNode,
	public IRenderer,
	private cStaticD3DDEVICE9
{
public:
	cTerrainNode();
	~cTerrainNode();
private:
	int					m_cxDIB;		/// DIB의 가로픽셀수
	int					m_czDIB;		/// DIB의 세로픽셀수
	int					m_cyDIB;		/// DIB의 최대높이값(즉 0 ~ 255사이의 값)
	D3DXVECTOR3			m_vfScale;		/// x scale, y scale, z scale
	TERRAINVERTEX*		m_pvHeightMap;	/// 높이맵의 정점배열

	LPDIRECT3DTEXTURE9	m_pTex;	/// 텍스처	
	int					m_nTriangles;	/// 출력할 삼각형의 개수
	cQuadTreeNode*		m_pQuadTree;	/// 쿼드트리 객체의 포인터

	cRscTexture*		m_pRscTexture;
	cRscVertexBuffer*	m_pRscVertexBuffer;		//버텍스 버퍼를 갖는다.
	cRscIndexBuffer*	m_pRscIndexBuffer;
private:
	
	HRESULT				Destroy();

	/// 지형객체에서 사용할 텍스처를 읽어들인다.
	HRESULT				LoadTextures( LPCTSTR lpTexFilename );
	/// BMP파일을 열어서 높이맵을 생성한다.
	HRESULT				BuildHeightMap( LPCTSTR lpFilename );
	/// 정점, 인덱스 버퍼를 생성한다.
	HRESULT				CreateVIB();

	
	virtual void		Render();
public:
	HRESULT				Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTexFilename );
	virtual	void		Init();
	virtual void		Uninit();
	virtual	void		Update(DWORD elapseTime);

	BOOL				GetHeight(float x,float z,float& y);

	/// 높이맵 BMP파일의 가로픽셀수
	int					GetCXDIB() { return m_cxDIB; }

	/// 높이맵 BMP파일의 가로픽셀수
	int					GetCZDIB() { return m_czDIB; }

	/// 높이맵의 실제 x축 폭(column)값
	float				GetCXTerrain() { return m_cxDIB * m_vfScale.x; }

	/// 높이맵의 실제 y축 높이(height)값
	float				GetCYTerrain() { return m_cyDIB * m_vfScale.y; }

	/// 높이맵의 실제 z축 길이(row)값
	float				GetCZTerrain() { return m_czDIB * m_vfScale.z; }

	
	HRESULT				FillIndexBuffer();

	TERRAINVERTEX*		GetHeightMap() const { return m_pvHeightMap; }

	virtual void		CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera );
};

#endif // _cTerrainNode_H_