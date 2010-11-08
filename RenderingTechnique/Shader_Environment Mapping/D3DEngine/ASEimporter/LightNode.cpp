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

	//���� ��Ʈ���� �ʱ�ȭ	
	if (IsParentObject())
	{
		m_LocalTM = m_WorldTM;
	}
	else
	{	// m_LocalTM �� �θ������ ��� ��ȯ���� �����Ѵ�. 
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