// Phong쉐이딩
// base : EffectEdit

//string XFile = "tiger.x";   // 메시
int    BCLR = 0xff202080;   // 배경색

// 광원의 방향 (view space)
float3 lightDir <  string UIDirectional = "Light Direction"; > = {0.577, -0.577, 0.577};

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

// 변환행렬
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float4x4 ViewProjection : ViewProjection;

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
VS_OUTPUT VS( 
   float3 Pos  : POSITION,
   float3 Norm : NORMAL,
   float2 Tex  : TEXCOORD0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0; 

    // 광원벡터 계산(view space)
    Out.Light = -lightDir;

    // wold*view행렬계산
    float4x4 WorldView = mul(World, View);

    // 정점을 view공간으로
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
    
    // 법선을 view공간으로
    Out.Norm = normalize(mul(Norm, (float3x3)WorldView));

    // view벡터를 구한다(view 공간)
    Out.View = -normalize(P);

    // 투영공간에서의 위치계산
    Out.Pos  = mul(float4(P, 1), Projection);
    
    Out.Tex = Tex;
    
    return Out;
}

VS_OUTPUT VS_Skinning( 
   float3 Pos  : POSITION,
   float3 Norm : NORMAL,
   float2 Tex  : TEXCOORD0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0; 

    // 광원벡터 계산(view space)
    Out.Light = -lightDir;

    // wold*view행렬계산
    float4x4 WorldView = mul(World, View);

    // 정점을 view공간으로
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
    
    // 법선을 view공간으로
    Out.Norm = normalize(mul(Norm, (float3x3)WorldView));

    // view벡터를 구한다(view 공간)
    Out.View = -normalize(P);

    // 투영공간에서의 위치계산
    Out.Pos  = mul(float4(P, 1), Projection);
    
    Out.Tex = Tex;
    
    return Out;
}


// 텍스처 샘플러
sampler Sampler = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

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
   float4 FinalColor = (AmbientColor + DiffuseColor) * tex2D( Sampler, Tex) + SpecularColor;
   
   return FinalColor;
}

// 테크닉 선언(쉐이더 사용않함)
technique TNoShader
{
    pass P0
    {
        // transforms
        WorldTransform[0]   = (World);
        ViewTransform       = (View);
        ProjectionTransform = (Projection);

        // material
        MaterialAmbient  = (k_a); 
        MaterialDiffuse  = (k_d); 
        MaterialSpecular = (k_s); 
        MaterialPower    = (n);
        
        // lighting
        LightType[0]      = DIRECTIONAL;
        LightAmbient[0]   = (I_a);
        LightDiffuse[0]   = (I_d);
        LightSpecular[0]  = (I_s); 
        LightDirection[0] = (lightDir);
        LightRange[0]     = 100000.0f;

        LightEnable[0] = TRUE;
        Lighting       = TRUE;
        SpecularEnable = TRUE;
        
        // samplers
        Sampler[0] = (Sampler);
        
        // texture stages
        ColorOp[0]   = MODULATE;
        ColorArg1[0] = TEXTURE;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = MODULATE;
        AlphaArg1[0] = TEXTURE;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;

        // shaders
        VertexShader = NULL;
        PixelShader  = NULL;
    }
}

// 테크닉 선언(쉐이더 & 픽셀 쉐이더 사용)
technique TVertexAndPixelShader
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
}