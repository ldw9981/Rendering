#pragma once

#include <d3dx9.h>

class cSphere;
class cLine;
class cPlane;
class cAABB;
// ���� ���ϵ����� Intersection������ �˰��� �ۼ��ϰ�
// ������Ʈ���� �浹�� ������Ʈ Ŭ�������� �ۼ�����

class cCollision
{
public:
	cCollision(void);
	virtual ~cCollision(void);

	enum { INSIDE,OUTSIDE,INTERSECT};

	

	// Sphere - Line
	static BOOL CollisionSphereSphere( cSphere& SphereA,D3DXVECTOR3& vecA,cSphere& SphereB,D3DXVECTOR3& vecB );
	static BOOL IntersectLineSphere(cLine& Line,cSphere& TargetSphere,float* pT=NULL,float* pPiercedDist=NULL,D3DXVECTOR3* pReflectionVec=NULL);
	// Sphere - Sphere
	static BOOL IntersectSphereSphere(cSphere& SphereA,cSphere& SphereB);

	// Sphere - Plane
	static int	IntersectSpherePlane(cSphere& Sphere,cPlane& TargetPlane,float *pDist=NULL,float Loose=0.0f);

	// Line - Plane
	static BOOL IntersectLinePlane(cLine& Line, cPlane& TargetPlane,D3DXVECTOR3* pCrossPos=NULL ,D3DXVECTOR3* pReflectionVec=NULL);

	// Sphere - AABB

	static BOOL IntersectAABBSphere(cAABB& AABB,cSphere& Sphere);
	static BOOL IntersectAABBLine( cAABB& AABB,cLine& Line,D3DXVECTOR3* pPosCross=NULL,D3DXVECTOR3* pVecReflection=NULL );
	static int	IntersectAABBPlane(cAABB& AABB,cPlane& Plane);
};
