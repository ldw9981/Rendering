// 텍스처
texture Tex0;
texture Tex1;
texture Tex2;
texture Tex3;
texture Opacity_Tex;
texture ShadowMap_Tex : RenderColorTarget;

// 변환행렬
float4x4 gWorldMatrix      : WORLD;
float4x4 gViewMatrix       : VIEW;
float4x4 gProjectionMatrix : PROJECTION;
float4x4 gViewProjectionMatrix : ViewProjection;
float4 gWorldLightPosition;
float4 gWorldCameraPosition;

float4x4 gLightViewMatrix;
float4x4 gLightProjectionMatrix : Projection;


// 텍스처 샘플러
sampler gDiffuseSampler = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gNormalSampler = sampler_state
{
    Texture   = (Tex1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gSpecularSampler = sampler_state
{
    Texture   = (Tex2);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler gLightSampler = sampler_state
{
    Texture   = (Tex3);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler gOpacitySampler = sampler_state
{
    Texture   = (Opacity_Tex);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
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

const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
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
};

struct VS_SKINNING_PHONG_DIFFUSE_INPUT
{
   float3 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
   float2 mTexCoord : TEXCOORD0;
   float2 mTexCoord1 : TEXCOORD1;   
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 
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
   
	float4 worldPosition = mul(float4(input.mPosition,1) , matWorldSkinned);	
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

float4 ps_Line(float4 Color   : COLOR0) : COLOR
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
	
	if( alphaSample < 0.5f )
	{
		discard;
	}
	
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

technique TGUI
{
    pass P0
    { 
		VertexShader = compile vs_2_0 vs_GUI();
        PixelShader  = compile ps_2_0 ps_GUI();
    }  
}

technique TSkinningPhong
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_2_0 ps_Phong();
    }  
}


technique TSkinningPhongDiffuse
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_2_0 ps_PhongDiffuse();
    }  
}

technique TLine
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_Line();
        PixelShader  = compile ps_2_0 ps_Line();
    }  
}

technique TPhong
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_PhongDiffuse();
        PixelShader  = compile ps_2_0 ps_Phong();
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
        VertexShader = compile vs_2_0 vs_PhongDiffuse();
        PixelShader  = compile ps_2_0 ps_PhongDiffuse();
    }  
}

technique TPhongDiffuseLight
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseLight();
    }  
}

technique TPhongDiffuseBump
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_PhongDiffuseBump();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseBump();
    }  
}

technique TPhongDiffuseOpacity
{
    pass P0
    {
        VertexShader = compile vs_2_0 vs_PhongDiffuse();
        PixelShader  = compile ps_3_0 ps_PhongDiffuseOpacity();
    }  
}



struct VS_SHADOW_NORMAL_INPUT 
{
   float4 mPosition: POSITION;
};

struct VS_SHADOW_OUTPUT 
{
   float4 mPosition: POSITION;
   float4 mClipPosition: TEXCOORD1;
};



VS_SHADOW_OUTPUT vs_Shadow_Normal( VS_SHADOW_NORMAL_INPUT Input )
{
   VS_SHADOW_OUTPUT Output;
 
   Output.mPosition = mul(Input.mPosition, gWorldMatrix);
   Output.mPosition = mul(Output.mPosition, gLightViewMatrix);
   Output.mPosition = mul(Output.mPosition, gLightProjectionMatrix);

   Output.mClipPosition = Output.mPosition;
   
   return Output;
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
   
	float4 worldPosition = mul(float4(input.mPosition,1) , matWorldSkinned);	
    output.mPosition = mul(worldPosition, gLightViewMatrix);
    output.mPosition = mul(output.mPosition, gLightProjectionMatrix);
    output.mClipPosition = output.mPosition;  
    return output;
}

struct PS_INPUT 
{
   float4 mClipPosition: TEXCOORD1;
};

float4 ps_Shadow(PS_INPUT Input) : COLOR
{   
   float depth = Input.mClipPosition.z / Input.mClipPosition.w;
   return float4(depth.xxx, 1);
}

//--------------------------------------------------------------//
// Technique Section for CreateShadowShader
//--------------------------------------------------------------//
technique CreateShadowShader
{
   pass P0
   {

      VertexShader = compile vs_2_0 vs_Shadow_Normal();
      PixelShader = compile ps_2_0 ps_Shadow();
   }
}
technique TShadowSkinning
{
   pass CreateShadow
   {

      VertexShader = compile vs_2_0 vs_Shadow_Skinning();
      PixelShader = compile ps_2_0 ps_Shadow();
   }
}




