#include "StdAfx.h"
#include "Entity.h"
#include "Graphics.h"
#include "Animation.h"
#include "MaterialEx.h"
#include "Vertex.h"
#include "Math/CollisionDetector.h"
#include "Resource/ResourceMng.h"
#include "ASEParser/ASEParser.h"
#include "Foundation/StringUtil.h"
#include "Scene/Skeleton.h"
#include "Foundation/Define.h"
#include "Scene/MeshNode.h"
namespace Sophia
{


#define ENTITY_LASTEST 1


Entity::Entity(void)
{
	m_strNodeName="Entity";	
	m_type = TYPE_ROOT;
	m_bShowBone = false;
	m_pEntityMaterial = NULL;
}


Entity::~Entity(void)
{
	for (auto it=m_vecAnimation.begin();it!=m_vecAnimation.end();++it )
	{
		EntityAnimation* pAnimation = *it;
		pAnimation->Release();
	}	

	SAFE_RELEASE(m_pEntityMaterial);
}
void Entity::SetBoundingSphere( cSphere& Sphere )
{
	m_BoundingSphere = Sphere;	
}

void Entity::RenderBound()
{
	int LineCount=32;
	float fScale=m_BoundingSphere.GetRadius();
	D3DCOLOR color=D3DCOLOR_RGBA(255,0,0,0);	

	// 최소한의 다각형을 그리기위한 
	if( LineCount < 6 ) 
		LineCount = 6; 

	float fRotationAngleAverage = ( D3DX_PI * 2) / LineCount; 

	LINEVERTEX CircleLine[2]; 
	D3DXVECTOR3 NewPos, Pos; 
	D3DXMATRIX matRot; 
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmWorld,&GetWorldTM());
	Graphics::m_pInstance->GetEffect()->CommitChanges();
	Graphics::m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 

	Pos = D3DXVECTOR3( 1.f, 0.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos;
	CircleLine[1].color = CircleLine[0].color = color; 
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationY( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos);
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 

	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos; 
	CircleLine[1].color = CircleLine[0].color = D3DCOLOR_RGBA(0,255,0,0);	
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationX( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos); 
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 	

	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos;
	CircleLine[1].color = CircleLine[0].color = D3DCOLOR_RGBA(0,0,255,0);	
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationZ( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos); 		
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 
}

void Entity::Update( DWORD elapseTime )
{
	if (m_basePrevAnimationDesc.playIndex != -1)
		UpdateAnimationDescription(elapseTime,m_basePrevAnimationDesc);
	
	if (m_baseAnimationDesc.playIndex != -1)
		UpdateAnimationDescription(elapseTime,m_baseAnimationDesc);	

	size_t index =0;
	for (auto it = m_listPartial.begin();it != m_listPartial.end();)
	{
		if(UpdatePartialDescription(elapseTime,it))
		{
			it++;
			index++;
		}
		else
		{
			for (auto it_child = m_vecChildNode.begin();it_child!=m_vecChildNode.end();it_child++)
			{
				(*it_child)->DelPartialIndex(index);
			}	
			it = m_listPartial.erase(it);
		}
	}

	cSceneNode::Update(elapseTime);

	const D3DXVECTOR3* pos = GetWorldPositionPtr();
	m_BoundingSphere.SetCenterPos(*pos);
}

bool Entity::Cull( Frustum* pFrustum ,float loose)
{
	if (!m_bShow)
		return false;

	cCollision::STATE retCS=cCollision::CheckWorldFrustum(*pFrustum,m_BoundingSphere,loose);
	if( retCS == cCollision::OUTSIDE)
		return false;

	return true;	
}

void Entity::Build()
{
	ClearRenderQueue();
	cSceneNode::BuildComposite(this);
}

void Entity::SerializeIn( std::ifstream& stream )
{
	unsigned short ver = 0;
	unsigned char count = 0;
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	float radius = 0.0f;
	stream.read((char*)&radius,sizeof(radius));
	m_BoundingSphere.SetRadius(radius);
	
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream.read((char*)&type,sizeof(type));
		cSceneNode* pNode = CreateNode(type);
		if (pNode==NULL)
		{
			return;
		}
		pNode->SetRootNode(this);
		pNode->SetParentNode(this);
		pNode->SetParentName(m_strNodeName.c_str());
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}	
}

