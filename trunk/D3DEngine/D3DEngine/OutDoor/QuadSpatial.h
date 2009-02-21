#pragma once

#include "../Geometrics/Sphere.h"

/*
	������ �ǹ��̹Ƿ� ���׷������� �ƴϴ�.
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
	void				CreateCellTriangles();		//���ο��� ��������϶� ���� �����.
public:	
	void				Update(DWORD elapseTime);

	//
	void				DevideSelf();
	BOOL				IsLeafNode() const { return m_bIsLeafNode; }
	void				GatherCellIndex(cCameraNode& Camera,UINT& nTris, LPVOID pIndex );

};