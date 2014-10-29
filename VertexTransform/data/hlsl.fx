// 텍스처
texture Tex_Diffuse;
texture Tex_Normal;
texture Tex_Specular;
texture Tex_Light;
texture Tex_Opacity;
texture Tex_Depth : RenderColorTarget;
texture Tex_BoneMatrix;
texture Tex_TransformedVertex;

// 변환행렬
float4x4 gWorldMatrix      : WORLD;
float4x4 gViewMatrix       : VIEW;
float4x4 gProjectionMatrix : PROJECTION;
float4x4 gViewProjectionMatrix : ViewProjection;
float4 gWorldLightPosition;
float4 gWorldCameraPosition;

float4x4 gLightViewMatrix;
float4x4 gLightProjectionMatrix : Projection;
float		gVertexTextureWidth = 1024;
float    gVertexTextureHeight = 768;

// 텍스처 샘플러
sampler gDiffuseSampler = sampler_state
{
    Texture   = (Tex_Diffuse);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gNormalSampler = sampler_state
{
    Texture   = (Tex_Normal);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gSpecularSampler = sampler_state
{
    Texture   = (Tex_Specular);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gLightSampler = sampler_state
{
    Texture   = (Tex_Light);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler gOpacitySampler = sampler_state
{
    Texture   = (Tex_Opacity);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler2D ShadowSampler = sampler_state
{
   Texture = (Tex_Depth);
};

sampler2D   gBoneSampler = sampler_state 
{
  texture = Tex_BoneMatrix;
  MipFilter = NONE;
  MagFilter = POINT;
  MinFilter = POINT;
  AddressU = Clamp;
  AddressV = Clamp;
};

sampler2D   gTransformedVertexSampler = sampler_state 
{
  texture = Tex_TransformedVertex;
  MipFilter = NONE;
  MagFilter = POINT;
  MinFilter = POINT;
  AddressU = Clamp;
  AddressV = Clamp;
};

/////////////////////
// BLENDING STATES //
/////////////////////
BlendState AlphaBlendingOn
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};

float gAmbientIntensity = 0.0f;
float4 gAmbientColor = { 0.0f, 0.0f, 0.0f, 0.0f };
float4 gSpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
float gSpecularPower = 32;


#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 64
#endif

static const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x4 Palette[ MATRIX_PALETTE_SIZE_DEFAULT ];

struct VS_GUI_INPUT
{
    float4 mPosition : POSITION;
    float2 mTexCoord : TEXCOORD0;
};

struct VS_LINE_INPUT
{
   float4 mPosition : POSITION;
   float4 mColor   : COLOR0;
};

struct VS_TERRAIN_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;    
   float2 mTexCoord : TEXCOORD0;    
};

struct VS_PHONG_DIFFUSE_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;    
   float3 mTangent : TANGENT;   
   float3 mBiNormal : BINORMAL;   
   float2 mTexCoord : TEXCOORD0;  
   float2 mTexCoord1 : TEXCOORD1;
	float2 mIndex : TEXCOORD2;
};

struct VS_PHONG_DIFFUSE_INSTANCE_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;    
   float3 mTangent : TANGENT;   
   float3 mBiNormal : BINORMAL;   
   float2 mTexCoord : TEXCOORD0;  
   float2 mTexCoord1 : TEXCOORD1;
	float2 mIndex : TEXCOORD2;
	
	
	float3 mInstanceMatrix0 : TEXCOORD5; 
   float3 mInstanceMatrix1 : TEXCOORD6; 
   float3 mInstanceMatrix2 : TEXCOORD7; 
	float3 mInstanceMatrix3 : TEXCOORD8; 
	float2 mInstanceIndex  : TEXCOORD4; 
};

struct VS_SKINNING_PHONG_DIFFUSE_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
   float2 mTexCoord : TEXCOORD0;
   float2 mTexCoord1 : TEXCOORD1;   
	float2 mIndex : TEXCOORD2;
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 
};

struct VS_SKINNING_PHONG_DIFFUSE_INSTANCING_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
   float2 mTexCoord : TEXCOORD0;
   float2 mTexCoord1 : TEXCOORD1;   
	float2 mIndex : TEXCOORD2;
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 	
	float2 mInstanceIndex  : TEXCOORD4; 
};

struct VS_SHADOW_NORMAL_INPUT 
{
   float4 mPosition: POSITION;
	float2 mTexCoord : TEXCOORD0;
	float2 mIndex : TEXCOORD2;
};

