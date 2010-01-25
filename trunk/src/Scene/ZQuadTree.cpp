#include "ZQuadTree.h"
#include "ZTerrain.h"
#include "../Math/Triangle.h"
#include "../Scene/CameraNode.h"
#include "../Math/Sphere.h"
#include "../Math/CollisionDetector.h"
#include "../Framework/App/App.h"
#include "../Scene/DebugInfoScene.h"
#include "../Scene/RendererQueue.h"
#include "../Foundation/Define.h"

// 최초 루트노드 생성자
ZQuadTree::ZQuadTree(ZTerrain* pTerrain,int tl,int tr,int bl,int br )
{
	m_pTerrain=pTerrain;
	m_pTerrainVertex=pTerrain->GetHeightMap();

	UINT i;
	for(  i = 0 ; i < 4 ; i++ )	m_pChild[i] = NULL;
	for(  i = 0 ; i < CELL_MAX ; i++ ) m_pCellTriangle[i] = NULL;
	for(  i = 0 ; i < 4 ; i++ ) m_pEdgePlane[i] = NULL;

	// 루트노드의 4개 코너값 설정
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

	// y축값 0.0f인 2차원 컬링 스피어 만들기
	CreateCullingSphere()->Make(D3DXVECTOR3(tempAxisMin.x,0.0f,tempAxisMin.z),D3DXVECTOR3(tempAxisMax.x,0.0f,tempAxisMax.z));
	
	if(m_bIsLeafNode)
	{		
		CreateCellTriangle();	
		// LeafNode에만 3차원 바운드 스피어가있다.
		CreateBoundingSphere()->Make(tempAxisMin,tempAxisMax);	
	}
	else
	{		
		DevideSelf();
	}
}



// 소멸자
ZQuadTree::~ZQuadTree()
{
	// 자식 노드들을 소멸 시킨다.
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



// Quadtree를 4개의 자식 트리로 부분분할(subdivide)한다.
void	ZQuadTree::DevideSelf()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// 상단변 가운데
	nTopEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
	// 하단변 가운데 
	nBottomEdgeCenter	= ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
	// 좌측변 가운데
	nLeftEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
	// 우측변 가운데
	nRightEdgeCenter	= ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
	// 한가운데
	nCentralPoint		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
		m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	// 4개의 자식노드 추가
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

// 출력할 폴리곤의 인덱스를 생성한다.
void ZQuadTree::GenTriIndex( CameraNode* pCamera,int& nTris, LPVOID pIndex )
{
	if( !m_bIsLeafNode )
	{	// 그룹노드일때는 2차원 구로 컬링
		if ( pCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere) == cCollision::OUTSIDE)
		{
			return;
		}		
	}
	else
	{
		// 리프노드일때는 3차원구로 컬링
		if ( pCamera->CheckWorldFrustum(m_pBoundingSphere) == cCollision::OUTSIDE)
		{
			return;
		}		

		LPWORD p = ((LPWORD)pIndex) + nTris * 3;

		*p++ = m_nCorner[0];
		*p++ = m_nCorner[1];
		*p++ = m_nCorner[2];
		nTris++;		

		*p++ = m_nCorner[2];
		*p++ = m_nCorner[1];
		*p++ = m_nCorner[3];
		nTris++;
		return;
	}

	// 자식 노드들 검색
	if( m_pChild[CORNER_TL] ) m_pChild[CORNER_TL]->GenTriIndex(pCamera, nTris, pIndex );
	if( m_pChild[CORNER_TR] ) m_pChild[CORNER_TR]->GenTriIndex(pCamera, nTris, pIndex );
	if( m_pChild[CORNER_BL] ) m_pChild[CORNER_BL]->GenTriIndex(pCamera, nTris, pIndex );
	if( m_pChild[CORNER_BR] ) m_pChild[CORNER_BR]->GenTriIndex(pCamera, nTris, pIndex );
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
	// 면의 외부에 있으면 무조건 외부

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

/*
	CheckWorldFrustumForQuadTree를 사용한다
*/
void ZQuadTree::CullRendererTraversal( cRendererQueue* pRendererQueue,CameraNode* pActiveCamera )
{
	int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere);
	if( ret == cCollision::OUTSIDE)
	{	//  밖에 있는것이면 노드순회 없음
		printf("d");
		return;
	}
	else if (ret == cCollision::INSIDE)
	{	// 완전히 내부면 순회하며 자식은 모두 TOP,BOTTON플랜만 테스한후 큐에 넣는다.
		InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera,PB_TOP|PB_BOTTOM);
		return;
	}			
	
	list<SceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}

void ZQuadTree::Init()
{

}

void ZQuadTree::Uninit()
{

}

void ZQuadTree::Update( DWORD elapseTime )
{
	UpdateChildren(elapseTime);
}