void Entity::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = ENTITY_LASTEST;
	unsigned char count = 0;
	stream.write((char*)&ver,sizeof(ver));
	WriteString(stream,m_strNodeName);
	float radius = m_BoundingSphere.GetRadius(); 
	stream.write((char*)&radius,sizeof(radius));

	//child
	count = m_vecChildNode.size();
	stream.write((char*)&count,sizeof(count));
	
	for (auto it=m_vecChildNode.begin();it!=m_vecChildNode.end();++it )
	{
		(*it)->SerializeOut(stream);
	}	
}

bool Entity::SaveScene( const char* fileName )
{
	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);
	SerializeOut(stream);
	return true;
}

bool Entity::LoadScene( const char* fileName )
{
	std::ifstream stream;
	stream.open(fileName, std::ios::in | std::ios::binary);
	SerializeIn(stream);
	return true;
}

void Entity::PushAnimation( EntityAnimation* pAnimation )
{
	pAnimation->AddRef();
	m_vecAnimation.push_back(pAnimation);

	for (auto it=m_vecChildNode.begin();it!=m_vecChildNode.end();++it )
	{
		(*it)->PushAnimation(pAnimation);
	}	
}

void Entity::PopAnimation()
{
	if (m_vecAnimation.empty())
		return;

	EntityAnimation* pAnimation = m_vecAnimation[m_vecAnimation.size()-1];
	m_vecAnimation.pop_back();
	pAnimation->Release();

	for (auto it=m_vecChildNode.begin();it!=m_vecChildNode.end();++it )
	{
		(*it)->PopAnimation();
	}	
}

bool Entity::SaveAnimation( const char* fileName ,int index)
{
	if (index >= (int)m_vecAnimation.size())
		return false;

	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);	

	EntityAnimation* pAnimation = m_vecAnimation[index];
	pAnimation->SerializeOut(stream);
	
	return true;
}

bool Entity::LoadAnimation( const char* fileName )
{
	EntityAnimation* pEntityAnimation = cResourceMng::m_pInstance->CreateEntityAnimation(fileName);
	if (pEntityAnimation->GetRefCounter()==0)
	{
		std::ifstream stream;
		stream.open(fileName, std::ios::in | std::ios::binary);
		pEntityAnimation->SerializeIn(stream);
	}
	
	PushAnimation(pEntityAnimation);
	return true;
}

bool Entity::SaveMaterial( const char* fileName ,int index)
{
	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);	

	
	m_pEntityMaterial->SerializeOut(stream);

	return true;
}

bool Entity::LoadMaterial( const char* fileName )
{
	EntityMaterial* pNew = cResourceMng::m_pInstance->CreateEntityMaterial(fileName);	
	pNew->AddRef();
	m_pEntityMaterial = pNew;
	if (pNew->GetRefCounter() > 1)
	{
		return true;
	}

	std::ifstream stream;
	stream.open(fileName, std::ios::in | std::ios::binary);
	m_pEntityMaterial->SerializeIn(stream);
	return true;
}

void Entity::CutAndPushEntityAnimation( int index,DWORD timeStart,DWORD timeEnd,const char* suffix )
{
	std::string key = GetNodeName() + std::string("_") + std::string(suffix);
	EntityAnimation* pNew = cResourceMng::m_pInstance->CreateEntityAnimation(key.c_str());	
	if (pNew->GetRefCounter()==0)
	{
		m_vecAnimation[index]->Cut(timeStart,timeEnd,pNew);
		PushAnimation(pNew);
	}
	else
	{
		assert(pNew->GetRefCounter()==0);
	}
}