struct VS_SHADOW_NORMAL_INSTANCING_INPUT 
{
   float4 mPosition: POSITION;
	float2 mTexCoord : TEXCOORD0;
	float2 mIndex : TEXCOORD2;
	float3 mInstanceMatrix0 : TEXCOORD5; 
   float3 mInstanceMatrix1 : TEXCOORD6; 
   float3 mInstanceMatrix2 : TEXCOORD7; 
	float3 mInstanceMatrix3 : TEXCOORD8; 	
};

struct VS_SHADOW_SKINNED_INSTANCING_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
   float2 mTexCoord : TEXCOORD0;
   float2 mTexCoord1 : TEXCOORD1;   
	float2 mIndex : TEXCOORD2;
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 
	float2 mInstanceIndex  : TEXCOORD4; 
};

struct VS_VERTEX_TRANSFORMATION_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL; 
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 
	float2 mIndex : TEXCOORD2; 
	
	float2 mInstanceIndex  : TEXCOORD4; 
	float3 mInstanceMatrix0 : TEXCOORD5; 
   float3 mInstanceMatrix1 : TEXCOORD6; 
   float3 mInstanceMatrix2 : TEXCOORD7; 
	float3 mInstanceMatrix3 : TEXCOORD8; 	
};


struct VS_GUI_OUTPUT
{
    float4 mPosition  : POSITION;
    float2 mTexCoord : TEXCOORD0;
};

struct VS_LINE_OUTPUT
{
   float4 mPosition : POSITION;
   float4 mColor   : COLOR0;
};

struct VS_PHONG_DIFFUSE_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
   float4 mLambert : TEXCOORD1;  
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mReflect : TEXCOORD4;
   float2 mTexCoord1 : TEXCOORD6;
   float4 mClipPosition: TEXCOORD7;
};

struct VS_PHONG_DIFFUSE_BUMP_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
   float3 mLightDir : TEXCOORD1;  
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mTangent : TEXCOORD4;
   float3 mBiNormal : TEXCOORD5;
   float2 mTexCoord1 : TEXCOORD6;
   float4 mClipPosition: TEXCOORD7;
};

struct VS_SHADOW_OUTPUT 
{
   float4 mPosition: POSITION;
	float2 mTexCoord : TEXCOORD0;
   float4 mClipPosition: TEXCOORD7;
};

struct VS_VERTEX_TRANSFORMATION_OUTPUT
{
   float4 mTexturePosition : POSITION;

	float3 mTransformedPosition: TEXCOORD1;
   float3 mNormal : TEXCOORD2;
   float3 mTangent : TEXCOORD4;
   float3 mBiNormal : TEXCOORD5;
};


//------------------------------------------------------------------------------
VS_GUI_OUTPUT vs_GUI(VS_GUI_INPUT input)
{
    VS_GUI_OUTPUT output;
    float4 worldPosition = mul(input.mPosition , gWorldMatrix);
    output.mPosition = mul(worldPosition , gViewProjectionMatrix);
    output.mTexCoord = input.mTexCoord;   
    return output;
}

VS_LINE_OUTPUT vs_Line( VS_LINE_INPUT input)
{
   VS_LINE_OUTPUT output;
   float4 worldPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(worldPosition , gViewProjectionMatrix);
   output.mColor = input.mColor;
   return output;
}





VS_PHONG_DIFFUSE_OUTPUT vs_Terrain( VS_TERRAIN_INPUT input)
{
   VS_PHONG_DIFFUSE_OUTPUT output;
   float4 worldPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(worldPosition , gViewProjectionMatrix);
   
   float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   float3 worldNormal = mul(input.mNormal,(float3x3)gWorldMatrix);
   worldNormal = normalize(worldNormal);
      
   output.mLambert = dot(-lightDir, worldNormal);
   output.mNormal = worldNormal;
   output.mCameraDir = cameraDir;
   output.mReflect = reflect(lightDir, worldNormal);
   output.mTexCoord = input.mTexCoord;   
   output.mTexCoord1 = input.mTexCoord;   

   output.mClipPosition = mul(worldPosition, gLightViewMatrix);
   output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix); 
   return output;
}



