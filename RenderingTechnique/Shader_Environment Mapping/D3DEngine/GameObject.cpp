#include "StdAfx.h"
#include "GameObject.h"
#include "./ASEImporter/Scene.h"
#include "./ASEImporter/ASEParser.h"

#include "./Geometrics/Line.h"
#include "./Geometrics/Sphere.h"
#include "./Geometrics/Plane.h"


cGameObject::cGameObject(cWorld* pWorld)
:m_pWorld(pWorld)
{
	D3DXMatrixIdentity(&m_WorldTM);			//로컬 축, 위치정보
	D3DXMatrixIdentity(&m_ScaleTM);
	D3DXMatrixIdentity(&m_RotationTM);
	D3DXMatrixIdentity(&m_TranslationTM);


	m_Position=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_PositionOld=D3DXVECTOR3(0.0f,0.0f,0.0f);;		// 이전 위치

	m_Speed=0.0f;;			// 속력
	m_SpeedOld=0.0f;;			

	m_Forward=D3DXVECTOR3(0.0f,0.0f,0.0f);;			// 전방
	m_ForwardOld=D3DXVECTOR3(0.0f,0.0f,0.0f);;		

	m_pTarget=NULL;
	

	//부가정보(존재하지않을수도있음)
	m_pScene=NULL;			//모델 정보	
	m_pSphere=NULL;			//구 정보 
	m_pPlane=NULL;			//평면 정보

}

cGameObject::~cGameObject(void)
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pSphere);
	SAFE_DELETE(m_pPlane);
}

void cGameObject::Init()
{

}

void cGameObject::Uninit()
{

}

void cGameObject::Update()
{

}

void cGameObject::Render()
{

}

BOOL cGameObject::LoadModel( LPCTSTR strFileName )
{
	m_pScene=new cScene;
	m_pScene->SetOwner(this);
	cASEParser Impoter;
	return Impoter.Load(strFileName,&(*m_pScene));
}

void cGameObject::SetPositionOld( D3DXVECTOR3& val )
{
	m_PositionOld = val;	
}

D3DXVECTOR3& cGameObject::GetPositionOld()
{
	return m_PositionOld;
}

void cGameObject::SetPosition( D3DXVECTOR3& val )
{
	if (m_pScene!=NULL)
	{
		m_pScene->SetPositionByLocalAxis(val);
	}
	else
	{
		m_WorldTM._41 =  val.x; 
		m_WorldTM._42 =  val.y; 
		m_WorldTM._43 =  val.z; 			
	}
	m_Position = val;
}

D3DXVECTOR3& cGameObject::GetPosition()
{
	if (m_pScene!=NULL)
	{
		m_pScene->GetPositionByLocalAxis(m_Position);
	}
	else
	{
		m_Position.x = m_WorldTM._41;
		m_Position.y = m_WorldTM._42;
		m_Position.z = m_WorldTM._43;
	}
	return m_Position;
}

void cGameObject::OnCollide( cGameObject* pObj )
{

}

void cGameObject::OnCollide( cGameObject* pObj,cMeshNode* pMyMesh )
{

}

void cGameObject::OnPicked( cMeshNode* pPickedMesh )
{

}


D3DXMATRIX& cGameObject::MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ )
{
	if (m_pScene!=NULL)
	{
		m_TranslationTM=m_pScene->MovePositionByLocalAxis(deltaX,deltaY,deltaZ);
	}
	else
	{
		D3DXMatrixTranslation(&m_TranslationTM,deltaX,deltaY,deltaZ);
		m_WorldTM =  m_TranslationTM * m_WorldTM;
	}
	return m_TranslationTM;
}

D3DXMATRIX& cGameObject::RotateByLocalAxis( float angleX,float angleY,float angleZ )
{
	if (m_pScene!=NULL)
	{
		m_RotationTM=m_pScene->RotateByLocalAxis(angleX,angleY,angleZ);
	}
	else
	{
		D3DXMATRIX tMatrix;
		D3DXMatrixRotationYawPitchRoll(&m_RotationTM,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
		m_WorldTM =  m_RotationTM * m_WorldTM;		
	}
	D3DXVec3TransformCoord(&m_Forward,&m_Forward,&m_RotationTM);
	return m_RotationTM;	
}

void cGameObject::CreatePlane()
{
	if (m_pPlane==NULL)
	{
		m_pPlane = new cPlane;
	}	
}

void cGameObject::DestroyPlane()
{
	SAFE_DELETE(m_pPlane);
}

void cGameObject::CreateSphere()
{
	if (m_pSphere==NULL)
	{
		m_pSphere = new cSphere;
	}
}

void cGameObject::DestroySphere()
{
	SAFE_DELETE(m_pSphere);
}