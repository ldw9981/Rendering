#pragma once
extern D3DVERTEXELEMENT9 declNormal[];
extern D3DVERTEXELEMENT9 declBlend[];

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
	D3DXVECTOR3 tangent;	
	D3DXVECTOR3 binormal;	
	TEXCOORD	uv0;	
	TEXCOORD	uv1;
};
struct BLENDVERTEX
{	
	D3DXVECTOR3	vertex;		//x,y,z
	D3DXVECTOR3 normal;		//normal
	D3DXVECTOR3 tangent;	
	D3DXVECTOR3 binormal;	
	TEXCOORD	uv0;	
	TEXCOORD	uv1;	
	float		weight[3];		
	BYTE		index[4];
	void SetWeight(float src_weight[3])
	{
		memcpy(&weight[0],&src_weight[0],sizeof(float)*3);
	}
	void SetIndex(BYTE src_index[4])
	{
		memcpy(&index[0],&src_index[0],sizeof(BYTE)*4);
	}
};


