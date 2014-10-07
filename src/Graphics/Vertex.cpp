#include "StdAfx.h"
#include "Vertex.h"

namespace Sophia
{

D3DVERTEXELEMENT9 declNormal[] = {  
	{0,  0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 12, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 24, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 36, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0, 48, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0, 56, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	D3DDECL_END()  
};  

D3DVERTEXELEMENT9 declBlend[] = {
	{0,  0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 12, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 24, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 36, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0, 48, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0, 56, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	{0, 64, D3DDECLTYPE_FLOAT3,		   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT ,0},
	{0, 76, D3DDECLTYPE_UBYTE4,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0},
	D3DDECL_END()  
};  

D3DVERTEXELEMENT9 declInstance[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 12, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 24, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 36, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0, 48, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0, 56, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1}, 
	{1, 72, D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	{1, 88, D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
	{1,104, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
	{1,120, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5},
	D3DDECL_END()
};

}