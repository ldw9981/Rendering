#include "StdAfx.h"
#include "ZTerrain.h"
#include "dib.h"
#include "Framework/D3DFramework.h"
#include "DebugInfoView.h"

#include "Resource/ResourceMng.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "Math/CollisionDetector.h"
#include "Foundation/Define.h"
#include "D3D9Server/Server.h"


/// ������
ZTerrain::ZTerrain()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	
	m_pTex = NULL;
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
HRESULT	ZTerrain::Create( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTEXFilename )
{	
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	
	
	try
	{
		if( FAILED( _BuildHeightMap( lpBMPFilename ) ) ) throw ("���� ���̸� BMP���Ͼ���\n");
		if( FAILED( _LoadTextures( lpTEXFilename ) ) ) throw ("���� �ؽ��� ���Ͼ���\n");
		if( FAILED( _CreateVIB() ) ) throw ("���ؽ� �ε��� ���� ��������\n");
	}
	catch (const char* msg)
	{
		OutputDebugString(msg);
		_Destroy(); 
		return E_FAIL;
	}
	m_pQuadTree = new ZQuadTree( this,0,m_cxDIB - 1,m_cxDIB * ( m_czDIB - 1 ), m_cxDIB * m_czDIB - 1 );
	AttachChildNode(m_pQuadTree);

	m_BoundingSphere = m_pQuadTree->GetBoundingSphere();

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
	SAFE_RELEASE( m_pTex );

	return S_OK;
}

/// ������ü���� ����� �ؽ�ó�� �о���δ�.
HRESULT	ZTerrain::_LoadTextures( const char* lpTexFilename )
{	
	D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename, &m_pTex );

	return S_OK;
}

/// BMP������ ��� ���̸��� �����Ѵ�.
HRESULT	ZTerrain::_BuildHeightMap( const char* lpFilename )
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
 	m_pRscVertexBuffer = m_ResourceMng.CreateRscVertexBuffer(m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
	m_pRscVertexBuffer->AddRef();

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

	m_pRscIndexBuffer = m_ResourceMng.CreateRscIndexBuffer((m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	m_pRscIndexBuffer->AddRef();

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
	m_pD3DDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();
	//�ؽ��� ����
	D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",m_pTex);	
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&m_matWorld);
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();

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



	char temp[256];
	itoa(m_nTriangles,temp,10);
	D3D9::Server::g_pServer->RenderDebugString(0,20,temp);


	return S_OK;
}

void ZTerrain::Update( DWORD elapseTime )
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

void ZTerrain::CullRendererIntoRendererQueue( cView* pView,cCameraNode* pActiveCamera )
{
	FillIndexBuffer();
	QueueRenderer(pView,false);
}

void ZTerrain::QueueRenderer( cView* pView,bool bTraversal )
{
	pView->m_listRenderTerrain.Insert(this);
}
