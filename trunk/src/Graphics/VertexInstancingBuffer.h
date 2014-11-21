#pragma once
#include "rscvertexbuffer.h"
#include "RendererQueue.h"

namespace Sophia
{

class VertexInstancingBuffer :
	public cRscVertexBuffer
{
public:
	VertexInstancingBuffer(void);
	virtual ~VertexInstancingBuffer(void);

public:
	cRscVertexBuffer*	m_key;

	const cRscVertexBuffer* GetKey() const { return m_key; }
	void SetKey(cRscVertexBuffer* val) { m_key = val; }
public:
	void Free();

};

}