#include "StdAfx.h"
#include "Math/Sphere.h"
#include "LightNode.h"


cLightNode::cLightNode(void)
{
//	SetNodeType(LIGHT);
}

cLightNode::~cLightNode(void)
{
}

void cLightNode::Update(DWORD elapseTime)
{
	UpdateMatrix(UpdateTransformAnm(elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

void cLightNode::Render()
{

}
