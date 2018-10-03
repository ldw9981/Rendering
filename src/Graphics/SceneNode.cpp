#include "StdAfx.h"
#include "SceneNode.h"
#include "ASEParser/ASEParser.h"
#include "Foundation/Trace.h"
#include "CameraNode.h"
#include "Graphics/RendererQueue.h"
#include "Math/Sphere.h"
#include "Graphics/MeshNode.h"
#include "Graphics/SkinnedMeshNode.h"
#include "Graphics/CameraNode.h"
#include "Math/Frustum.h"
#include "Math/CollisionDetector.h"
#include "ZQuadTree.h"
#include "Foundation/Define.h"
#include "ASEParser/ASEParser.h"
#include "Graphics/Animation.h"
#include "Graphics/Graphics.h"
#include "Graphics/Skeleton.h"
namespace Sophia
{

#define SCENENODE_LASTEST 1

// 모든 노드 이름은 초기에 루트가 되며 부모는 없다.
cSceneNode::cSceneNode(void)
:m_strNodeName(""),m_strParentName("")
{	
	m_pRootNode=NULL;
	m_pParentNode=NULL;
//	m_NodeType=ROOT;	

	m_bIsActiveAnimation = false;
	m_bShow = true;
	m_type = SCENE_NODE;
	D3DXMatrixIdentity(&m_nodeTM);
	m_basePrevAnimationKeyIndex = 0;
	m_baseAnimationKeyIndex = 0;
	m_partialAnimationKeyIndex = 0;
	m_partialPrevAnimationKeyIndex = 0;
}

cSceneNode::~cSceneNode(void)
{
	Release();
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
	m_vecChildNode.push_back(pItem);
}

cSceneNode* cSceneNode::FindNode( std::string& nodename )
{
//	if ( (type==ANYTHING)||(type==m_NodeType) )
//	{
		// 자신이면 자신의 주소 리턴
		if( nodename.compare(m_strNodeName)==0)
			return this;
//	}

	// 아니면 자식에서 검색
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		pFoundItem=m_vecChildNode[i]->FindNode(nodename);
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

/*
	SceneAnimation이 NULL일수 있다. 
*/
void cSceneNode::UpdateLocalMatrix()
{	
	ENTITY_ANIMATION_DESCRIPTION& base = m_pRootNode->m_baseAnimationDesc;
	ENTITY_ANIMATION_DESCRIPTION& basePrev = m_pRootNode->m_basePrevAnimationDesc;
	std::list<ENTITY_ANIMATION_DESCRIPTION>& listPartial = m_pRootNode->m_listPartial;

	assert(m_bIsActiveAnimation==true);
	assert(m_vecSceneAnimation.empty()==false);	

	SceneAnimation* pSceneAnimationCurr =NULL;
	SceneAnimation* pSceneAnimationPrev =NULL;	

	// 전체 노드에서 재생되는 애니메이션
	if ( base.playIndex != -1 )
	{	
		pSceneAnimationCurr = m_vecSceneAnimation[base.playIndex];
		if (pSceneAnimationCurr)
			pSceneAnimationCurr->GetMatrix(m_matLocal,base.playTime,m_baseAnimationKeyIndex);

		if (base.fadeInTime > 0 && basePrev.playIndex != -1)
		{
			D3DXMATRIX matPrev;
			pSceneAnimationPrev = m_vecSceneAnimation[basePrev.playIndex];
			assert(pSceneAnimationPrev!=NULL);
			pSceneAnimationPrev->GetMatrix(matPrev,basePrev.playTime,m_basePrevAnimationKeyIndex);

			m_matLocal = m_matLocal * base.fadeWeight;
			m_matLocal += matPrev * (1.0f-base.fadeWeight);
		}		
	}

	if (pSceneAnimationCurr==NULL)
	{
		m_matLocal = m_referenceTM;
	}

	size_t index=0;
	for (auto it = listPartial.begin();it!=listPartial.end();it++,index++)
	{
		ENTITY_ANIMATION_DESCRIPTION& partial = *it;
		pSceneAnimationCurr = m_vecSceneAnimation[partial.playIndex];
		if (pSceneAnimationCurr && pSceneAnimationCurr->m_partialWeight != 0.0f )
		{
			D3DXMATRIX matPartial;
			pSceneAnimationCurr->GetMatrix(matPartial,partial.playTime,m_partialIndex[index]);	

			m_matLocal = m_matLocal * (1.0f - pSceneAnimationCurr->m_partialWeight);
			m_matLocal += matPartial * pSceneAnimationCurr->m_partialWeight;
		}
	}	
}

void cSceneNode::FreeChildren()
{
	cSceneNode* pItem=NULL;
	for (auto iter=m_vecChildNode.begin() ; iter!=m_vecChildNode.end() ; ++iter)
	{
		pItem=*iter;
		pItem->FreeChildren();
	}	
	
	for (auto iter=m_vecChildNode.begin() ; iter!=m_vecChildNode.end() ; ++iter)
	{
		pItem=*iter;
		delete pItem;
		
	}	

}



cSceneNode& cSceneNode::operator =(const cSceneNode& other)
{
	assert(0);
	return *this;
}

void cSceneNode::DettachChildNode( cSceneNode* pItem )
{
	for (auto iter=m_vecChildNode.begin() ; iter!=m_vecChildNode.end() ; ++iter)
	{
		cSceneNode* pNode =*iter;
		if ( pNode == pItem)
		{
			delete pItem;
			m_vecChildNode.erase(iter);
			return;
		}
	}	
}

void cSceneNode::BuildComposite(Entity* pEntity)
{
	m_pRootNode = pEntity;

	D3DXMATRIX temp;
	if (m_pParentNode == NULL)
	{
		m_matLocal = m_nodeTM;
	}
	else
	{
		D3DXMATRIX worldParentReferenceInv;
		D3DXMatrixInverse(&worldParentReferenceInv,NULL,&m_pParentNode->GetNodeTM());
		m_matLocal = m_nodeTM * worldParentReferenceInv;
	}

	m_referenceTM = m_matLocal;

	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		m_vecChildNode[i]->BuildComposite(pEntity);	
	}	
}

void cSceneNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);

	//child
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream.read((char*)&type,sizeof(type));
		cSceneNode* pNode = CreateNode(type);
		pNode->SetRootNode(m_pRootNode);
		pNode->SetParentNode(this);
		pNode->SetParentName(m_strNodeName.c_str());
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}	
}