void Entity::PlayBaseAnimation( int index,bool loop ,DWORD skipStartTime,DWORD earlyEndTime,DWORD fadeInTime,DWORD playTime )
{
	assert(index < (int)m_vecAnimation.size());
	assert( (skipStartTime + earlyEndTime ) <= m_vecAnimation[index]->m_dwTimeLength );

	if (  m_vecAnimation[index]->m_dwTimeLength <= (skipStartTime+playTime))
	{
		return;
	}


	m_basePrevAnimationDesc = m_baseAnimationDesc;
	
	m_baseAnimationDesc.playIndex = index;
	m_baseAnimationDesc.loop = loop;
	m_baseAnimationDesc.skipStartTime = skipStartTime;
	m_baseAnimationDesc.earlyEndTime = earlyEndTime;
	m_baseAnimationDesc.playTime = playTime + skipStartTime;	
	m_baseAnimationDesc.fadeInTime = fadeInTime;
	m_baseAnimationDesc.fadeTime = 0;
	m_baseAnimationDesc.fadeWeight = 0.0f;
	m_baseAnimationDesc.length = m_vecAnimation[m_baseAnimationDesc.playIndex]->m_dwTimeLength;

	
	assert( (playTime ) < m_vecAnimation[m_baseAnimationDesc.playIndex]->m_dwTimeLength - (skipStartTime+earlyEndTime));
}

bool Entity::LoadASE( const char* fileName )
{
	cASEParser parser;
	parser.Load(fileName,this);
	parser.Close();
	return true;
}

bool Entity::SaveAnimationSet( const char* fileName )
{
	std::string dir;
	StringUtil::SplitPath(std::string(fileName),NULL,&dir,NULL,NULL);
	std::ofstream stream;
	stream.open(fileName, std::ofstream::out );	
		
	stream << m_vecAnimation.size() << "\n";
	
	for (size_t i=0;i<m_vecAnimation.size();i++)
	{
		EntityAnimation* pAnimation = m_vecAnimation[i];

		std::string name = pAnimation->GetUniqueKey() + std::string(".animation");
		std::string full = dir + name;
		stream << name << "\n";

		SaveAnimation( full.c_str(),i);
	}
	return true;
}

bool Entity::LoadAnimationSet( const char* fileName )
{
	std::string dir;
	StringUtil::SplitPath(std::string(fileName),NULL,&dir,NULL,NULL);
	std::ifstream stream;
	stream.open(fileName, std::ifstream::in );	

	int size;
	stream >> size;

	for (int i=0;i<size;i++)
	{
		std::string name;
		stream >> name;
		std::string full = dir + name;
		LoadAnimation(full.c_str());
	}

	return true;
}

void Entity::InsertBone(cSceneNode* pBone )
{
	m_mapBones[pBone->GetNodeName()] = pBone;
}

void Entity::EraseAnimation( int index )
{
	assert(index<(int)m_vecAnimation.size());
	
	// 자식에 설정된 SceneAnimation포인터 선제거
	for (auto it=m_vecChildNode.begin();it!=m_vecChildNode.end();++it )
	{
		(*it)->EraseAnimation(index);
	}	

	int i=0;
	for (auto it=m_vecAnimation.begin() ;it!=m_vecAnimation.end() ;it++ ,i++)
	{
		if(i != index)
			continue;

		EntityAnimation* pItem = *it;
		m_vecAnimation.erase(it);
		pItem->Release();
		break;
	}	
}

void Entity::StopBaseAnimation()
{
	m_basePrevAnimationDesc = m_baseAnimationDesc;
	
	m_baseAnimationDesc.loop = false;
	m_baseAnimationDesc.playTime = m_baseAnimationDesc.length - m_baseAnimationDesc.earlyEndTime;
}

