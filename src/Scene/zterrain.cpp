#include "StdAfx.h"
#include "ZTerrain.h"
#include "dib.h"
#include "Graphics/Entity.h"
#include "Resource/ResourceMng.h"
#include "Graphics/RscIndexBuffer.h"
#include "Math/CollisionDetector.h"
#include "Math/Frustum.h"
#include "Foundation/Define.h"
#include "Graphics/Graphics.h"

namespace Sophia
{


/// 생성자
ZTerrain::ZTerrain()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	m_pRscIndexBuffer = NULL;
	m_pRscVertexBuffer = NULL;
	m_pTex = NULL;
	m_nTriangles	= 0;
	m_pQuadTree		= NULL;
}

/// 소멸자
ZTerrain::~ZTerrain()
{
	_Destroy();
}

/**
 * @brief 지형객체를 초기화한다.
 * @param pDev : 출력용 D3D디바이스 
 * @param pvfScale : 높이맵의 x,y,z값에 곱할 척도값(scale vector)
 * @param lpBMPFilename : 높이맵용 BMP파일명
 * @param lpTexFilename : 출력용 D3D디바이스 
 */
HRESULT	ZTerrain::Create( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTEXFilename )
{	
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	
	
	try
	{
		if( FAILED( _BuildHeightMap( lpBMPFilename ) ) ) throw ("지형 높이맵 BMP파일없음\n");
		if( FAILED( _LoadTextures( lpTEXFilename ) ) ) throw ("지형 텍스쳐 파일없음\n");
		if( FAILED( _CreateVIB() ) ) throw ("버텍스 인덱스 버퍼 생성에러\n");
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
	QueueRenderer(this,true);
	return S_OK;
}

/// 지형객체를 메모리에서 소거한다.
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

/// 지형객체에서 사용할 텍스처를 읽어들인다.
HRESULT	ZTerrain::_LoadTextures( const char* lpTexFilename )
{	
	D3DXCreateTextureFromFile( Graphics::m_pDevice, lpTexFilename, &m_pTex );

	return S_OK;
}

/// BMP파일을 열어서 높이맵을 생성한다.
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

	char filename[256];
	_splitpath_s(lpFilename,NULL,0,NULL,0,filename,256,NULL,0);
	m_strNodeName = filename;

	m_cxDIB = DIB_CX( pDIB );
	m_czDIB = DIB_CY( pDIB );
	
	// 여기서 m_cxDIB나 m_czDIB가 (2^n+1)이 아닌경우 E_FAIL을 반환하도록 수정할 것

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



/// 정점, 인덱스 버퍼를 생성한다.
HRESULT	ZTerrain::_CreateVIB()
{
 	m_pRscVertexBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(m_strNodeName.c_str(),m_strNodeName.c_str(),m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
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

	m_pRscIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(m_strNodeName.c_str(),m_strNodeName.c_str(),(m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	m_pRscIndexBuffer->AddRef();

	if (m_pRscIndexBuffer == NULL)
	{
		_Destroy();
		return E_FAIL;
	}	
    return S_OK;
}

/// 화면에 지형을 출력한다.
void	ZTerrain::Render(MultiSub* pMultiSub,Material* pMaterial)
{	
	Graphics::m_pDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();
	//텍스쳐 적용
	Graphics::m_pInstance->GetEffect()->SetTexture("Tex0",m_pTex);	
	Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmWorld,&m_matWorld);
	Graphics::m_pInstance->GetEffect()->CommitChanges();

	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );


	char temp[256];
	_itoa_s(m_nTriangles,temp,sizeof(temp),10);
	Graphics::m_pInstance->RenderDebugString(0,Graphics::m_pInstance->m_viewPortInfo.Height-20,temp);
}

HRESULT ZTerrain::FillIndexBuffer(Frustum& frustum )
{
	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pRscIndexBuffer->Lock();	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(frustum, m_nTriangles, pI,false );
	m_pRscIndexBuffer->Unlock();
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

bool ZTerrain::Cull(  Frustum* pFrustum ,float loose )
{
	if (!m_bShow)
		return false;

	cCollision::STATE retCS=cCollision::CheckWorldFrustum(*pFrustum,m_BoundingSphere,0.0f);
	if( retCS == cCollision::OUTSIDE)
		return false;

	FillIndexBuffer(*pFrustum);
	return true;	
}

void ZTerrain::QueueRenderer( Entity* pEntity,bool bTraversal )
{
	//pEntity->m_renderQueueTerrain.Insert(this,0,0);
}
}