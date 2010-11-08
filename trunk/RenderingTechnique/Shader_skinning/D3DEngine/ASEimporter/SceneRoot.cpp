#include "StdAfx.h"
#include "SceneRoot.h"
#include "../Geometrics/Line.h"
#include "../Geometrics/Sphere.h"

cSceneRoot::cSceneRoot(void)
{
	SetNodeName(L"Root");
	GetWorldInverseTM();
	
	m_BoundBoxMax=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_BoundBoxMin=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_bCompleteBoundBox=FALSE;
	m_pOwner=NULL;
	m_pBoundSphere=new cSphere;	
}

cSceneRoot::~cSceneRoot(void)
{
	delete m_pBoundSphere;
	FreeChildren();
}

/*
void cSceneRoot::InsertSceneNode( cSceneNode* pItem )
{
	pItem->SetRoot(this);	
	// 부모 이름이 없으면
	if (pItem->GetParentName().empty())
	{			
		pItem->SetParentName("Root");				//부모는 루트				
	}	
	pItem->LinkToParent();
	
	
	if (pItem->GetNodeType()==cSceneNode::MESH)
	{
		m_listMeshObject.push_back(pItem);
	}

	m_listASEObject.push_back(pItem);
}*/


/*
cSceneNode* cSceneRoot::FindSceneNode( LPCSTR nodename,int type/ *=ANYTHING* / )
{	
	string strNodeName;
	strNodeName=nodename;
	cSceneNode* pFoundItem=NULL;
	cSceneNode* pItem=NULL;
	list<cSceneNode*>::iterator it;
	for ( it= m_listASEObject.begin() ; it!= m_listASEObject.end() ; ++it)
	{
		pItem=*it;		
		
		if ( (type==cSceneNode::ANYTHING)||(type==pItem->GetNodeType()) )
		{
			// 자신이면 자신의 주소 리턴
			if( strNodeName == pItem->GetNodeName() )
			{
				pFoundItem=pItem;
				return pFoundItem;
			}
		}
	}
	return pFoundItem;
}
*/

void cSceneRoot::Init()
{
	//로컬 매트릭스 초기화	
	SetTotalTransformAnm(InterpolateTransformAnm(DEF_INTERPOLATE_TIME));

	InitChildren();	
}

void cSceneRoot::Uninit()
{
	UninitChildren();
}

void cSceneRoot::Update(DWORD elapseTime)
{
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm( elapseTime);
		m_LocalTM = GetAnimationTM();
	}
	if (IsParentObject())	
	{		
		m_WorldTM = m_LocalTM;	
	}
	else
	{			
		m_WorldTM = m_LocalTM * GetParentPtr()->GetWorldTM();
	}	
	UpdateChildren(elapseTime);
}

void cSceneRoot::Render()
{
	RenderChildren();
}




void cSceneRoot::MakeBoundSphere()
{
	if (D3DXVec3LengthSq(&D3DXVECTOR3(m_BoundBoxMax-m_BoundBoxMin))!=0)
	{
		
		m_pBoundSphere->Make(m_BoundBoxMin,m_BoundBoxMax);
	}

}

void cSceneRoot::SetBoundBoxMin( D3DXVECTOR3& min )
{
	m_BoundBoxMin.x = min(min.x,m_BoundBoxMin.x);
	m_BoundBoxMin.y = min(min.y,m_BoundBoxMin.y);
	m_BoundBoxMin.z = min(min.z,m_BoundBoxMin.z);
}

void cSceneRoot::SetBoundBoxMax( D3DXVECTOR3& max )
{
	m_BoundBoxMax.x = min(max.x,m_BoundBoxMax.x);
	m_BoundBoxMax.y = min(max.y,m_BoundBoxMax.y);
	m_BoundBoxMax.z = min(max.z,m_BoundBoxMax.z);
}



cMaterialNode* cSceneRoot::GetMatrial( UINT index )
{
	if ( index >= m_arrMatrial.size())
	{
		return NULL;
	}
	return &m_arrMatrial[index];	
}

void cSceneRoot::InsertMatrial( cMaterialNode& Matrial )
{
	m_arrMatrial.push_back(Matrial);
}