#include "StdAfx.h"


#include "DebugInfoView.h"
#include "Framework/D3DFramework.h"
#include "Math/CollisionDetector.h"
#include "Math/Plane.h"
#include "Math/Line.h"
#include "CameraNode.h"
#include "Foundation/Define.h"
#include "D3D9Server/Server.h"

cCameraNode*	cCameraNode::m_pActiveCamera=NULL;

cCameraNode::cCameraNode()
{

	//SetNodeType(CAMEARA);
	D3DXMatrixIdentity(&m_matProjection);
	D3DXMatrixIdentity(&m_matView);

	D3DXMatrixIdentity(&m_matViewProjection);
	D3DXMatrixIdentity(&m_matViewProjectionInv);

	m_FOV=0.0f;
	m_Near=0.0f;
	m_Far=0.0f;
	m_ScreenWidth=0.0f;
	m_ScreenHeight=0.0f;

	m_bViewModified=FALSE;
	m_bProjectionModified=FALSE;
	
	m_pWorldFrustumPlane = new cPlane[6];	
}

cCameraNode::~cCameraNode(void)
{
	SAFE_DELETEARRAY(m_pWorldFrustumPlane);
}



/*
	cScene내부에서 ActiveCamera를 얻어 Render를 호출한다.
*/
void cCameraNode::Render()
{	
#ifdef _DEBUG
//	if (m_pActiveCamera!=this)
//	{
//		g_pD3DFramework->GetDebugInfoScene()->AddDebugString("cScene내부에서 ActiveCamera를 얻어 Render를 호출해야한다.\n");
//	}
#endif

	if (m_bViewModified)
	{		
		D3DXMatrixInverse(&m_matView,NULL,&GetWorldTM());		
#if USE_EFFECT
		D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmView,&m_matView);
#else
		m_pD3DDevice->SetTransform (D3DTS_VIEW, &m_matView );		
#endif
	}	
	if ( m_bProjectionModified)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProjection,m_FOV,m_ScreenWidth/m_ScreenHeight,m_Near,m_Far);
#if USE_EFFECT
		D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmProjection,&m_matProjection);
#else
		m_pD3DDevice->SetTransform (D3DTS_PROJECTION, &m_matProjection );
#endif
		
	}		

	//둘중 하나라도 바뀌었다면 월드프러스텀 플랜을 다시만든다.
	if (m_bProjectionModified || m_bViewModified)
	{	
		m_matViewProjection = m_matView * m_matProjection;				
		m_bProjectionModified=FALSE;
		m_bViewModified=FALSE;
#if USE_EFFECT
		D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmViewProjection,&m_matViewProjection);
#endif

#if USE_EFFECT
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();
#endif
		MakeWorldFrustum();	
	}		
}

/*
WorldTM이 
*/
void cCameraNode::Update(DWORD elapseTime)
{

	UpdateMatrix();

	if(m_bModifiedMatWorld)
	{
		m_bViewModified=TRUE;	// 카메라가 월드변환이 바뀟었다. 뷰도 업데이트해야한다.
	}	
	UpdateChildren(elapseTime);
}


void cCameraNode::SetLookAt( const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp )
{
	D3DXMATRIX temp;
//	D3DXMatrixLookAtLH(&temp,pEye,pAt,pUp);
	D3DXVECTOR3 xaxis,yaxis,zaxis,tVector3;

	tVector3=*pAt-*pEye;
	D3DXVec3Normalize(&zaxis,&tVector3);

	D3DXVec3Cross(&tVector3,pUp,&zaxis);
	D3DXVec3Normalize(&xaxis,&tVector3);
	D3DXVec3Cross(&yaxis,&zaxis,&xaxis);

	D3DXMatrixIdentity(&temp);
	// X axis 
	temp._11 = xaxis.x; 
	temp._21 = xaxis.y;
	temp._31 = xaxis.z;

	// Y axis
	temp._12 = yaxis.x;
	temp._22 = yaxis.y;
	temp._32 = yaxis.z;

	// Z axis
	temp._13 = zaxis.x;
	temp._23 = zaxis.y;
	temp._33 = zaxis.z;	

	temp._41 = pEye->x;
	temp._42 = pEye->y;
	temp._43 = pEye->z;

	SetLocalTM(temp);
	m_bViewModified=TRUE;
}

void cCameraNode::SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight)
{	
	m_FOV=fovy;
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_Near=zn;
	m_Far=zf;
	m_bProjectionModified=TRUE;		
}



cPlane& cCameraNode::GetWorldFrustumPlane( int side )
{
	return m_pWorldFrustumPlane[side];
}

