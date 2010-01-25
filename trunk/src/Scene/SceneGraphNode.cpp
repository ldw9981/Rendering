
#include "SceneGraphNode.h"
#include "../ASEParser/ASEParser.h"
#include "../Foundation/Trace.h"
#include "../Foundation/Define.h"
#include "../Framework/App/App.h"
#include "../Math/Sphere.h"
#include "../Math/CollisionDetector.h"
#include "MeshNode.h"
#include "CameraNode.h"
#include "DebugInfoScene.h"
#include "ZQuadTree.h"
#include "CameraNode.h"
#include "RendererQueue.h"


#include <algorithm>

using namespace std;
using namespace Application;

// ��� ��� �̸��� �ʱ⿡ ��Ʈ�� �Ǹ� �θ�� ����.
SceneGraphNode::SceneGraphNode(void)
:m_strNodeName(L"Root"),m_strParentName(L"")
{	
	m_pRootNode=this;
	m_pParentNode=NULL;
	m_NodeType=ROOT;	
	m_TotalTransformAnm=0;
	m_IndexTransformAnm=0;
	m_AccumTime=0;
	m_bIsActiveAnimation=FALSE;
	m_pRscTransformAnm=NULL;		
	m_pCullingSphere=NULL;
	m_pBoundingSphere=NULL;

}

SceneGraphNode::~SceneGraphNode(void)
{
	SAFE_DELETE(m_pCullingSphere);

	if( m_pRscTransformAnm !=NULL )
		m_pRscTransformAnm->Release();

	for_each(m_listChildNode.begin(),m_listChildNode.end(),FuncDeleteType<SceneGraphNode*>);
	m_listChildNode.clear();
}

void SceneGraphNode::SetNodeName( LPCWSTR nodename )
{
	m_strNodeName=nodename;
}

wstring& SceneGraphNode::GetNodeName()
{
	return m_strNodeName;
}

void SceneGraphNode::SetParentName( LPCWSTR nodename )
{
	m_strParentName=nodename;
}

wstring& SceneGraphNode::GetParentName()
{
	return m_strParentName;
}



BOOL SceneGraphNode::IsExistParentName()
{
	if (m_strParentName.empty())
	{
		return FALSE;
	}
	return TRUE;
}

void SceneGraphNode::AttachChildNode( SceneGraphNode* pItem )
{
	m_listChildNode.push_back(pItem);
	pItem->m_ParentListIt = --m_listChildNode.end();
}

SceneGraphNode* SceneGraphNode::FindNode( wstring& nodename,int type/*=ANYTHING*/ )
{
	if ( (type==ANYTHING)||(type==m_NodeType) )
	{
		// �ڽ��̸� �ڽ��� �ּ� ����
		if( nodename.compare(m_strNodeName)==0)
			return this;
	}

	// �ƴϸ� �ڽĿ��� �˻�
	list<SceneGraphNode*>::iterator iter;	
	SceneGraphNode* pItem=NULL;
	SceneGraphNode* pFoundItem=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pFoundItem=pItem->FindNode(nodename);
		if (pFoundItem!=NULL)
		{
			return pFoundItem;
		}			 
	}	
	return NULL;
}

BOOL SceneGraphNode::IsRootNode()
{
	if (m_pParentNode!=NULL)
	{		
		return FALSE;
	}
	return TRUE;
}

D3DXMATRIX* SceneGraphNode::UpdateTransformAnm(DWORD elapseTime)
{	
	if ( m_pRscTransformAnm==NULL)
		return NULL;

	if (!GetIsActiveAnimation())
		return NULL;

	m_AccumTime +=  elapseTime ;	
	m_AnimationTM = m_pRscTransformAnm->GetTransformAnm(m_IndexTransformAnm)->TM ;				

	//���� AnmTick���� ���ų�ũ�� �ε�������
	DWORD IndexTime = m_pRscTransformAnm->GetTransformAnm(m_IndexTransformAnm)->anmTime ;		
	DWORD CurrTime =  m_AccumTime;

	//���� �ε����� AnmTick�� ���ų� ũ��
	if (CurrTime >= IndexTime)
	{			
		do 
		{	// �ε����� ������Ű��
			m_IndexTransformAnm++;
			if (m_IndexTransformAnm >= m_TotalTransformAnm )
			{
				m_IndexTransformAnm= m_TotalTransformAnm-1;
				if (m_AccumTime >= m_pRscTransformAnm->GetTransformAnm(m_IndexTransformAnm)->anmTime)
				{		
					m_IndexTransformAnm=0;
					m_AccumTime=0;			
					CurrTime =  m_AccumTime;					
				}		
				break;
			}	
			// �ε��� AnmTick�� ��� �ٽú��Ѵ�.
			IndexTime = m_pRscTransformAnm->GetTransformAnm(m_IndexTransformAnm)->anmTime;							
		} while(CurrTime > IndexTime);
	}
	return &m_AnimationTM;
}

void SceneGraphNode::UpdateChildren(DWORD elapseTime)
{
	list<SceneGraphNode*>::iterator iter;	
	SceneGraphNode* pItem=NULL;
	SceneGraphNode* pFoundItem=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Update(elapseTime);
	}	
}

void SceneGraphNode::InitChildren()
{
	list<SceneGraphNode*>::iterator iter;	
	SceneGraphNode* pItem=NULL;	
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Init();
	}	
}