VS_PHONG_DIFFUSE_OUTPUT vs_PhongDiffuse( VS_PHONG_DIFFUSE_INPUT input)
{
   VS_PHONG_DIFFUSE_OUTPUT output;
   float4 worldPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(worldPosition , gViewProjectionMatrix);
   
   float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   float3 worldNormal = mul(input.mNormal,(float3x3)gWorldMatrix);
   worldNormal = normalize(worldNormal);
      
   output.mLambert = dot(-lightDir, worldNormal);
   output.mNormal = worldNormal;
   output.mCameraDir = cameraDir;
   output.mReflect = reflect(lightDir, worldNormal);
   output.mTexCoord = input.mTexCoord;   
   output.mTexCoord1 = input.mTexCoord1;   
   
   output.mClipPosition = mul(worldPosition, gLightViewMatrix);
   output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix); 
   return output;
}

float4x4 loadTransformedVertex(float vertexIndex,float vertexSize,float instanceIndex)
{	
	float instanceOffSet = instanceIndex * vertexSize + vertexIndex; 
	float quotient = floor(instanceOffSet / gVertexTextureWidth);	//0~N
	float2 texcoord;	
	texcoord.y = quotient/gVertexTextureHeight;		//
	texcoord.x = (instanceOffSet - gVertexTextureWidth *quotient) / gVertexTextureWidth;

	float4x4 mat = 
	{
		tex2Dlod(gTransformedVertexSampler, float4(texcoord,0,0)),
		tex2Dlod(gTransformedVertexSampler, float4(texcoord,0,1)),
		tex2Dlod(gTransformedVertexSampler, float4(texcoord,0,2)),
		tex2Dlod(gTransformedVertexSampler, float4(texcoord,0,3))
	};
	return mat; 	
}


VS_PHONG_DIFFUSE_OUTPUT vs_PhongDiffuse_Instancing( VS_PHONG_DIFFUSE_INSTANCE_INPUT input)
{
   VS_PHONG_DIFFUSE_OUTPUT output;
/* float4
	0. position
	1. normal
	2. tangent
	3. binormal
*/
	float4x4 transformedVertex = loadTransformedVertex(input.mIndex.x,input.mIndex.y,input.mInstanceIndex.x);
	
   float4 worldPosition = transformedVertex[0];	//position
   output.mPosition = mul(worldPosition , gViewProjectionMatrix);
   
   float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   float3 worldNormal = transformedVertex[1];	//normal
   worldNormal = normalize(worldNormal);
      
   output.mLambert = dot(-lightDir, worldNormal);
   output.mNormal = worldNormal;
   output.mCameraDir = cameraDir;
   output.mReflect = reflect(lightDir, worldNormal);
   output.mTexCoord = input.mTexCoord;   
   output.mTexCoord1 = input.mTexCoord1;   
   
   output.mClipPosition = mul(worldPosition, gLightViewMatrix);
   output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix); 
   return output;
}



VS_PHONG_DIFFUSE_BUMP_OUTPUT vs_PhongDiffuseBump( VS_PHONG_DIFFUSE_INPUT input)
{
   VS_PHONG_DIFFUSE_BUMP_OUTPUT output;
   
   float4 worldPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(worldPosition , gViewProjectionMatrix);

   output.mLightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   output.mNormal = normalize(mul(input.mNormal,(float3x3)gWorldMatrix));
   output.mCameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   output.mTexCoord = input.mTexCoord;
   output.mTexCoord1 = input.mTexCoord1;   
   
   float3 worldTangent = mul(input.mTangent,(float3x3)gWorldMatrix);
   output.mTangent = normalize(worldTangent);
   
   float3 worldBiNormal = mul(input.mBiNormal,(float3x3)gWorldMatrix);
   output.mBiNormal = normalize(worldBiNormal);
   
   output.mClipPosition = mul(worldPosition, gLightViewMatrix);
   output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix);
   
   return output;
}



