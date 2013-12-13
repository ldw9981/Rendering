// Phong쉐이딩
// base : EffectEdit

//string XFile = "tiger.x";   // 메시
int    BCLR = 0xff202080;   // 배경색

// 광원의 방향 (view space)
float3 lightDir <  string UIDirectional = "Light Direction"; > = {0.0, -1.0, 1.0};

// 광원의 밝기
float4 I_a = { 0.1f, 0.1f, 0.1f, 1.0f };    // 주변광
float4 I_d = { 1.0f, 1.0f, 1.0f, 1.0f };    // 확산광
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // 반사광

// 재질설정
float4 k_a : MATERIALAMBIENT = { 1.0f, 1.0f, 1.0f, 1.0f };    // 주변광 재질
float4 k_d : MATERIALDIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };    // 확산광 재질
float4 k_s : MATERIALSPECULAR= { 1.0f, 1.0f, 1.0f, 1.0f };    // 반사광 재질
int    n   : MATERIALPOWER = 32;                            // 제곱값

// 텍스처
//texture Tex0 < string name = "tiger.bmp"; >;
texture Tex0;
texture Tex1;
texture Tex2;
texture Tex3;

// 변환행렬
float4x4 gWorldMatrix      : WORLD;
float4x4 gViewMatrix       : VIEW;
float4x4 gProjectionMatrix : PROJECTION;
float4x4 gViewProjectionMatrix : ViewProjection;
float4 gWorldLightPosition;
float4 gWorldCameraPosition;

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
float gAmbientIntensity;
float4 gAmbientColor = { 1.0f, 1.0f, 1.0f, 1.0f };
float4 gSpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
float gSpecularPower = 32;


#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 64
#endif

const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x4 Palette[ MATRIX_PALETTE_SIZE_DEFAULT ];



// 출력정점
struct VS_OUTPUT
{
   float4 Pos    : POSITION;
   float2 Tex    : TEXCOORD0;
   float3 Norm   : TEXCOORD1;
   float3 View   : TEXCOORD2;
   float3 Light  : TEXCOORD3; 
};

// 정점쉐이더
VS_OUTPUT VPhongDiffuse( 
   float3 Pos  : POSITION,
   float3 Norm : NORMAL,
   float2 Tex  : TEXCOORD0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;    
   
    float4x4 WorldView = mul(gWorldMatrix, gViewMatrix);    					 // wold*view행렬계산
    float3 P = mul(float4(Pos, 1),(float4x3)WorldView);		// 정점을 view공간으로    
    Out.Norm = normalize(mul(Norm,(float3x3)WorldView));		// 법선을 view공간으로    
    Out.View = -normalize(P);									// view벡터를 구한다(view 공간)   
    Out.Pos  = mul(float4(P, 1), gProjectionMatrix);    				 // 투영공간에서의 위치계산
    Out.Tex = Tex;    
	Out.Light = -normalize( Pos - gWorldLightPosition.xyz);										 // 광원벡터 계산(view space)   
    return Out;
}

struct VS_PHONGDIFFUSE_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float2 mTexCoord : TEXCOORD0;  
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
};

struct VS_PHONGDIFFUSE_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
   float4 mLambert : TEXCOORD1;  
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mReflect : TEXCOORD4;
};

VS_PHONGDIFFUSE_OUTPUT vs_PhongDiffuse( VS_PHONGDIFFUSE_INPUT input)
{
   VS_PHONGDIFFUSE_OUTPUT output;
   output.mPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(output.mPosition , gViewMatrix);
   output.mPosition = mul(output.mPosition , gProjectionMatrix);
   
   float3 lightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   float3 worldNormal = mul(input.mNormal,(float3x3)gWorldMatrix);
   worldNormal = normalize(worldNormal);
      
   output.mLambert = dot(-lightDir, worldNormal);
   output.mNormal = worldNormal;
   output.mCameraDir = cameraDir;
   output.mReflect = reflect(lightDir, worldNormal);
   output.mTexCoord = input.mTexCoord;   
   return output;
}

