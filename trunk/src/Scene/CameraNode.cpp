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
}

cCameraNode::~cCameraNode(void)
{

}



/*
	cScene내부에서 ActiveCamera를 얻어 Render를 호출한다.
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

	m_frustum.Make(m_matViewProjection);
}

/*
WorldTM이 
*/



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



void cCameraNode::Control()
{
	D3DXVECTOR3 vecPos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vecRot(0.0f,0.0f,0.0f);
	if (m_bProcessInput)
	{
		
		if (m_pWinInput->IsCurrDn(DIK_W))
		{
			vecPos.z = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn(DIK_S))
		{
			vecPos.z = -200.0f;
		}	
		if (m_pWinInput->IsCurrDn(DIK_A))
		{
			vecPos.x = -200.0f;
		}
		else if (m_pWinInput->IsCurrDn(DIK_D))
		{
			vecPos.x = 200.0f;
		}	
		if (m_pWinInput->IsCurrDn(DIK_R))
		{
			vecPos.y = 200.0f;
		}
		else if (m_pWinInput->IsCurrDn(DIK_F))
		{
			vecPos.y = -200.0f;
		}	
		
		if(m_pWinInput->Mouse_IsCurrDn(0))
		{
			LONG deltaX,deltaY;

			m_pWinInput->GetMouseDelta(deltaX,deltaY);

			if ( deltaX > 0)
			{
				vecRot.y = 10.0f * float(deltaX);
			}
			else if ( deltaX < 0)
			{
				vecRot.y = 10.0f * deltaX ;
			}

			if ( deltaY > 0)
			{
				vecRot.x = 10.0f * deltaY;
			}
			else if ( deltaY < 0)
			{
				vecRot.x = 10.0f * deltaY;
			}
		}		
	}	

	SetVelocityPosition(vecPos);	
	SetVelocityRotation(vecRot);
}
