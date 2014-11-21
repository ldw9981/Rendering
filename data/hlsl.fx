// 텍스처
texture Tex_Diffuse;
texture Tex_Normal;
texture Tex_Specular;
texture Tex_Light;
texture Tex_Opacity;
texture Tex_Depth : RenderColorTarget;
texture Tex_MatrixInstanceData;
texture Tex_MatrixPallete;

// 변환행렬
float4x4	gWorldMatrix      : WORLD;
float4x4	gViewMatrix       : VIEW;
float4x4	gProjectionMatrix : PROJECTION;
float4x4	gViewProjectionMatrix : ViewProjection;
float4		gWorldLightPosition;
float4		gWorldCameraPosition;

float4x4	gLightViewMatrix;
float4x4	gLightProjectionMatrix : Projection;
float		gMatrixTextureSize = 512;


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
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler2D   gMatrixInstanceDataSampler = sampler_state 
{
  Texture = (Tex_MatrixInstanceData);
  MipFilter = NONE;
  MagFilter = POINT;
  MinFilter = POINT;
  AddressU = Clamp;
  AddressV = Clamp;
};

sampler2D   gMatrixPalleteSampler = sampler_state 
{
  Texture = (Tex_MatrixPallete);
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
#define MATRIX_PALETTE_SIZE_DEFAULT 50
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
	float4 mPosition		: POSITION;
	float3 mNormal			: NORMAL;    
	float3 mTangent			: TANGENT;   
	float3 mBiNormal		: BINORMAL;   
	float2 mTexCoord		: TEXCOORD0;  
	float2 mTexCoord1		: TEXCOORD1;
};

struct VS_PHONG_DIFFUSE_INSTANCE_INPUT
{
	float4 mPosition		: POSITION;
	float3 mNormal			: NORMAL;    
	float3 mTangent			: TANGENT;   
	float3 mBiNormal		: BINORMAL;   
	float2 mTexCoord		: TEXCOORD0;  
	float2 mTexCoord1		: TEXCOORD1;
	float2 mInstanceIndex	: TEXCOORD2; 
};

struct VS_SKINNING_PHONG_DIFFUSE_INPUT
{
	float4 mPosition		: POSITION;
	float3 mNormal			: NORMAL;
	float3 mTangent			: TANGENT;
	float3 mBiNormal		: BINORMAL;
	float2 mTexCoord		: TEXCOORD0;
	float2 mTexCoord1		: TEXCOORD1;   
	float3 mBlendWeights    : BLENDWEIGHT;
	int4   mBlendIndices    : BLENDINDICES; 
};

struct VS_SKINNING_PHONG_DIFFUSE_INSTANCING_INPUT
{
	float4 mPosition		: POSITION;
	float3 mNormal			: NORMAL;
	float3 mTangent			: TANGENT;
	float3 mBiNormal		: BINORMAL;
	float2 mTexCoord		: TEXCOORD0;
	float2 mTexCoord1		: TEXCOORD1;   
	float3 mBlendWeights    : BLENDWEIGHT;
	int4   mBlendIndices    : BLENDINDICES; 
	float2 mInstanceIndex	: TEXCOORD2; 	
};

struct VS_SHADOW_NORMAL_INPUT 
{
	float4 mPosition		: POSITION;
	float2 mTexCoord		: TEXCOORD0;
};

