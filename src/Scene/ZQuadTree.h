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
class cView;
class Frustum;
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
	ZQuadTree*			m_pChild[4];		/// QuadTree�� 4���� �ڽĳ��
	WORD				m_nCenter;			/// QuadTree�� ������ ù��° ��
	WORD				m_nCorner[4];		/// QuadTree�� ������ �ι�° ��
	ZTerrain*			m_pTerrain;
	TERRAINVERTEX*		m_pTerrainVertex;
	BOOL				m_bIsLeafNode;
	cTriangle*			m_pCellTriangle[CELL_MAX];		//�浹�˻縦 ���� �ﰢ��

	cPlane*				m_pEdgePlane[4];		// �� 4��
	float				m_InsideRadius;			//���η� �����ִ� ���� ������
	
	
private:	
	
	void				DevideSelf();			// Quadtree�� 4���� ���� Ʈ���� �����Ѵ�.
	void				CreateCellTriangle();	// ��������ΰ��� ���� �ﰢ���� �����.
	void				CreateEdgePlane();
public:				
	ZQuadTree(ZTerrain* pTerrain,int tl,int tr,int bl,int br );					
	~ZQuadTree();		

	virtual void		Update(DWORD elapseTime);

	void				GenTriIndex(Frustum& frustum, int& nTris, LPVOID pIndex ); /// ����� �ε����� �����Ѵ�.	
	

	BOOL				CheckInside(D3DXVECTOR3& pos,float loose=0.0f);
	BOOL				CheckInside(cSphere& sphere,float loose=0.0f);
	
	BOOL				GetCellIntersection(D3DXVECTOR3& pos);
	virtual void		CullRendererIntoRendererQueue(cView* pView,Frustum* pFrustum );
	virtual void		Render();
};

#endif // _ZQUADTREE_H_
