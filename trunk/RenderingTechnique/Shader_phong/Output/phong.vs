vs.2.0
//in
//c0	float4x4	World
//c4	float4x4	View
//c8	float4x4	Projection
//c12	float3		lightDir
dcl_position	v0
dcl_normal		v1
dcl_texcoord0	v2

//out
//oPos  float4		Pos    
//oT0   float2		Tex    
//oT1   float3		Norm  
//oT2   float3		Eye  
//oT3   float3		Light  

// 1) Position��World/view/projection matrix�����غ�ȯ�Ѵ�.
m4x4	r1,v0,c0
m4x4	r0,r1,c4			;r0=position	ī�޶� ���� �̵�
m4x4	oPos,r0,c8			;oPos= �������� ���� position

mov		oT0,v2			//tex

// ī�޶� ������ �����븻���
m3x3	r2.xyz,v1,c0		; Scale,Rotation�κи� �����ϰ�
nrm		r1,r2				; Normalize Normal(Scale�̵��°Ͷ�����Normalize����������Ѵ�)
mov		oT1,r1			//ī�޶������ Normal

// ī�޶� ������ ����->����(ī�޶�)�� ���ϴ� �ü�����
nrm		r3,-r0				;r2= position���� ī�޶� ���ϴº���(ī�޶� ���� �̵� )
mov		oT2,r3			//ī�޶������ Eye

nrm		r4,-c12
mov		oT3,r4 		//ī�޶������ Light

