#include "CameraNode.h"

#include "../Scene/DebugInfoScene.h"
#include "../Framework/App/App.h"
#include "../Math/CollisionDetector.h"
#include "../Math/Plane.h"
#include "../Math/Line.h"
#include "../Foundation/Define.h"
#include <assert.h>


CameraNode*	CameraNode::m_pActiveCamera=NULL;

CameraNode::CameraNode()
{

	SetNodeType(CAMEARA);
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

CameraNode::~CameraNode(void)
{
	SAFE_DELETEARRAY(m_pWorldFrustumPlane);
}

void CameraNode::Init()
{



}

void CameraNode::Uninit()
{

}


/*
	Scene���ο��� ActiveCamera�� ��� Render�� ȣ���Ѵ�.
*/
void CameraNode::Render()
{	
	if (m_bViewModified)
	{		
		D3DXMatrixInverse(&m_matView,NULL,&GetWorldTM());		
		m_SharedLPDIRECT3DDEVICE9->SetTransform (D3DTS_VIEW, &m_matView );				
	}	
	if ( m_bProjectionModified)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProjection,m_FOV,m_ScreenWidth/m_ScreenHeight,m_Near,m_Far);
		m_SharedLPDIRECT3DDEVICE9->SetTransform (D3DTS_PROJECTION, &m_matProjection );
	}		

	//���� �ϳ��� �ٲ���ٸ� ������������ �÷��� �ٽø����.
	if (m_bProjectionModified || m_bViewModified)
	{	
		m_matViewProjection = m_matView * m_matProjection;				
		m_bProjectionModified=FALSE;
		m_bViewModified=FALSE;
		MakeWorldFrustum();		
	}		
}

/*
WorldTM�� 
*/
void CameraNode::Update(DWORD elapseTime)
{

	if(UpdateWorldTM())	
	{
		m_bViewModified=TRUE;	// ī�޶� ���庯ȯ�� �م�����. �䵵 ������Ʈ�ؾ��Ѵ�.
	}	
	UpdateChildren(elapseTime);
}


void CameraNode::SetLookAt( const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp )
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

void CameraNode::SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight)
{	
	m_FOV=fovy;
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_Near=zn;
	m_Far=zf;
	m_bProjectionModified=TRUE;		
}



cPlane& CameraNode::GetWorldFrustumPlane( int side )
{
	return m_pWorldFrustumPlane[side];
}

void CameraNode::MakeWorldFrustum()
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

int CameraNode::CheckWorldFrustum( cSphere* pSphere )
{
	assert(pSphere!=NULL);
	int ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[i]);				
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

int CameraNode::CheckWorldFrustum( cSphere* pSphere,WORD plane )
{
	int ret;
	BOOL bInside=TRUE;


	if (plane&PB_TOP)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_TOP]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_BOTTOM)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_BOTTOM]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_LEFT)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_LEFT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_RIGHT)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_RIGHT]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_NEAR)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_NEAR]);				
		if (ret==cCollision::OUTSIDE)	return cCollision::OUTSIDE;	// �ٱ����̸� ������ �ٱ�
		else if (ret==cCollision::INTERSECT)	bInside=FALSE;				
	}
	if (plane&PB_FAR)
	{
		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[PN_FAR]);				
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


BOOL CameraNode::InsideWorldFrustum( D3DXVECTOR3& pos )
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
void CameraNode::SetScreenWidthHeight( float ScreenWidth,float ScreenHeight )
{
	m_ScreenHeight=ScreenHeight;
	m_ScreenWidth=ScreenWidth;
	m_bProjectionModified=TRUE;
}

void CameraNode::MakeWorldPickingRay( float ScreenX,float ScreenY,cLine& Output )
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



void CameraNode::SetActive()
{
	m_pActiveCamera = this;
}

/*
	PLANE_TOP , PLANE_BOTTOM�� �˻���Ѵ�.
*/
int CameraNode::CheckWorldFrustumWithoutYAxis( cSphere* pSphere )
{
	assert(pSphere!=NULL);
	int ret;
	BOOL bInside=TRUE;
	for (int i=0;i<6;i++)
	{	
		if ( i==PN_TOP || i==PN_BOTTOM)
		{
			continue;
		}

		ret=cCollision::IntersectSpherePlane(*pSphere,m_pWorldFrustumPlane[i]);				
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