#include "TerrainNode.h"
#include "../Framework/App/App.h"
#include "../D3D9/Texture.h"
#include "../D3D9/IndexBuffer.h"
#include "QuadSpatial.h"
#include "../Scene/dib.h"
#include "../Scene/CameraNode.h"
#include "../Foundation/Define.h"
#include "ZTerrain.h"
#include <assert.h>

cTerrainNode::cTerrainNode()
{
	SetNodeName(L"Terrain");
	m_pTerrainVertex=NULL;
	m_pVertexBuffer=NULL;
	m_VertexScale = D3DXVECTOR3(1.0f,0.1f,1.f);	
	m_nTriangles=0;
}

cTerrainNode::~cTerrainNode()
{
	SAFE_DELETEARRAY(m_pTerrainVertex);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pIndexBuffer);
}



BOOL cTerrainNode::Create(LPCWSTR strHeightBitmapFile,LPCWSTR strTextureFile)
{
	char temp[256]={0,};
	WideCharToMultiByte(CP_ACP, 0, strHeightBitmapFile, -1, temp, 256, NULL, NULL);

	// strHeightBitmapFile 로드
	LPBYTE	pDIB = DibLoadHandle( temp );
	if( !pDIB ) return FALSE;

	m_cxHeightMap = DIB_CX( pDIB );
	m_czHeightMap = DIB_CY( pDIB );

	// 사이즈 검사
	if (!IsValidHeightMap(m_cxHeightMap,m_czHeightMap))
	{	
		assert(0 || L"2^n+1의 크기가 아님");
		return FALSE;
	}

	m_TotalVertex = m_cxHeightMap * m_czHeightMap;
	m_pTerrainVertex = new TERRAINVERTEX[m_cxHeightMap * m_czHeightMap];
	TERRAINVERTEX v;
	for( int z = 0 ; z < m_czHeightMap ; z++ )
	{
		for( int x = 0 ; x < m_cxHeightMap ; x++ )
		{
			v.p.x = (float)( ( x - m_cxHeightMap / 2 ) * m_VertexScale.x );
			v.p.z = -(float)( ( z - m_czHeightMap / 2 ) * m_VertexScale.z );
			v.p.y = (float)( *( DIB_DATAXY_INV( pDIB, x, z ) ) ) * m_VertexScale.y;
			D3DXVec3Normalize( &v.n, &v.p );
			v.u = (float)x / (float)( m_cxHeightMap - 1 );
			v.v = (float)z / (float)( m_czHeightMap - 1 );
			m_pTerrainVertex[x + z * m_czHeightMap] = v;
		}
	}

	DibDeleteHandle( pDIB );

	
	// 텍스쳐 로드
	m_pTexture = m_SharedResourceMng.CreateTexture(strTextureFile);
	if (m_pTexture==NULL)
		return FALSE;

	// 버텍스버퍼 생성
	UINT BufferSize=m_cxHeightMap*m_czHeightMap*sizeof(TERRAINVERTEX);
	m_pVertexBuffer = m_SharedResourceMng.CreateVertexBuffer(BufferSize);

	// 버텍스 복사
	TERRAINVERTEX* pVB= (TERRAINVERTEX*)m_pVertexBuffer->Lock();
	memcpy(pVB,&m_pTerrainVertex[0],sizeof(BufferSize));
	m_pVertexBuffer->Unlock();	

	// 인덱스 버퍼 생성		셀은 삼각형2개

	BufferSize=(m_cxHeightMap-1)*(m_czHeightMap-1)*2* sizeof(TERRAININDEX);
	m_pIndexBuffer = m_SharedResourceMng.CreateIndexBuffer(BufferSize);


	TERRAININDEX tempI;
	TERRAININDEX *pIB=NULL;


	pIB=(TERRAININDEX*)m_pIndexBuffer->Lock();
	

	for ( int z=0 ; z<m_czHeightMap-1 ; z++)
	{
		for (int x=0 ;  x<m_cxHeightMap-1 ; x++)
		{
			// ++++++++++
			// + 0   1
			// +
			// + 2
			tempI._0 = (WORD)(z*m_cxHeightMap+x);				
			tempI._1 = (WORD)(z*m_cxHeightMap+x+1);				
			tempI._2 = (WORD)((z+1)*m_cxHeightMap+x);			
			*pIB = tempI;
			pIB++;

		

			// ++++++++
			// +     4
			// +
			// + 3   5 
			tempI._0 = (WORD)((z+1)*m_cxHeightMap+x);			
			tempI._1 = (WORD)(z*m_cxHeightMap+x+1);				
			tempI._2 = (WORD)((z+1)*m_cxHeightMap+x+1);			
			*pIB = tempI;
			pIB++;
			
		}
	}	
	m_pIndexBuffer->Unlock();


	// 공간 생성,분할
	m_pQuadSpatial = new cQuadSpatial(this,
		0, m_cxHeightMap-1, m_cxHeightMap*(m_czHeightMap-1), m_cxHeightMap*m_czHeightMap-1);
	m_pQuadSpatial->DevideSelf();

	CreateBoundingSphere();
	GetBoundingSphere()->Make(D3DXVECTOR3(0.0f,0.0f,0.0f),100000.0f);
	return TRUE;
}

void cTerrainNode::Init()
{

}
void cTerrainNode::Uninit()
{

}
void cTerrainNode::Update( DWORD elapseTime )
{

}

/*
	그릴때는 인덱스버퍼에 프러스텀내부의 인덱스들만 모아서 인덱스버퍼를 만들어 출력한다.

*/
void cTerrainNode::Render()
{	
	FillIndexBuffer();
	m_pVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));	
	m_pIndexBuffer->SetIndices();
	m_pTexture->SetTexture(0);

	m_SharedLPDIRECT3DDEVICE9->SetFVF(TERRAINVERTEX::FVF);
	m_SharedLPDIRECT3DDEVICE9->SetTransform(D3DTS_WORLD, &GetWorldTM());	
   	m_SharedLPDIRECT3DDEVICE9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
   		0,  
   		0, 
   		m_TotalVertex,
   		0,
   		m_nTriangles );	

// 	m_SharedLPDIRECT3DDEVICE9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_cxHeightMap*m_czHeightMap,0,
// 		(m_cxHeightMap-1)*(m_czHeightMap-1)*2 );
}

BOOL cTerrainNode::IsValidHeightMap( UINT cx,UINT cz )
{
	int tempCX,tempCZ,bit;
	
	if ((cx<3)||(cz<3))
	{
		return FALSE;
	}
	tempCX=cx-1;	tempCZ=cz-1;

	bit=1;
	do 
	{	
		bit = bit<<1;	
	} while(bit<tempCX);		
	if (bit != tempCX) 
		return FALSE;

	bit=1;
	do 
	{	
		bit = bit<<1;	
	} while(bit<tempCZ);	
	if (bit != tempCZ) 
		return FALSE;

	return TRUE;
}

void cTerrainNode::FillIndexBuffer()
{
	m_nTriangles=0;
	WORD* pIndex=(WORD*)m_pIndexBuffer->Lock();		
	CameraNode* pActiveCamera=CameraNode::GetActiveCamera();
	m_pQuadSpatial->GatherCellIndex(*pActiveCamera,m_nTriangles,pIndex);
	m_pIndexBuffer->Unlock();	
}
