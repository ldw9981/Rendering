#include "StdAfx.h"

#include "SceneGraphNode.h"
#include "ShapeNode.h"
#include "../Geometrics/Sphere.h"

cShapeNode::cShapeNode(void)
{
	SetNodeType(SHAPE);
	m_NUMLINE=0;
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
	UpdateWorldTM(UpdateTransformAnm(elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

void cShapeNode::Render()
{
	
}



