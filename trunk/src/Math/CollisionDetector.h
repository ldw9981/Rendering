#pragma once

class cSphere;
class cLine;
class cPlane;
class cAABB;
class Frustum;
// 순수 기하도형의 Intersection에대한 알고리즘만 작성하고
// 오브젝트간의 충돌은 오브젝트 클래스에서 작성하자

class cCollision
{
public:
	cCollision(void);
	virtual ~cCollision(void);

	enum STATE { INSIDE,OUTSIDE,INTERSECT};

	

	// Sphere - Line
	static BOOL CollisionSphereSphere( cSphere& SphereA,D3DXVECTOR3& vecA,cSphere& SphereB,D3DXVECTOR3& vecB );
	static BOOL IntersectLineSphere(cLine& Line,cSphere& TargetSphere,float* pT=NULL,float* pPiercedDist=NULL,D3DXVECTOR3* pReflectionVec=NULL);
	// Sphere - Sphere
	static BOOL IntersectSphereSphere(cSphere& SphereA,cSphere& SphereB);

	// Sphere - Plane
	static cCollision::STATE	IntersectSpherePlane(cSphere& sphere,cPlane& TargetPlane,float *pDist=NULL,float Loose=0.0f);

	// Line - Plane
	static BOOL IntersectLinePlane(cLine& Line, cPlane& TargetPlane,D3DXVECTOR3* pCrossPos=NULL ,D3DXVECTOR3* pReflectionVec=NULL);

	// Sphere - AABB

	static BOOL IntersectAABBSphere(cAABB& AABB,cSphere& Sphere);
	static BOOL IntersectAABBLine( cAABB& AABB,cLine& Line,D3DXVECTOR3* pPosCross=NULL,D3DXVECTOR3* pVecReflection=NULL );
	static cCollision::STATE	IntersectAABBPlane(cAABB& AABB,cPlane& Plane);
	
	
	static cCollision::STATE    CheckWorldFrustum(Frustum& frustum, cSphere& sphere );
	static cCollision::STATE	CheckWorldFrustum(Frustum& frustum, cSphere& sphere,WORD plane );
	static BOOL					InsideWorldFrustum(Frustum& frustum, D3DXVECTOR3& pos );
	static cCollision::STATE	CheckWorldFrustumWithoutYAxis(Frustum& frustum,cSphere& sphere);
};
