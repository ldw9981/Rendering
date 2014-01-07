#include "StdAfx.h"
#include "CollisionDetector.h"
#include "Line.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Frustum.h"

#include "Foundation/Trace.h"
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
	float		CrossToCenter_LengthSQ;	//������������ ������ ���� �����Ǳ���SQ
	float		CrossToPierced_Legnth;		//���ΰ����������������� ������������ ����
	float		projection_length;	//������������ ���� ������ ���������� ��������
	
	vecLineStartToSphereCenter= TargetSphere.GetCenterPos() - Line.GetStart();
	
	//	 ���������� �����ǽ����� �Ÿ��˻�� ���ȿ� ������ �������� �ִ��� �˻��Ѵ�. (�浹���θ� �˻��Ҷ� �ʿ�)
	if ((pT==NULL)&&(pPiercedDist==NULL)&&(pReflectionVec==NULL))
	{
		if (D3DXVec3LengthSq(&vecLineStartToSphereCenter) < TargetSphere.GetRadiusSQ())		
			return TRUE;
	}
	
	// ������ ���⺤�͸� �������� 90~270 �� �� ���ʿ� �ִ��� �˻� 
	projection_length=D3DXVec3Dot(&vecLineStartToSphereCenter,&Line.GetDirection());
	if( projection_length< 0 ) 
		return FALSE;	
	
	// ���������� ������������ �Ÿ��˻�
	CrossToCenter_LengthSQ = D3DXVec3LengthSq(&vecLineStartToSphereCenter) -  projection_length*projection_length;
	if (CrossToCenter_LengthSQ >= TargetSphere.GetRadiusSQ())
		return FALSE;
	
	// ���� ���� �浹
	// ������ ���������������� �Ÿ�
	if (pT!=NULL)	*pT=projection_length;

	//���������� ������ ������ �Ÿ��� ���Ѵ�.
	if (pPiercedDist!=NULL)
	{		
		CrossToPierced_Legnth = sqrt(TargetSphere.GetRadiusSQ() - CrossToCenter_LengthSQ);
		*pPiercedDist=CrossToPierced_Legnth;
	}

	// �ݻ纤�͸� ���Ѵ�.
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
	//��� ���� ���ϱ�
	if ( NULL ==  D3DXPlaneIntersectLine(&posCross,&tempPlane,&Line.GetStart(),&D3DXVECTOR3(Line.GetDirection()+Line.GetStart())) )
		return FALSE;
	
	//�ݻ纤�� ���ϱ�
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
	//�Ѵ� �����ִ� ����� 
	float LengthSqA=D3DXVec3LengthSq(&vecA);
	float LengthSqB=D3DXVec3LengthSq(&vecB);
	if ((LengthSqA==0)&&(LengthSqB==0))
	{	// �׳� ��ħ�˻�
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
	

	//��� ���ʿ� ��ġ
	if( lengthCenterToPlane > radius )
	{				
		if (pDist!=NULL)	*pDist= lengthCenterToPlane - radius;
		return INSIDE;
	}

	//��� ���ʿ� ��ġ
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
	{	// ���� ��������� �ٱ���, AABB�� ���ο� �ִ�.
		return INSIDE;
	}
	else if (lengthSQ_CenterToAABB > Sphere.GetRadiusSQ())
	{
		return OUTSIDE;
	}
	return INTERSECT;
}


cCollision::STATE cCollision::CheckWorldFrustum(Frustum& frustum, cSphere& sphere ,float loose)
{
	int ret;
	BOOL bIntersect=false;
	for (int i=0;i<6;i++)
	{	
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[i],NULL,loose);				
		if (ret==cCollision::OUTSIDE)	
		{	// �ٱ����̸� ������ �ٱ�
			return cCollision::OUTSIDE;
		}
		else if (ret==cCollision::INTERSECT)
		{	//��ġ�� ������ �ƴϴ�.
			bIntersect=true;
		}			
	}

	// 6�� �˻��� 
	if (bIntersect)
	{	// OUTSIDE�� �ƴ����� ��ģ��.
		return cCollision::INTERSECT;	
	}
	//�����̴�.
	return cCollision::INSIDE;
}

cCollision::STATE cCollision::CheckWorldFrustum(Frustum& frustum, cSphere& sphere,WORD plane )
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;


	if (plane&Frustum::PB_TOP)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_TOP]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_BOTTOM)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_BOTTOM]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_LEFT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_LEFT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_RIGHT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_RIGHT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_NEAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_NEAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&Frustum::PB_FAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[Frustum::PN_FAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}

	// 6�� �˻��� 
	if (!bInside)
	{	// ������ �ƴϸ� ��ģ��.
		return cCollision::INTERSECT;	
	}	
	//�����̴�.
	return cCollision::INSIDE;
}


BOOL cCollision::InsideWorldFrustum(Frustum& frustum, D3DXVECTOR3& pos )
{
	BOOL bInside=TRUE;
	// ����� left, right, far plane�� �����Ѵ�.
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
	PLANE_TOP , PLANE_BOTTOM�� �˻���Ѵ�.
*/
cCollision::STATE cCollision::CheckWorldFrustumWithoutYAxis(Frustum& frustum,cSphere& sphere)
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		if ( i== Frustum::PN_TOP || i== Frustum::PN_BOTTOM)
		{
			continue;
		}

		ret=cCollision::IntersectSpherePlane(sphere,frustum.m_plane[i]);				
		if (ret==cCollision::OUTSIDE)	
		{	// �ٱ����̸� ������ �ٱ�
			return cCollision::OUTSIDE;
		}
		else if (ret==cCollision::INTERSECT)
		{	//��ġ�� ������ �ƴϴ�.
			bInside=FALSE;
		}			
	}

	// 6�� �˻��� 
	if (!bInside)
	{	// ������ �ƴϸ� ��ģ��.
		return cCollision::INTERSECT;	
	}
	//�����̴�.
	return cCollision::INSIDE;
}