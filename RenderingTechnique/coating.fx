// ��Ż(metal) ���̵�
// base : EffectEdit
// by : newtype@chollian.net

string XFile = "tiger.x";   // �޽�
int    BCLR = 0xc0c0c0c0;  // ����

// ������ ���� (view space)
float3 lightDir <  string UIDirectional = "Light Direction"; > = {0.577, -0.577, 0.577};

// ������ ���
float4 I_a = { 0.1f, 0.1f, 0.1f, 1.0f };    // �ֺ���
float4 I_d = { 1.0f, 1.0f, 1.0f, 1.0f };    // Ȯ�걤
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // �ݻ籤

// ��������
float4 k_a : MATERIALAMBIENT = { 1.0f, 1.0f, 1.0f, 1.0f };    // �ֺ��� ����
float4 k_d : MATERIALDIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };    // Ȯ�걤 ����
float4 k_s : MATERIALSPECULAR= { 1.0f, 1.0f, 1.0f, 1.0f };    // �ݻ籤 ����
int    n   : MATERIALPOWER = 32;                            // ������

// �ؽ�ó
texture Tex0 < string name = "tiger.bmp"; >;
texture Tex1 < string name = "coating.bmp"; >;

// ��ȯ���
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

// �������
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Spec : COLOR1;
    float2 Tex0 : TEXCOORD0;    // diffuse
    float2 Tex1 : TEXCOORD1;    // specular
};

// �������̴�
VS_OUTPUT VS(
    float3 Pos  : POSITION, 
    float3 Norm : NORMAL, 
    float2 Tex  : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    // �������� ���(view space)
    float3 L = -lightDir;

    // wold*view��İ��
    float4x4 WorldView = mul(World, View);

    // ������ view��������
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
    
    // ������ view��������
    float3 N = normalize(mul(Norm, (float3x3)WorldView));

    // �ݻ纤�͸� ���Ѵ�(view ����)
    float3 R = normalize(2 * dot(N, L) * N - L);

    // view���͸� ���Ѵ�(view ����)
    float3 V = -normalize(P);

    // �������������� ��ġ���
    Out.Pos  = mul(float4(P, 1), Projection);

     // �ֺ���(ambient) + Ȯ�걤(diffuse)
    Out.Diff = I_a * k_a + I_d * k_d * max(0, dot(N, L));

    // �ݻ籤(specular)
    Out.Spec = I_s * k_s * pow(max(0, dot(R, V)), n/4);

    // �ؽ�ó
    Out.Tex0 = Tex;

    // �ݻ籤�� ��⸦ �ؽ�ó�� x��ǥ�� ���
    Out.Tex1 = float2( pow(max(0, dot(R, V)), 1), 0.5 )/2 + 0.5;

    return Out;
}

// �ؽ�ó ���÷�
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

// �ȼ����̴�
float4 PS(
    float4 Diff : COLOR0,
    float4 Spec : COLOR1,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1 ) : COLOR
{
    return tex2D(Sampler0, Tex0) * Diff + tex2D(Sampler1, Tex1 ) * Spec;
}

// ��ũ�� ����(���̴� ������)
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

// ��ũ�� ����(���� ���̴��� ���)
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

// ��ũ�� ����(���� & �ȼ� ���̴� ���)
technique TVertexAndPixelShader
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
    }  
}