#include "StdAfx.h"
#include "CollisionDetector.h"
#include "Line.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Frustum.h"

#include "Foundation/Trace.h"
namespace Sophia
{

cCollision::cCollision(void)
{
}

cCollision::~cCollision(void)
{
}


BOOL cCollision::IntersectLineSphere( cLine& Line,cSphere& TargetSphere,float* pT/*=NULL*/,float* pPiercedDist/*=NULL*/,D3DXVECTOR3* pReflectionVec/*=NULL*/)
{	
	D3DXVECTOR3 vecLineStartToSphereCenter;	
	D3DXVECTOR3 NearPiercedPos,FarPiercedPos;
	float		CrossToCenter_LengthSQ;	//구의중점에서 직선에 내린 수선의길이SQ
	float		CrossToPierced_Legnth;		//라인과수선교점에서부터 관통점까지의 길이
	float		projection_length;	//직선시작점과 구의 중점의 내적에의한 투영길이
	
	vecLineStartToSphereCenter= TargetSphere.GetCenterPos() - Line.GetStart();
	
	//	 구의중점과 라인의시작점 거리검사로 원안에 라인의 시작점이 있는지 검사한다. (충돌여부만 검사할때 필요)
	if ((pT==NULL)&&(pPiercedDist==NULL)&&(pReflectionVec==NULL))
	{
		if (D3DXVec3LengthSq(&vecLineStartToSphereCenter) < TargetSphere.GetRadiusSQ())		
			return TRUE;
	}
	
	// 라인의 방향벡터를 기준으로 90~270 도 즉 뒤쪽에 있는지 검사 
	projection_length=D3DXVec3Dot(&vecLineStartToSphereCenter,&Line.GetDirection());
	if( projection_length< 0 ) 
		return FALSE;	
	
	// 수선교점과 구중점사이의 거리검사
	CrossToCenter_LengthSQ = D3DXVec3LengthSq(&vecLineStartToSphereCenter) -  projection_length*projection_length;
	if (CrossToCenter_LengthSQ >= TargetSphere.GetRadiusSQ())
		return FALSE;
	
	// 이제 완전 충돌
	// 직선의 시작점에서부터의 거리
	if (pT!=NULL)	*pT=projection_length;

	//수선교점과 관통점 사이의 거리를 구한다.
	if (pPiercedDist!=NULL)
	{		
		CrossToPierced_Legnth = sqrt(TargetSphere.GetRadiusSQ() - CrossToCenter_LengthSQ);
		*pPiercedDist=CrossToPierced_Legnth;
	}

	// 반사벡터를 구한다.
	if (pReflectionVec!=NULL)
	{
		D3DXVECTOR3 CrossPos,CrossNormal;
		CrossPos= Line.GetDirection()*projection_length;
		CrossNormal = TargetSphere.GetCenterPos() - CrossPos;
		*pReflectionVec =  (2.0f * CrossNormal) + vecLineStartToSphereCenter;
	}
	return TRUE;
}

BOOL cCollision::IntersectLinePlane( cLine& Line,cPlane& TargetPlane,D3DXVECTOR3* pCrossPos/*=NULL*/ ,D3DXVECTOR3* pReflectionVec/*=NULL*/)
{	
	D3DXVECTOR3 posCross;
	D3DXPLANE tempPlane(TargetPlane.GetNormal().x,TargetPlane.GetNormal().y,TargetPlane.GetNormal().z,TargetPlane.GetDistance());
	//평면 교점 구하기
	if ( NULL ==  D3DXPlaneIntersectLine(&posCross,&tempPlane,&Line.GetStart(),&D3DXVECTOR3(Line.GetDirection()+Line.GetStart())) )
		return FALSE;
	
	//반사벡터 구하기
	if (pReflectionVec!=NULL)
	{
		D3DXVECTOR3 CrossNormal,vecLineStartToCross;	
		vecLineStartToCross = posCross - Line.GetStart();
		*pReflectionVec =  (2.0f * TargetPlane.GetNormal()) + vecLineStartToCross;
	}


	return TRUE;
}

BOOL cCollision::IntersectSphereSphere( cSphere& SphereA,cSphere& SphereB )
{
	float LengthSq=D3DXVec3LengthSq( &D3DXVECTOR3(SphereB.GetCenterPos() - SphereA.GetCenterPos()));

	if ((SphereA.GetRadiusSQ()+SphereB.GetRadiusSQ())<=LengthSq)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL cCollision::CollisionSphereSphere( cSphere& SphereA,D3DXVECTOR3& vecA,cSphere& SphereB,D3DXVECTOR3& vecB )
{
	//둘다 멈춰있는 구라면 
	float LengthSqA=D3DXVec3LengthSq(&vecA);
	float LengthSqB=D3DXVec3LengthSq(&vecB);
	if ((LengthSqA==0)&&(LengthSqB==0))
	{	// 그냥 겹침검사
		return IntersectSphereSphere(SphereA,SphereB);
	}
	
	cLine LineA,LineB;
//	float CrossTA,CrossTB;

	if ((LengthSqA!=0.0f)&&(LengthSqB!=0.0f))
	{
		LineA.Make(SphereA.GetCenterPos(),SphereA.GetCenterPos()+vecA);
		LineB.Make(SphereB.GetCenterPos(),SphereB.GetCenterPos()+vecB);

		LineA.GetDirection();

	}
	else if (LengthSqA==0.0f)
	{
		LineB.Make(SphereB.GetCenterPos(),SphereB.GetCenterPos()+vecB);
	}
	else
	{ 
		LineA.Make(SphereA.GetCenterPos(),SphereA.GetCenterPos()+vecA);
	}

	

//	IntersectLineSphere()

	return TRUE;
}
cCollision::STATE cCollision::IntersectSpherePlane(cSphere& sphere,cPlane& TargetPlane,float *pDist/*=NULL*/,float Loose /*=0.0f*/)
{
	float lengthCenterToPlane,radius;
	lengthCenterToPlane= TargetPlane.GetDistance(sphere.GetCenterPos()) + Loose;
	radius= sphere.GetRadius();
	

	//평면 앞쪽에 위치
	if( lengthCenterToPlane > radius )
	{				
		if (pDist!=NULL)	*pDist= lengthCenterToPlane - radius;
		return INSIDE;
	}

	//평면 뒤쪽에 위치
	if( lengthCenterToPlane < -radius )
	{				
		if (pDist!=NULL)	*pDist= lengthCenterToPlane + radius;
		return OUTSIDE;
	}
	return INTERSECT;	
}

BOOL cCollision::IntersectAABBLine( cAABB& AABB,cLine& Line,D3DXVECTOR3* pPosCross/*=NULL*/,D3DXVECTOR3* pVecReflection/*=NULL*/ )
{
	for (int iside=0;iside<6;iside++)
	{
		if (IntersectLinePlane(Line,AABB.GetPlane(iside),pPosCross,pVecReflection))
		{
			return TRUE;
		}
	}
	return FALSE;
}

cCollision::STATE cCollision::IntersectAABBPlane( cAABB& AABB,cPlane& Plane )
{	
	int cntoutside=0,cntinside=0;
	for (int ipos=0;ipos<8;ipos++)	
	{
		float dist=Plane.GetDistance(AABB.GetPos(ipos));
		if (dist > 0.0f ) cntoutside++;
		else if (dist < 0.0f ) cntinside++;
	}
	
	if (cntoutside==0)
	{
		return INSIDE;
	}
	else if (cntinside==0)
	{
		return OUTSIDE;
	}
	return INTERSECT;
}

int cCollision::IntersectAABBSphere( cAABB& AABB,cSphere& Sphere )
{
	float lengthSQ_CenterToAABB=0.0f;
	int cntoutside=0;
	for (int iplane=0;iplane<6;iplane++)
	{
		float Dist=0.0f;
		int side=IntersectSpherePlane(Sphere,AABB.GetPlane(iplane),&Dist);
		if ( side == INSIDE )
		{
			lengthSQ_CenterToAABB += Dist*Dist;
		}
		else if (OUTSIDE)	cntoutside++;
	}
	if (cntoutside==6)
	{	// 구가 모든평면들의 바깥쪽, AABB의 내부에 있다.
		return INSIDE;
	}
	else if (lengthSQ_CenterToAABB > Sphere.GetRadiusSQ())
	{
		return OUTSIDE;
	}
	return INTERSECT;
}


cCollision::STATE cCollision::CheckWorldFrustum(Frustum& frustum, cSphere& sphere ,float* pDistFromNear,float loose)
{
	int ret;
	BOOL bIntersect=false;
	for (int i=0 ;i<Frustum::PN_MAX;i++)
	{	
		if (i==Frustum::PN_NEAR)
			ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[i],pDistFromNear,loose);				
		else
			ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[i],NULL,loose);				

		if (ret==cCollision::OUTSIDE)	
		{	// 바깥쪽이면 무조건 바깥
			return cCollision::OUTSIDE;
		}
		else if (ret==cCollision::INTERSECT)
		{	//겹치면 안쪽은 아니다.
			bIntersect=true;
		}			
	}

