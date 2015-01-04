#include "StdAfx.h"
#include "ZQuadTree.h"
#include "ZTerrain.h"
#include "Math/Triangle.h"
#include "CameraNode.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"
#include "Math/CollisionDetector.h"
#include "Entity.h"
#include "RendererQueue.h"
#include "Foundation/Define.h"
namespace Sophia
{


// ���� ��Ʈ��� ������
ZQuadTree::ZQuadTree(ZTerrain* pTerrain,int tl,int tr,int bl,int br )
{
	m_pTerrain=pTerrain;
	m_pTerrainVertex=pTerrain->GetHeightMap();

	UINT i;
	for(  i = 0 ; i < 4 ; i++ )	m_pChild[i] = NULL;
	for(  i = 0 ; i < CELL_MAX ; i++ ) m_pCellTriangle[i] = NULL;
	for(  i = 0 ; i < 4 ; i++ ) m_pEdgePlane[i] = NULL;

	// ��Ʈ����� 4�� �ڳʰ� ����
	m_nCorner[CORNER_TL]	= tl;
	m_nCorner[CORNER_TR]	= tr;
	m_nCorner[CORNER_BL]	= bl;
	m_nCorner[CORNER_BR]	= br;
	if( (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1 )
	{
		m_bIsLeafNode=TRUE;
		m_nCenter = 0;
	}
	else
	{
		m_bIsLeafNode=FALSE;
		m_nCenter				= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
			m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;	
	}

	D3DXVECTOR3 temp=m_pTerrainVertex[m_nCorner[CORNER_TL]].p - m_pTerrainVertex[m_nCorner[CORNER_BL]].p;
	m_InsideRadius			= D3DXVec3Length(&temp)/2;

	CreateEdgePlane();
	
	D3DXVECTOR3 tempAxisMin,tempAxisMax;
	D3DXVECTOR3 tempPos=m_pTerrainVertex[m_nCorner[0]].p;
	tempAxisMin= tempPos;	
	tempAxisMax= tempPos;	
	for (UINT i = 1 ; i < 4 ; i++)
	{	
		tempPos=m_pTerrainVertex[m_nCorner[i]].p;
		tempAxisMin.x= min(tempAxisMin.x,tempPos.x);
		tempAxisMin.y= min(tempAxisMin.y,tempPos.y);
		tempAxisMin.z= min(tempAxisMin.z,tempPos.z);

		tempAxisMax.x= max(tempAxisMax.x,tempPos.x);
		tempAxisMax.y= max(tempAxisMax.y,tempPos.y);
		tempAxisMax.z= max(tempAxisMax.z,tempPos.z);		
	}	

	// y�ప 0.0f�� 2���� �ø� ���Ǿ� �����
	m_BoundingSphere.Make(D3DXVECTOR3(tempAxisMin.x,0.0f,tempAxisMin.z),D3DXVECTOR3(tempAxisMax.x,0.0f,tempAxisMax.z));
	
	if(m_bIsLeafNode)
	{		
		CreateCellTriangle();	
	}
	else
	{		
		DevideSelf();
	}
}



// �Ҹ���
ZQuadTree::~ZQuadTree()
{
	// �ڽ� ������ �Ҹ� ��Ų��.
	UINT i;
	for( i = 0 ; i < 4 ; i++ ) SAFE_DELETE( m_pEdgePlane[i] );
	for( i = 0 ; i < 2 ; i++ ) SAFE_DELETE( m_pCellTriangle[i] );
	for( i = 0 ; i < 4 ; i++ ) 
	{
		if (m_pChild[i]!=NULL)	
			DettachChildNode(m_pChild[i]);

		SAFE_DELETE( m_pChild[i] );	
	}
}



// Quadtree�� 4���� �ڽ� Ʈ���� �κк���(subdivide)�Ѵ�.
void	ZQuadTree::DevideSelf()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// ��ܺ� ���
	nTopEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
	// �ϴܺ� ��� 
	nBottomEdgeCenter	= ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
	// ������ ���
	nLeftEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
	// ������ ���
	nRightEdgeCenter	= ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
	// �Ѱ��
	nCentralPoint		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
		m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	// 4���� �ڽĳ�� �߰�
	m_pChild[CORNER_TL] = new ZQuadTree(m_pTerrain, 
		m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint );	
	m_pChild[CORNER_TR] = new ZQuadTree(m_pTerrain, 
		nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter );
	m_pChild[CORNER_BL] = new ZQuadTree(m_pTerrain, 
		nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter );
	m_pChild[CORNER_BR] = new ZQuadTree(m_pTerrain, 
		nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR] );

