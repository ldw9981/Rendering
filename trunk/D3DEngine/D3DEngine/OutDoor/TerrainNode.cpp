#include "StdAfx.h"
#include "TerrainNode.h"
#include "../dib/dib.h"
#include "../Framework/D3DFramework.h"
#include "../Log/DebugInfoScene.h"

#include "../Resource/ResourceMng.h"
#include "../Resource/RscIndexBuffer.h"
#include "../Resource/RscTexture.h"
#include "../Geometrics/CollisionDetector.h"

/// ������
cTerrainNode::cTerrainNode()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	
//	for( int i = 0 ; i < MAX_TERRAIN_TEX ; i++ ) m_pTex[i] = NULL;
	m_pTex = NULL;
	m_nTriangles	= 0;
	m_pQuadTree		= NULL;
}

/// �Ҹ���
cTerrainNode::~cTerrainNode()
{
	Destroy();
}

/**
 * @brief ������ü�� �ʱ�ȭ�Ѵ�.
 * @param pDev : ��¿� D3D����̽� 
 * @param pvfScale : ���̸��� x,y,z���� ���� ô����(scale vector)
 * @param lpBMPFilename : ���̸ʿ� BMP���ϸ�
 * @param lpTexFilename : ��¿� D3D����̽� 
 */
HRESULT	cTerrainNode::Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTEXFilename )
{		
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	

	try
	{
		if( FAILED( BuildHeightMap( lpBMPFilename ) ) ) throw _T("���� ���̸� BMP���Ͼ���\n");
		if( FAILED( LoadTextures( lpTEXFilename ) ) ) throw _T("���� �ؽ��� ���Ͼ���\n");
		if( FAILED( CreateVIB() ) ) throw _T("���ؽ� �ε��� ���� ��������\n");
	}
	catch (LPCTSTR msg)
	{
		::MessageBox(NULL,msg,MB_OK,0);
		Destroy(); 
		return E_FAIL;
	}
	m_pQuadTree = new cQuadTreeNode( this,0,m_cxDIB - 1,m_cxDIB * ( m_czDIB - 1 ), m_cxDIB * m_czDIB - 1 );
	AttachChildNode(m_pQuadTree);

	CreateCullingSphere();
	(*GetCullingSphere())=*m_pQuadTree->GetCullingSphere();
	CreateBoundingSphere();
	(*GetBoundingSphere())=*m_pQuadTree->GetCullingSphere();

	return S_OK;
}

/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
HRESULT	cTerrainNode::Destroy()
{
	DettachChildNode(m_pQuadTree);
	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETEARRAY( m_pvHeightMap );
	SAFE_RELEASE( m_pRscVertexBuffer );
	SAFE_RELEASE( m_pRscIndexBuffer );
//	for( int i = 0 ; i < 4 ; i++ ) SAFE_RELEASE( m_pTex[i] );
	SAFE_RELEASE(m_pTex);
	return S_OK;
}

/// ������ü���� ����� �ؽ�ó�� �о���δ�.
HRESULT	cTerrainNode::LoadTextures( LPCTSTR lpTexFilename )
{
// 	for( int i = 0 ; i < 1 ; i++ )
// 		D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename[i], &m_pTex[i] );

//	D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename, &m_pTex );

	m_pRscTexture = g_pD3DFramework->GetResourceMng()->CreateRscTexture(lpTexFilename);
	if (m_pRscTexture==NULL)
	{
		return E_FAIL;
	}
	return S_OK;
}

