#include "StdAfx.h"

#include "../Geometrics/CollisionDetector.h"
#include "../Geometrics/Plane.h"
#include "../Geometrics/Line.h"

#include "../ASEimporter/Scene.h"
#include "../Shader.h"
#include "Camera.h"




cCamera::cCamera()
{
	D3DXMatrixIdentity(&m_matRotation);
	D3DXMatrixIdentity(&m_matProjection);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matCamera);
	D3DXMatrixIdentity(&m_matCameraBackup);
	D3DXMatrixIdentity(&m_matViewProjection);
	D3DXMatrixIdentity(&m_matViewProjectionInv);

	m_FOV=0.0f;
	m_Near=0.0f;
	m_Far=0.0f;
	m_ScreenWidth=0.0f;
	m_ScreenHeight=0.0f;

	m_bModifiedCamera=TRUE;
	m_bModifiedProjection=TRUE;
	m_bValidViewProjectionInv=FALSE;
	m_NumOutSide=0;	

	m_pAttachedModel=NULL;
	m_pWorldFrustumPlane=NULL;
}

cCamera::~cCamera(void)
{
	SAFE_DELETEARRAY(m_pWorldFrustumPlane);
}

void cCamera::Init()
{
	D3DXMatrixIdentity(&m_matProjection);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matRotation);
	D3DXMatrixIdentity(&m_matCamera);
	m_FOV=0;
	m_ScreenWidth=0;
	m_ScreenHeight=0;
	m_Near=0;
	m_Far=0;
	m_bModifiedCamera=TRUE;
	m_bModifiedProjection=TRUE;
	
	m_pWorldFrustumPlane = new cPlane[6];
}

void cCamera::Uninit()
{

}

void cCamera::Render()
{
	m_NumOutSide=0;
	if (m_pAttachedModel!=NULL)
	{
		m_matCamera=m_pAttachedModel->GetRootASEObject().GetWorldTM();
//		MoveOnLocalAxis(0.0f,15.0f,9.0f);
		MoveOnLocalAxis(0.0f,50.0f,-950.f);
		m_bModifiedCamera=TRUE;
	}
	


	if (m_bModifiedCamera)
	{	
		D3DXVECTOR3 axisX(m_matCamera._11,m_matCamera._12,m_matCamera._13);
		D3DXVECTOR3 axisY(m_matCamera._21,m_matCamera._22,m_matCamera._23);
		D3DXVECTOR3	axisZ(m_matCamera._31,m_matCamera._32,m_matCamera._33);
		D3DXVECTOR3 pos(m_matCamera._41,m_matCamera._42,m_matCamera._43);

		// View Matrix �����
		// X axis 
		m_matView._11 = axisX.x; 
		m_matView._21 = axisX.y; 
		m_matView._31 = axisX.z; 

		// Y axis
		m_matView._12 = axisY.x;
		m_matView._22 = axisY.y;
		m_matView._32 = axisY.z;

		// Z axis
		m_matView._13 = axisZ.x;
		m_matView._23 = axisZ.y;
		m_matView._33 = axisZ.z;

		m_matView._14 = m_matView._24 = m_matView._34 = 0.0f;
		m_matView._41 = -D3DXVec3Dot(&axisX,&pos);
		m_matView._42 = -D3DXVec3Dot(&axisY,&pos);
		m_matView._43 = -D3DXVec3Dot(&axisZ,&pos);

		m_matView._44 = 1.0f;

		
		m_pD3DDevice9->SetTransform (D3DTS_VIEW, &m_matView );
// 		D3DXMATRIX temp;
// 		D3DXMatrixTranspose(&temp,&m_matView);
// 		g_pD3DDevice9->SetVertexShaderConstantF( 4, (float*) &temp,4);	


		m_pGraphics->AddDebugString(_T("Camera\n"));
		m_pGraphics->AddDebugString(_T("C x axis: %3.3f,%3.3f,%3.3f\n"),m_matCamera._11,m_matCamera._21,m_matCamera._31);
		m_pGraphics->AddDebugString(_T("C y axis: %3.3f,%3.3f,%3.3f\n"),m_matCamera._12,m_matCamera._22,m_matCamera._32);
		m_pGraphics->AddDebugString(_T("C z axis: %3.3f,%3.3f,%3.3f\n"),m_matCamera._13,m_matCamera._23,m_matCamera._33);
		m_pGraphics->AddDebugString(_T("C pos: %3.3f,%3.3f,%3.3f\n"),m_matCamera._41,m_matCamera._42,m_matCamera._43);

		m_pGraphics->AddDebugString(_T("\n"));
		m_pGraphics->AddDebugString(_T("VMat : %3.3f,%3.3f,%3.3f,%3.3f\n"),m_matView._11,m_matView._12,m_matView._13,m_matView._14);
		m_pGraphics->AddDebugString(_T("VMat : %3.3f,%3.3f,%3.3f,%3.3f\n"),m_matView._21,m_matView._22,m_matView._23,m_matView._24);
		m_pGraphics->AddDebugString(_T("VMat : %3.3f,%3.3f,%3.3f,%3.3f\n"),m_matView._31,m_matView._32,m_matView._33,m_matView._34);
		m_pGraphics->AddDebugString(_T("VMat : %3.3f,%3.3f,%3.3f,%3.3f\n"),m_matView._41,m_matView._42,m_matView._43,m_matView._44);

		D3DXVECTOR4 cpos=D3DXVECTOR4(m_matCamera._41,m_matCamera._42,m_matCamera._43,1.0f);
		g_pD3DDevice9->SetVertexShaderConstantF( 3, (float*)&cpos,1);	
		
	}	
	if ( m_bModifiedProjection)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProjection,m_FOV,m_ScreenWidth/m_ScreenHeight,m_Near,m_Far);
		m_pD3DDevice9->SetTransform (D3DTS_PROJECTION, &m_matProjection );
