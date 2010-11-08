vs.2.0
//c4 world
//c8 view
//c12 projection
//c16 viewprojection

def					c0,	0,1,-1,3

dcl_position		v0
dcl_blendweight		v1
dcl_blendindices	v2
dcl_normal			v3
dcl_texcoord0		v4

// 1) 일단, 4번째 weight를 구한다.(3개만 넘어오므로...)
; r4.z가 4번째 Weight다 c0.z는 -1이고  c0.y는 1이다.
dp4		r1.z,	c0.zzzy,	v1

// 2) Blend의 위치를 계산해낸다.
mul	r5,	v2,	c0.w		; c0.w는 3이다.


// 3) Blend의 Matrix를 구해서 곱한다.
mova	a0.x,	r5.x			; 주소의 address값을 얻었다.
mul		r2,	v1.x,	c[a0.x+26]		; 곱한다~~
mul		r3,	v1.x,	c[a0.x+27]		; 곱한다~~
mul		r4,	v1.x,	c[a0.x+28]		; 곱한다~~

mova	a0.x,	r5.y	; 주소의 address값을 얻었다.
mad		r2,	v1.y,	c[a0.x+26],	r2	; 곱한다~~
mad		r3,	v1.y,	c[a0.x+27],	r3	; 곱한다~~
mad		r4,	v1.y,	c[a0.x+28],	r4	; 곱한다~~

mova	a0.x,	r5.z	; 주소의 address값을 얻었다.
mad		r2,	v1.z,	c[a0.x+26],	r2	; 곱한다~~
mad		r3,	v1.z,	c[a0.x+27],	r3	; 곱한다~~
mad		r4,	v1.z,	c[a0.x+28],	r4	; 곱한다~~

mova	a0.x,	r5.w	; 주소의 address값을 얻었다.
mad		r2,	r1.z,	c[a0.x+26],	r2	; 곱한다~~
mad		r3,	r1.z,	c[a0.x+27],	r3	; 곱한다~~
mad		r4,	r1.z,	c[a0.x+28],	r4	; 곱한다~~

// 4) 최종적으로 vertex와 구해진 WorldTM 그리고 ViewTransform Matrix를 곱해준다.
m4x3	r0.xyz,	v0,	r2 
mov		r0.w,	c0.y		; r5의 w를 1.0로 채운다.
m4x4	oPos,	r0,	c16	; ViewProjection을 곱한다.

//5)  Normal도 Transform해준다.
m3x3	r5.xyz,	v3,	r2

//* 픽셀쉐이더를 이용하지않으면 라이트입력값(Diffuse,Ambient,Direction)에의해 Diffuse (oD0)값을 설정해야하고
//* 픽셀쉐이더를 이용하면 관련값 처리코드가 들어가야한다.


//6) 텍스쳐 좌표 설정
mov		oT0,v4
