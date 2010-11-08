#ifndef _ZQUADTREE_H_
#define _ZQUADTREE_H_

#include "Math/Sphere.h"
#include "Math/Plane.h"
#include "Scene/SceneNode.h"

#define	CELL_MAX	2

class ZTerrain;
class cTriangle;
class cCameraNode;
class cRendererQueue;
struct TERRAINVERTEX;


/*

*/
class ZQuadTree:
	public cSceneNode
{
	///    TopLeft(TL)      TopRight(TR)
	///              0-+-1
	///              + + +
	///              2-+-3
	/// BottomLeft(BL)      BottomRight(BR)
	enum			CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	enum			EdgeType	{ EDGE_TOP,EDGE_BOTTOM,EDGE_LEFT,EDGE_RIGHT };
private:
	ZQuadTree*			m_pChild[4];		/// QuadTree의 4개의 자식노드
	WORD				m_nCenter;			/// QuadTree에 보관할 첫번째 값
	WORD				m_nCorner[4];		/// QuadTree에 보관할 두번째 값
	ZTerrain*			m_pTerrain;
	TERRAINVERTEX*		m_pTerrainVertex;
	BOOL				m_bIsLeafNode;
	cTriangle*			m_pCellTriangle[CELL_MAX];		//충돌검사를 위한 삼각형

	cPlane*				m_pEdgePlane[4];		// 각 4면
	float				m_InsideRadius;			//내부로 들어갈수있는 원의 반지름
	
	
private:	
	
	void				DevideSelf();			// Quadtree를 4개의 하위 트리로 분할한다.
	void				CreateCellTriangle();	// 리프노드인경우는 셀의 삼각형을 만든다.
	void				CreateEdgePlane();
public:				
	ZQuadTree(ZTerrain* pTerrain,int tl,int tr,int bl,int br );					
	~ZQuadTree();		

	virtual void		Update(DWORD elapseTime);

	void				GenTriIndex(cCameraNode* pCamera, int& nTris, LPVOID pIndex ); /// 출력할 인덱스를 생성한다.	
	

	BOOL				CheckInside(D3DXVECTOR3& pos,float loose=0.0f);
	BOOL				CheckInside(cSphere& sphere,float loose=0.0f);
	
	BOOL				GetCellIntersection(D3DXVECTOR3& pos);
	virtual void		CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera );
	virtual void		Render();
};

#endif // _ZQUADTREE_H_