// 		D3DXMATRIX temp;
// 		D3DXMatrixTranspose(&temp,&m_matProjection);
// 		g_pD3DDevice9->SetVertexShaderConstantF( 8, (float*) &temp,4);	
	}		
	
	D3DXPLANE	test[6];
	//���� �ϳ��� �ٲ���ٸ� ������������ �÷��� �ٽø����.
	if (m_bModifiedProjection || m_bModifiedCamera)
	{	
		m_matViewProjection = m_matView * m_matProjection;		
		m_bValidViewProjectionInv=FALSE;
		m_bModifiedProjection=FALSE;
		m_bModifiedCamera=FALSE;

		D3DXMATRIX temp;
		D3DXMatrixTranspose(&temp,&m_matViewProjection);
		g_pD3DDevice9->SetVertexShaderConstantF( 8, (float*) &temp,4);	
		MakeWorldFrustum();
		
	}
	m_pGraphics->AddDebugString(_T("ī�޶��̵� wasd qe rf"));
}

void cCamera::Update()
{
	m_NumOutSide=0;	
}

void cCamera::MoveOnLocalAxis( float deltaX,float deltaY,float deltaZ )
{		
	if (deltaX!=0.0f)
	{
		m_matCamera._41 +=  m_matCamera._11 * deltaX; 
		m_matCamera._42 +=  m_matCamera._12 * deltaX; 
		m_matCamera._43 +=  m_matCamera._13 * deltaX;
	}
	if (deltaY!=0.0f)
	{
		m_matCamera._41 +=  m_matCamera._21 * deltaY; 
		m_matCamera._42 +=  m_matCamera._22 * deltaY; 
		m_matCamera._43 +=  m_matCamera._23 * deltaY;
	}
	if (deltaZ!=0.0f)
	{
		m_matCamera._41 +=  m_matCamera._31 * deltaZ; 
		m_matCamera._42 +=  m_matCamera._32 * deltaZ; 
		m_matCamera._43 +=  m_matCamera._33 * deltaZ;
	}
	m_bModifiedCamera=TRUE;
}



void cCamera::RotateByLocalAxis( float angleX,float angleY,float angleZ )
{	

	D3DXMATRIX tMatrix;

	D3DXMatrixRotationYawPitchRoll(&tMatrix,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
//	D3DXQUATERNION temp;
//	D3DXQuaternionRotationYawPitchRoll(&temp,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
//	D3DXMatrixRotationQuaternion(&tMatrix,&temp);

	m_matCamera =  tMatrix * m_matCamera;
	m_bModifiedCamera=TRUE;
}


void cCamera::SetLookAt( const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp )
{
	D3DXVECTOR3 xaxis,yaxis,zaxis,tVector3;

	tVector3=*pAt-*pEye;
	D3DXVec3Normalize(&zaxis,&tVector3);

	D3DXVec3Cross(&tVector3,pUp,&zaxis);
	D3DXVec3Normalize(&xaxis,&tVector3);
	D3DXVec3Cross(&yaxis,&zaxis,&xaxis);

	D3DXMatrixIdentity(&m_matCamera);
	// X axis 
	m_matCamera._11 = xaxis.x; 
	m_matCamera._21 = xaxis.y;
	m_matCamera._31 = xaxis.z;

	// Y axis
	m_matCamera._12 = yaxis.x;
	m_matCamera._22 = yaxis.y;
	m_matCamera._32 = yaxis.z;

	// Z axis
	m_matCamera._13 = zaxis.x;
	m_matCamera._23 = zaxis.y;
	m_matCamera._33 = zaxis.z;	

	m_matCamera._41 = pEye->x;
	m_matCamera._42 = pEye->y;
	m_matCamera._43 = pEye->z;

	m_bModifiedCamera=TRUE;
}

void cCamera::SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight)
{	
	m_FOV=fovy;
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_Near=zn;
	m_Far=zf;
	m_bModifiedProjection=TRUE;
}