	// 6면 검사후 
	if (bIntersect)
	{	// OUTSIDE는 아니지만 겹친다.
		return cCollision::INTERSECT;	
	}
	//안쪽이다.
	return cCollision::INSIDE;
}

cCollision::STATE cCollision::CheckWorldFrustum(Frustum& frustum, cSphere& sphere,WORD plane )
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;


	if (plane&Frustum::PB_TOP)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_TOP]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_BOTTOM)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_BOTTOM]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_LEFT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_LEFT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_RIGHT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_RIGHT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_NEAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_NEAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_FAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_FAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}

	// 6면 검사후 
	if (!bInside)
	{	// 안쪽이 아니면 겹친다.
		return cCollision::INTERSECT;	
	}	
	//안쪽이다.
	return cCollision::INSIDE;
}


BOOL cCollision::InsideWorldFrustum(Frustum& frustum, D3DXVECTOR3& pos )
{
	BOOL bInside=TRUE;
	// 현재는 left, right, far plane만 적용한다.
	for(UINT i = 0 ; i < 6 ; i++ )
	{
		if(frustum.m_plane[i].GetDistance(pos) < 0.0f )
		{
			bInside=FALSE;
		}
	}
	return bInside;
}

/*
	PLANE_TOP , PLANE_BOTTOM을 검사안한다.
*/
cCollision::STATE cCollision::CheckWorldFrustumWithoutYAxis(Frustum& frustum,cSphere& sphere,float loose)
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		if ( i== Frustum::PN_TOP || i== Frustum::PN_BOTTOM)
		{
			continue;
		}

		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[i],NULL,loose);				
		if (ret==cCollision::OUTSIDE)	
		{	// 바깥쪽이면 무조건 바깥
			return cCollision::OUTSIDE;
		}
		else if (ret==cCollision::INTERSECT)
		{	//겹치면 안쪽은 아니다.
			bInside=FALSE;
		}			
	}

	// 6면 검사후 
	if (!bInside)
	{	// 안쪽이 아니면 겹친다.
		return cCollision::INTERSECT;	
	}
	//안쪽이다.
	return cCollision::INSIDE;
}
}