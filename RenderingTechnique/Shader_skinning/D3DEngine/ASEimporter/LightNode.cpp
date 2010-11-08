#include "StdAfx.h"
#include "LightNode.h"

cLightNode::cLightNode(void)
{
	SetNodeType(LIGHT);
}

cLightNode::~cLightNode(void)
{
}

void cLightNode::Init()
{	
	InitChildren();
}

void cLightNode::Uninit()
{
	UninitChildren();
}

void cLightNode::Update(DWORD elapseTime)
{
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm(elapseTime);
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
	UpdateChildren(elapseTime);
}

void cLightNode::Render()
{
	RenderChildren();
}