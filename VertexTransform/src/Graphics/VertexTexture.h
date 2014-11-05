#pragma once
#include "rsctexture.h"
#include "RendererQueue.h"

namespace Sophia
{

class VertexTexture :
	public cRscTexture
{
public:
	VertexTexture(void);
	virtual ~VertexTexture(void);

public:
	SCENE_KEY	m_key;
	bool	m_valid;

	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
	bool GetValid() const { return m_valid; }
	void SetValid(bool val) { m_valid = val; }
public:
	void Free();

};

}