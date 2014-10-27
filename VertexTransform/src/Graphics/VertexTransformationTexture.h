#pragma once
#include "rsctexture.h"
#include "RendererQueue.h"

namespace Sophia
{

class VertexTransformationTexture :
	public cRscTexture
{
public:
	VertexTransformationTexture(void);
	virtual ~VertexTransformationTexture(void);

public:
	SCENE_KEY	m_key;
	
	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
public:
	void Free();

};

}