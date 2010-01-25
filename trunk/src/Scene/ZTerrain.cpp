
#include "ZTerrain.h"
#include "dib.h"
#include "../Framework/App/App.h"
#include "../Scene/DebugInfoScene.h"

#include "../Foundation/ResourceMng.h"
#include "../D3D9/IndexBuffer.h"
#include "../Math/CollisionDetector.h"
#include <tchar.h>
#include "../Foundation/Define.h"
/// 생성자


using namespace Application;

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
HRESULT	ZTerrain::Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTEXFilename[4] )
{	
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	
	
	try
	{
		if( FAILED( _BuildHeightMap( lpBMPFilename ) ) ) throw _T("지형 높이맵 BMP파일없음\n");
		if( FAILED( _LoadTextures( lpTEXFilename ) ) ) throw _T("지형 텍스쳐 파일없음\n");
		if( FAILED( _CreateVIB() ) ) throw _T("버텍스 인덱스 버퍼 생성에러\n");
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

/// 지형객체를 메모리에서 소거한다.
HRESULT	ZTerrain::_Destroy()
{
	DettachChildNode(m_pQuadTree);
	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETEARRAY( m_pvHeightMap );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pIndexBuffer );
	for( int i = 0 ; i < 4 ; i++ ) SAFE_RELEASE( m_pTex[i] );

	return S_OK;
}

/// 지형객체에서 사용할 텍스처를 읽어들인다.
HRESULT	ZTerrain::_LoadTextures( LPCTSTR lpTexFilename[4] )
{
	for( int i = 0 ; i < 1 ; i++ )
		D3DXCreateTextureFromFile( m_SharedLPDIRECT3DDEVICE9, lpTexFilename[i], &m_pTex[i] );

	return S_OK;
}

/// BMP파일을 열어서 높이맵을 생성한다.
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
	
	m_pVertexBuffer = App::Instance()->GetResourceMng()->CreateVertexBuffer(m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
 	if (m_pVertexBuffer == NULL)
 	{
		_Destroy();
		return E_FAIL;
 	}
	VOID* pVertices;
	pVertices=m_pVertexBuffer->Lock();
	if (pVertices == NULL)
	{
		_Destroy();
		return E_FAIL;
	}
	memcpy( pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX) );
	m_pVertexBuffer->Unlock();

	m_pIndexBuffer = App::Instance()->GetResourceMng()->CreateIndexBuffer((m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	if (m_pIndexBuffer == NULL)
	{
		_Destroy();
		return E_FAIL;
	}	
	
    return S_OK;
}

/// 화면에 지형을 출력한다.
void	ZTerrain::Render()
{
	FillIndexBuffer();
	m_pVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pIndexBuffer->SetIndices();

	m_SharedLPDIRECT3DDEVICE9->SetTexture( 0, m_pTex[0] );								// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
	m_SharedLPDIRECT3DDEVICE9->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	// 0번 텍스처 스테이지의 확대 필터
 	m_SharedLPDIRECT3DDEVICE9->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0번 텍스처 : 0번 텍스처 인덱스 사용
 	m_SharedLPDIRECT3DDEVICE9->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	m_SharedLPDIRECT3DDEVICE9->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 	m_SharedLPDIRECT3DDEVICE9->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	


	m_SharedLPDIRECT3DDEVICE9->SetFVF( TERRAINVERTEX::FVF );
	m_SharedLPDIRECT3DDEVICE9->SetTransform(D3DTS_WORLD, &GetWorldTM() );	
	m_SharedLPDIRECT3DDEVICE9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );

}

HRESULT ZTerrain::FillIndexBuffer()
{
	CameraNode* pCamera=CameraNode::GetActiveCamera();

	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pIndexBuffer->Lock();	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(pCamera, m_nTriangles, pI );
	m_pIndexBuffer->Unlock();

	App::Instance()->GetDebugInfoScene()->AddDebugString(_T("tri %d\n"),m_nTriangles);
	
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

void ZTerrain::CullRendererTraversal( cRendererQueue* pRendererQueue,CameraNode* pActiveCamera )
{
	IRenderer* pIRenderer=dynamic_cast<IRenderer*>(this);

	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{			
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  밖에 있는것이면 노드순회 없음
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere OUTSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// 완전히 내부면 자식은 모두 큐에 넣고 순회없음
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INTERSECTION\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}

		// cCollision::INTERSECT 겹치면 자신의 바운딩 스피어랑 검사. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE는 큐에 넣는다.
			{	
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere INSIDE or INTERSECTION\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				pRendererQueue->Insert(pIRenderer);						
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere OUTSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}
	}	

	list<SceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}