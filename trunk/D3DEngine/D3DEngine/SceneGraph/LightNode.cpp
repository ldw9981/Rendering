#include "StdAfx.h"
#include "../Geometrics/Sphere.h"
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
	UpdateWorldTM(UpdateTransformAnm(elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