VS_PHONG_DIFFUSE_OUTPUT vs_SkinningPhongDiffuse( VS_SKINNING_PHONG_DIFFUSE_INPUT input)
{
    VS_PHONG_DIFFUSE_OUTPUT output;
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;
	
	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);

	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x, Palette[input.mBlendIndices.x]);
	matWorldSkinned += mul(input.mBlendWeights.y, Palette[input.mBlendIndices.y]);
	matWorldSkinned += mul(input.mBlendWeights.z, Palette[input.mBlendIndices.z]);
	matWorldSkinned += mul(fLastWeight, Palette[input.mBlendIndices.w]);		
   
	float4 worldPosition = mul(input.mPosition , matWorldSkinned);	
    output.mPosition = mul(worldPosition , gViewProjectionMatrix);
    
    float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
    float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
    float3 worldNormal = mul(input.mNormal,(float3x3)matWorldSkinned);
    worldNormal = normalize(worldNormal);
       
    output.mLambert = dot(-lightDir, worldNormal);
    output.mNormal = worldNormal;
    output.mCameraDir = cameraDir;
    output.mReflect = reflect(lightDir, worldNormal);
    output.mTexCoord = input.mTexCoord;   
	output.mTexCoord1 = input.mTexCoord1;   
	
    output.mClipPosition = mul(worldPosition, gLightViewMatrix);
    output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix);
	
    return output;
}

float4x4 loadBoneMatrix(int indexInstance,int indexBone)
{	
	int numBonePerLine=64;
	float width= 256.0f;	// 1Matrix = 4 , 256 Matrix = 1024  
	float height = 128.0f;	// 128 Instance
	float4 uvCol = float4( indexBone*4/width, (float)indexInstance/ height, 0.0f, 0.0f);
	float4x4 mat = 
	{
		tex2Dlod(gBoneSampler, uvCol + float4(0.0f 			,0,0,0)),
		tex2Dlod(gBoneSampler, uvCol + float4(1.0f / width	,0,0,0)),
		tex2Dlod(gBoneSampler, uvCol + float4(2.0f / width	,0,0,0)),
		tex2Dlod(gBoneSampler, uvCol + float4(3.0f / width	,0,0,0))
	};
 
	return mat; 	
}

VS_PHONG_DIFFUSE_OUTPUT vs_SkinningPhongDiffuseInstancing( VS_SKINNING_PHONG_DIFFUSE_INSTANCING_INPUT input)
{
    VS_PHONG_DIFFUSE_OUTPUT output;
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;

	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);
		
	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.x));
	matWorldSkinned += mul(input.mBlendWeights.y,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.y));
   matWorldSkinned += mul(input.mBlendWeights.z,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.z));
 	matWorldSkinned += mul(fLastWeight,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.w));
   
	float4 worldPosition = mul(input.mPosition , matWorldSkinned);	
    output.mPosition = mul(worldPosition , gViewProjectionMatrix);
    
    float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
    float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
    float3 worldNormal = mul(input.mNormal,(float3x3)matWorldSkinned);
    worldNormal = normalize(worldNormal);
       
    output.mLambert = dot(-lightDir, worldNormal);
    output.mNormal = worldNormal;
    output.mCameraDir = cameraDir;
    output.mReflect = reflect(lightDir, worldNormal);
    output.mTexCoord = input.mTexCoord;   
	output.mTexCoord1 = input.mTexCoord1;   
	
    output.mClipPosition = mul(worldPosition, gLightViewMatrix);
    output.mClipPosition = mul(output.mClipPosition, gLightProjectionMatrix);	 
    return output;
}

VS_SHADOW_OUTPUT vs_Shadow_Normal( VS_SHADOW_NORMAL_INPUT Input )
{
   VS_SHADOW_OUTPUT Output;
 
   Output.mPosition = mul(Input.mPosition, gWorldMatrix);
   Output.mPosition = mul(Output.mPosition, gLightViewMatrix);
   Output.mPosition = mul(Output.mPosition, gLightProjectionMatrix);

   Output.mClipPosition = Output.mPosition;
	Output.mTexCoord = Input.mTexCoord;   
   
   return Output;
}

VS_SHADOW_OUTPUT vs_Shadow_Normal_Instancing( VS_SHADOW_NORMAL_INSTANCING_INPUT input )
{
   VS_SHADOW_OUTPUT output;
 
 	float4x4 mInstanceMatrix = float4x4( float4(input.mInstanceMatrix0,0.0f),
													float4(input.mInstanceMatrix1,0.0f),
													float4(input.mInstanceMatrix2,0.0f),
													float4(input.mInstanceMatrix3,1.0f));

   output.mPosition = mul(input.mPosition, mInstanceMatrix);
   output.mPosition = mul(output.mPosition, gLightViewMatrix);
   output.mPosition = mul(output.mPosition, gLightProjectionMatrix);

   output.mClipPosition = output.mPosition;
	output.mTexCoord = input.mTexCoord;   
   
   return output;
}

