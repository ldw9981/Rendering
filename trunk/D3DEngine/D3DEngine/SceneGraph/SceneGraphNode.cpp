#include "StdAfx.h"
#include "SceneGraphNode.h"
#include "../ASEParser/ASEParser.h"
#include "../Log/Trace.h"
#include "../SceneGraph/CameraNode.h"
#include "../Scene/RendererQueue.h"
#include "../Framework/D3DFramework.h"
#include "../Log/DebugInfoScene.h"
#include "../Geometrics/Sphere.h"
#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/CameraNode.h"
#include "../Geometrics/CollisionDetector.h"
#include "../OutDoor/QuadTreeNode.h"



// ��� ��� �̸��� �ʱ⿡ ��Ʈ�� �Ǹ� �θ�� ����.
cSceneGraphNode::cSceneGraphNode(void)
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

cSceneGraphNode::~cSceneGraphNode(void)
{
	SAFE_DELETE(m_pCullingSphere);

	if( m_pRscTransformAnm !=NULL )
		m_pRscTransformAnm->Release();

	for_each(m_listChildNode.begin(),m_listChildNode.end(),FuncDeleteType<cSceneGraphNode*>);
	m_listChildNode.clear();
}

void cSceneGraphNode::SetNodeName( LPCWSTR nodename )
{
	m_strNodeName=nodename;
}

wstring& cSceneGraphNode::GetNodeName()
{
	return m_strNodeName;
}

void cSceneGraphNode::SetParentName( LPCWSTR nodename )
{
	m_strParentName=nodename;
}

wstring& cSceneGraphNode::GetParentName()
{
	return m_strParentName;
}



BOOL cSceneGraphNode::IsExistParentName()
{
	if (m_strParentName.empty())
	{
		return FALSE;
	}
	return TRUE;
}

void cSceneGraphNode::AttachChildNode( cSceneGraphNode* pItem )
{
	m_listChildNode.push_back(pItem);
	pItem->m_ParentListIt = --m_listChildNode.end();
}

cSceneGraphNode* cSceneGraphNode::FindNode( wstring& nodename,int type/*=ANYTHING*/ )
{
	if ( (type==ANYTHING)||(type==m_NodeType) )
	{
		// �ڽ��̸� �ڽ��� �ּ� ����
		if( nodename.compare(m_strNodeName)==0)
			return this;
	}

	// �ƴϸ� �ڽĿ��� �˻�
	list<cSceneGraphNode*>::iterator iter;	
	cSceneGraphNode* pItem=NULL;
	cSceneGraphNode* pFoundItem=NULL;
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

BOOL cSceneGraphNode::IsRootNode()
{
	if (m_pParentNode!=NULL)
	{		
		return FALSE;
	}
	return TRUE;
}

D3DXMATRIX* cSceneGraphNode::UpdateTransformAnm(DWORD elapseTime)
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

void cSceneGraphNode::UpdateChildren(DWORD elapseTime)
{
	list<cSceneGraphNode*>::iterator iter;	
	cSceneGraphNode* pItem=NULL;
	cSceneGraphNode* pFoundItem=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Update(elapseTime);
	}	
}

void cSceneGraphNode::InitChildren()
{
	list<cSceneGraphNode*>::iterator iter;	
	cSceneGraphNode* pItem=NULL;	
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Init();
	}	
}

void cSceneGraphNode::UninitChildren()
{
	list<cSceneGraphNode*>::iterator iter;	
	cSceneGraphNode* pItem=NULL;	
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Uninit();
	}	
}

// �θ��带 ã�Ƽ� �ڽ����� �ִ´�.
BOOL cSceneGraphNode::LinkToParent()
{
	//��Ʈ ���� �ݵ�� �־���Ѵ�.
	ASSERT(m_pRootNode!=NULL);

	cSceneGraphNode* pFoundObject=NULL;
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





D3DXMATRIX& cSceneGraphNode::GetAnimationTM()
{
	return m_AnimationTM;
}


void cSceneGraphNode::FreeChildren()
{
	list<cSceneGraphNode*>::iterator iter;	
	cSceneGraphNode* pItem=NULL;
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

void cSceneGraphNode::SetRscTransformAnm( cRscTransformAnm* val )
{
	m_pRscTransformAnm = val; 
	if (val!=NULL)
	{
		m_pRscTransformAnm->AddRef();
		m_TotalTransformAnm = m_pRscTransformAnm->GetArrayTransformAnm().size();
		m_bIsActiveAnimation = TRUE;
	}
}


cSceneGraphNode& cSceneGraphNode::operator =(const cSceneGraphNode& other)
{
	assert(0);
	return *this;
}

void cSceneGraphNode::DettachChildNode( cSceneGraphNode* pItem )
{
	m_listChildNode.erase(pItem->m_ParentListIt);
}


cSphere* cSceneGraphNode::CreateCullingSphere()
{
	SAFE_DELETE(m_pCullingSphere);
	m_pCullingSphere = new cSphere;
	return m_pCullingSphere;
}

/*
�θ� �߿� BS�� ��带 ã�� �ڽ��� �����ϴ� BS�� �����Ѵ�. 
*/
void cSceneGraphNode::UpdateParentCullingSphere(cSphere& Sphere)
{
	cSceneGraphNode* pNode=this;
	do 
	{
		pNode = pNode->GetParentNode();
		if (pNode==NULL)
			return;

	} while(pNode->GetCullingSphere()==NULL);

	pNode->GetCullingSphere()->Merge(Sphere);	
}

cSphere* cSceneGraphNode::CreateBoundingSphere()
{
	assert(m_pBoundingSphere==NULL);
	m_pBoundingSphere = new cSphere;
	return m_pBoundingSphere;
}


void cSceneGraphNode::CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera )
{
	IRenderer* pIRenderer=dynamic_cast<IRenderer*>(this);
	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{
			// �ڽĳ�忡���� ���ŵǴ� �ø����� Ȱ��ȭ�� ī�޶� ����ü�� ���������Ȯ�� Ȯ��
			int ret=pActiveCamera->CheckWorldFrustum(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  �ۿ� �ִ°��̸� ����ȸ ����
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
		}
		
		// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustum(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE�� ť�� �ִ´�.
			{	
				pRendererQueue->Insert(pIRenderer);						
			}
		}
	}	

	list<cSceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}


/*
	��ȸ���ϸ鼭 cIRenderer�� ť�� �ִ´�. 
*/
void cSceneGraphNode::InsertToRendererQueueTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera,WORD testPlane/*=0*/ )
{
	IRenderer* pIRenderer=dynamic_cast<IRenderer*>(this);
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
		
	list<cSceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera,testPlane);
	}
}