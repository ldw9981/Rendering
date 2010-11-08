#include "StdAfx.h"
#include "TerrainNode.h"
#include "Framework/D3DFramework.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "QuadSpatial.h"
#include "dib.h"
#include "CameraNode.h"
#include "Foundation/Define.h"
#include "ZTerrain.h"

cTerrainNode::cTerrainNode()
{
	SetNodeName("Terrain");
	m_pTerrainVertex=NULL;
	m_pRscVertexBuffer=NULL;
	m_VertexScale = D3DXVECTOR3(1.0f,0.1f,1.f);	
	m_nTriangles=0;
}

cTerrainNode::~cTerrainNode()
{
	SAFE_DELETEARRAY(m_pTerrainVertex);
	SAFE_RELEASE(m_pRscVertexBuffer);
	SAFE_RELEASE(m_pRscTexture);
	SAFE_RELEASE(m_pRscIndexBuffer);
}



BOOL cTerrainNode::Create(const char* strHeightBitmapFile,const char* strTextureFile)
{
//	char temp[256]={0,};
//	WideCharToMultiByte(CP_ACP, 0, strHeightBitmapFile, -1, temp, 256, NULL, NULL);

	// strHeightBitmapFile �ε�
	LPBYTE	pDIB = DibLoadHandle( strHeightBitmapFile );
	if( !pDIB ) return FALSE;

	m_cxHeightMap = DIB_CX( pDIB );
	m_czHeightMap = DIB_CY( pDIB );

	// ������ �˻�
	if (!IsValidHeightMap(m_cxHeightMap,m_czHeightMap))
	{	
		assert(0 || "2^n+1�� ũ�Ⱑ �ƴ�");
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

	
	// �ؽ��� �ε�
	m_pRscTexture = g_pD3DFramework->GetResourceMng()->CreateRscTexture(strTextureFile);
	if (m_pRscTexture==NULL)
		return FALSE;

	// ���ؽ����� ����
	UINT BufferSize=m_cxHeightMap*m_czHeightMap*sizeof(TERRAINVERTEX);
	m_pRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(BufferSize);

	// ���ؽ� ����
	TERRAINVERTEX* pVB= (TERRAINVERTEX*)m_pRscVertexBuffer->Lock();
	memcpy(pVB,&m_pTerrainVertex[0],sizeof(BufferSize));
	m_pRscVertexBuffer->Unlock();	

	// �ε��� ���� ����		���� �ﰢ��2��

	BufferSize=(m_cxHeightMap-1)*(m_czHeightMap-1)*2* sizeof(TERRAININDEX);
	m_pRscIndexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscIndexBuffer(BufferSize);


	TERRAININDEX tempI;
	TERRAININDEX *pIB=NULL;


	pIB=(TERRAININDEX*)m_pRscIndexBuffer->Lock();
	

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
	m_pRscIndexBuffer->Unlock();


	// ���� ����,����
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
	�׸����� �ε������ۿ� �������ҳ����� �ε����鸸 ��Ƽ� �ε������۸� ����� ����Ѵ�.

*/
void cTerrainNode::Render()
{	
	FillIndexBuffer();
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));	
	m_pRscIndexBuffer->SetIndices();
	m_pRscTexture->SetTexture(0);

	m_pD3DDevice->SetFVF(TERRAINVERTEX::FVF);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTM());	
   	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
   		0,  
   		0, 
   		m_TotalVertex,
   		0,
   		m_nTriangles );	

// 	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_cxHeightMap*m_czHeightMap,0,
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
	WORD* pIndex=(WORD*)m_pRscIndexBuffer->Lock();		
	cCameraNode* pActiveCamera=cCameraNode::GetActiveCamera();
	m_pQuadSpatial->GatherCellIndex(*pActiveCamera,m_nTriangles,pIndex);
	m_pRscIndexBuffer->Unlock();	
}
