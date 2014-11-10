#pragma once
#include "rsctexture.h"
#include "RendererQueue.h"

namespace Sophia
{
class BoneStreamTexture :
	public cRscTexture
{
public:
	BoneStreamTexture(void);
	virtual ~BoneStreamTexture(void);

public:
	UINT		m_size;

	SCENE_KEY	m_key;
	bool		m_valid;

	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
public:
	void Free();

	bool GetValid() const { return m_valid; }
	void SetValid(bool val) { m_valid = val; }
	UINT GetSize() const { return m_size; }
	void SetSize(UINT val) { m_size = val; m_width=m_size; m_height = m_size;}
};

}