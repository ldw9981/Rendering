#pragma once
#include "Resource/Resource.h"
#include "Resource/Restore.h"

namespace Sophia
{



// ���� ���ؽ��� �������������� �ϴ� �����ڴ´�.
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
	D3DPOOL					m_pool;
	int						m_nCount;
	DWORD					m_usage;
	UINT					m_OffSet;
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


	void*					Lock(UINT SizeToLock,DWORD Flags);
	void					Unlock();
	void					SetStreamSource(UINT streamNumber,UINT stride);
	void					SetStreamSourceFreq(UINT streamNumber,UINT setting);

	D3DPOOL					GetPool() const { return m_pool; }
	void					SetPool(D3DPOOL val) { m_pool = val; }
	DWORD GetUsage() const { return m_usage; }
	void SetUsage(DWORD val) { m_usage = val; }
};

}