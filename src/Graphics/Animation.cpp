#include "StdAfx.h"
#include "Animation.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
namespace Sophia
{

SceneAnimation::SceneAnimation( void )
{
	m_pTimeLength = NULL;
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

void SceneAnimation::GetTransform( D3DXMATRIX& out,DWORD animationTime )
{
	D3DXMATRIX tmSCL;
	D3DXMATRIX tmROT;
	D3DXMATRIX tmPOS;	

	ANMKEY stTempAnmKey;
	GetAnimationKey(stTempAnmKey,animationTime);
	
	// 각성분에대한  TM구하기
	D3DXMatrixScaling(&tmSCL,
		stTempAnmKey.ScaleAccum.x,
		stTempAnmKey.ScaleAccum.y,
		stTempAnmKey.ScaleAccum.z);

	D3DXMatrixRotationQuaternion(&tmROT,
		&stTempAnmKey.RotationAccum);					

	D3DXMatrixTranslation(&tmPOS,
		stTempAnmKey.TranslationAccum.x,
		stTempAnmKey.TranslationAccum.y,
		stTempAnmKey.TranslationAccum.z);			

	// TM	
	out = tmSCL * tmROT * tmPOS;		
}

void SceneAnimation::SerializeIn( std::ifstream& stream )
{
	unsigned short count = 0;
	stream.read((char*)&count,sizeof(count));
	if (count==0)
		return;

	for (unsigned short i=0;i<count;i++)
	{
		ANMKEY item;
		stream.read((char*)&item,sizeof(item));
		m_arrayANMKEY.push_back(item);
	}

	
}

void SceneAnimation::SerializeOut( std::ofstream& stream )
{
	unsigned short count = 0;
	count = m_arrayANMKEY.size();
	stream.write((char*)&count,sizeof(count));
	if (count==0)
		return;

	for (unsigned short i=0;i<count;i++)
	{
		ANMKEY& item = m_arrayANMKEY[i];
		stream.write((char*)&item,sizeof(item));
	}
	
}

void SceneAnimation::Cut(DWORD timeStart,DWORD timeEnd,SceneAnimation* pOut )
{		
	ANMKEY itemFirst,itemLast;
	GetAnimationKey(itemFirst,timeStart);
	pOut->m_arrayANMKEY.push_back(itemFirst);
	
	for (unsigned short i=0;i<m_arrayANMKEY.size();i++)
	{
		ANMKEY& item = m_arrayANMKEY[i];
		if( item.AnmTick > timeStart && item.AnmTick < timeEnd)
		{
			pOut->m_arrayANMKEY.push_back(item);
		}
	}
	GetAnimationKey(itemLast,timeEnd);
	if (itemLast.AnmTick == 0)
	{
		__debugbreak();
	}
	pOut->m_arrayANMKEY.push_back(itemLast);
}

void SceneAnimation::GetAnimationKey(ANMKEY& out,DWORD animationTime)
{
	assert(m_arrayANMKEY.empty() == false);
	float fIndexRate = (float) animationTime / (float)(*m_pTimeLength);		
	assert(fIndexRate <= 1.0f);
	
	size_t nIndexMax = m_arrayANMKEY.size()-1;
	size_t nIndex = (size_t)((float)nIndexMax * fIndexRate);
	size_t nIndexPrev=nIndex;
	size_t nIndexAfter=nIndex;

	ANMKEY& inter = m_arrayANMKEY[nIndex];

	// 추정인덱스의 시간을 살표보고  크면 인덱스 -1
	if ( animationTime < inter.AnmTick)
	{
		if(nIndex>0)	nIndexPrev--;
	}
	else if( animationTime >= inter.AnmTick) 
	{
		if (nIndex < nIndexMax)		nIndexAfter++;
	}

	ANMKEY& prev = m_arrayANMKEY[nIndexPrev];
	ANMKEY& after = m_arrayANMKEY[nIndexAfter]; 

	float fValue=GetInterpolateValue(prev.AnmTick,after.AnmTick,animationTime);
	SceneAnimation::InterpolateAnimationnKey(out,prev,after,fValue);
	out.AnmTick = animationTime;
}



void SceneAnimation::InterpolateAnimationnKey( ANMKEY& out,ANMKEY& in1,ANMKEY& in2,float v )
{
	D3DXQuaternionSlerp(&out.RotationAccum,&in1.RotationAccum,&in2.RotationAccum,v);
	D3DXVec3Lerp(&out.TranslationAccum,&in1.TranslationAccum,&in2.TranslationAccum,v);
	D3DXVec3Lerp(&out.ScaleAccum,&in1.ScaleAccum,&in2.ScaleAccum,v);
}

void SceneAnimation::GetAnimationKeyByIndex( ANMKEY& out,DWORD animationTime,size_t& index )
{
	size_t indexMax = m_arrayANMKEY.size()-1;
	ANMKEY* pKey1=NULL;
	ANMKEY* pKey2=NULL;
	if (index >= indexMax)
	{
		index = 0;
	}

	if (animationTime < m_arrayANMKEY[index].AnmTick)
	{
		index = 0;
	}

	for ( ; index < indexMax ; index++)
	{
		pKey1 = &m_arrayANMKEY[index];
		pKey2 = &m_arrayANMKEY[index+1];
		
		if( animationTime <  pKey2->AnmTick)
		{
			break;
		}
	}

	float fValue=GetInterpolateValue(pKey1->AnmTick,pKey2->AnmTick,animationTime);
	SceneAnimation::InterpolateAnimationnKey(out,*pKey1,*pKey2,fValue);
	out.AnmTick = animationTime;
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
	pSceneAnimation->m_pTimeLength = &m_dwTimeLength;

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