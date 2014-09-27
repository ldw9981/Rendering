#pragma once
#include "SceneNode.h"

namespace Sophia
{

class Skeleton : public cSceneNode
{
public:
	Skeleton(void);
	virtual ~Skeleton(void);

	virtual void SerializeIn( std::ifstream& stream );
};

}