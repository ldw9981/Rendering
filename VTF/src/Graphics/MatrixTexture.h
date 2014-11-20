#pragma once
#include "rsctexture.h"
#include "RendererQueue.h"

namespace Sophia
{
class MatrixTexture :
	public cRscTexture
{
public:
	MatrixTexture(void);
	virtual ~MatrixTexture(void);

public:
	SCENE_KEY	m_key;
	bool		m_valid;
	DWORD		m_size;
	DWORD GetSize() const { return m_size; }
	void SetSize(DWORD val);
	const Sophia::SCENE_KEY GetKey() const { return m_key; }
	void SetKey(Sophia::SCENE_KEY& val) { m_key = val; }
public:
	void Free();

	bool GetValid() const { return m_valid; }
	void SetValid(bool val) { m_valid = val; }

};

}