struct VS_SHADOW_NORMAL_INSTANCING_INPUT 
{
	float4 mPosition		: POSITION;
	float2 mTexCoord		: TEXCOORD0;
	float2 mInstanceIndex	: TEXCOORD2	;
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

float4x4 loadMatrixPallete(float boneIndex)
{		
	float4 texcoord;		
	texcoord.x = 4*boneIndex / 1024.0f;
	texcoord.y = 0.0f;	
	texcoord.z = 0.0f;
	texcoord.w = 0.0f;

	float4x4 mat = 
	{
		tex2Dlod(gMatrixPalleteSampler, texcoord + float4(0.0f				,0,0,0)),
		tex2Dlod(gMatrixPalleteSampler, texcoord + float4(1.0f / 1024.0f	,0,0,0)),
		tex2Dlod(gMatrixPalleteSampler, texcoord + float4(2.0f / 1024.0f	,0,0,0)),
		tex2Dlod(gMatrixPalleteSampler, texcoord + float4(3.0f / 1024.0f	,0,0,0))
	};
 
	return mat; 	
}
float4x4 loadMatrix(float indexInstance)
{	
	float result = 4*indexInstance / gMatrixTextureSize;
	float quotient = floor(result);	//0~N
	float4 texcoord;		
	texcoord.x = result - quotient;
	texcoord.y = quotient/gMatrixTextureSize;		//
	texcoord.z = 0.0f;
	texcoord.w = 0.0f;

	float4x4 mat = 
	{
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(0.0f 									,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(1.0f / gMatrixTextureSize	,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(2.0f / gMatrixTextureSize	,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(3.0f / gMatrixTextureSize	,0,0,0))
	};
 
	return mat; 	
}
float4x4 loadBoneMatrix(float instanceIndex,float boneIndex,float boneSize )
{	
	float result = 4* (boneSize*instanceIndex+boneIndex) / gMatrixTextureSize;
	float quotient = floor(result);	//0~N
	float4 texcoord;		
	texcoord.x = result - quotient;
	texcoord.y = quotient/gMatrixTextureSize;		//
	texcoord.z = 0.0f;
	texcoord.w = 0.0f;

	float4x4 mat = 
	{
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(0.0f 									,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(1.0f / gMatrixTextureSize	,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(2.0f / gMatrixTextureSize	,0,0,0)),
		tex2Dlod(gMatrixInstanceDataSampler, texcoord + float4(3.0f / gMatrixTextureSize	,0,0,0))
	};
 
	return mat; 	
}

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
	
	//mul(input.mPosition , gWorldMatrix);
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



VS_PHONG_DIFFUSE_OUTPUT vs_PhongDiffuse_Instancing( VS_PHONG_DIFFUSE_INSTANCE_INPUT input)
{
	VS_PHONG_DIFFUSE_OUTPUT output;
	float4x4 mInstanceMatrix = loadMatrix(input.mInstanceIndex.x);
	float4 worldPosition = mul(input.mPosition , mInstanceMatrix);
	output.mPosition = mul(worldPosition , gViewProjectionMatrix);

	float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
	float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
	float3 worldNormal =  normalize(mul(input.mNormal,(float3x3)mInstanceMatrix)); 
    
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
	matWorldSkinned = mul(input.mBlendWeights.x,loadMatrixPallete(input.mBlendIndices.x));
	matWorldSkinned += mul(input.mBlendWeights.y,loadMatrixPallete(input.mBlendIndices.y));
	matWorldSkinned += mul(input.mBlendWeights.z,loadMatrixPallete(input.mBlendIndices.z));
 	matWorldSkinned += mul(fLastWeight,loadMatrixPallete(input.mBlendIndices.w));			
   
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




VS_PHONG_DIFFUSE_OUTPUT vs_SkinningPhongDiffuseInstancing( VS_SKINNING_PHONG_DIFFUSE_INSTANCING_INPUT input)
{  

	VS_PHONG_DIFFUSE_OUTPUT output;
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;
	
	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);

	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.x,input.mInstanceIndex.y));
	matWorldSkinned += mul(input.mBlendWeights.y,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.y,input.mInstanceIndex.y));
   matWorldSkinned += mul(input.mBlendWeights.z,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.z,input.mInstanceIndex.y));
 	matWorldSkinned += mul(fLastWeight,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.w,input.mInstanceIndex.y));	
   
	float4 worldPosition = mul(input.mPosition , matWorldSkinned);	
    output.mPosition = mul(worldPosition , gViewProjectionMatrix);
    
    float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
    float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);	
	float3 worldNormal =  normalize(mul(input.mNormal,(float3x3)matWorldSkinned)); 


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
	float4x4 mInstanceMatrix = loadMatrix(input.mInstanceIndex.x);

	output.mPosition = mul(input.mPosition, mInstanceMatrix);
	output.mPosition = mul(output.mPosition, gLightViewMatrix);
	output.mPosition = mul(output.mPosition, gLightProjectionMatrix);
	output.mClipPosition = output.mPosition;
	output.mTexCoord = input.mTexCoord;      
	return output;
}

