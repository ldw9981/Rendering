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
#include "Math/Frustum.h"
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
	m_bRender=true;
	m_animationTime = 0;
}

cSceneNode::~cSceneNode(void)
{
	std::list<cSceneNode*>::iterator it = m_listChildNode.begin();
	std::list<cSceneNode*>::iterator it_end = m_listChildNode.end();

	for ( ;it!=it_end ; it++ )
	{
		delete *it;
	}	
	m_listChildNode.clear();
	
	if( m_pRscTransformAnm !=NULL )
		m_pRscTransformAnm->Release();
}

void cSceneNode::SetNodeName( const char* nodename )
{
	m_strNodeName=nodename;
}

std::string& cSceneNode::GetNodeName()
{
	return m_strNodeName;
}

void cSceneNode::SetParentName( const char* nodename )
{
	m_strParentName=nodename;
}

std::string& cSceneNode::GetParentName()
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

cSceneNode* cSceneNode::FindNode( std::string& nodename )
{
//	if ( (type==ANYTHING)||(type==m_NodeType) )
//	{
		// �ڽ��̸� �ڽ��� �ּ� ����
		if( nodename.compare(m_strNodeName)==0)
			return this;
//	}

	// �ƴϸ� �ڽĿ��� �˻�
	std::list<cSceneNode*>::iterator iter;	
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

D3DXMATRIX* cSceneNode::UpdateTransformAnm(DWORD& animationTime,DWORD elapseTime)
{	
	if ( m_pRscTransformAnm==NULL)
		return NULL;

	if (!m_bIsActiveAnimation)
		return NULL;

	m_pRscTransformAnm->GetTransform(m_AnimationTM,animationTime,elapseTime);
	return &m_AnimationTM;
}

void cSceneNode::UpdateChildren(DWORD elapseTime)
{
	std::list<cSceneNode*>::iterator iter;	
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
	std::list<cSceneNode*>::iterator iter;	
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

/*
�θ� �߿� BS�� ��带 ã�� �ڽ��� �����ϴ� BS�� �����Ѵ�. 
*/

// �⺻�����
// bRenderüũ�� �ڽĸ� ����.
void cSceneNode::CullRendererIntoRendererQueue( cView* pView,Frustum* pFrustum )
{				
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererIntoRendererQueue(pView,pFrustum);
	}
}



void cSceneNode::BuildComposite()
{
	std::list<cSceneNode*>::iterator iter;	
	cSceneNode* pNode=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pNode=*iter;
		pNode->BuildComposite();
	}	
}



void cSceneNode::SetNodeInfo( SCENENODEINFO& stInfo )
{
	m_BoundingSphere = stInfo.boundingSphere;
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

void cSceneNode::SerializeIn( std::fstream& in )
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeIn(in);
	}
}

void cSceneNode::SerializeOut( std::fstream& out )
{
//	out.write((char*)m_strNodeName.c_str(),m_strNodeName.length());
//	out.write((char*)m_strParentName.c_str(),m_strParentName.length());

//	out << m_strNodeName;

	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeIn(out);
	}
}

void cSceneNode::SetBoundingSphere( cSphere& Sphere )
{
	m_BoundingSphere = Sphere;	
}

void cSceneNode::QueueRenderer(cView* pView,bool bTraversal)
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->QueueRenderer(pView,bTraversal);
	}
}

void cSceneNode::Render()
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->Render();
	}
}

void cSceneNode::Update( DWORD elapseTime )
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix(UpdateTransformAnm(m_animationTime,elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

void cSceneNode::RenderShadow()
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->RenderShadow();
	}
}

void cSceneNode::QueueRendererShadow( cView* pView,bool bTraversal )
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->QueueRendererShadow(pView,bTraversal);
	}
}


