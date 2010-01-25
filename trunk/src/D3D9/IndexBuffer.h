#pragma once
#include "../Foundation/Resource.h"
#include "../Foundation/Restore.h"
#include "SharedDevice.h"
#include <d3dx9math.h>

struct FACEINDEX16
{
	WORD index[3];	
};


struct INDEX_FACE_SUBMATERIAL 
{
	FACEINDEX16	faceIndex;
	WORD		subMaterialIndex;
	
	static BOOL LessFaceIndex(INDEX_FACE_SUBMATERIAL a,INDEX_FACE_SUBMATERIAL b)
	{
		if( a.subMaterialIndex < b.subMaterialIndex)
			return TRUE;

		return FALSE;
	}
};



class IndexBuffer:
	public cResource,
	public IRestore,
	private SharedLPDIRECT3DDEVICE9
{
public:
	IndexBuffer(void);
	virtual ~IndexBuffer(void);

private:
	LPDIRECT3DINDEXBUFFER9					m_pD3DIndexBuffer;				// D3D  Memory POOL
	DWORD									m_BufferSize;	
	D3DPOOL									m_Type;
public:
	// cResource
	virtual void			ProcessMakeUniqueKey();
	virtual	BOOL			Create();
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DINDEXBUFFER9	GetD3DIndexBuffer() const { return m_pD3DIndexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }
	void*					Lock();
	void					Unlock();
	void					SetIndices();
	D3DPOOL					GetType() const { return m_Type; }
	void					SetType(D3DPOOL val) { m_Type = val; }
};