VS_SHADOW_OUTPUT vs_Shadow_Skinned_Instancing( VS_SKINNING_PHONG_DIFFUSE_INSTANCING_INPUT input )
{
    VS_SHADOW_OUTPUT output;
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[3] = (float[3])input.mBlendWeights;
	int aiIndices[4] = (int[4])input.mBlendIndices;
	fLastWeight = 1.0 - (input.mBlendWeights.x + input.mBlendWeights.y + input.mBlendWeights.z);
		
	float4x4 matWorldSkinned;
	matWorldSkinned = mul(input.mBlendWeights.x,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.x,input.mInstanceIndex.y));
	matWorldSkinned += mul(input.mBlendWeights.y,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.y,input.mInstanceIndex.y));
	matWorldSkinned += mul(input.mBlendWeights.z,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.z,input.mInstanceIndex.y));
 	matWorldSkinned += mul(fLastWeight,loadBoneMatrix(input.mInstanceIndex.x,input.mBlendIndices.w,input.mInstanceIndex.y));
   
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
	matWorldSkinned = mul(input.mBlendWeights.x,loadMatrixPallete(input.mBlendIndices.x));
	matWorldSkinned += mul(input.mBlendWeights.y,loadMatrixPallete(input.mBlendIndices.y));
	matWorldSkinned += mul(input.mBlendWeights.z,loadMatrixPallete(input.mBlendIndices.z));
 	matWorldSkinned += mul(fLastWeight,loadMatrixPallete(input.mBlendIndices.w));	
   
	float4 worldPosition = mul(input.mPosition , matWorldSkinned);	
    output.mPosition = mul(worldPosition, gLightViewMatrix);
    output.mPosition = mul(output.mPosition, gLightProjectionMatrix);
    output.mClipPosition = output.mPosition;  
	 output.mTexCoord = input.mTexCoord;   
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



technique TLine
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_Line();
        PixelShader  = compile ps_3_0 ps_Line();
    }  
}
technique TGUI
{
    pass P0
    { 
		VertexShader = compile vs_3_0 vs_GUI();
        PixelShader  = compile ps_3_0 ps_GUI();
    }  
}
technique TSceneNormal
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


technique TSceneNormalDiffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse();
    }  
}

technique TSceneNormalDiffuseLight
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseLight();
    }  
}

technique TSceneNormalDiffuseBump
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuseBump();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseBump();
    }  
}

technique TSceneNormalDiffuseOpacity
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseOpacity();
    }  
}

technique TSceneNormalDiffuseSpecular
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseSpecular();
    }  
}

technique TSceneNormalDiffuseBumpSpecular
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseBumpSpecular();
    }  
}



technique TShadowNormal
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowNormalInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal_Instancing();
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

technique TShadowNormalInstancingAlphaTest
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Normal_Instancing();
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

technique TSceneNormalInstancingDiffuseOpacity
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse_Instancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseOpacity();
    }  
}

technique TSceneNormalInstancingDiffuseLight
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_PhongDiffuse_Instancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseLight();
    }  
}



technique TSceneSkinned
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_3_0 ps_Phong();
    }  
}


technique TSceneSkinnedDiffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse();
    }  
}

technique TSceneSkinnedInstancingDiffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_SkinningPhongDiffuseInstancing();
        PixelShader  = compile ps_3_0 ps_PhongDiffuse(); // ps_DebugColor
    }  
}

technique TShadowSkinnedInstancing
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinned_Instancing();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}


technique TShadowSkinned
{
   pass P0
   {
      VertexShader = compile vs_3_0 vs_Shadow_Skinning();
      PixelShader = compile ps_3_0 ps_Shadow();
   }
}

technique TShadowSkinnedInstancingAlphaTest
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




