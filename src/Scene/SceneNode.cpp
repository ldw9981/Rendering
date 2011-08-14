#include "StdAfx.h"
#include "SceneNode.h"
#include "ASEParser/ASEParser.h"
#include "Foundation/Trace.h"
#include "CameraNode.h"
#include "RendererQueue.h"
#include "Framework/D3DFramework.h"
#include "DebugInfoView.h"
#include "Math/Sphere.h"
#include "Scene/MeshNode.h"
#include "Scene/CameraNode.h"
#include "Math/CollisionDetector.h"
#include "ZQuadTree.h"
#include "Foundation/Define.h"
#include "ASEParser/ASEParser.h"

// ��� ��� �̸��� �ʱ⿡ ��Ʈ�� �Ǹ� �θ�� ����.
cSceneNode::cSceneNode(void)
:m_strNodeName("Root"),m_strParentName("")
{	
	m_pRootNode=this;
	m_pParentNode=NULL;
//	m_NodeType=ROOT;	

	m_bIsActiveAnimation=FALSE;
	m_pRscTransformAnm=NULL;		
	m_pCullingSphere=NULL;
	m_pBoundingSphere=NULL;
	m_bRender=true;
}

cSceneNode::~cSceneNode(void)
{
	list<cSceneNode*>::iterator it = m_listChildNode.begin();
	list<cSceneNode*>::iterator it_end = m_listChildNode.end();

	for ( ;it!=it_end ; it++ )
	{
		delete *it;
	}	
	m_listChildNode.clear();

	SAFE_DELETE(m_pCullingSphere);
	SAFE_DELETE(m_pBoundingSphere);

	if( m_pRscTransformAnm !=NULL )
		m_pRscTransformAnm->Release();
}

void cSceneNode::SetNodeName( const char* nodename )
{
	m_strNodeName=nodename;
}

string& cSceneNode::GetNodeName()
{
	return m_strNodeName;
}

void cSceneNode::SetParentName( const char* nodename )
{
	m_strParentName=nodename;
}

string& cSceneNode::GetParentName()
{
	return m_strParentName;
}



BOOL cSceneNode::IsExistParentName()
{
	if (m_strParentName.empty())
	{
		return FALSE;
	}
	return TRUE;
}

void cSceneNode::AttachChildNode( cSceneNode* pItem )
{
	m_listChildNode.push_back(pItem);
	pItem->m_ParentListIt = --m_listChildNode.end();
}

cSceneNode* cSceneNode::FindNode( string& nodename )
{
//	if ( (type==ANYTHING)||(type==m_NodeType) )
//	{
		// �ڽ��̸� �ڽ��� �ּ� ����
		if( nodename.compare(m_strNodeName)==0)
			return this;
//	}

	// �ƴϸ� �ڽĿ��� �˻�
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
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

BOOL cSceneNode::IsRootNode()
{
	if (m_pParentNode!=NULL)
	{		
		return FALSE;
	}
	return TRUE;
}

D3DXMATRIX* cSceneNode::UpdateTransformAnm(DWORD elapseTime)
{	
	if ( m_pRscTransformAnm==NULL)
		return NULL;

	if (!m_bIsActiveAnimation)
		return NULL;

	m_AnimationTM = m_pRscTransformAnm->GetTransform(elapseTime);
	return &m_AnimationTM;
}

void cSceneNode::UpdateChildren(DWORD elapseTime)
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Update(elapseTime);
	}	
}



D3DXMATRIX& cSceneNode::GetAnimationTM()
{
	return m_AnimationTM;
}


void cSceneNode::FreeChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
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

void cSceneNode::SetRscTransformAnm( cRscTransformAnm* val )
{
	m_pRscTransformAnm = val; 
	if (val!=NULL)
	{
		m_pRscTransformAnm->AddRef();
		m_bIsActiveAnimation = TRUE;
	}
}


cSceneNode& cSceneNode::operator =(const cSceneNode& other)
{
	assert(0);
	return *this;
}