void SceneGraphNode::UninitChildren()
{
	list<SceneGraphNode*>::iterator iter;	
	SceneGraphNode* pItem=NULL;	
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Uninit();
	}	
}

// �θ��带 ã�Ƽ� �ڽ����� �ִ´�.
BOOL SceneGraphNode::LinkToParent()
{
	//��Ʈ ���� �ݵ�� �־���Ѵ�.
	ASSERT(m_pRootNode!=NULL);

	SceneGraphNode* pFoundObject=NULL;
	if (!IsExistParentName())	//�θ� ������ �θ�� ��Ʈ�� ���Ѵ�.
	{		
		SetParentName(L"Root");
	}

	pFoundObject=m_pRootNode->FindNode(GetParentName());
	if (pFoundObject!=NULL)
	{
		pFoundObject->AttachChildNode(this);
		//�θ��ּҸ� �ִ´�.
		SetParentNode(pFoundObject);
	}			
	else
	{
		ASSERT(pFoundObject!=NULL);
		return FALSE;
	}	
	

	SetWorldTM(GetNodeTM());	

	// Root�� ������ ������ ChildObject �̱��ϴ�.
	if (IsRootNode())
	{
		SetLocalTM( GetWorldTM() );
	}
	else
	{	// m_LocalTM �� �θ������ ��� ��ȯ���� �����Ѵ�. 
		SetLocalTM(	GetWorldTM() * GetParentNode()->GetWorldInverseTM() );	
	}		
	return TRUE;
}





D3DXMATRIX& SceneGraphNode::GetAnimationTM()
{
	return m_AnimationTM;
}


void SceneGraphNode::FreeChildren()
{
	list<SceneGraphNode*>::iterator iter;	
	SceneGraphNode* pItem=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->FreeChildren();
	}	
	
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		delete pItem;
		
	}	

}

void SceneGraphNode::SetRscTransformAnm( TransformAnm* val )
{
	m_pRscTransformAnm = val; 
	if (val!=NULL)
	{
		m_pRscTransformAnm->AddRef();
		m_TotalTransformAnm = static_cast<UINT>(m_pRscTransformAnm->GetArrayTransformAnm().size());
		m_bIsActiveAnimation = TRUE;
	}
}


SceneGraphNode& SceneGraphNode::operator =(const SceneGraphNode& other)
{
	assert(0);
	return *this;
}

void SceneGraphNode::DettachChildNode( SceneGraphNode* pItem )
{
	m_listChildNode.erase(pItem->m_ParentListIt);
}


cSphere* SceneGraphNode::CreateCullingSphere()
{
	SAFE_DELETE(m_pCullingSphere);
	m_pCullingSphere = new cSphere;
	return m_pCullingSphere;
}

/*
�θ� �߿� BS�� ��带 ã�� �ڽ��� �����ϴ� BS�� �����Ѵ�. 
*/
void SceneGraphNode::UpdateParentCullingSphere(cSphere& Sphere)
{
	SceneGraphNode* pNode=this;
	do 
	{
		pNode = pNode->GetParentNode();
		if (pNode==NULL)
			return;

	} while(pNode->GetCullingSphere()==NULL);

	pNode->GetCullingSphere()->Merge(Sphere);	
}

cSphere* SceneGraphNode::CreateBoundingSphere()
{
	assert(m_pBoundingSphere==NULL);
	m_pBoundingSphere = new cSphere;
	return m_pBoundingSphere;
}


void SceneGraphNode::CullRendererTraversal( cRendererQueue* pRendererQueue,CameraNode* pActiveCamera )
{
	Framework::IRenderer* pIRenderer=dynamic_cast<Framework::IRenderer*>(this);
	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{
			// �ڽĳ�忡���� ���ŵǴ� �ø����� Ȱ��ȭ�� ī�޶� ����ü�� ���������Ȯ�� Ȯ��
			int ret=pActiveCamera->CheckWorldFrustum(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  �ۿ� �ִ°��̸� ����ȸ ����
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere OUTSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" CullingSphere INTERSECTION\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}
		
		// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustum(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE�� ť�� �ִ´�.
			{	
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere INSIDE or INTERSECTION\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
				pRendererQueue->Insert(pIRenderer);						
			}
			else
			{
#ifdef _DEBUG
				wstring temp=GetNodeName();
				temp += L" BoundingSphere OUTSIDE\n";
				App::Instance()->GetDebugInfoScene()->AddDebugString(temp.c_str());
#endif	
			}
		}
	}	

	list<SceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}


/*
	��ȸ���ϸ鼭 cIRenderer�� ť�� �ִ´�. 
*/
void SceneGraphNode::InsertToRendererQueueTraversal( cRendererQueue* pRendererQueue,CameraNode* pActiveCamera,WORD testPlane/*=0*/ )
{
	Framework::IRenderer* pIRenderer=dynamic_cast<Framework::IRenderer*>(this);
	if (pIRenderer!=NULL)
	{
		if (testPlane!=0 && m_pBoundingSphere!=NULL)
		{
			if( cCollision::INSIDE == pActiveCamera->CheckWorldFrustum(m_pBoundingSphere,testPlane) )
			{
				pRendererQueue->Insert(pIRenderer);
			}
		}
		else
			pRendererQueue->Insert(pIRenderer);
	}
		
	list<SceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera,testPlane);
	}
}