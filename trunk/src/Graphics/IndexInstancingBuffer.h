#pragma once
#include "rscindexbuffer.h"
#include "RendererQueue.h"
namespace Sophia
{
class IndexInstancingBuffer :
	public cRscIndexBuffer
{
public:
	IndexInstancingBuffer(void);
	virtual ~IndexInstancingBuffer(void);

public:
	cRscIndexBuffer*	m_key;
	bool		m_valid;

	const cRscIndexBuffer* GetKey() const { return m_key; }
	void SetKey(cRscIndexBuffer* val) { m_key = val; }
public:
	void Free();

	bool GetValid() const { return m_valid; }
	void SetValid(bool val) { m_valid = val; }
};
}
