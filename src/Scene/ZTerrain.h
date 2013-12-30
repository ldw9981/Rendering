#ifndef _ZTERRAIN_H_
#define _ZTERRAIN_H_

#include "Scene/SceneNode.h"
#include "Math/Sphere.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "Foundation/Interface.h"
#include "ZQuadTree.h"
#include "Resource/ResourceMng.h"

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
class Frustum;




/*
	������Ʈ ����� ���Է�ƾ�� �������Ѵ�. ������Ʈ�� �ø����Ǿ���� ��ƮƮ�� ��忡 �߰��Ѵ�.

*/
class ZTerrain:
	public cSceneNode,
	public StaticD3DDEVICE9,
	public cStaticResourceMng
{
private:
	int						m_cxDIB;		/// DIB�� �����ȼ���
	int						m_czDIB;		/// DIB�� �����ȼ���
	int						m_cyDIB;		/// DIB�� �ִ���̰�(�� 0 ~ 255������ ��)
	D3DXVECTOR3				m_vfScale;		/// x scale, y scale, z scale
	TERRAINVERTEX*			m_pvHeightMap;	/// ���̸��� �����迭

	LPDIRECT3DTEXTURE9		m_pTex;	/// �ؽ�ó	
	int						m_nTriangles;	/// ����� �ﰢ���� ����
	ZQuadTree*				m_pQuadTree;	/// ����Ʈ�� ��ü�� ������

	cRscTexture*		m_pRscTexture;
	cRscVertexBuffer*	m_pRscVertexBuffer;		//���ؽ� ���۸� ���´�.
	cRscIndexBuffer*	m_pRscIndexBuffer;
	

public:
	ZTerrain();
	~ZTerrain();
	/**
	 * @brief ������ü�� �ʱ�ȭ�Ѵ�.
	 * @param pvfScale : ���̸��� x,y,z���� ���� ô����(scale vector)
	 * @param lpBMPFilename : ���̸ʿ� BMP���ϸ�	
	 */
	HRESULT		Create( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTexFilename );

	// ZTerrain���ο��� ���Ǵ� �Լ���
private:
	/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
	HRESULT		_Destroy();

	/// ������ü���� ����� �ؽ�ó�� �о���δ�.
	HRESULT		_LoadTextures( const char* lpTexFilename );

	/// BMP������ ��� ���̸��� �����Ѵ�.
	HRESULT		_BuildHeightMap( const char* lpFilename );


	/// ����, �ε��� ���۸� �����Ѵ�.
	HRESULT		_CreateVIB();


public:
	/// ȭ�鿡 ������ ����Ѵ�.
	virtual void	CullRendererIntoRendererQueue(cView* pView,Frustum* pFrustum );
	virtual void	Render();
	virtual	void	Update(DWORD elapseTime);
	virtual	void	QueueRenderer(cView* pView,bool bTraversal);
	/// x, z��ġ�� �������� ����.
	TERRAINVERTEX*	GetVertex( int x, int z ) { return (m_pvHeightMap+x+z*m_cxDIB); }

	/// x, z��ġ�� ����(y)���� ����.
	float		GetHeight( int x, int z ) { return (GetVertex( x, z ))->p.y; }

	BOOL		GetHeight(float x,float z,float& y);

	/// ���̸� BMP������ �����ȼ���
	int			GetCXDIB() { return m_cxDIB; }

	/// ���̸� BMP������ �����ȼ���
	int			GetCZDIB() { return m_czDIB; }

	/// ���̸��� ���� x�� ��(column)��
	float		GetCXTerrain() { return m_cxDIB * m_vfScale.x; }

	/// ���̸��� ���� y�� ����(height)��
	float		GetCYTerrain() { return m_cyDIB * m_vfScale.y; }

	/// ���̸��� ���� z�� ����(row)��
	float		GetCZTerrain() { return m_czDIB * m_vfScale.z; }

	
	HRESULT		FillIndexBuffer(Frustum& frustum);

	TERRAINVERTEX* GetHeightMap() const { return m_pvHeightMap; }
};

#endif // _ZTERRAIN_H_