VS_SHADOW_OUTPUT vs_Shadow_Skinned_Instancing( VS_SHADOW_SKINNED_INSTANCING_INPUT input )
{
    VS_SHADOW_OUTPUT output;
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;

	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);
		
	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.x));
	matWorldSkinned += mul(input.mBlendWeights.y,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.y));
   matWorldSkinned += mul(input.mBlendWeights.z,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.z));
 	matWorldSkinned += mul(fLastWeight,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.w));
   
   output.mPosition = mul(input.mPosition, matWorldSkinned);
   output.mPosition = mul(output.mPosition, gLightViewMatrix);
   output.mPosition = mul(output.mPosition, gLightProjectionMatrix);

   output.mClipPosition = output.mPosition;
	output.mTexCoord = input.mTexCoord;   
	return output;
}

VS_SHADOW_OUTPUT vs_Shadow_Skinning( VS_SKINNING_PHONG_DIFFUSE_INPUT input )
{
    VS_SHADOW_OUTPUT output;
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;
	
	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);

	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x, Palette[input.mBlendIndices.x]);
	matWorldSkinned += mul(input.mBlendWeights.y, Palette[input.mBlendIndices.y]);
	matWorldSkinned += mul(input.mBlendWeights.z, Palette[input.mBlendIndices.z]);
	matWorldSkinned += mul(fLastWeight, Palette[input.mBlendIndices.w]);		
   
	float4 worldPosition = mul(input.mPosition , matWorldSkinned);	
    output.mPosition = mul(worldPosition, gLightViewMatrix);
    output.mPosition = mul(output.mPosition, gLightProjectionMatrix);
    output.mClipPosition = output.mPosition;  
	 output.mTexCoord = input.mTexCoord;   
    return output;
}

VS_VERTEX_TRANSFORMATION_OUTPUT vs_VertexTransformation(VS_VERTEX_TRANSFORMATION_INPUT input)
{ 
	VS_VERTEX_TRANSFORMATION_OUTPUT output;
	
	float4x4 mInstanceMatrix = float4x4( float4(input.mInstanceMatrix0,0.0f),
													float4(input.mInstanceMatrix1,0.0f),
													float4(input.mInstanceMatrix2,0.0f),
													float4(input.mInstanceMatrix3,1.0f));
	
	float instanceOffSet = input.mInstanceIndex.x * input.mIndex.y + input.mIndex.x; 
	float quotient = floor(instanceOffSet / gVertexTextureWidth);	//0~N
	float2 texcoord;	
	texcoord.y = quotient/gVertexTextureHeight;		//
	texcoord.x = (instanceOffSet - gVertexTextureWidth *quotient) / gVertexTextureWidth;
	
	float2 outputPos = texcoord;
	
	outputPos.x = outputPos.x * 2.0f - 1.0f;
	outputPos.y = outputPos.y * -2.0f + 1.0f;	
	output.mTexturePosition = float4( outputPos,0.0f,1.0f);
		
	
	// 센터가 0.0  Left,Top(-1.0f,1.0f)  RIGHT,BOTTOM(1.0f,-1.0f);

	//output.mTexturePosition = float4( input.mIndex.x/input.mIndex.y,input.mInstanceIndex.x/768.0f,0.0f,1.0f);
	output.mTransformedPosition = mul(input.mPosition, mInstanceMatrix);	
	output.mNormal = mul(input.mNormal,(float3x3)mInstanceMatrix);
	output.mTangent = mul(input.mTangent,(float3x3)mInstanceMatrix);
	output.mBiNormal = mul(input.mBiNormal,(float3x3)mInstanceMatrix);		
	
	return output;
}



struct PS_PHONG_DIFFUSE_INPUT
{ 
   float2 mTexCoord : TEXCOORD0;
   float3 mLambert : TEXCOORD1;  
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mReflect : TEXCOORD4;
   float2 mTexCoord1 : TEXCOORD6;   
   float4 mClipPosition : TEXCOORD7;
};

struct PS_PHONG_DIFFUSE_BUMP_INPUT
{ 
   float2 mTexCoord : TEXCOORD0;
   float3 mLightDir : TEXCOORD1;
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mTangent : TEXCOORD4;
   float3 mBiNormal : TEXCOORD5;
   float2 mTexCoord1 : TEXCOORD6;  
   float4 mClipPosition : TEXCOORD7;
};

