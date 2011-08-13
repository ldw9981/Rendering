#include "StdAfx.h"

#include "SceneNode.h"
#include "ShapeNode.h"
#include "Math/Sphere.h"

cShapeNode::cShapeNode(void)
{
//	SetNodeType(SHAPE);
	m_NUMLINE=0;
}

cShapeNode::~cShapeNode(void)
{
}

void cShapeNode::Update(DWORD elapseTime)
{
	UpdateMatrix(UpdateTransformAnm(elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

void cShapeNode::Render()
{
	
}



