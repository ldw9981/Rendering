#pragma once

#include "Math/Sphere.h"

/*
	공간의 의미이므로 씬그래프노드는 아니다.
*/
class cCameraNode;
class cTerrainNode;
class cTriangle;
class cQuadSpatial:	
	public cSphere
{
public:
	cQuadSpatial();
	cQuadSpatial(cTerrainNode* pTerrain,UINT IndexTL,UINT IndexTR,UINT IndexBL,UINT IndexBR);
	virtual ~cQuadSpatial();

	enum SPATIAL	{ SPATIAL_TL,SPATIAL_TR,SPATIAL_BL,SPATIAL_BR,SPATIAL_MAX };
//	enum INDEX	{ INDEX_TL,INDEX_TR,INDEX_BL,INDEX_BR,INDEX_CENTER,INDEX_MAX };
protected:
	WORD				m_CornerVertexnIndex[SPATIAL_MAX];		
	WORD				m_CenterVertexnIndex;		
	cTerrainNode*		m_pTerrain;
private:
	cQuadSpatial*		m_pSpatialChild[SPATIAL_MAX];			
	BOOL				m_bExistChild;
	cTriangle*			m_pCellTriangle[2];
	BOOL				m_bIsLeafNode;
	
	static UINT			m_CountLeaf;
	
		
	
private:
	void				CreateCellTriangles();		//내부에서 리프노드일때 셀을 만든다.
public:	
	void				Update(DWORD elapseTime);

	//
	void				DevideSelf();
	BOOL				IsLeafNode() const { return m_bIsLeafNode; }
	void				GatherCellIndex(cCameraNode& Camera,UINT& nTris, LPVOID pIndex );

};