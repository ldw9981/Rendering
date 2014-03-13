#include "StdAfx.h"
#include "Skeleton.h"
#include "Graphics/Entity.h"

namespace Sophia
{
Skeleton::Skeleton(void)
{
	m_type = TYPE_SKELETON;
	m_bShow = false;
}


Skeleton::~Skeleton(void)
{
}

void Skeleton::SerializeIn( std::ifstream& stream )
{
	cMeshNode::SerializeIn(stream);
	m_pRootNode->InsertBone(this);
}



}