/// BMP������ ��� ���̸��� �����Ѵ�.
HRESULT	cTerrainNode::BuildHeightMap( LPCTSTR lpFilename )
{	
#ifdef _UNICODE
	char temp[256]={0,};
	memset(temp,0,sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpFilename, -1, temp, 256, NULL, NULL);	
	LPBYTE	pDIB = DibLoadHandle( temp );	
#else
	LPBYTE	pDIB = DibLoadHandle( lpFilename );
#endif
	if( pDIB==NULL ) 
	{
		return E_FAIL;
	}
	
	m_cxDIB = DIB_CX( pDIB );
	m_czDIB = DIB_CY( pDIB );
	
	// ���⼭ m_cxDIB�� m_czDIB�� (2^n+1)�� �ƴѰ�� E_FAIL�� ��ȯ�ϵ��� ������ ��

	m_pvHeightMap = new TERRAINVERTEX[m_cxDIB * m_czDIB];

	TERRAINVERTEX v;
	for( int z = 0 ; z < m_czDIB ; z++ )
	{
		for( int x = 0 ; x < m_cxDIB ; x++ )
		{
			v.p.x = (float)( ( x - m_cxDIB / 2 ) * m_vfScale.x );
			v.p.z = -(float)( ( z - m_czDIB / 2 ) * m_vfScale.z );
			v.p.y = (float)( *( DIB_DATAXY_INV( pDIB, x, z ) ) ) * m_vfScale.y;
			D3DXVec3Normalize( &v.n, &v.p );
			v.u = (float)x / (float)( m_cxDIB - 1 );
			v.v = (float)z / (float)( m_czDIB - 1 );
			m_pvHeightMap[x + z * m_czDIB] = v;
		}
	}

	DibDeleteHandle( pDIB );
	return S_OK;
}



/// ����, �ε��� ���۸� �����Ѵ�.
HRESULT	cTerrainNode::CreateVIB()
{
 	m_pRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
 	if (m_pRscVertexBuffer == NULL)
 	{
		Destroy();
		return E_FAIL;
 	}
	VOID* pVertices;
	pVertices=m_pRscVertexBuffer->Lock();
	if (pVertices == NULL)
	{
		Destroy();
		return E_FAIL;
	}
	memcpy( pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX) );
	m_pRscVertexBuffer->Unlock();

	m_pRscIndexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscIndexBuffer((m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	if (m_pRscIndexBuffer == NULL)
	{
		Destroy();
		return E_FAIL;
	}	
    return S_OK;
}

/// ȭ�鿡 ������ ����Ѵ�.
void	cTerrainNode::Render()
{
	FillIndexBuffer();
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();

//	m_pD3DDevice->SetTexture( 0, m_pTex );								// 0�� �ؽ��� ���������� �ؽ��� ����(�����)
	m_pRscTexture->SetTexture(0);

	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	// 0�� �ؽ�ó ���������� Ȯ�� ����
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	


	m_pD3DDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTM() );	
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );

}

HRESULT cTerrainNode::FillIndexBuffer()
{
	cCameraNode* pCamera=cCameraNode::GetActiveCamera();

	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pRscIndexBuffer->Lock();	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(pCamera, m_nTriangles, pI );
	m_pRscIndexBuffer->Unlock();

	g_pD3DFramework->GetDebugInfoScene()->AddDebugString(_T("���� ���� Tri %d\n"),m_nTriangles);
	
	return S_OK;
}

void cTerrainNode::Update( DWORD elapseTime )
{

}

void cTerrainNode::Init()
{

}

void cTerrainNode::Uninit()
{

}

BOOL cTerrainNode::GetHeight( float x,float z,float& y )
{
	D3DXVECTOR3 pos(x,0,z);
	if(m_pQuadTree->GetCellIntersection(pos))
	{
		y=pos.y;
		return TRUE;
	}
	return FALSE;
}

/*
	SceneGraphNode�� �Լ��� �ٸ����� CheckWorldFrustumWithoutYAxis�� ���ٴ°�
*/
void cTerrainNode::CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera )
{
	IRenderer* pIRenderer=dynamic_cast<IRenderer*>(this);

	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{			
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  �ۿ� �ִ°��̸� ����ȸ ����
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
		}

		// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE�� ť�� �ִ´�.
			{	
				pRendererQueue->Insert(pIRenderer);						
			}
		}
	}	

	list<cSceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}