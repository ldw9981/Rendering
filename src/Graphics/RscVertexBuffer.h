#pragma once
#include "Resource/Resource.h"
#include "Resource/Restore.h"


// 블렌드 버텍스랑 나뉠스도있지만 일단 때려박는다.
class cRscVertexBuffer:
		public cResource,
		public cIRestore
{
public:
	cRscVertexBuffer(void);
	virtual ~cRscVertexBuffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVertexBuffer;
	DWORD					m_BufferSize;
	D3DPOOL					m_Type;
	int						m_nCount;

public:	
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }

	int GetCount() const { return m_nCount; }
	void SetCount(int val) { m_nCount = val; }


	void*					Lock();
	void					Unlock();
	void					SetStreamSource(UINT stride);
	D3DPOOL					GetType() const { return m_Type; }
	void					SetType(D3DPOOL val) { m_Type = val; }
};
