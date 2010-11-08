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

// 1) Position을World/view/projection matrix에의해변환한다.
m4x4	r1,v0,c0
m4x4	r0,r1,c4			;r0=position	카메라 공간 이동
m4x4	oPos,r0,c8			;oPos= 투영공간 내의 position

mov		oT0,v2			//tex

// 카메라 공간의 정점노말계산
m3x3	r2.xyz,v1,c0		; Scale,Rotation부분만 적용하고
nrm		r1,r2				; Normalize Normal(Scale이들어가는것때문에Normalize를꼭해줘야한다)
mov		oT1,r1			//카메라공간의 Normal

// 카메라 공간의 정점->중점(카메라)로 향하는 시선벡터
nrm		r3,-r0				;r2= position에서 카메라를 향하는벡터(카메라 공간 이동 )
mov		oT2,r3			//카메라공간의 Eye

nrm		r4,-c12
mov		oT3,r4 		//카메라공간의 Light

