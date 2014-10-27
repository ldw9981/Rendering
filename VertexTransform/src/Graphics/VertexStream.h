#pragma once
#include "rscvertexbuffer.h"
#include "RendererQueue.h"

namespace Sophia
{

class VertexStream :
	public cRscVertexBuffer
{
public:
	VertexStream(void);
	virtual ~VertexStream(void);

public:
	SCENE_KEY	m_key;

	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
public:
	void Free();

};

}