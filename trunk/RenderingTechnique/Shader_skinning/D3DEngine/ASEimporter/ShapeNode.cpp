#include "StdAfx.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "SceneNode.h"
#include "ShapeNode.h"

cShapeNode::cShapeNode(void)
{
	SetNodeType(SHAPE);
}

cShapeNode::~cShapeNode(void)
{
}




void cShapeNode::Init()
{
	InitChildren();
}

void cShapeNode::Uninit()
{
	UninitChildren();
}

void cShapeNode::Update(DWORD elapseTime)
{
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm( elapseTime);
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

void cShapeNode::Render()
{
	RenderChildren();
}