	AttachChildNode(m_pChild[CORNER_TL]);
	AttachChildNode(m_pChild[CORNER_TR]);
	AttachChildNode(m_pChild[CORNER_BL]);
	AttachChildNode(m_pChild[CORNER_BR]);
}



void ZQuadTree::GenTriIndex(Frustum& frustum,int& nTris, LPVOID pIndex ,bool bTraverse )
{
	LPWORD p = ((LPWORD)pIndex) + nTris * 3;
	if (!m_bIsLeafNode)
	{
		cCollision::STATE state = cCollision::CheckWorldFrustumWithoutYAxis(frustum,m_BoundingSphere);
		if ( state == cCollision::INSIDE)
		{
			LPWORD p = ((LPWORD)pIndex) + nTris * 3;
			int tl = m_nCorner[CORNER_TL];
			int size = m_nCorner[CORNER_TR]-tl;
			int czdib = m_pTerrain->GetCZDIB();
			int point=0;
			for (int ch = 0 ; ch < size ; ch++)
			{
				for (int cw = 0; cw < size ; cw++)
				{
					point = ch * czdib + tl + cw;
					*p++ = point ;					 	// + - +
					*p++ = point + 1;				    // | /
					*p++ = point + czdib;			    // +
					nTris++;		

					*p++ = point + 1;				    //      +
					*p++ = point + czdib + 1;			//    / |
					*p++ = point + czdib;				//  + - +
					nTris++;
				}

			}
		}
		else if ( state == cCollision::INTERSECT)
		{
			m_pChild[CORNER_TL]->GenTriIndex(frustum, nTris, pIndex ,false);
			m_pChild[CORNER_TR]->GenTriIndex(frustum, nTris, pIndex ,false);
			m_pChild[CORNER_BL]->GenTriIndex(frustum, nTris, pIndex ,false);
			m_pChild[CORNER_BR]->GenTriIndex(frustum, nTris, pIndex ,false);
		}			
	}
	else
	{
		*p++ = m_nCorner[CORNER_TL];			// + - +
		*p++ = m_nCorner[CORNER_TR];			// | /
		*p++ = m_nCorner[CORNER_BL];            // +
		nTris++;		

		*p++ = m_nCorner[CORNER_BL];			//      +
		*p++ = m_nCorner[CORNER_TR];			//    / |
		*p++ = m_nCorner[CORNER_BR];			//  + - +
		nTris++;
	}
}

void ZQuadTree::CreateCellTriangle()
{
	D3DXVECTOR3 pos[3];
	// ++++++++++
	// + 0   1
	// +
	// + 2
	pos[0]=m_pTerrainVertex[m_nCorner[CORNER_TL]].p ;
	pos[1]=m_pTerrainVertex[m_nCorner[CORNER_TR]].p ;
	pos[2]=m_pTerrainVertex[m_nCorner[CORNER_BL]].p ;
	m_pCellTriangle[0]=new cTriangle(pos[0],pos[1],pos[2]);

	// ++++++++
	// +     1
	// +
	// + 0   2 
	pos[0]=m_pTerrainVertex[m_nCorner[CORNER_BL]].p ;
	pos[1]=m_pTerrainVertex[m_nCorner[CORNER_TR]].p ;
	pos[2]=m_pTerrainVertex[m_nCorner[CORNER_BR]].p ;
	m_pCellTriangle[1]=new cTriangle(pos[0],pos[1],pos[2]);
}

