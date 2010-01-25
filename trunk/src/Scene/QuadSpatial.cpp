#include "QuadSpatial.h"
#include "TerrainNode.h"
#include "../Math/Triangle.h"
#include "../Scene/CameraNode.h"
#include "../Scene/ZTerrain.h"
#include "../Foundation/Define.h"

UINT cQuadSpatial::m_CountLeaf=0;


cQuadSpatial::cQuadSpatial()
{
	for (UINT i=0;i<SPATIAL_MAX;i++)
	{
		m_pSpatialChild[i]=NULL;
	}
	m_bExistChild=FALSE;
	m_bIsLeafNode=FALSE;

	m_pCellTriangle[0]=NULL;
	m_pCellTriangle[1]=NULL;		
}

cQuadSpatial::cQuadSpatial( cTerrainNode* pTerrain,UINT IndexTL,UINT IndexTR,UINT IndexBL,UINT IndexBR )
{
	for (UINT i=0;i<SPATIAL_MAX;i++)
	{
		m_pSpatialChild[i]=NULL;
	}
	m_bExistChild=FALSE;
	m_bIsLeafNode=FALSE;
	
	m_pCellTriangle[0]=NULL;
	m_pCellTriangle[1]=NULL;

	m_pTerrain = pTerrain;
	
	m_CornerVertexnIndex[SPATIAL_TL]=IndexTL;
	m_CornerVertexnIndex[SPATIAL_TR]=IndexTR;
	m_CornerVertexnIndex[SPATIAL_BL]=IndexBL;
	m_CornerVertexnIndex[SPATIAL_BR]=IndexBR;
	m_CenterVertexnIndex = ( IndexTL + IndexTR + IndexBL + IndexBR ) / 4;

	TERRAINVERTEX* pVertexArray=pTerrain->GetTerrainVertex();
	D3DXVECTOR3 centerPos=pVertexArray[m_CenterVertexnIndex].p;
	D3DXVECTOR3 temp = centerPos - pVertexArray[IndexTL].p;
	float		radius=D3DXVec3Length(&temp);	

	this->Make(centerPos,radius);
}


cQuadSpatial::~cQuadSpatial()
{
	for (UINT i=0;i<SPATIAL_MAX;i++)
	{
		SAFE_DELETE(m_pSpatialChild[i]);
	}
}

void cQuadSpatial::Update( DWORD elapseTime )
{

}


void cQuadSpatial::DevideSelf()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;


	// ���̻� ������ �Ұ����Ѱ�? �׷��ٸ� �������� ������ �� �����
	if( (m_CornerVertexnIndex[SPATIAL_TR] - m_CornerVertexnIndex[SPATIAL_TL]) <= 1 )
	{
		m_bIsLeafNode = TRUE;
		CreateCellTriangles();
		m_CountLeaf++;
		return;
	}

	// ��ܺ� ���
	nTopEdgeCenter		= ( m_CornerVertexnIndex[SPATIAL_TL] + m_CornerVertexnIndex[SPATIAL_TR] ) / 2;
	// �ϴܺ� ��� 
	nBottomEdgeCenter	= ( m_CornerVertexnIndex[SPATIAL_BL] + m_CornerVertexnIndex[SPATIAL_BR] ) / 2;
	// ������ ���
	nLeftEdgeCenter		= ( m_CornerVertexnIndex[SPATIAL_TL] + m_CornerVertexnIndex[SPATIAL_BL] ) / 2;
	// ������ ���
	nRightEdgeCenter	= ( m_CornerVertexnIndex[SPATIAL_TR] + m_CornerVertexnIndex[SPATIAL_BR] ) / 2;
	// �Ѱ��
	nCentralPoint		= ( m_CornerVertexnIndex[SPATIAL_TL] + m_CornerVertexnIndex[SPATIAL_TR] + 
		m_CornerVertexnIndex[SPATIAL_BL] + m_CornerVertexnIndex[SPATIAL_BR] ) / 4;


	// 4���� �ڽĳ�� �߰�
	m_pSpatialChild[SPATIAL_TL] = new cQuadSpatial(m_pTerrain,
		m_CornerVertexnIndex[SPATIAL_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint );
	m_pSpatialChild[SPATIAL_TR] = new cQuadSpatial(m_pTerrain,
		nTopEdgeCenter, m_CornerVertexnIndex[SPATIAL_TR], nCentralPoint, nRightEdgeCenter );
	m_pSpatialChild[SPATIAL_BL] = new cQuadSpatial(m_pTerrain,
		nLeftEdgeCenter, nCentralPoint, m_CornerVertexnIndex[SPATIAL_BL], nBottomEdgeCenter );
	m_pSpatialChild[SPATIAL_BR] = new cQuadSpatial(m_pTerrain,
		nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_CornerVertexnIndex[SPATIAL_BR] );	

	m_pSpatialChild[SPATIAL_TL]->DevideSelf();
	m_pSpatialChild[SPATIAL_TR]->DevideSelf();
	m_pSpatialChild[SPATIAL_BL]->DevideSelf();
	m_pSpatialChild[SPATIAL_BR]->DevideSelf();
}

