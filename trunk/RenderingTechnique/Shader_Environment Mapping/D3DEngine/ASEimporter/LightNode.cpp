#include "StdAfx.h"
#include "LightNode.h"

cLightNode::cLightNode(void)
{
	SetNodeType(LIGHT);
}

cLightNode::~cLightNode(void)
{
}

BOOL cLightNode::LoadObjInfo( cASEParser* pParser )
{

	return TRUE;
}

void cLightNode::Init()
{

	//로컬 매트릭스 초기화	
	if (IsParentObject())
	{
		m_LocalTM = m_WorldTM;
	}
	else
	{	// m_LocalTM 을 부모기준의 상대 변환으로 설정한다. 
		m_LocalTM =	m_WorldTM * GetParentPtr()->GetWorldInverseTM();	
	}

	CalculateANMKEY(INTERPOLATE_TIME);

	InitChildren();
}

void cLightNode::Uninit()
{

}

void cLightNode::Update()
{
	if (GetIsActiveAnimation())
	{
		SetAnimationTM();
		m_LocalTM = GetAnimationTM();
	}
	if (IsParentObject())	
	{		
		m_WorldTM = m_LocalTM;	
	}
	else
	{			
		m_WorldTM = m_LocalTM * GetParentPtr()->GetWorldTM();
	}	
	UpdateChildren();
}

void cLightNode::Render()
{
	RenderChildren();
}