#pragma once
#include "Resource.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/Restore.h"

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


// ���� ���ؽ��� �������������� �ϴ� �����ڴ´�.
class cRscVertexBuffer:
		public cResource,
		public cIRestore,
		private cStaticD3DGraphics,
		private cStaticD3DDEVICE9
{
public:
	cRscVertexBuffer(void);
	virtual ~cRscVertexBuffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVertexBuffer;
	DWORD					m_BufferSize;

public:	

	LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

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