void cSceneNode::SerializeOut( std::ofstream& stream )
{
	stream.write((char*)&m_type,sizeof(m_type));
	unsigned short ver = SCENENODE_LASTEST;
	unsigned char count =0 ;

	stream.write((char*)&ver,sizeof(ver));
	WriteString(stream,m_strNodeName);
	WriteString(stream,m_strParentName);
	WriteMatrix(stream,m_nodeTM);

	//child
	count = m_vecChildNode.size();
	stream.write((char*)&count,sizeof(count));
	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		m_vecChildNode[i]->SerializeOut(stream);
	}	
}



void cSceneNode::GatherRender(Entity* pEntity,bool bTraversal)
{
	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		m_vecChildNode[i]->GatherRender(pEntity,bTraversal);
	}	
}

void cSceneNode::Render(DWORD elapseTime)
{
	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		m_vecChildNode[i]->Render(elapseTime);
	}
}

void cSceneNode::Update( DWORD elapseTime )
{
	if (m_bIsActiveAnimation && !m_vecSceneAnimation.empty())
	{
		UpdateLocalMatrix();
	}	

	if (m_pParentNode==NULL)	
	{
		m_matWorld = m_matLocal;		// 부모가 없으면 LocalTM이 WorldTM이 된다.					
	}
	else
	{
//		m_matWorld = m_matLocal * m_pParentNode->m_matWorld;	// 부모가 있으면 WorldTM은 LocalTM * GetWorldTM 이 된다.
		D3DXMatrixMultiply(&m_matWorld,&m_matLocal,&m_pParentNode->m_matWorld);
	}

	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		m_vecChildNode[i]->Update(elapseTime);
	}
}


void cSceneNode::Release()
{
	size_t size = m_vecChildNode.size();
	for (size_t i=0;i<size;i++)
	{
		delete m_vecChildNode[i];
	}
	m_vecChildNode.clear();
}



D3DXMATRIX& cSceneNode::GetNodeTM()
{
	return m_nodeTM;
}


cSceneNode* cSceneNode::CreateNode( SCENETYPE type )
{
	cSceneNode* ret = NULL;
	if(type == SCENE_NODE)
		ret = new cSceneNode;
	else if(type == SCENE_MESH)
		ret = new cMeshNode;
	else if(type == SCENE_SKINNEDMESH)
		ret = new SkinnedMeshNode;
	else if(type == SCENE_SKELETON)
		ret = new Skeleton;

	assert(ret!=NULL);
	return ret;
}

void cSceneNode::PushAnimation( EntityAnimation* pAnimation )
{
	SceneAnimation* pSceneAnimation = pAnimation->GetSceneAnimtion(m_strNodeName);	
	m_vecSceneAnimation.push_back(pSceneAnimation);
	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->PushAnimation(pAnimation);
	}
}

void cSceneNode::PopAnimation()
{
	m_vecSceneAnimation.pop_back();

	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->PopAnimation();
	}
}

void cSceneNode::EraseAnimation( int index )
{	
	int i=0;
	for (auto it=m_vecSceneAnimation.begin() ;it!=m_vecSceneAnimation.end() ;it++ ,i++)
	{
		if(i != index)
			continue;

		SceneAnimation* pItem = *it;
		m_vecSceneAnimation.erase(it);
		break;
	}	

	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->EraseAnimation(index);
	}
}




void cSceneNode::WorkRecursive( void(*Func)(cSceneNode*) )
{
	Func(this);
	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->WorkRecursive(Func);
	}
}

void cSceneNode::AddPatialIndex()
{
	m_partialIndex.push_back(size_t(0));
	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->AddPatialIndex();
	}
}

void cSceneNode::DelPartialIndex(size_t index)
{
	m_partialIndex.erase(m_partialIndex.begin()+index);
	for ( auto it=m_vecChildNode.begin() ;it!=m_vecChildNode.end();++it )
	{
		(*it)->DelPartialIndex(index);
	}
}

SceneAnimation* cSceneNode::GetSceneAnimation( size_t index )
{
	assert(index < m_vecSceneAnimation.size()); 
	return m_vecSceneAnimation[index];
}

int cSceneNode::CountSceneAnimation()
{
	return m_vecSceneAnimation.size();
}


}