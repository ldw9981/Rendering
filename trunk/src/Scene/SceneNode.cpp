#include "StdAfx.h"
#include "SceneNode.h"
#include "ASEParser/ASEParser.h"
#include "Foundation/Trace.h"
#include "CameraNode.h"
#include "Graphics/RendererQueue.h"
#include "Math/Sphere.h"
#include "Scene/MeshNode.h"
#include "Scene/SkinnedMeshNode.h"
#include "Scene/CameraNode.h"
#include "Math/Frustum.h"
#include "Math/CollisionDetector.h"
#include "ZQuadTree.h"
#include "Foundation/Define.h"
#include "ASEParser/ASEParser.h"
#include "Graphics/Animation.h"
#include "Graphics/Graphics.h"
#include "Scene/Skeleton.h"
namespace Sophia
{

#define SCENENODE_LASTEST 1

// ��� ��� �̸��� �ʱ⿡ ��Ʈ�� �Ǹ� �θ�� ����.
cSceneNode::cSceneNode(void)
:m_strNodeName(""),m_strParentName("")
{	
	m_pRootNode=NULL;
	m_pParentNode=NULL;
//	m_NodeType=ROOT;	

	m_bIsActiveAnimation = true;
	m_bShow = true;
	m_type = TYPE_SCENE;
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

/*
	SceneAnimation�� NULL�ϼ� �ִ�. 
*/
void cSceneNode::UpdateLocalMatrix()
{	
	ENTITY_ANIMATION_DESCRIPTION& base = m_pRootNode->m_baseAnimationDesc;
	ENTITY_ANIMATION_DESCRIPTION& basePrev = m_pRootNode->m_basePrevAnimationDesc;
	std::list<ENTITY_ANIMATION_DESCRIPTION>& listPartial = m_pRootNode->m_listPartial;

	if (!m_bIsActiveAnimation)
		return;
	
	if ( m_vecSceneAnimation.empty())
	{
		m_referenceAnimationKey.GetTrasnform(&m_matLocal);	
		return;
	}

	ANMKEY anmKeyCurr;
	ANMKEY anmKeyTemp;
	
	SceneAnimation* pSceneAnimation =NULL;	

	// ��ü ��忡�� ����Ǵ� �ִϸ��̼�
	if ( base.playIndex != -1 )
	{	
		pSceneAnimation = m_vecSceneAnimation[base.playIndex];
		if (pSceneAnimation)
		{
			pSceneAnimation->GetAnimationKey(anmKeyCurr,base.playTime,m_baseAnimationKeyIndex);

			if (base.fadeInTime > 0 ) 
			{
				if (basePrev.playIndex != -1)
				{
					pSceneAnimation = m_vecSceneAnimation[basePrev.playIndex];
					assert(pSceneAnimation!=NULL);
					pSceneAnimation->GetAnimationKey(anmKeyTemp,basePrev.playTime,m_basePrevAnimationKeyIndex);
					SceneAnimation::InterpolateAnimationnKey(anmKeyCurr,anmKeyTemp,anmKeyCurr,base.fadeWeight);
				}
			}
		}
	}
	
	if (pSceneAnimation==NULL)
	{
		anmKeyCurr=m_referenceAnimationKey;
	}

	size_t index=0;
	for (auto it = listPartial.begin();it!=listPartial.end();it++,index++)
	{
		ENTITY_ANIMATION_DESCRIPTION& partial = *it;
		pSceneAnimation = m_vecSceneAnimation[partial.playIndex];
		if (pSceneAnimation)
		{
			pSceneAnimation->GetAnimationKey(anmKeyTemp,partial.playTime,m_partialIndex[index]);	
			SceneAnimation::InterpolateAnimationnKey(anmKeyCurr,anmKeyCurr,anmKeyTemp,pSceneAnimation->m_partialWeight);
		}
	}
	anmKeyCurr.GetTrasnform(&m_matLocal);	
}

void cSceneNode::UpdateChildren(DWORD elapseTime)
{
	for ( auto iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{		
		(*iter)->Update(elapseTime);
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



cSceneNode& cSceneNode::operator =(const cSceneNode& other)
{
	assert(0);
	return *this;
}

void cSceneNode::DettachChildNode( cSceneNode* pItem )
{
	m_listChildNode.erase(pItem->m_ParentListIt);
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

	D3DXMatrixDecompose(&m_referenceAnimationKey.ScaleAccum,
		&m_referenceAnimationKey.RotationAccum,
		&m_referenceAnimationKey.TranslationAccum,&m_matLocal);

	std::list<cSceneNode*>::iterator iter;	
	cSceneNode* pNode=NULL;
	for ( iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{
		pNode=*iter;
		pNode->BuildComposite(pEntity);
	}	
}

void cSceneNode::SerializeIn( std::ifstream& stream )
{
	// �̹� �տ��� Ÿ���� �о���.
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
	count = m_listChildNode.size();
	stream.write((char*)&count,sizeof(count));
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeOut(stream);
	}	
}



void cSceneNode::QueueRenderer(Entity* pEntity,bool bTraversal)
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->QueueRenderer(pEntity,bTraversal);
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
	UpdateLocalMatrix();
	UpdateWorldMatrix(m_pParentNode);
	for ( auto iter=m_listChildNode.begin() ; iter!=m_listChildNode.end() ; ++iter)
	{		
		(*iter)->Update(elapseTime);
	}	
}

void cSceneNode::RenderShadow()
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->RenderShadow();
	}
}

void cSceneNode::QueueRendererShadow(Entity* pEntity,bool bTraversal )
{
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->QueueRendererShadow(pEntity,bTraversal);
	}
}

void cSceneNode::Release()
{
	std::list<cSceneNode*>::iterator it = m_listChildNode.begin();
	std::list<cSceneNode*>::iterator it_end = m_listChildNode.end();

	for ( ;it!=it_end ; it++ )
	{
		delete *it;
	}	
	m_listChildNode.clear();
}



D3DXMATRIX& cSceneNode::GetNodeTM()
{
	return m_nodeTM;
}


cSceneNode* cSceneNode::CreateNode( SCENETYPE type )
{
	cSceneNode* ret = NULL;
	if(type == TYPE_SCENE)
		ret = new cSceneNode;
	else if(type == TYPE_MESH)
		ret = new cMeshNode;
	else if(type == TYPE_SKINNEDMESH)
		ret = new SkinnedMeshNode;
	else if(type == TYPE_SKELETON)
		ret = new Skeleton;

	assert(ret!=NULL);
	return ret;
}

void cSceneNode::PushAnimation( EntityAnimation* pAnimation )
{
	SceneAnimation* pSceneAnimation = pAnimation->GetSceneAnimtion(m_strNodeName);
	m_vecSceneAnimation.push_back(pSceneAnimation);

	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->PushAnimation(pAnimation);
	}
}

void cSceneNode::PopAnimation()
{
	m_vecSceneAnimation.pop_back();

	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
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

	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->EraseAnimation(index);
	}
}



void cSceneNode::PushMaterial( EntityMaterial* pEntityMaterial )
{
	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->PushMaterial(pEntityMaterial);
	}
}

void cSceneNode::PopMaterial()
{
	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->PopMaterial();
	}
}

void cSceneNode::Test( void(*Func)(cSceneNode*) )
{
	Func(this);
	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->Test(Func);
	}
}

void cSceneNode::AddPatialIndex()
{
	m_partialIndex.push_back(size_t(0));
	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->AddPatialIndex();
	}
}

void cSceneNode::DelPartialIndex(size_t index)
{
	m_partialIndex.erase(m_partialIndex.begin()+index);
	for ( auto it=m_listChildNode.begin() ;it!=m_listChildNode.end();++it )
	{
		(*it)->DelPartialIndex(index);
	}
}

SceneAnimation* cSceneNode::GetSceneAnimation( int index )
{
	return m_vecSceneAnimation[index];
}


}