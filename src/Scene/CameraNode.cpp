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

	m_bProcessInput = false;
	m_bProjectionModified = false;
	
	m_pWorldFrustumPlane = new cPlane[6];	
}

cCameraNode::~cCameraNode(void)
{
	SAFE_DELETEARRAY(m_pWorldFrustumPlane);
}



/*
	cScene���ο��� ActiveCamera�� ��� Render�� ȣ���Ѵ�.
*/
void cCameraNode::Render()
{			
	D3DXMatrixInverse(&m_matView,NULL,&m_matWorld);		
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmView,&m_matView);
	D3DXVECTOR4 pos( m_matWorld._41,m_matWorld._42,m_matWorld._43,m_matWorld._44);
	D3D9::Server::g_pServer->GetEffect()->SetVector(D3D9::Server::g_pServer->m_hvWorldCameraPosition,&pos);

	if (m_bProjectionModified)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProjection,m_FOV,m_ScreenWidth/m_ScreenHeight,m_Near,m_Far);
		D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmProjection,&m_matProjection);
		m_bProjectionModified = false;
	}		

	m_matViewProjection = m_matView * m_matProjection;				
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmViewProjection,&m_matViewProjection);
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();

	MakeWorldFrustum();	
}

/*
WorldTM�� 
*/
void cCameraNode::Update(DWORD elapseTime)
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix();
	/*
	if(m_bModifiedMatWorld)
	{
		m_bViewModified=TRUE;	// ī�޶� ���庯ȯ�� �م�����. �䵵 ������Ʈ�ؾ��Ѵ�.
	}	
	*/
	UpdateChildren(elapseTime);
}


void cCameraNode::SetLookAt( const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp )
{
	D3DXMATRIX temp;
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

	temp._41 = -D3DXVec3Dot(&xaxis,pEye);
	temp._42 = -D3DXVec3Dot(&yaxis,pEye);
	temp._43 = -D3DXVec3Dot(&zaxis,pEye);

	D3DXMatrixInverse(&m_matLocal,NULL,&temp);
}

void cCameraNode::SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight)
{	
	m_FOV=fovy;
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_Near=zn;
	m_Far=zf;
	m_bProjectionModified = true;
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

cCollision::STATE cCameraNode::CheckWorldFrustum( cSphere& sphere )
{
	int ret;
	BOOL bIntersect=false;
	for (int i=0;i<6;i++)
	{	
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[i]);				
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

cCollision::STATE cCameraNode::CheckWorldFrustum( cSphere& sphere,WORD plane )
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;


	if (plane&PB_TOP)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_TOP]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_BOTTOM)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_BOTTOM]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_LEFT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_LEFT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_RIGHT)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_RIGHT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_NEAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_NEAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_FAR)
	{
		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[PN_FAR]);				
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


BOOL cCameraNode::InsideWorldFrustum( D3DXVECTOR3& pos )
{
	BOOL bInside=TRUE;
	// ����� left, right, far plane�� �����Ѵ�.
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
	m_bProjectionModified = true;
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
	PLANE_TOP , PLANE_BOTTOM�� �˻���Ѵ�.
*/
cCollision::STATE cCameraNode::CheckWorldFrustumWithoutYAxis(cSphere& sphere)
{
	cCollision::STATE ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		if ( i==PN_TOP || i==PN_BOTTOM)
		{
			continue;
		}

		ret=cCollision::IntersectSpherePlane(sphere,m_pWorldFrustumPlane[i]);				
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

void cCameraNode::Control()
{
	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (m_bProcessInput)
	{
		if (m_pWinInput->IsCurrDn('W'))
		{
			vecPos.z = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn('S'))
		{
			vecPos.z = -200.0f;
		}	
		if (m_pWinInput->IsCurrDn('A'))
		{
			vecPos.x = -200.0f;
		}
		else if (m_pWinInput->IsCurrDn('D'))
		{
			vecPos.x = 200.0f;
		}	
		if (m_pWinInput->IsCurrDn('R'))
		{
			vecPos.y = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn('F'))
		{
			vecPos.y = -200.0f;
		}	


		if (m_pWinInput->IsCurrDn('Q'))
		{
			vecRot.y = -45.0f;
		}
		else if (m_pWinInput->IsCurrDn('E'))
		{
			vecRot.y = 45.0f;
		}


	}	

	SetVelocityPosition(vecPos);	
	SetVelocityRotation(vecRot);
}
