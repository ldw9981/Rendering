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

	struct NORMAL_VERTEX
	{	
		D3DXVECTOR3	position;		//x,y,z
		D3DXVECTOR3 normal;		//normal	
		D3DXVECTOR3 tangent;	
		D3DXVECTOR3 binormal;	
		TEXCOORD	uv0;	
		TEXCOORD	uv1;

	};

	struct NORMAL_VERTEX_INSTANCEDATA
	{	
		NORMAL_VERTEX	vertex;
		float		vertexIndex,vertexSize;
		float		instanceIndex,boneSize;
	};

	struct BLEND_VERTEX
	{	
		D3DXVECTOR3	position;		//x,y,z
		D3DXVECTOR3 normal;		//normal
		D3DXVECTOR3 tangent;	
		D3DXVECTOR3 binormal;	
		TEXCOORD	uv0;	
		TEXCOORD	uv1;	
		float		boneWeight[3];		
		BYTE		boneIndex[4];	

		void SetWeight(float src_weight[3])
		{
			memcpy(&boneWeight[0],&src_weight[0],sizeof(float)*3);
		}
		void SetIndex(BYTE src_index[4])
		{
			memcpy(&boneIndex[0],&src_index[0],sizeof(BYTE)*4);
		}
	};

	struct BLEND_VERTEX_INSTANCEDATA
	{	
		BLEND_VERTEX vertex;
		float		vertexIndex,vertexSize;
		float		instanceIndex,boneSize;
	};


	struct GUIVERTEX
	{
		//x,y,z
		D3DXVECTOR4	vertex;
		TEXCOORD	tex;
	};



}