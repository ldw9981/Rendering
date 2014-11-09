#pragma once
#include "rscvertexbuffer.h"
#include "RendererQueue.h"
namespace Sophia
{
class IndexStreamVertexBuffer :
	public cRscVertexBuffer
{
public:
	IndexStreamVertexBuffer(void);
	virtual ~IndexStreamVertexBuffer(void);

public:
	SCENE_KEY	m_key;
	bool		m_valid;

	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
public:
	void Free();

	bool GetValid() const { return m_valid; }
	void SetValid(bool val) { m_valid = val; }
};
}