struct PS_SHADOW_INPUT 
{
	float2 mTexCoord : TEXCOORD0;
	float4 mClipPosition: TEXCOORD7;
};

struct PS_VERTEX_TRANSFORMATION_INPUT
{
	float3 mTransformedPosition: TEXCOORD1;
   float3 mNormal : TEXCOORD2;
   float3 mTangent : TEXCOORD4;
   float3 mBiNormal : TEXCOORD5;
};

struct PS_VERTEX_TRANSFORMATION_OUTPUT
{
	float4 mTransformedPosition : COLOR0;
	float4 mNormal : COLOR1;
	float4 mTangent : COLOR2;
	float4 mBiNormal : COLOR3;
};

float4 ps_GUI(VS_GUI_OUTPUT input) : COLOR
{
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   return float4(diffuseSample,0.0f);
}

float4 ps_Phong(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = float3(1.0f,1.0f,1.0f);
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
  
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
   
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   }
   else
   {
      color += gSpecularColor.xyz * specular;   
   }
   
   return float4(color,0.0f);
}

float4 ps_DebugColor( PS_PHONG_DIFFUSE_INPUT input)  : COLOR
{
	return input.mClipPosition;
}

float4 ps_Line(float4 Color: COLOR) : COLOR
{   
   return Color;
}
float4 ps_Terrain(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
  
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * 1.0f;
  
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   } 
   else
   {
      //color += gSpecularColor.xyz * specular;
   }
   
   return float4(color,0.0f);
}


float4 ps_PhongDiffuse(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
  
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
  
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   } 
   else
   {
      color += gSpecularColor.xyz * specular;
   }
   
   return float4(color,0.0f);
}

float4 ps_PhongDiffuseBump(PS_PHONG_DIFFUSE_BUMP_INPUT input) : COLOR
{  
   float3 color;  
   float3 cameraDir = normalize(input.mCameraDir);
   float3 tangentNormal = tex2D(gNormalSampler,input.mTexCoord);
   tangentNormal = normalize(tangentNormal*2 - 1);
   float3x3 matTBN = float3x3(normalize(input.mTangent),normalize(input.mBiNormal),normalize(input.mNormal));
   matTBN = transpose(matTBN);
  
   float3 worldNormal = mul(matTBN,tangentNormal);
   float3 lambert = saturate(dot(-input.mLightDir,worldNormal));
   float3 reflectDir = normalize(reflect(input.mLightDir, worldNormal)); 
   
   float3 diffuseSample = tex2D( gDiffuseSampler,input.mTexCoord);  
  
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);   
 
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
   
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   }
   else
   {
      color += gSpecularColor.xyz * specular;
   }
  
   
   return float4(color,0.0f);
}


float4 ps_PhongDiffuseLight(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   float3 lightSample = tex2D( gLightSampler , input.mTexCoord1 );
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
    
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;   
   color += diffuseSample * lightSample; 
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   }
   else
   {
       color += gSpecularColor.xyz * specular;
   }   
   return float4(color,0.0f);	
}

float4 ps_PhongDiffuseOpacity(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   float alphaSample = tex2D( gOpacitySampler , input.mTexCoord ).a;

	
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
  
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
  
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   } 
   else
   {
      color += gSpecularColor.xyz * specular;
   }
	  
	return float4(color,alphaSample);	
}

float4 ps_PhongDiffuseSpecular(PS_PHONG_DIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 diffuseSample = tex2D( gDiffuseSampler , input.mTexCoord );
   	
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
   specular = specular * specularIntensity.xyz;
  
   color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
  
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   } 
   else
   {
      color += gSpecularColor.xyz * specular;
   }
	  
	return float4(color,0.0f);	
}

float4 ps_PhongDiffuseBumpSpecular(PS_PHONG_DIFFUSE_BUMP_INPUT input) : COLOR
{  
   float3 color;  
   float3 cameraDir = normalize(input.mCameraDir);
   float3 tangentNormal = tex2D(gNormalSampler,input.mTexCoord);
   tangentNormal = normalize(tangentNormal*2 - 1);
   float3x3 matTBN = float3x3(normalize(input.mTangent),normalize(input.mBiNormal),normalize(input.mNormal));
   matTBN = transpose(matTBN);
  
   float3 worldNormal = mul(matTBN,tangentNormal);
   float3 lambert = saturate(dot(-input.mLightDir,worldNormal));
   float3 reflectDir = normalize(reflect(input.mLightDir, worldNormal)); 
   
   float3 diffuseSample = tex2D( gDiffuseSampler,input.mTexCoord);  
  
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);   
	float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
   specular = specular * specularIntensity.xyz;
	
	color = diffuseSample * gAmbientColor.xyz * gAmbientIntensity;
   color += diffuseSample * lambert;
   
   
   float currentDepth = input.mClipPosition.z / input.mClipPosition.w;   
   float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;   
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;   
   if (currentDepth > shadowDepth + 0.00125f)
   {
      color *= 0.5f;
   }
   else
   {
      color += gSpecularColor.xyz * specular;
   }
  
   
   return float4(color,0.0f);
}


