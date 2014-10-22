#include "StdAfx.h"
#include "Animation.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
namespace Sophia
{

SceneAnimation::SceneAnimation( void )
{
	m_partialWeight = 0.0f;
}

SceneAnimation::~SceneAnimation( void )
{

}

float SceneAnimation::GetInterpolateValue( int start_time,int end_time,int inter_time )
{
	float ret;
	int delta_time,offset_time;
	delta_time = end_time - start_time;
	offset_time = inter_time - start_time;

	if (delta_time==0)
	{
		return 0.0f;
	}

	ret=(float)offset_time / (float)delta_time;	
	return ret;
}

void SceneAnimation::SerializeIn( std::ifstream& stream )
{
	stream.read((char*)&m_partialWeight,sizeof(m_partialWeight));

	unsigned short count = 0;
	stream.read((char*)&count,sizeof(count));
	if (count==0)
		return;

	for (unsigned short i=0;i<count;i++)
	{
		ANMKEY_MATRIX item;
		stream.read((char*)&item,sizeof(item));
		m_arrKey.push_back(item);
	}

	
}

void SceneAnimation::SerializeOut( std::ofstream& stream )
{	
	stream.write((char*)&m_partialWeight,sizeof(m_partialWeight));

	unsigned short count = 0;
	count = m_arrKey.size();
	stream.write((char*)&count,sizeof(count));
	if (count==0)
		return;

	for (unsigned short i=0;i<count;i++)
	{
		ANMKEY_MATRIX& item = m_arrKey[i];
		stream.write((char*)&item,sizeof(item));
	}
	
}

void SceneAnimation::Cut(DWORD timeStart,DWORD timeEnd,SceneAnimation* pOut )
{	
	size_t index = 0;
	ANMKEY_MATRIX itemFirst,itemLast;
	itemFirst.AnmTick = timeStart;
	GetMatrix(itemFirst.mat,timeStart,index);

	pOut->m_arrKey.push_back(itemFirst);
	
	for (unsigned short i=0;i<m_arrKey.size();i++)
	{
		ANMKEY_MATRIX& item = m_arrKey[i];
		if( item.AnmTick > timeStart && item.AnmTick < timeEnd)
		{
			pOut->m_arrKey.push_back(item);
		}
	}
	itemLast.AnmTick = timeEnd;
	GetMatrix(itemLast.mat,timeEnd,index);
	if (itemLast.AnmTick == 0)
	{
		__debugbreak();
	}
	pOut->m_arrKey.push_back(itemLast);
}

void SceneAnimation::BlendMatrix( D3DXMATRIX& out,D3DXMATRIX& in1,D3DXMATRIX& in2,float v1,float v2 )
{
	out = in1 * v1;
	out += in2 * v2;
}

void SceneAnimation::GetMatrix( D3DXMATRIX& out,DWORD animationTime,size_t& index )
{
	size_t indexMax = m_arrKey.size()-1;
	ANMKEY_MATRIX* pKey1=NULL;
	ANMKEY_MATRIX* pKey2=NULL;

	if ( index >=indexMax ||  animationTime < m_arrKey[index].AnmTick)
	{
		index = 0;
	}
	
	for ( ; index < indexMax ; index++)
	{
		pKey1 = &m_arrKey[index];
		pKey2 = &m_arrKey[index+1];
		
		if( animationTime <  pKey2->AnmTick)
		{
			break;
		}
	}

	float fValue=GetInterpolateValue(pKey1->AnmTick,pKey2->AnmTick,animationTime);
	
	out = pKey1->mat * (1.0f-fValue);
	out += pKey2->mat * fValue;
}

EntityAnimation::EntityAnimation(void)
{
	
}

EntityAnimation::~EntityAnimation(void)
{
	for ( auto it = m_container.begin() ; it != m_container.end() ; it++)
	{
		delete it->second;
	}
	m_container.clear();
}


BOOL EntityAnimation::Create()
{
	assert(m_RefCounter>=0);
	if (m_RefCounter > 0 )
		return TRUE;

	return TRUE;
}

void EntityAnimation::Free()
{
	cResourceMng::m_pInstance->EraseEntityAnimation(GetUniqueKey());
	delete this;
}

SceneAnimation* EntityAnimation::CreateSceneAnimation( std::string& nodeName )
{
	SceneAnimation* pSceneAnimation = new SceneAnimation;
	auto pairIB = m_container.insert(make_pair(nodeName,pSceneAnimation));
	assert(pairIB.second!=false);
	return pSceneAnimation;
}

SceneAnimation* EntityAnimation::GetSceneAnimtion( std::string& nodeNAme )
{
	SceneAnimation* pSceneAnimation = NULL;
	auto it = m_container.find(nodeNAme);
	if (it != m_container.end())
	{
		pSceneAnimation = it->second;
	}
	return pSceneAnimation;
}

void EntityAnimation::SerializeIn( std::ifstream& stream )
{
	unsigned short count = 0;
	stream.read((char*)&count,sizeof(count));
	if (count==0)
		return;

	for ( unsigned short i=0 ; i<count ; i++)
	{
		std::string nodeName;
		ReadString(stream,nodeName);
		SceneAnimation* pSceneAnimation = CreateSceneAnimation(nodeName);
		pSceneAnimation->SerializeIn(stream);
	}
	stream.read((char*)&m_dwTimeLength,sizeof(m_dwTimeLength));	
}

void EntityAnimation::SerializeOut( std::ofstream& stream )
{
	unsigned short count = 0;
	count = m_container.size();
	stream.write((char*)&count,sizeof(count));
	if (count==0)
		return;

	for ( auto it = m_container.begin() ; it != m_container.end() ; it++)
	{
		WriteString(stream,std::string(it->first));
		(it->second)->SerializeOut(stream);
	}
	stream.write((char*)&m_dwTimeLength,sizeof(m_dwTimeLength));
}

void EntityAnimation::Cut( DWORD start,DWORD end,EntityAnimation* pOut )
{
	pOut->m_dwTimeLength = end - start;
	for ( auto it = m_container.begin() ; it != m_container.end() ; it++)
	{
		SceneAnimation* pSceneAnimationOut = pOut->CreateSceneAnimation(std::string(it->first));
		(it->second)->Cut(start,end,pSceneAnimationOut);
	}
}

}