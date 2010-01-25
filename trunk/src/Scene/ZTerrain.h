#ifndef _ZTERRAIN_H_
#define _ZTERRAIN_H_

#include "SceneGraphNode.h"
#include "../Math/Sphere.h"
#include "../D3D9/SharedDevice.h"
#include "../D3D9/VertexBuffer.h"
#include "../Framework/Interface.h"
#include "../Framework/App/App.h"
#include "ZQuadTree.h"

struct TERRAINVERTEX
{
	enum { FVF=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	D3DXVECTOR3	p;
	D3DXVECTOR3 n;
	float		u,v;
};

#define MAX_TERRAIN_TEX	4
#define _USE_INDEX16

class CameraNode;
class cRscTexture;
class VertexBuffer;
class IndexBuffer;

namespace Application
{
	class App;
}

/*
	오브젝트 노드의 삽입루틴을 만들어야한다. 오브젝트의 컬링스피어에따라 쿼트트리 노드에 추가한다.

*/
class ZTerrain:
	public SceneGraphNode,
	public Framework::IRenderer,
	private SharedLPDIRECT3DDEVICE9
{
private:
	int						m_cxDIB;		/// DIB의 가로픽셀수
	int						m_czDIB;		/// DIB의 세로픽셀수
	int						m_cyDIB;		/// DIB의 최대높이값(즉 0 ~ 255사이의 값)
	D3DXVECTOR3				m_vfScale;		/// x scale, y scale, z scale
	TERRAINVERTEX*			m_pvHeightMap;	/// 높이맵의 정점배열

	LPDIRECT3DTEXTURE9		m_pTex[MAX_TERRAIN_TEX];	/// 텍스처	
	int						m_nTriangles;	/// 출력할 삼각형의 개수
	ZQuadTree*				m_pQuadTree;	/// 쿼드트리 객체의 포인터

	Texture*		m_pTexture;
	VertexBuffer*	m_pVertexBuffer;		//버텍스 버퍼를 갖는다.
	IndexBuffer*	m_pIndexBuffer;
	

public:
	ZTerrain();
	~ZTerrain();
	/**
	 * @brief 지형객체를 초기화한다.
	 * @param pvfScale : 높이맵의 x,y,z값에 곱할 척도값(scale vector)
	 * @param lpBMPFilename : 높이맵용 BMP파일명	
	 */
	HRESULT		Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTexFilename[MAX_TERRAIN_TEX] );

	// ZTerrain내부에서 사용되는 함수들
private:
	/// 지형객체를 메모리에서 소거한다.
	HRESULT		_Destroy();

	/// 지형객체에서 사용할 텍스처를 읽어들인다.
	HRESULT		_LoadTextures( LPCTSTR lpTexFilename[MAX_TERRAIN_TEX] );

	/// BMP파일을 열어서 높이맵을 생성한다.
	HRESULT		_BuildHeightMap( LPCTSTR lpFilename );


	/// 정점, 인덱스 버퍼를 생성한다.
	HRESULT		_CreateVIB();

	/// 화면에 지형을 출력한다.
	virtual void	Render();
public:
	virtual	void	Init();
	virtual void	Uninit();
	virtual	void	Update(DWORD elapseTime);

	/// x, z위치의 정점값을 얻어낸다.
	TERRAINVERTEX*	GetVertex( int x, int z ) { return (m_pvHeightMap+x+z*m_cxDIB); }

	/// x, z위치의 높이(y)값만 얻어내다.
	float		GetHeight( int x, int z ) { return (GetVertex( x, z ))->p.y; }

	BOOL		GetHeight(float x,float z,float& y);

	/// 높이맵 BMP파일의 가로픽셀수
	int			GetCXDIB() { return m_cxDIB; }

	/// 높이맵 BMP파일의 가로픽셀수
	int			GetCZDIB() { return m_czDIB; }

	/// 높이맵의 실제 x축 폭(column)값
	float		GetCXTerrain() { return m_cxDIB * m_vfScale.x; }

	/// 높이맵의 실제 y축 높이(height)값
	float		GetCYTerrain() { return m_cyDIB * m_vfScale.y; }

	/// 높이맵의 실제 z축 길이(row)값
	float		GetCZTerrain() { return m_czDIB * m_vfScale.z; }

	
	HRESULT		FillIndexBuffer();

	TERRAINVERTEX* GetHeightMap() const { return m_pvHeightMap; }

	virtual void		CullRendererTraversal( cRendererQueue* pRendererQueue,CameraNode* pActiveCamera );
};



#endif // _ZTERRAIN_H_