float4 ps_Shadow(PS_SHADOW_INPUT Input) : COLOR
{   
   float depth = Input.mClipPosition.z / Input.mClipPosition.w;
   return float4(depth.xxx, 0.0f);
}

float4 ps_Shadow_AlphaTest(PS_SHADOW_INPUT Input) : COLOR
{   
	float alphaSample = tex2D( gOpacitySampler , Input.mTexCoord ).a;
   float depth = Input.mClipPosition.z / Input.mClipPosition.w;
   return float4(depth.xxx, alphaSample);
}

PS_VERTEX_TRANSFORMATION_OUTPUT ps_VertexTransformation(PS_VERTEX_TRANSFORMATION_INPUT input)
{
	PS_VERTEX_TRANSFORMATION_OUTPUT output;
	output.mTransformedPosition = float4(input.mTransformedPosition.xyz,0.0f);
	output.mNormal = float4(input.mNormal.xyz,0.0f);
	output.mTangent = float4(input.mTangent.xyz,0.0f);
	output.mBiNormal = float4(input.mBiNormal.xyz,0.0f);
	return output;
}




technique TGUI
{
    pass P0
    { 
		VertexShader = compile vs_3_0 vs_GUI();
        PixelShader  = compile ps_3_0 ps_GUI();
    }  
}

technique TSkinningPhong
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_3_0 ps_Phong();
    }  
}


technique TSkinningPhongDiffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse();
    }  
}

technique TSkinningPhongDiffuseInstancing
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuseInstancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse(); // ps_DebugColor
    }  
}

technique TLine
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_Line();
        PixelShader  = compile ps_3_0 ps_Line();
    }  
}

technique TPhong
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_Phong();
    }  
}

technique TTerrain
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_Terrain();
        PixelShader  = compile ps_2_0 ps_Terrain();
    }  
}


technique TPhongDiffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse();
    }  
}

technique TPhongDiffuseLight
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseLight();
    }  
}

technique TPhongDiffuseBump
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuseBump();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseBump();
    }  
}

technique TPhongDiffuseOpacity
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseOpacity();
    }  
}

technique TPhongDiffuseSpecular
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseSpecular();
    }  
}

technique TPhongDiffuseBumpSpecular
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseBumpSpecular();
    }  
}


//--------------------------------------------------------------//
// Technique Section for TShadowNormal
//--------------------------------------------------------------//
technique TShadowNormalNotAlphaTest
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowNormalNotAlphaTestInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal_Instancing();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowSkinnedNotAlphaTestInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinned_Instancing();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowSkinnedNotAlphaTest
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinning();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowNormalAlphaTest
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal();
      PixelShader = compile ps_3_0 ps_Shadow_AlphaTest();
   }
}

technique TShadowNormalAlphaTestInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal_Instancing();
      PixelShader = compile ps_3_0 ps_Shadow_AlphaTest();
   }
}

technique TShadowSkinnedAlphaTestInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinned_Instancing();
      PixelShader = compile ps_3_0 ps_Shadow_AlphaTest();
   }
}

technique TShadowSkinnedAlphaTest
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinning();
      PixelShader = compile ps_3_0 ps_Shadow_AlphaTest();
   }
}

technique TPhongDiffuseInstancing
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse_Instancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse();
    }  
}

technique TPhongDiffuseOpacityInstancing
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse_Instancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseOpacity();
    }  
}

technique TPhongDiffuseLightInstancing
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse_Instancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseLight();
    }  
}


technique TVertexTransformation
{
	pass P0
	{
        VertexShader = compile vs_3_0 vs_VertexTransformation();
        PixelShader  = compile ps_3_0 ps_VertexTransformation();
	}
}