void cSceneNode::DettachChildNode( cSceneNode* pItem )
{
	m_listChildNode.erase(pItem->m_ParentListIt);
}


cSphere* cSceneNode::CreateCullingSphere()
{
	SAFE_DELETE(m_pCullingSphere);
	m_pCullingSphere = new cSphere;
	return m_pCullingSphere;
}

/*
�θ� �߿� BS�� ��带 ã�� �ڽ��� �����ϴ� BS�� �����Ѵ�. 
*/
void cSceneNode::UpdateParentCullingSphere(cSphere& Sphere)
{
	cSceneNode* pNode=this;
	do 
	{
		pNode = pNode->GetParentNode();
		if (pNode==NULL)
			return;

	} while(pNode->GetCullingSphere()==NULL);

	pNode->GetCullingSphere()->Merge(Sphere);	
}

cSphere* cSceneNode::CreateBoundingSphere()
{
	assert(m_pBoundingSphere==NULL);
	m_pBoundingSphere = new cSphere;
	return m_pBoundingSphere;
}


void cSceneNode::CullRendererTraversal( cCameraNode* pActiveCamera )
{			
	if (!m_bRender)
	{
		goto children;
	}

	if (m_pCullingSphere!=NULL)
	{
		// �ڽĳ�忡���� ���ŵǴ� �ø����� Ȱ��ȭ�� ī�޶� ����ü�� ���������Ȯ�� Ȯ��
		int ret=pActiveCamera->CheckWorldFrustum(m_pCullingSphere);
		if( ret == cCollision::OUTSIDE)
		{	
			//  �ۿ� �ִ°��̸� ����ȸ ����
			return;	
		}
		else if (ret == cCollision::INSIDE)
		{	
			// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����
			PushTraversal(pActiveCamera);
			return;
		}
	}
	
	// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
	if (m_pBoundingSphere!=NULL)
	{
		int ret=pActiveCamera->CheckWorldFrustum(m_pBoundingSphere);
		if( ret != cCollision::OUTSIDE)	
		{	
			// INTERSECT or INSIDE�� ť�� �ִ´�.
			SendQueue();			
		}
	}		
children:
	list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pActiveCamera);
	}
}


/*
	��ȸ���ϸ鼭 cIRenderer�� ť�� �ִ´�. 
*/
void cSceneNode::PushTraversal( cCameraNode* pActiveCamera,WORD testPlane/*=0 */ )
{
	if (!m_bRender)
	{
		goto children;
	}	
	SendQueue();
children:
	list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->PushTraversal(pActiveCamera,testPlane);
	}
}

void cSceneNode::BuildComposite()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pNode=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pNode=*iter;
		pNode->BuildComposite();
	}	
}



void cSceneNode::SetNodeInfo( SCENENODEINFO& stInfo )
{
	this->SetNodeName(stInfo.strNodeName.c_str());
	this->SetParentName(stInfo.strParentName.c_str());
	this->SetLocalTM(stInfo.tmLocal);
	this->SetWorldTM(stInfo.tmWorld);
	this->SetRscTransformAnm(stInfo.pRscTransform);
	if (m_pRscTransformAnm)
	{
		m_pRscTransformAnm->AddRef();
	}	
}

void cSceneNode::SerializeIn( fstream& in )
{
	list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeIn(in);
	}
}

void cSceneNode::SerializeOut( fstream& out )
{
//	out.write((char*)m_strNodeName.c_str(),m_strNodeName.length());
//	out.write((char*)m_strParentName.c_str(),m_strParentName.length());

//	out << m_strNodeName;

	list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeIn(out);
	}
}

void cSceneNode::SetBoundingSphere( cSphere& Sphere )
{
	if (m_pBoundingSphere)
	{
		*m_pBoundingSphere = Sphere;
	}
}

void cSceneNode::SetCullingSphere( cSphere& Sphere )
{
	if (m_pCullingSphere)
	{
		*m_pCullingSphere = Sphere;
	}
}

void cSceneNode::SendQueue()
{

}