void cCamera::AttachToModel( cScene* pModel )
{
	assert(pModel!=NULL);
	if (m_pAttachedModel==NULL)
	{
		m_matCameraBackup=m_matCamera;		
	}
	m_pAttachedModel=pModel; 	

	m_bModifiedCamera=TRUE;
}

void cCamera::DettachToModel()
{
	m_pAttachedModel=NULL;
	m_matCamera=m_matCameraBackup;
	m_bModifiedCamera=TRUE;
}

BOOL cCamera::IsAttached()
{
	if (m_pAttachedModel!=NULL)
	{
		return TRUE;
	}
	else return FALSE;
}

cPlane& cCamera::GetWorldFrustumPlane( int side )
{
	return m_pWorldFrustumPlane[side];
}

void cCamera::MakeWorldFrustum()
{	

	D3DXVECTOR3 Normal;
	float dist;	

	Normal.x=m_matViewProjection._14 + m_matViewProjection._11;
	Normal.y=m_matViewProjection._24 + m_matViewProjection._21;
	Normal.z=m_matViewProjection._34 + m_matViewProjection._31;
	dist=	 m_matViewProjection._44 + m_matViewProjection._41;
	m_pWorldFrustumPlane[PLANE_LEFT].Make(Normal,dist);

	Normal.x=m_matViewProjection._14 - m_matViewProjection._11;
	Normal.y=m_matViewProjection._24 - m_matViewProjection._21;
	Normal.z=m_matViewProjection._34 - m_matViewProjection._31;
	dist=	 m_matViewProjection._44 - m_matViewProjection._41;
	m_pWorldFrustumPlane[PLANE_RIGHT].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 - m_matViewProjection._12;
	Normal.y= m_matViewProjection._24 - m_matViewProjection._22;
	Normal.z= m_matViewProjection._34 - m_matViewProjection._32;
	dist=	  m_matViewProjection._44 - m_matViewProjection._42;
	m_pWorldFrustumPlane[PLANE_TOP].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 + m_matViewProjection._12;
	Normal.y= m_matViewProjection._24 + m_matViewProjection._22;
	Normal.z= m_matViewProjection._34 + m_matViewProjection._32;
	dist=	  m_matViewProjection._44 + m_matViewProjection._42;
	m_pWorldFrustumPlane[PLANE_BOTTOM].Make(Normal,dist);

	Normal.x= m_matViewProjection._14 + m_matViewProjection._13;
	Normal.y= m_matViewProjection._24 + m_matViewProjection._23;
	Normal.z= m_matViewProjection._34 + m_matViewProjection._33;
	dist=	  m_matViewProjection._44 + m_matViewProjection._43;
	m_pWorldFrustumPlane[PLANE_NEAR].Make(Normal,dist);


	Normal.x= m_matViewProjection._14 - m_matViewProjection._13;
	Normal.y= m_matViewProjection._24 - m_matViewProjection._23;
	Normal.z= m_matViewProjection._34 - m_matViewProjection._33;
	dist=	  m_matViewProjection._44 - m_matViewProjection._43;
	m_pWorldFrustumPlane[PLANE_FAR].Make(Normal,dist);

	for( int i = 0; i < 6; ++i )
		m_pWorldFrustumPlane[i].Normalize();
}

BOOL cCamera::CullWorldFrustum( cSphere& Sphere )
{
	for (int i=0;i<6;i++)
	{
		if (cCollision::IntersectSpherePlane(Sphere,GetWorldFrustumPlane(i))==cCollision::OUTSIDE)
		{
			m_NumOutSide++;
			return FALSE;
		}		
	}
	return TRUE;
}

void cCamera::SetScreenWidthHeight( float ScreenWidth,float ScreenHeight )
{
	m_ScreenHeight=ScreenHeight;
	m_ScreenWidth=ScreenWidth;
	m_bModifiedProjection=TRUE;

}

void cCamera::MakeWorldPickingRay( float ScreenX,float ScreenY,cLine& Output )
{
	D3DXVECTOR3 posEnd,posStart;
	float x,y;
	
	x=2.0f *  ScreenX/m_ScreenWidth - 1.0f;
	y=2.0f * -ScreenY/m_ScreenHeight + 1.0f;

	posStart=D3DXVECTOR3(x,y,0.0f);
	posEnd=D3DXVECTOR3(x,y,1.0f);
	
	if (!m_bValidViewProjectionInv)
	{
		D3DXMatrixInverse(&m_matViewProjectionInv,NULL,&m_matViewProjection);
		m_bValidViewProjectionInv=TRUE;
	}
	D3DXVec3TransformCoord(&posStart,&posStart,&m_matViewProjectionInv);
	D3DXVec3TransformCoord(&posEnd,&posEnd,&m_matViewProjectionInv);	
	Output.Make(posStart,posEnd);
}

