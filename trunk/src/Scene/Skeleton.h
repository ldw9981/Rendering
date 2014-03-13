#pragma once
#include "MeshNode.h"

namespace Sophia
{

class Skeleton : public cMeshNode
{
public:
	Skeleton(void);
	virtual ~Skeleton(void);

	virtual void SerializeIn( std::ifstream& stream );
};

}