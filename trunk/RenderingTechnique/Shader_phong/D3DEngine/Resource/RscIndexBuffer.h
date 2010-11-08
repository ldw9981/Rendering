#pragma once


struct FACEINDEX
{
	WORD index[3];	
};




class cRscIndexBuffer
{
public:
	cRscIndexBuffer(void);
	virtual ~cRscIndexBuffer(void);

private:
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuffer;				// D3D  Memory POOL

	LPWORD					m_pHeapIndexBuffer;		// Heap Memory

	DWORD					m_dwSizeByte;
	UINT					m_Num;
	
public:
	// cResource
	virtual	BOOL			Create();
	virtual	BOOL			Destroy();
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
};
