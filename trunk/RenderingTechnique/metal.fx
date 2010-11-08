// 메탈(metal) 쉐이딩
// base : EffectEdit
// by : newtype@chollian.net

string XFile = "tiger.x";   // 메시
int    BCLR = 0xc0c0c0c0;  // 배경색

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
texture Tex0 < string name = "tiger.bmp"; >;
texture Tex1 < string name = "metal.bmp"; >;

// 변환행렬
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

// 출력정점
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Spec : COLOR1;
    float2 Tex0 : TEXCOORD0;    // diffuse
    float2 Tex1 : TEXCOORD1;    // specular
};

// 정점쉐이더
VS_OUTPUT VS(
    float3 Pos  : POSITION, 
    float3 Norm : NORMAL, 
    float2 Tex  : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    // 광원벡터 계산(view space)
    float3 L = -lightDir;

    // wold*view행렬계산
    float4x4 WorldView = mul(World, View);

    // 정점을 view공간으로
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
    
    // 법선을 view공간으로
    float3 N = normalize(mul(Norm, (float3x3)WorldView));

    // 반사벡터를 구한다(view 공간)
    float3 R = normalize(2 * dot(N, L) * N - L);

    // view벡터를 구한다(view 공간)
    float3 V = -normalize(P);

    // 투영공간에서의 위치계산
    Out.Pos  = mul(float4(P, 1), Projection);

     // 주변광(ambient) + 확산광(diffuse)
    Out.Diff = I_a * k_a + I_d * k_d * max(0, dot(N, L));

    // 반사광(specular)
    Out.Spec = I_s * k_s * pow(max(0, dot(R, V)), n/4);

    // 텍스처
    Out.Tex0 = Tex;

    // 반사광의 밝기를 텍스처의 x좌표로 사용
    Out.Tex1 = float2( pow(max(0, dot(R, V)), 1), 0.5 )/2 + 0.5;

    return Out;
}

// 텍스처 샘플러
sampler Sampler0 = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Sampler1 = sampler_state
{
    Texture   = (Tex1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// 픽셀쉐이더
float4 PS(
    float4 Diff : COLOR0,
    float4 Spec : COLOR1,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1 ) : COLOR
{
    return tex2D(Sampler0, Tex0) * Diff + tex2D(Sampler1, Tex1 ) * Spec;
}

// 테크닉 선언(쉐이더 사용없음)
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
        Sampler[0] = (Sampler0);
        Sampler[1] = (Sampler1);
        
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

// 테크닉 선언(정점 쉐이더만 사용)
technique TVertexShaderOnly
{
    pass P0
    {
        // lighting
        Lighting       = FALSE;
        SpecularEnable = TRUE;

        // samplers
        Sampler[0] = (Sampler0);
        Sampler[1] = (Sampler1);

        // texture stages
        ColorOp[0]   = MODULATE;
        ColorArg1[0] = TEXTURE;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = MODULATE;
        AlphaArg1[0] = TEXTURE;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = ADD;
        ColorArg1[1] = TEXTURE;
        ColorArg2[1] = CURRENT;
        AlphaOp[1]   = DISABLE;

        // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = NULL;
    }
}

// 테크닉 선언(정점 & 픽셀 쉐이더 사용)
technique TVertexAndPixelShader
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
    }  
}