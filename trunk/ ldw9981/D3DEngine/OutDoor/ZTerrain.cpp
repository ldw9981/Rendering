#include "StdAfx.h"
#include "ZTerrain.h"
#include "../dib/dib.h"
#include "../Framework/D3DFramework.h"
#include "../Log/DebugInfoScene.h"

#include "../Resource/ResourceMng.h"
#include "../Resource/RscIndexBuffer.h"
#include "../Geometrics/CollisionDetector.h"

/// ������
ZTerrain::ZTerrain()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	
	for( int i = 0 ; i < MAX_TERRAIN_TEX ; i++ ) m_pTex[i] = NULL;
	m_nTriangles	= 0;
	m_pQuadTree		= NULL;
}

/// �Ҹ���
ZTerrain::~ZTerrain()
{
	_Destroy();
}

/**
 * @brief ������ü�� �ʱ�ȭ�Ѵ�.
 * @param pDev : ��¿� D3D����̽� 
 * @param pvfScale : ���̸��� x,y,z���� ���� ô����(scale vector)
 * @param lpBMPFilename : ���̸ʿ� BMP���ϸ�
 * @param lpTexFilename : ��¿� D3D����̽� 
 */
HRESULT	ZTerrain::Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTEXFilename[4] )
{	
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	
	
	try
	{
		if( FAILED( _BuildHeightMap( lpBMPFilename ) ) ) throw _T("���� ���̸� BMP���Ͼ���\n");
		if( FAILED( _LoadTextures( lpTEXFilename ) ) ) throw _T("���� �ؽ��� ���Ͼ���\n");
		if( FAILED( _CreateVIB() ) ) throw _T("���ؽ� �ε��� ���� ��������\n");
	}
	catch (LPCTSTR msg)
	{
		OutputDebugStr(msg);
		_Destroy(); 
		return E_FAIL;
	}
	m_pQuadTree = new ZQuadTree( this,0,m_cxDIB - 1,m_cxDIB * ( m_czDIB - 1 ), m_cxDIB * m_czDIB - 1 );
	AttachChildNode(m_pQuadTree);

	CreateCullingSphere();
	(*GetCullingSphere())=*m_pQuadTree->GetCullingSphere();
	CreateBoundingSphere();
	(*GetBoundingSphere())=*m_pQuadTree->GetCullingSphere();

	return S_OK;
}

/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
HRESULT	ZTerrain::_Destroy()
{
	DettachChildNode(m_pQuadTree);
	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETEARRAY( m_pvHeightMap );
	SAFE_RELEASE( m_pRscVertexBuffer );
	SAFE_RELEASE( m_pRscIndexBuffer );
	for( int i = 0 ; i < 4 ; i++ ) SAFE_RELEASE( m_pTex[i] );

	return S_OK;
}

/// ������ü���� ����� �ؽ�ó�� �о���δ�.
HRESULT	ZTerrain::_LoadTextures( LPCTSTR lpTexFilename[4] )
{
	for( int i = 0 ; i < 1 ; i++ )
		D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename[i], &m_pTex[i] );

	return S_OK;
}

/// BMP������ ��� ���̸��� �����Ѵ�.
HRESULT	ZTerrain::_BuildHeightMap( LPCTSTR lpFilename )
{
#ifdef _UNICODE
	char temp[256]={0,};
	WideCharToMultiByte(CP_ACP, 0, lpFilename, -1, temp, 256, NULL, NULL);
	LPBYTE	pDIB = DibLoadHandle( temp );

#else
	LPBYTE	pDIB = DibLoadHandle( lpFilename );
#endif
	
	if( !pDIB ) return E_FAIL;

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
HRESULT	ZTerrain::_CreateVIB()
{
 	m_pRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
 	if (m_pRscVertexBuffer == NULL)
 	{
		_Destroy();
		return E_FAIL;
 	}
	VOID* pVertices;
	pVertices=m_pRscVertexBuffer->Lock();
	if (pVertices == NULL)
	{
		_Destroy();
		return E_FAIL;
	}
	memcpy( pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX) );
	m_pRscVertexBuffer->Unlock();

	m_pRscIndexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscIndexBuffer((m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	if (m_pRscIndexBuffer == NULL)
	{
		_Destroy();
		return E_FAIL;
	}	
    return S_OK;
}

/// ȭ�鿡 ������ ����Ѵ�.
void	ZTerrain::Render()
{
	FillIndexBuffer();
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();

	m_pD3DDevice->SetTexture( 0, m_pTex[0] );								// 0�� �ؽ��� ���������� �ؽ��� ����(�����)
	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	// 0�� �ؽ�ó ���������� Ȯ�� ����
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	


	m_pD3DDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTM() );	
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );

}

HRESULT ZTerrain::FillIndexBuffer()
{
	cCameraNode* pCamera=cCameraNode::GetActiveCamera();

	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pRscIndexBuffer->Lock();	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(pCamera, m_nTriangles, pI );
	m_pRscIndexBuffer->Unlock();

	g_pD3DFramework->GetDebugInfoScene()->AddDebugString(_T("tri %d\n"),m_nTriangles);
	
	return S_OK;
}

void ZTerrain::Update( DWORD elapseTime )
{

}

void ZTerrain::Init()
{

}

void ZTerrain::Uninit()
{

}

BOOL ZTerrain::GetHeight( float x,float z,float& y )
{
	D3DXVECTOR3 pos(x,0,z);
	if(m_pQuadTree->GetCellIntersection(pos))
	{
		y=pos.y;
		return TRUE;
	}
	return FALSE;
}

void ZTerrain::CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera )
{
	cIRenderer* pIRenderer=dynamic_cast<cIRenderer*>(this);

	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{			
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  �ۿ� �ִ°��̸� ����ȸ ����
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere OUTSIDE\n";
				g_pD3DFramework->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INSIDE\n";
				g_pD3DFramework->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INTERSECTION\n";
				g_pD3DFramework->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}

		// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE�� ť�� �ִ´�.
			{	
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere INSIDE or INTERSECTION\n";
				g_pD3DFramework->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				pRendererQueue->Insert(pIRenderer);						
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere OUTSIDE\n";
				g_pD3DFramework->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}
	}	

	list<cSceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}