/*
	m_pTerrain�� m_CornerVertexnIndex,m_CenterVertexIndex�� ����Ͽ� Cell�� �����Ѵ�.
*/
void cQuadSpatial::CreateCellTriangles()
{
	TERRAINVERTEX* pVertex=m_pTerrain->GetTerrainVertex();

	WORD index[3];

	// ++++++++++
	// + 0   1
	// +
	// + 2
	index[0]=m_CornerVertexnIndex[SPATIAL_TL];
	index[1]=m_CornerVertexnIndex[SPATIAL_TR];
	index[2]=m_CornerVertexnIndex[SPATIAL_BL];
	m_pCellTriangle[0]=new cTriangle(pVertex[index[0]].p,pVertex[index[1]].p,pVertex[index[2]].p);

	// ++++++++
	// +     4
	// +
	// + 3   5 
	index[0]=m_CornerVertexnIndex[SPATIAL_BL];
	index[1]=m_CornerVertexnIndex[SPATIAL_TR];
	index[2]=m_CornerVertexnIndex[SPATIAL_BR];
	m_pCellTriangle[1]=new cTriangle(pVertex[index[0]].p,pVertex[index[1]].p,pVertex[index[2]].p);
}


// ����� �������� �ε����� �����Ѵ�.
void cQuadSpatial::GatherCellIndex(CameraNode& Camera,UINT& nTris, LPVOID pIndex )
{
	// �ø��� ����� �׳� ����
//	if(!Camera.CheckInsideWorldFrustum((cSphere*)this))
//		return;

	// Cell�� ���� ��忡�� ����
	if( IsLeafNode() )
	{
		LPWORD p = ((LPWORD)pIndex) + nTris * 3;		
		*p++ = m_CornerVertexnIndex[SPATIAL_TL];
		*p++ = m_CornerVertexnIndex[SPATIAL_TR];
		*p++ = m_CornerVertexnIndex[SPATIAL_BL];
		nTris++;
		*p++ = m_CornerVertexnIndex[SPATIAL_BL];
		*p++ = m_CornerVertexnIndex[SPATIAL_TR];
		*p++ = m_CornerVertexnIndex[SPATIAL_BR];
		nTris++;					
		return;
	}

	// �ڽ� ���� �˻�
	if( m_pSpatialChild[SPATIAL_TL] ) m_pSpatialChild[SPATIAL_TL]->GatherCellIndex(Camera,nTris, pIndex );
	if( m_pSpatialChild[SPATIAL_TR] ) m_pSpatialChild[SPATIAL_TR]->GatherCellIndex(Camera,nTris, pIndex );
	if( m_pSpatialChild[SPATIAL_BL] ) m_pSpatialChild[SPATIAL_BL]->GatherCellIndex(Camera,nTris, pIndex );
	if( m_pSpatialChild[SPATIAL_BR] ) m_pSpatialChild[SPATIAL_BR]->GatherCellIndex(Camera,nTris, pIndex );
}