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

// 1) �ϴ�, 4��° weight�� ���Ѵ�.(3���� �Ѿ���Ƿ�...)
; r4.z�� 4��° Weight�� c0.z�� -1�̰�  c0.y�� 1�̴�.
dp4		r1.z,	c0.zzzy,	v1

// 2) Blend�� ��ġ�� ����س���.
mul	r5,	v2,	c0.w		; c0.w�� 3�̴�.


// 3) Blend�� Matrix�� ���ؼ� ���Ѵ�.
mova	a0.x,	r5.x			; �ּ��� address���� �����.
mul		r2,	v1.x,	c[a0.x+26]		; ���Ѵ�~~
mul		r3,	v1.x,	c[a0.x+27]		; ���Ѵ�~~
mul		r4,	v1.x,	c[a0.x+28]		; ���Ѵ�~~

mova	a0.x,	r5.y	; �ּ��� address���� �����.
mad		r2,	v1.y,	c[a0.x+26],	r2	; ���Ѵ�~~
mad		r3,	v1.y,	c[a0.x+27],	r3	; ���Ѵ�~~
mad		r4,	v1.y,	c[a0.x+28],	r4	; ���Ѵ�~~

mova	a0.x,	r5.z	; �ּ��� address���� �����.
mad		r2,	v1.z,	c[a0.x+26],	r2	; ���Ѵ�~~
mad		r3,	v1.z,	c[a0.x+27],	r3	; ���Ѵ�~~
mad		r4,	v1.z,	c[a0.x+28],	r4	; ���Ѵ�~~

mova	a0.x,	r5.w	; �ּ��� address���� �����.
mad		r2,	r1.z,	c[a0.x+26],	r2	; ���Ѵ�~~
mad		r3,	r1.z,	c[a0.x+27],	r3	; ���Ѵ�~~
mad		r4,	r1.z,	c[a0.x+28],	r4	; ���Ѵ�~~

// 4) ���������� vertex�� ������ WorldTM �׸��� ViewTransform Matrix�� �����ش�.
m4x3	r0.xyz,	v0,	r2 
mov		r0.w,	c0.y		; r5�� w�� 1.0�� ä���.
m4x4	oPos,	r0,	c16	; ViewProjection�� ���Ѵ�.

//5)  Normal�� Transform���ش�.
m3x3	r5.xyz,	v3,	r2

//* �ȼ����̴��� �̿����������� ����Ʈ�Է°�(Diffuse,Ambient,Direction)������ Diffuse (oD0)���� �����ؾ��ϰ�
//* �ȼ����̴��� �̿��ϸ� ���ð� ó���ڵ尡 �����Ѵ�.


//6) �ؽ��� ��ǥ ����
mov		oT0,v4
