#pragma once
#include "Resource/Resource.h"
#include "Resource/Restore.h"
#include "D3D9Server/StaticD3DDevice9.h"

struct TEXCOORD
{
	float u;
	float v;
	TEXCOORD()
	{
		u=0.0f; v=0.0f;
	}
	TEXCOORD(float fU,float fV)
	{
		u=fU; v=fV;
	}
	bool operator==(const TEXCOORD &Other)
	{
		if (u == Other.u && v == Other.v)
		{
			return true;
		}
		return false;
	}
};

struct NORMALVERTEX
{	
	D3DXVECTOR3	vertex;		//x,y,z
	D3DXVECTOR3 normal;		//normal	
	TEXCOORD	tex;
	D3DXVECTOR3 tangent;	
	D3DXVECTOR3 binormal;	
};

struct BLENDVERTEX
{
	//x,y,z
	D3DXVECTOR3	vertex;
	// weight
	float		weight[3];	
	// index
	BYTE		index[4];
	//normal
	D3DXVECTOR3 normal;
	TEXCOORD	tex;
	D3DXVECTOR3 tangent;	
	D3DXVECTOR3 binormal;	

	void SetWeight(float src_weight[3])
	{
		memcpy(&weight[0],&src_weight[0],sizeof(float)*3);
	}
	void SetIndex(BYTE src_index[4])
	{
		memcpy(&index[0],&src_index[0],sizeof(BYTE)*4);
	}
};

#define FVF_NORMAL			(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_NORMAL|D3DFVF_TEX2|D3DFVF_TEX3)  
#define FVF_NORMAL_BINORMAL (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0))  

#define FVF_BLEND			(D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEX2|D3DFVF_TEX3)
#define FVF_BLEND_BINORMAL	(D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL|D3DFVF_TEX1)


// 블렌드 버텍스랑 나뉠스도있지만 일단 때려박는다.
class cRscVertexBuffer:
		public cResource,
		public cIRestore,
		private StaticD3DDEVICE9
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
	virtual void			ProcessMakeUniqueKey();  
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
