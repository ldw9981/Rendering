
#pragma once

struct NORMAL_VERTEX
{
	//x,y,z
	D3DXVECTOR3	vertex;
	//normal
	D3DXVECTOR3 normal;

};


struct BLEND_VERTEX
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

};

class cRscVertexBuffer
{
public:
	cRscVertexBuffer(void);
	virtual ~cRscVertexBuffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9			m_pD3DVertexBuffer;
	LPDWORD							m_pHeapVertexBuffer;

	DWORD							m_dwSizeByte;
	DWORD							m_dwSizeFormat;
	DWORD							m_FVF;
public:
	

	// cResource
	virtual	BOOL			Create();
	virtual	BOOL			Destroy();
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
};
