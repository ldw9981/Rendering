#include "StdAfx.h"
#include "Animation.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

SceneAnimation::SceneAnimation( void )
{

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

void SceneAnimation::GetTransform( D3DXMATRIX& out,DWORD& animationTime, DWORD dwTimeDelta )
{
	D3DXMATRIX tmSCL;
	D3DXMATRIX tmROT;
	D3DXMATRIX tmPOS;


	animationTime += dwTimeDelta;
	animationTime %= m_dwTimeLength;

	float fIndexRate = (float)animationTime / (float)m_dwTimeLength;	
	int nIndex = (int)((float)m_arrayANMKEY.size() * fIndexRate);
	int nIndexPrev=nIndex;
	int nIndexAfter=nIndex;

	// 추정인덱스의 시간을 살표보고  크면 인덱스 -1
	if(animationTime < m_arrayANMKEY[nIndex].AnmTick)
	{
		if(nIndex>0)	nIndexPrev--;
	}
	else if( animationTime >= m_arrayANMKEY[nIndex].AnmTick) 
	{
		if (nIndex<(int)m_arrayANMKEY.size()-1)		nIndexAfter++;
	}

	float fValue=GetInterpolateValue(m_arrayANMKEY[nIndexPrev].AnmTick,m_arrayANMKEY[nIndexAfter].AnmTick,animationTime);

	ANMKEY stTempAnmKey=m_arrayANMKEY[nIndex];

	// Todo: scale이 빠졋음
	D3DXQuaternionSlerp(&stTempAnmKey.RotationAccum,&m_arrayANMKEY[nIndexPrev].RotationAccum,&m_arrayANMKEY[nIndexAfter].RotationAccum,fValue);
	D3DXVec3Lerp(&stTempAnmKey.TranslationAccum,&m_arrayANMKEY[nIndexPrev].TranslationAccum,&m_arrayANMKEY[nIndexAfter].TranslationAccum,fValue);

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

	stream.read((char*)&m_dwTimeLength,sizeof(m_dwTimeLength));	
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
	stream.write((char*)&m_dwTimeLength,sizeof(m_dwTimeLength));
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
}

