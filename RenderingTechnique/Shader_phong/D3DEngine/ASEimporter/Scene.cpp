#include "StdAfx.h"
#include "Scene.h"
#include "../Geometrics/Line.h"
#include "../Geometrics/Sphere.h"

cScene::cScene(void)
{
	m_RootASEObject.SetNodeName("Root");
	m_RootASEObject.GetWorldInverseTM();
	m_MeshRenderType=ALL;
	m_BoundBoxMax=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_BoundBoxMin=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_bCompleteBoundBox=FALSE;
	m_pOwner=NULL;
	m_pBoundSphere=new cSphere;	
}

cScene::~cScene(void)
{
	delete m_pBoundSphere;

	m_listMeshObject.clear();

	for_each(m_listASEObject.begin(),m_listASEObject.end(),FuncDeleteType<cSceneNode*>);
	m_listASEObject.clear();
}

void cScene::InsertSceneNode( cSceneNode* pItem )
{
	pItem->SetRootPtr(&m_RootASEObject);	
	pItem->SetWorldTM(pItem->GetNodeTM());	// ����Ʈ�������� ���� ��Ʈ������ ����
	
	// �ڽ��� �θ� �̸��� ������
	if (pItem->GetParentName().empty())
	{			
		pItem->GetWorldInverseTM();					//������ ����� ���س��´�.
		pItem->SetParentName("Root");				//�θ�� ��Ʈ			
	}	
	pItem->LinkToParent();
	pItem->SetScenePtr(this);						//�ڽ��� ������� ������Ʈ���� �� �����͸� �ִ´�.
	
	
	if (pItem->GetNodeType()==cSceneNode::MESH)
	{
		m_listMeshObject.push_back(pItem);
	}

	m_listASEObject.push_back(pItem);
}

cSceneNode* cScene::FindSceneNode( LPCSTR nodename,int type/*=ANYTHING*/ )
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
			// �ڽ��̸� �ڽ��� �ּ� ����
			if( strNodeName == pItem->GetNodeName() )
			{
				pFoundItem=pItem;
				return pFoundItem;
			}
		}
	}
	return pFoundItem;
}

void cScene::Init()
{
	m_RootASEObject.Init();
}

void cScene::Uninit()
{
	m_RootASEObject.Uninit();
}

void cScene::Update()
{
	m_RootASEObject.Update();
}

void cScene::Render()
{
	m_RootASEObject.Render();
}

D3DXMATRIX&	 cScene::MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ )
{		
	return m_RootASEObject.MovePositionByLocalAxis(deltaX,deltaY,deltaZ);
}

D3DXMATRIX&	 cScene::RotateByLocalAxis( float angleX,float angleY,float angleZ )
{
	return m_RootASEObject.RotateByLocalAxis(angleX,angleY,angleZ);
}

void cScene::SetPositionByLocalAxis( D3DXVECTOR3& pos )
{
	m_RootASEObject.SetPosition(pos);
}

void cScene::MakeBoundSphere()
{
	if (D3DXVec3LengthSq(&D3DXVECTOR3(m_BoundBoxMax-m_BoundBoxMin))!=0)
	{
		
		m_pBoundSphere->Make(m_BoundBoxMin,m_BoundBoxMax);
	}

}

void cScene::SetBoundBoxMin( D3DXVECTOR3& min )
{
	m_BoundBoxMin.x = min(min.x,m_BoundBoxMin.x);
	m_BoundBoxMin.y = min(min.y,m_BoundBoxMin.y);
	m_BoundBoxMin.z = min(min.z,m_BoundBoxMin.z);
}

void cScene::SetBoundBoxMax( D3DXVECTOR3& max )
{
	m_BoundBoxMax.x = min(max.x,m_BoundBoxMax.x);
	m_BoundBoxMax.y = min(max.y,m_BoundBoxMax.y);
	m_BoundBoxMax.z = min(max.z,m_BoundBoxMax.z);
}

void cScene::GetPositionByLocalAxis( D3DXVECTOR3& pos )
{
	m_RootASEObject.GetPosition(pos);
}

cMaterialNode* cScene::GetMatrial( UINT index )
{
	if ( index >= m_arrMatrial.size())
	{
		return NULL;
	}
	return &m_arrMatrial[index];	
}

void cScene::InsertMatrial( cMaterialNode& Matrial )
{
	m_arrMatrial.push_back(Matrial);
}