struct PS_PHONGDIFFUSE_INPUT
{ 
   float2 mTexCoord : TEXCOORD0;
   float3 mLambert : TEXCOORD1;  
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mReflect : TEXCOORD4;
};


float4 ps_PhongDiffuse(PS_PHONGDIFFUSE_INPUT input) : COLOR
{  
   float3 color;
   float3 lambert = saturate(input.mLambert);
   float3 worldNormal = normalize(input.mNormal);
   float3 cameraDir = normalize(input.mCameraDir);
   float3 reflectDir = normalize(input.mReflect);
   float3 albedo = tex2D( gDiffuseSampler , input.mTexCoord );
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);
  
//   float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
//   specular = specular * specularIntensity.xyz;
  
   color = gAmbientColor.xyz * gAmbientIntensity * albedo +  lambert * albedo + gSpecularColor.xyz * specular; 
   //color = gAmbientColor * gAmbientIntensity +  lambert + gSpecularColor * specular; 
   //color =  gSpecularColor * specular; 
   return float4(color,0.0f);
}


struct VS_PHONG_DIFFUSE_BUMP_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float2 mTexCoord : TEXCOORD0;   
   float3 mBiNormal : TEXCOORD1;
   float3 mTangent : TEXCOORD2;
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
};

VS_PHONG_DIFFUSE_BUMP_OUTPUT vs_PhongDiffuseBump( VS_PHONG_DIFFUSE_BUMP_INPUT input)
{
   VS_PHONG_DIFFUSE_BUMP_OUTPUT output;
   output.mPosition = mul(input.mPosition , gWorldMatrix);
   output.mPosition = mul(output.mPosition , gViewMatrix);
   output.mPosition = mul(output.mPosition , gProjectionMatrix);

   output.mLightDir = normalize( output.mPosition.xyz - gWorldLightPosition.xyz);
   output.mNormal = normalize(mul(input.mNormal,(float3x3)gWorldMatrix));
   output.mCameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
   output.mTexCoord = input.mTexCoord;
   
   float3 worldTangent = mul(input.mTangent,(float3x3)gWorldMatrix);
   output.mTangent = normalize(worldTangent);
   
   float3 worldBiNormal = mul(input.mBiNormal,(float3x3)gWorldMatrix);
   output.mBiNormal = normalize(worldBiNormal);
   
   return output;
}



struct PS_PHONG_DIFFUSE_BUMP_INPUT
{ 
   float2 mTexCoord : TEXCOORD0;
   float3 mLightDir : TEXCOORD1;
   float3 mNormal: TEXCOORD2;
   float3 mCameraDir : TEXCOORD3;
   float3 mTangent : TEXCOORD4;
   float3 mBiNormal : TEXCOORD5;
};


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
   
   float3 albedo = tex2D( gDiffuseSampler,input.mTexCoord);
   
  
   float3 specular = 0;
   specular = dot(reflectDir,-cameraDir);
   specular = saturate(specular);
   specular = pow(specular, gSpecularPower);   
  
   //float4 specularIntensity = tex2D(gSpecularSampler,input.mTexCoord);
   //specular = specular * specularIntensity.xyz;
  
   color = gAmbientColor.xyz * gAmbientIntensity * albedo +  lambert * albedo + gSpecularColor.xyz * specular; 
   //color = gAmbientColor * gAmbientIntensity +  lambert + gSpecularColor * specular; 
   //color =  gSpecularColor * specular; 
   return float4(color,0.0f);
}

