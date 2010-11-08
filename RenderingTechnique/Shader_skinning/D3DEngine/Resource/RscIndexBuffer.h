#pragma once
#include "../ASEimporter/MaterialNode.h"
#include "Resource.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/Restore.h"
struct FACEINDEX16
{
	WORD index[3];	
};


struct FACE_SUBMTL 
{
	FACEINDEX16	faceIndex;
	WORD		subMtlIndex;
	
	BOOL LessFaceIndex(FACE_SUBMTL a,FACE_SUBMTL b)
	{
		if( a.subMtlIndex < b.subMtlIndex)
			return TRUE;

		return FALSE;
	}
};



class cRscIndexBuffer:
	public cResource,
	public cIRestore,
	public cStaticD3DGraphics,
	public cStaticD3DDEVICE9
{
public:
	cRscIndexBuffer(void);
	virtual ~cRscIndexBuffer(void);

private:
	LPDIRECT3DINDEXBUFFER9					m_pD3DIndexBuffer;				// D3D  Memory POOL
	DWORD					m_BufferSize;	
public:
	LPDIRECT3DINDEXBUFFER9					GetD3DIndexBuffer() const { return m_pD3DIndexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }
	LPVOID					Lock();
	VOID					Unlock();

	// cResource
	virtual	BOOL			Create();

	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
};
