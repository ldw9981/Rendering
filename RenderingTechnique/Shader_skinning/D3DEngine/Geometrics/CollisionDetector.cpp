#include "StdAfx.h"
#include "CollisionDetector.h"
#include "Line.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

#include "../Log/Trace.h"
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
	
	vecLineStartToSphereCenter= TargetSphere.GetCenter() - Line.GetStart();
	
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
		CrossNormal = TargetSphere.GetCenter() - CrossPos;
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
	float LengthSq=D3DXVec3LengthSq( &D3DXVECTOR3(SphereB.GetCenter() - SphereA.GetCenter()));

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
		LineA.Make(SphereA.GetCenter(),SphereA.GetCenter()+vecA);
		LineB.Make(SphereB.GetCenter(),SphereB.GetCenter()+vecB);

		LineA.GetDirection();

	}
	else if (LengthSqA==0.0f)
	{
		LineB.Make(SphereB.GetCenter(),SphereB.GetCenter()+vecB);
	}
	else
	{ 
		LineA.Make(SphereA.GetCenter(),SphereA.GetCenter()+vecA);
	}

	

//	IntersectLineSphere()

	return TRUE;
}
int cCollision::IntersectSpherePlane( cSphere& Sphere,cPlane& TargetPlane,float *pDist)
{
	D3DXVECTOR3 center=Sphere.GetCenter();
	float lengthCenterToPlane,radius;
	lengthCenterToPlane= TargetPlane.a*center.x + TargetPlane.b*center.y + TargetPlane.c*center.z + TargetPlane.d;
	radius= Sphere.GetRadius();
	

	//��� ���ʿ� ��ġ
	if( lengthCenterToPlane > radius )
	{				
		if (pDist!=NULL)	*pDist= lengthCenterToPlane - radius;
		return INSIDE;
	}

	//��� ���ʿ� ��ġ
	if( lengthCenterToPlane < -radius )
	{				
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

int cCollision::IntersectAABBPlane( cAABB& AABB,cPlane& Plane )
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
