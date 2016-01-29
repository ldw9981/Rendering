#include "StdAfx.h"
#include "Skeleton.h"
#include "Graphics/SceneNode.h"
#include "Graphics/Entity.h"

namespace Sophia
{
Skeleton::Skeleton(void)
{
	m_type = SCENE_SKELETON;
	m_bShow = false;
	m_bIsActiveAnimation = true;
}


Skeleton::~Skeleton(void)
{
}

void Skeleton::SerializeIn( std::ifstream& stream )
{
	cSceneNode::SerializeIn(stream);
	m_pRootNode->InsertBone(this);
}



}