VS_OUTPUT VS_Skinning( 
   float3 Pos  : POSITION,
   float3 BlendWeights    : BLENDWEIGHT,
   int4   BlendIndices    : BLENDINDICES,
   float3 Norm : NORMAL,
   float2 Tex  : TEXCOORD0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;     
	
    float fLastWeight = 1.0;
    float fWeight;
    float afBlendWeights[ 3 ] = (float[ 3 ]) BlendWeights;
	int aiIndices[ 4 ] = (int[ 4 ])BlendIndices;
	
	fLastWeight = 1.0 - (BlendWeights.x + BlendWeights.y + BlendWeights.z);

	float4x4 matWorldSkinned;
	matWorldSkinned = mul(BlendWeights.x, Palette[BlendIndices.x]);
	matWorldSkinned += mul(BlendWeights.y, Palette[BlendIndices.y]);
	matWorldSkinned += mul(BlendWeights.z, Palette[BlendIndices.z]);
	matWorldSkinned += mul(fLastWeight, Palette[BlendIndices.w]);		
		
	
    float4x4 WorldView = mul(matWorldSkinned, gViewMatrix);			 // wold*view행렬계산    
    float3 P = mul(float4(Pos, 1),(float4x3)WorldView);					 // 정점을 view공간으로
	Out.Norm = normalize(mul(Norm,(float3x3)WorldView));		 // 법선을 view공간으로    
    Out.View = -normalize(P);									 // view벡터를 구한다(view 공간)        
    Out.Pos  = mul(float4(P, 1), gProjectionMatrix);					// 투영공간에서의 위치계산
	Out.Tex = Tex;    
	Out.Light = -normalize( Pos - gWorldLightPosition.xyz);									 // 광원벡터 계산(view space)     
    return Out;	
}




// 픽셀쉐이더
float4 PS( float4 Diff   : COLOR0,
		   float2 Tex    : TEXCOORD0,
           float3 Normal : TEXCOORD1,
           float3 View   : TEXCOORD2,
           float3 Light  : TEXCOORD3
            ) : COLOR
{
   // 반사벡터를 구한다
   float3 vReflect = normalize( 2 * dot( Normal, Light) * Normal - Light );       

   // 주변광
   float4 AmbientColor = I_a * k_a;

   // 확산광
   float4 DiffuseColor = I_d * k_d * max( 0, dot( Normal, Light ));

   // 반사광
   float4 SpecularColor = I_s * k_s * pow( max( 0, dot(vReflect, View)), n/4 );
   
   // 최종색
   float4 FinalColor = (AmbientColor + DiffuseColor) * tex2D( gDiffuseSampler, Tex) + SpecularColor;
   
   return FinalColor;
}

struct VS_SKINNINGPHONGDIFFUSE_INPUT
{
   float3 mPosition : POSITION;
   float3 mBlendWeights    : BLENDWEIGHT;
   int4   mBlendIndices    : BLENDINDICES; 
   float3 mNormal : NORMAL;
   float2 mTexCoord : TEXCOORD0;  
   float3 mTangent : TANGENT;
   float3 mBiNormal : BINORMAL;
};

VS_PHONGDIFFUSE_OUTPUT vs_SkinningPhongDiffuse( VS_SKINNINGPHONGDIFFUSE_INPUT input)
{
    VS_PHONGDIFFUSE_OUTPUT output;
	
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
   
    output.mPosition = mul(float4(input.mPosition,1) , matWorldSkinned);
    output.mPosition = mul(output.mPosition , gViewMatrix);
    output.mPosition = mul(output.mPosition , gProjectionMatrix);
    
    
    float3 cameraDir = normalize( output.mPosition.xyz - gWorldCameraPosition.xyz);
    float3 worldNormal = mul(input.mNormal,(float3x3)matWorldSkinned);
    worldNormal = normalize(worldNormal);
       
    output.mLambert = dot(-lightDir, worldNormal);
    output.mNormal = worldNormal;
    output.mCameraDir = cameraDir;
    output.mReflect = reflect(lightDir, worldNormal);
    output.mTexCoord = input.mTexCoord;   
    return output;
}


technique TSkinningPhongDiffuse
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_2_0 vs_SkinningPhongDiffuse();
        PixelShader  = compile ps_2_0 ps_PhongDiffuse();
    }  
}


// 테크닉 선언(쉐이더 & 픽셀 쉐이더 사용)
technique TPhongDiffuse
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_2_0 vs_PhongDiffuse();
        PixelShader  = compile ps_2_0 ps_PhongDiffuse();
    }  
}

technique TPhongDiffuseBump
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_2_0 vs_PhongDiffuseBump();
        PixelShader  = compile ps_2_0 ps_PhongDiffuseBump();
    }  
}