void cCameraNode::MakeWorldFrustum()
{	
	D3DXVECTOR3 Normal;
	float dist;	
	
	Normal.x=m_matViewProjection._14 + m_matViewProjection._11;
	Normal.y=m_matViewProjection._24 + m_matViewProjection._21;
	Normal.z=m_matViewProjection._34 + m_matViewProjection._31;
	dist=	 m_matViewProjection._44 + m_matViewProjection._41;
	m_pWorldFrustumPlane[PN_LEFT].Make(Normal,dist);

	Normal.x=m_matViewProjection._14 - m_matViewProjection._11;
	Normal.y=m_matViewProjection._24 - m_matViewProjection._21;
	Normal.z=m_matViewProjection._34 - m_matViewProjection._31;
	dist=	 m_matViewProjection._44 - m_matViewProjection._41;
	m_pWorldFrustumPlane[PN_RIGHT].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 - m_matViewProjection._12;
	Normal.y= m_matViewProjection._24 - m_matViewProjection._22;
	Normal.z= m_matViewProjection._34 - m_matViewProjection._32;
	dist=	  m_matViewProjection._44 - m_matViewProjection._42;
	m_pWorldFrustumPlane[PN_TOP].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 + m_matViewProjection._12;
	Normal.y= m_matViewProjection._24 + m_matViewProjection._22;
	Normal.z= m_matViewProjection._34 + m_matViewProjection._32;
	dist=	  m_matViewProjection._44 + m_matViewProjection._42;
	m_pWorldFrustumPlane[PN_BOTTOM].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 + m_matViewProjection._13;
	Normal.y= m_matViewProjection._24 + m_matViewProjection._23;
	Normal.z= m_matViewProjection._34 + m_matViewProjection._33;
	dist=	  m_matViewProjection._44 + m_matViewProjection._43;
	m_pWorldFrustumPlane[PN_NEAR].Make(Normal,dist);


	Normal.x= m_matViewProjection._14 - m_matViewProjection._13;
	Normal.y= m_matViewProjection._24 - m_matViewProjection._23;
	Normal.z= m_matViewProjection._34 - m_matViewProjection._33;
	dist=	  m_matViewProjection._44 - m_matViewProjection._43;
	m_pWorldFrustumPlane[PN_FAR].Make(Normal,dist);

	for( int i = 0; i < 6; ++i )
		m_pWorldFrustumPlane[i].Normalize();
}

cCollision::STATE cCameraNode::CheckWorldFrustum( cSphere* pSphere )
{
	assert(pSphere!=NULL);
	int ret;
	BOOL bIntersect=false;
	for (int i=0;i<6;i++)
	{	
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[i]);				
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

cCollision::STATE cCameraNode::CheckWorldFrustum( cSphere* pSphere,WORD plane )
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;


	if (plane&PB_TOP)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_TOP]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_BOTTOM)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_BOTTOM]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_LEFT)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_LEFT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_RIGHT)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_RIGHT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_NEAR)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_NEAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// 바깥쪽이면 무조건 바깥
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_FAR)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_FAR]);				
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


BOOL cCameraNode::InsideWorldFrustum( D3DXVECTOR3& pos )
{
	BOOL bInside=TRUE;
	// 현재는 left, right, far plane만 적용한다.
	for(UINT i = 0 ; i < 6 ; i++ )
	{
		if(m_pWorldFrustumPlane[i].GetDistance(pos) < 0.0f )
		{
			bInside=FALSE;
		}
	}
	return bInside;
}
void cCameraNode::SetScreenWidthHeight( float ScreenWidth,float ScreenHeight )
{
	m_ScreenHeight=ScreenHeight;
	m_ScreenWidth=ScreenWidth;
	m_bProjectionModified=TRUE;
}

void cCameraNode::MakeWorldPickingRay( float ScreenX,float ScreenY,cLine& Output )
{
	D3DXVECTOR3 posEnd,posStart;
	float x,y;

	x=2.0f *  ScreenX/m_ScreenWidth - 1.0f;
	y=2.0f * -ScreenY/m_ScreenHeight + 1.0f;

	posStart=D3DXVECTOR3(x,y,0.0f);
	posEnd=D3DXVECTOR3(x,y,1.0f);

	D3DXMATRIX matViewProjectionInv;

	D3DXMatrixInverse(&matViewProjectionInv,NULL,&m_matViewProjection);

	D3DXVec3TransformCoord(&posStart,&posStart,&matViewProjectionInv);

	D3DXVec3TransformCoord(&posEnd,&posEnd,&matViewProjectionInv);	
	Output.Make(posStart,posEnd);
}



void cCameraNode::SetActive()
{
	m_pActiveCamera = this;
}

/*
	PLANE_TOP , PLANE_BOTTOM을 검사안한다.
*/
cCollision::STATE cCameraNode::CheckWorldFrustumWithoutYAxis( cSphere* pSphere )
{
	assert(pSphere!=NULL);
	cCollision::STATE ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		if ( i==PN_TOP || i==PN_BOTTOM)
		{
			continue;
		}

		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[i]);				
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