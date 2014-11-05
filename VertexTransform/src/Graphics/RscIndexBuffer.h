#pragma once
#include "MaterialEx.h"
#include "Resource/Resource.h"
#include "Resource/Restore.h"

namespace Sophia
{


struct TRIANGLE
{
	WORD index[3];	
};


struct TRIANGLE_SUBMATERIAL 
{
	TRIANGLE	triangle;
	WORD		subMaterialIndex;
	
	static BOOL LessFaceIndex(TRIANGLE_SUBMATERIAL a,TRIANGLE_SUBMATERIAL b)
	{
		if( a.subMaterialIndex < b.subMaterialIndex)
			return TRUE;

		return FALSE;
	}
};



class cRscIndexBuffer:
	public cResource,
	public cIRestore
{
public:
	cRscIndexBuffer(void);
	virtual ~cRscIndexBuffer(void);

protected:
	LPDIRECT3DINDEXBUFFER9					m_pD3DIndexBuffer;				// D3D  Memory POOL
	DWORD									m_BufferSize;	
	D3DPOOL									m_Type;
	DWORD									m_usage;
	int										m_triangleCount;
public:
	// cResource

	virtual	BOOL			Create();
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DINDEXBUFFER9	GetD3DIndexBuffer() const { return m_pD3DIndexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }
	void*					Lock(UINT SizeToLock,DWORD Flags);
	void					Unlock();
	void					SetIndices();
	D3DPOOL					GetType() const { return m_Type; }
	void					SetType(D3DPOOL val) { m_Type = val; }
	int GetTriangleCount() const { return m_triangleCount; }
	void SetTriangleCount(int val) { m_triangleCount = val; }
};

}