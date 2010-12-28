// Phong���̵�
// base : EffectEdit

//string XFile = "tiger.x";   // �޽�
int    BCLR = 0xff202080;   // ����

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
//texture Tex0 < string name = "tiger.bmp"; >;
texture Tex0;

// ��ȯ���
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

// �������
struct VS_OUTPUT
{
   float4 Pos    : POSITION;
   float2 Tex    : TEXCOORD0;
   float3 Norm   : TEXCOORD1;
   float3 View   : TEXCOORD2;
   float3 Light  : TEXCOORD3;  
};

// �������̴�
VS_OUTPUT VS( 
   float3 Pos  : POSITION,
   float3 Norm : NORMAL,
   float2 Tex  : TEXCOORD0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0; 

    // �������� ���(view space)
    Out.Light = -lightDir;

    // wold*view��İ��
    float4x4 WorldView = mul(World, View);

    // ������ view��������
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
    
    // ������ view��������
    Out.Norm = normalize(mul(Norm, (float3x3)WorldView));

    // view���͸� ���Ѵ�(view ����)
    Out.View = -normalize(P);

    // �������������� ��ġ���
    Out.Pos  = mul(float4(P, 1), Projection);
    
    Out.Tex = Tex;
    
    return Out;
}

// �ؽ�ó ���÷�
sampler Sampler = sampler_state
{
    Texture   = (Tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// �ȼ����̴�
float4 PS( float4 Diff   : COLOR0,
		   float2 Tex    : TEXCOORD0,
           float3 Normal : TEXCOORD1,
           float3 View   : TEXCOORD2,
           float3 Light  : TEXCOORD3
            ) : COLOR
{
   // �ݻ纤�͸� ���Ѵ�
   float3 vReflect = normalize( 2 * dot( Normal, Light) * Normal - Light );       

   // �ֺ���
   float4 AmbientColor = I_a * k_a;

   // Ȯ�걤
   float4 DiffuseColor = I_d * k_d * max( 0, dot( Normal, Light ));

   // �ݻ籤
   float4 SpecularColor = I_s * k_s * pow( max( 0, dot(vReflect, View)), n/4 );
   
   // ������
   float4 FinalColor = (AmbientColor + DiffuseColor) * tex2D( Sampler, Tex) + SpecularColor;
   
   return FinalColor;
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

// ��ũ�� ����(���̴� & �ȼ� ���̴� ���)
technique TVertexAndPixelShader
{
    pass P0
    {
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
}