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
	������Ʈ ����� ���Է�ƾ�� �������Ѵ�. ������Ʈ�� �ø����Ǿ��� 
	������ũ��� ��ġ������ ��ƮƮ�� ��忡 �߰��Ѵ�.
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
	int					m_cxDIB;		/// DIB�� �����ȼ���
	int					m_czDIB;		/// DIB�� �����ȼ���
	int					m_cyDIB;		/// DIB�� �ִ���̰�(�� 0 ~ 255������ ��)
	D3DXVECTOR3			m_vfScale;		/// x scale, y scale, z scale
	TERRAINVERTEX*		m_pvHeightMap;	/// ���̸��� �����迭

	LPDIRECT3DTEXTURE9	m_pTex;	/// �ؽ�ó	
	int					m_nTriangles;	/// ����� �ﰢ���� ����
	cQuadTreeNode*		m_pQuadTree;	/// ����Ʈ�� ��ü�� ������

	cRscTexture*		m_pRscTexture;
	cRscVertexBuffer*	m_pRscVertexBuffer;		//���ؽ� ���۸� ���´�.
	cRscIndexBuffer*	m_pRscIndexBuffer;
private:
	
	HRESULT				Destroy();

	/// ������ü���� ����� �ؽ�ó�� �о���δ�.
	HRESULT				LoadTextures( LPCTSTR lpTexFilename );
	/// BMP������ ��� ���̸��� �����Ѵ�.
	HRESULT				BuildHeightMap( LPCTSTR lpFilename );
	/// ����, �ε��� ���۸� �����Ѵ�.
	HRESULT				CreateVIB();

	
	virtual void		Render();
public:
	HRESULT				Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTexFilename );
	virtual	void		Init();
	virtual void		Uninit();
	virtual	void		Update(DWORD elapseTime);

	BOOL				GetHeight(float x,float z,float& y);

	/// ���̸� BMP������ �����ȼ���
	int					GetCXDIB() { return m_cxDIB; }

	/// ���̸� BMP������ �����ȼ���
	int					GetCZDIB() { return m_czDIB; }

	/// ���̸��� ���� x�� ��(column)��
	float				GetCXTerrain() { return m_cxDIB * m_vfScale.x; }

	/// ���̸��� ���� y�� ����(height)��
	float				GetCYTerrain() { return m_cyDIB * m_vfScale.y; }

	/// ���̸��� ���� z�� ����(row)��
	float				GetCZTerrain() { return m_czDIB * m_vfScale.z; }

	
	HRESULT				FillIndexBuffer();

	TERRAINVERTEX*		GetHeightMap() const { return m_pvHeightMap; }

	virtual void		CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera );
};

#endif // _cTerrainNode_H_