void Entity::UpdateAnimationDescription( DWORD elapseTime,ENTITY_ANIMATION_DESCRIPTION& desc )
{
	assert(desc.playIndex!=-1);
	desc.playTime += elapseTime;
	DWORD endTime = desc.length - desc.earlyEndTime;
	if( desc.playTime >= endTime  )
	{
		if ( desc.loop)
		{
			desc.playTime %= (endTime - desc.skipStartTime);
			desc.playTime += desc.skipStartTime;
		}
		else
		{
			desc.playTime = endTime;
		}
	}

	if (desc.fadeInTime!=0)
	{
		desc.fadeTime += elapseTime;
		if (desc.fadeTime >= desc.fadeInTime)
		{
			desc.fadeTime = desc.fadeInTime;
			desc.fadeInTime = 0;
		}
		desc.fadeWeight = (float)desc.fadeTime/(float)desc.fadeInTime;
	}
}

void Entity::UpdateLocalMatrix()
{

}

void Entity::PlayPartialAnimation( int index,bool loop,DWORD skipStartTime/*=0*/,DWORD earlyEndTime/*=0*/ )
{
	ENTITY_ANIMATION_DESCRIPTION desc;
	desc.playIndex = index;
	desc.loop = loop;
	desc.skipStartTime = skipStartTime;
	desc.earlyEndTime = earlyEndTime;
	desc.playTime = desc.skipStartTime;	
	desc.length = m_vecAnimation[index]->m_dwTimeLength;

	m_listPartial.push_back(desc);
	

	for (auto it = m_vecChildNode.begin();it!=m_vecChildNode.end();it++)
	{
		(*it)->AddPatialIndex();
	}
}


bool Entity::UpdatePartialDescription( DWORD elapseTime,std::list<ENTITY_ANIMATION_DESCRIPTION>::iterator& it )
{
	ENTITY_ANIMATION_DESCRIPTION& desc = *it;
	desc.playTime += elapseTime;
	DWORD endTime = desc.length - desc.earlyEndTime;
	if( desc.playTime >= endTime  )
	{
		if ( desc.loop)
		{
			desc.playTime %= (endTime - desc.skipStartTime);
			desc.playTime += desc.skipStartTime;
			
		}
		else
		{		
			return false;
		}
	}
	return true;	
}

void Entity::StopPartialAnimation( int index )
{
	for (auto it = m_listPartial.begin();it != m_listPartial.end();it++)
	{
		ENTITY_ANIMATION_DESCRIPTION& desc = (*it);
		if( desc.playIndex == index )
		{
			desc.loop = false;
			desc.playTime = desc.length - desc.earlyEndTime;
		}
	}
}

void Entity::SetShowSkeleton( bool bShow )
{
	for (auto it = m_mapBones.begin();it != m_mapBones.end();it++)
	{
		cSceneNode* pNode = (*it).second;		
		pNode->SetShow(bShow);
	}
	ResetRenderQueue();
}

void Entity::ClearRenderQueue()
{
	m_renderQueueNormal.Clear();
	m_renderQueueSkinned.Clear();
	m_renderQueueNormalAlphaBlend.Clear();
	m_renderQueueSkinnedAlphaBlend.Clear();

	m_renderQueueTerrain.Clear();

}

cSceneNode* Entity::FindNode( std::string& nodename )
{	
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

void Entity::ResetRenderQueue()
{
	ClearRenderQueue();
	QueueRenderer(this,true);
}

void Entity::WorkEnableInstancing(cSceneNode* pNode)
{
	cMeshNode* pMesh = dynamic_cast<cMeshNode*>(pNode);
	if (pMesh)
	{
		pMesh->ChangeInstancingEnable(true);
	}	
}

void Entity::ChangeInstanceEnable( bool enable )
{
	for (size_t i=0;i<m_vecChildNode.size();i++)
	{
		if (enable)
			WorkRecursive( WorkEnableInstancing);	
		else
			WorkRecursive( WorkDisableInstancing);	
	}	
}

void Entity::WorkDisableInstancing( cSceneNode* pNode )
{
	cMeshNode* pMesh = dynamic_cast<cMeshNode*>(pNode);
	if (pMesh)
	{
		pMesh->ChangeInstancingEnable(false);
	}	
}


}
