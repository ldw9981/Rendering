#pragma once
#include "rscvertexbuffer.h"

namespace Sophia
{

class InstanceDataBuffer :
	public cRscVertexBuffer
{
public:
	InstanceDataBuffer(void);
	virtual ~InstanceDataBuffer(void);

public:
	cRscVertexBuffer*	m_key;

	virtual void Free();
};

}