BOOL ZQuadTree::CheckInside( D3DXVECTOR3& pos,float loose/*=0.0f*/ )
{	
	// ���� �ܺο� ������ ������ �ܺ�

	float dist;

	dist = m_pEdgePlane[EDGE_TOP]->GetDistance(pos) + loose;
	if( dist < 0.0f )		
		return FALSE;

	dist = m_pEdgePlane[EDGE_BOTTOM]->GetDistance(pos) + loose;
	if( dist < 0.0f )	
		return FALSE;

	dist = m_pEdgePlane[EDGE_LEFT]->GetDistance(pos) + loose;
	if( dist < 0.0f )		
		return FALSE;

	dist = m_pEdgePlane[EDGE_RIGHT]->GetDistance(pos) + loose;
	if( dist < 0.0f )		
		return FALSE;

	return TRUE;
}

BOOL ZQuadTree::CheckInside( cSphere& sphere ,float loose/*=0.0f*/)
{

	if( cCollision::INSIDE != cCollision::IntersectSpherePlane(sphere,*m_pEdgePlane[EDGE_TOP],NULL,loose)) 
		return FALSE;

	if( cCollision::INSIDE != cCollision::IntersectSpherePlane(sphere,*m_pEdgePlane[EDGE_BOTTOM],NULL,loose)) 
		return FALSE;

	if( cCollision::INSIDE != cCollision::IntersectSpherePlane(sphere,*m_pEdgePlane[EDGE_LEFT],NULL,loose)) 
		return FALSE;

	if( cCollision::INSIDE != cCollision::IntersectSpherePlane(sphere,*m_pEdgePlane[EDGE_RIGHT],NULL,loose)) 
		return FALSE;

	return TRUE;

}
void ZQuadTree::CreateEdgePlane()
{
	D3DXVECTOR3 normal;
	float		d;		

	d = m_pTerrainVertex[m_nCorner[CORNER_TL]].p.z;		
	d = abs(d);
	normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
	m_pEdgePlane[EDGE_TOP] = new cPlane(normal,d);	

	d = m_pTerrainVertex[m_nCorner[CORNER_BL]].p.z;
	d = abs(d);
	normal = D3DXVECTOR3(0.0f,0.0f,1.0f);
	m_pEdgePlane[EDGE_BOTTOM] = new cPlane(normal,d);

	d = m_pTerrainVertex[m_nCorner[CORNER_TL]].p.x;
	d = abs(d);
	normal = D3DXVECTOR3(1.0f,0.0f,0.0f);
	m_pEdgePlane[EDGE_LEFT] = new cPlane(normal,d);	

	d = m_pTerrainVertex[m_nCorner[CORNER_TR]].p.x;
	d = abs(d);
	normal = D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_pEdgePlane[EDGE_RIGHT] = new cPlane(normal,d);
}

BOOL ZQuadTree::GetCellIntersection( D3DXVECTOR3& pos )
{
	if(!CheckInside(pos))
		return FALSE;

	if (!m_bIsLeafNode)
	{
		for (UINT i=0;i<4;i++)	
		{
			if(m_pChild[i]->GetCellIntersection(pos)) return TRUE;
		}
		return FALSE;
	}


	D3DXVECTOR3 posGround=D3DXVECTOR3(pos.x,0.0f,pos.z);
	D3DXVECTOR3 vecUp=D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVECTOR3 posIntersect;

	float dist;
	UINT i;
	for (i=0;i<CELL_MAX;i++)
	{		
		if (D3DXIntersectTri(&m_pCellTriangle[i]->GetP1(),
			&m_pCellTriangle[i]->GetP3(),
			&m_pCellTriangle[i]->GetP2(),
			&posGround,&vecUp,NULL,NULL,&dist))
		{
			posIntersect = dist * vecUp;
			posIntersect += posGround;		
			pos.y=dist;
			return TRUE;
		}		
	}
	return FALSE;
}



void ZQuadTree::Update( DWORD elapseTime )
{
	for ( auto iter=m_vecChildNode.begin() ; iter!=m_vecChildNode.end() ; ++iter)
	{		
		(*iter)->Update(elapseTime);
	}	
}

void ZQuadTree::Render()
{

}
}