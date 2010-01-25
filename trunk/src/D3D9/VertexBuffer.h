#pragma once
#include "../Foundation/Resource.h"
#include "../Foundation/Restore.h"
#include "SharedDevice.h"
#include <d3dx9math.h>

struct TEXCOORD:public pair<float,float>
{
	TEXCOORD()
	{
		first=0.0f; second=0.0f;
	}
	TEXCOORD(float u,float v)
	{
		first=u; second=v;
	}
};

struct NORMALVERTEX
{	
	D3DXVECTOR3	vertex;		//x,y,z
	D3DXVECTOR3 normal;		//normal
	TEXCOORD	tex;
};


struct BLENDVERTEX
{
	//x,y,z
	D3DXVECTOR3	vertex;
	// weight
	float		weight0;
	float		weight1;
	float		weight2;
	// index
	BYTE		index0;
	BYTE		index1;
	BYTE		index2;
	BYTE		index3;
	//normal
	D3DXVECTOR3 normal;
	TEXCOORD	tex;
};

#define FVF_NORMALVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)  
#define FVF_BLENDVERTEX (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL|D3DFVF_TEX1)


// 블렌드 버텍스랑 나뉠스도있지만 일단 때려박는다.
class VertexBuffer:
		public cResource,
		public IRestore,
		public SharedLPDIRECT3DDEVICE9
{
public:
	VertexBuffer(void);
	virtual ~VertexBuffer(void);

private:	
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVertexBuffer;
	DWORD					m_BufferSize;
	D3DPOOL					m_Type;
	
public:	
	// cResource
	virtual void			ProcessMakeUniqueKey();  
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }

	void*					Lock();
	void					Unlock();
	void					SetStreamSource(UINT stride);
	D3DPOOL					GetType() const { return m_Type; }
	void					SetType(D3DPOOL val) { m_Type = val; }
};
