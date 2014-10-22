#pragma once

namespace Sophia
{


extern D3DVERTEXELEMENT9 declNormal[];
extern D3DVERTEXELEMENT9 declBlend[];
extern D3DVERTEXELEMENT9 declNormalInstance[];
extern D3DVERTEXELEMENT9 declBlendInstance[];
#define FVF_GUIVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)  

struct LINEVERTEX 
{ 
	D3DXVECTOR3 pos; 
	D3DCOLOR color; 
}; 

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

struct NORMALINSTANCEVERTEX
{
	D3DXVECTOR3 instanceMatrix0;
	D3DXVECTOR3 instanceMatrix1;
	D3DXVECTOR3 instanceMatrix2;
	D3DXVECTOR3 instanceMatrix3;
};

struct GUIVERTEX
{
	//x,y,z
	D3DXVECTOR4	vertex;
	TEXCOORD	tex;
};

struct BLENDINSTANCEVERTEX
{		
	float instanceIndex;	
	float a;	
	float b;	
};

}