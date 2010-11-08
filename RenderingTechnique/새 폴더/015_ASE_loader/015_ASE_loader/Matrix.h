#ifndef _MATRIX4_H
#define _MATRIX4_H

/* matica vyzera takto:
  (  m11  m12  m13  m14  )      (  m[ 0]  m[ 4]  m[ 8]  m[12]  )      ( e[0][0] e[1][0] e[2][0] e[3][0] )
  (  m21  m22  m23  m24  ) ---- (  m[ 1]  m[ 5]  m[ 9]  m[13]  ) ---- ( e[0][1] e[1][1] e[2][1] e[3][1] )
  (  m31  m32  m33  m34  ) ---- (  m[ 2]  m[ 6]  m[10]  m[14]  ) ---- ( e[0][2] e[1][2] e[2][2] e[3][2] )
  (  m41  m42  m43  m44  )      (  m[ 3]  m[ 7]  m[11]  m[15]  )      ( e[0][3] e[1][3] e[2][3] e[3][3] )
  v OpenGL sa matice ukladaju nie po riadkoch, ale po stlcoch, t.j. 11,21,31,41,12,22,32,42,...
  mij, i je riadok, j je stlpec  mij == m[ (j-1)*4+i-1 ] == e[j][i]
  vec sa pouziva ako stlpcovy vektor t.j. vec = mat4*vec
*/

#include "Vector.h"
//#include <math.h>

#ifndef PI180
#define PI180	0.0174532925199432957692369076848861f	// pi / 180
#endif

class mat4 
{
public:
	union 
	{
		float m[16];
		float e[4][4];
		struct 
		{
			float m11; float m21; float m31; float m41;
			float m12; float m22; float m32; float m42;
			float m13; float m23; float m33; float m43;
			float m14; float m24; float m34; float m44;
		};
	};

	inline mat4();					// konstruktor, vytvori jednotkovu maticu
	inline mat4( const mat4 &a);	// kopirovaci konstruktor
	// zadanie je po riadkoch, v opacnom porati ako su v OpenGL (tam su po stlpcoch)
	inline mat4( const float Am11, const float Am12, const float Am13, const float Am14,
				 const float Am21, const float Am22, const float Am23, const float Am24,
				 const float Am31, const float Am32, const float Am33, const float Am34,
				 const float Am41, const float Am42, const float Am43, const float Am44);

	inline mat4 operator+() const;		// pozitive
	inline mat4 operator-() const;		// negative

	inline mat4 operator!() const;		// Inverse
	inline mat4 operator~() const;		// Transpose

	inline void operator+= ( const mat4 &a );
	inline void operator-= ( const mat4 &a );
	inline void operator*= ( const mat4 &b );		// sprava
	inline void operator/= ( const mat4 &a );
	inline void operator*= ( const float f );	// vynasobi, kazdy prvok matice s f
	inline void operator/= ( const float f );

	inline friend mat4 operator+ ( const mat4 &a, const mat4 &b );
	inline friend mat4 operator- ( const mat4 &a, const mat4 &b );
	inline friend mat4 operator* ( const mat4 &a, const mat4 &b );
	inline friend mat4 operator/ ( const mat4 &a, const mat4 &b );

	inline friend mat4 operator* ( const mat4 &a, const float f );
	inline friend mat4 operator* ( const float f, const mat4 &a );
	inline friend mat4 operator/ ( const mat4 &a, const float f );
	inline friend mat4 operator/ ( const float f, const mat4 &a );

	inline friend vec operator*( const mat4 &m, const vec &v );

	inline float Det2();
	inline float Det3();
	inline float Det4();

	inline void Identity();	
	inline void RotX( const float angle );
	inline void RotY( const float angle );
	inline void RotZ( const float angle );
	inline void Scale( const float x, const float y, const float z );
	inline void Scale( const vec &V );
	inline void Translate( const float x, const float y, const float z );
	inline void Translate( const vec &V );
	
//	inline void glGetMatrix();
//	inline void glSetMatrix() const;
//	inline void glMultMatrix() const;

	inline void MakeMatrix( const vec vr, const vec vu, const vec ve );
	inline void Rotate( const vec &p, const vec &vec, const float angle);

	inline void MirrorMatrix( const vec &normal, const vec &pointOnPlane);
	inline void MirrorMatrix( const vec &normal, float distance);

//	inline void Invert();		// ??
//	void GetEulerAngles( float &xT, float &yT, float &zT );
//	void BuildMirrorPlane( const UPlane plane );
};

inline mat4::mat4()		// konstruktor, vytvori jednotkovu maticu
{
	m11 = 1; m12 = 0; m13 = 0; m14 = 0;	// 1 0 0 0
	m21 = 0; m22 = 1; m23 = 0; m24 = 0;	// 0 1 0 0
	m31 = 0; m32 = 0; m33 = 1; m34 = 0;	// 0 0 1 0
	m41 = 0; m42 = 0; m43 = 0; m44 = 1;	// 0 0 0 1
}

inline mat4::mat4( const mat4 &a)	// kopirovaci konstruktor
{
	for(int i=0; i<16; i++) m[i]=a.m[i];
}

// zadanie je po riadkoch, v opacnom porati ako su v OpenGL (tam su po stlpcoch)
inline mat4::mat4( const float Am11, const float Am12, const float Am13, const float Am14,
				   const float Am21, const float Am22, const float Am23, const float Am24,
				   const float Am31, const float Am32, const float Am33, const float Am34,
				   const float Am41, const float Am42, const float Am43, const float Am44)
{
	m11 = Am11; m12 = Am12; m13 = Am13; m14 = Am14;
	m21 = Am21; m22 = Am22; m23 = Am23; m24 = Am24;
	m31 = Am31; m32 = Am32; m33 = Am33; m34 = Am34;
	m41 = Am41; m42 = Am42; m43 = Am43; m44 = Am44;
}

inline void mat4::Identity()
{
	m11 = 1; m12 = 0; m13 = 0; m14 = 0;	// 1 0 0 0
	m21 = 0; m22 = 1; m23 = 0; m24 = 0;	// 0 1 0 0
	m31 = 0; m32 = 0; m33 = 1; m34 = 0;	// 0 0 1 0
	m41 = 0; m42 = 0; m43 = 0; m44 = 1;	// 0 0 0 1
}

inline mat4 mat4::operator+() const
{	return *this;}

inline mat4 mat4::operator-() const
{
	return mat4( -m11, -m12, -m13, -m14,
				 -m21, -m22, -m23, -m24,
				 -m31, -m32, -m33, -m34,
				 -m41, -m42, -m43, -m44 );
}

inline mat4 mat4::operator!() const		// Inverse ???
{
	mat4 A;

	const float M3344 = m33 * m44 - m34 * m43;
	const float M3244 = m32 * m44 - m34 * m42;
	const float M3243 = m32 * m43 - m33 * m42;
	const float M2344 = m23 * m44 - m24 * m43;
	const float M2244 = m22 * m44 - m24 * m42;
	const float M2243 = m22 * m43 - m23 * m42;
	const float M2334 = m23 * m34 - m24 * m33;
	const float M2234 = m22 * m34 - m24 * m32;
	const float M2233 = m22 * m33 - m23 * m32;
	const float M3144 = m31 * m44 - m34 * m41;
	const float M3143 = m31 * m43 - m33 * m41;
	const float M2144 = m21 * m44 - m24 * m41;
	const float M2143 = m21 * m43 - m23 * m41;
	const float M2134 = m21 * m34 - m24 * m31;
	const float M2133 = m21 * m33 - m23 * m31;
	const float M3142 = m31 * m42 - m32 * m41;
	const float M2142 = m21 * m42 - m22 * m41;
	const float M2132 = m21 * m32 - m22 * m31;

	A.m11 =  m22 * M3344 - m23 * M3244 + m24 * M3243;
	A.m12 = -m12 * M3344 + m13 * M3244 - m14 * M3243;
	A.m13 =  m12 * M2344 - m13 * M2244 + m14 * M2243;
	A.m14 = -m12 * M2334 + m13 * M2234 - m14 * M2233;

	A.m21 = -m21 * M3344 + m23 * M3144 - m24 * M3143;
	A.m22 =  m11 * M3344 - m13 * M3144 + m14 * M3143;
	A.m23 = -m11 * M2344 + m13 * M2144 - m14 * M2143;
	A.m24 =  m11 * M2334 - m13 * M2134 + m14 * M2133;

	A.m31 =  m21 * M3244 - m22 * M3144 + m24 * M3142;
	A.m32 = -m11 * M3244 + m12 * M3144 - m14 * M3142;
	A.m33 =  m11 * M2244 - m12 * M2144 + m14 * M2142;
	A.m34 = -m11 * M2234 + m12 * M2134 - m14 * M2132;

	A.m41 = -m21 * M3243 + m22 * M3143 - m23 * M3142;
	A.m42 =  m11 * M3243 - m12 * M3143 + m13 * M3142;
	A.m43 = -m11 * M2243 + m12 * M2143 - m13 * M2142;
	A.m44 =  m11 * M2233 - m12 * M2133 + m13 * M2132;

	A /= m11 * A.m11 + m12 * A.m21 + m13 * A.m31 + m14 * A.m41;   // Determinant
	return A;
}

inline mat4 mat4::operator~() const		// Transpose
{
	return mat4(m11, m21, m31, m41,
				m12, m22, m32, m42,
				m13, m23, m33, m43,
				m14, m24, m34, m44 );
}

inline void mat4::operator+= ( const mat4 &a )
{
	for(int i=0; i < 16; i++) m[i] += a.m[i];
}

inline void mat4::operator-= ( const mat4 &a )
{
	for(int i=0; i < 16; i++) m[i] -= a.m[i];
}

inline void mat4::operator*= ( const mat4 &b )
{
	mat4 a(*this);

	m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
	m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
	m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
	m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

	m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
	m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
	m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
	m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

	m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
	m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
	m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
	m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

	m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
	m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
	m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
	m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;
}

inline void mat4::operator/= ( const mat4 &a )
{	*this *= !a;}

inline void mat4::operator *= (float f)
{
	for(int i=0; i < 16; i++) m[i] *= f;
}

inline void mat4::operator /= (float f)
{
	float d=1.0f/f;
	for(int i=0; i < 16; i++) m[i] *= d;
}

inline mat4 operator+ ( const mat4 &a, const mat4 &b )
{
	return mat4( a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
				 a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
				 a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34,
				 a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44 );
}

inline mat4 operator- ( const mat4 &a, const mat4 &b )
{
	return mat4( a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14,
				 a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24,
				 a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34,
				 a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44 );
}
inline mat4 operator* ( const mat4 &a, const mat4 &b )
{
	mat4 c;
	
	c.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
	c.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
	c.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
	c.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

	c.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
	c.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
	c.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
	c.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

	c.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
	c.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
	c.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
	c.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

	c.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
	c.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
	c.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
	c.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;
	return c;
}

inline mat4 operator/ ( const mat4 &a, const mat4 &b )
{
	return a * !b;
}

inline mat4 operator* ( const mat4 &a, const float f )
{
	return mat4( a.m11*f, a.m12*f, a.m13*f, a.m14*f,
				 a.m21*f, a.m22*f, a.m23*f, a.m24*f,
				 a.m31*f, a.m32*f, a.m33*f, a.m34*f,
				 a.m41*f, a.m42*f, a.m43*f, a.m44*f );
}
inline mat4 operator* ( const float f, const mat4 &a )
{
	return mat4( a.m11*f, a.m12*f, a.m13*f, a.m14*f,
				 a.m21*f, a.m22*f, a.m23*f, a.m24*f,
				 a.m31*f, a.m32*f, a.m33*f, a.m34*f,
				 a.m41*f, a.m42*f, a.m43*f, a.m44*f );
}
inline mat4 operator/ ( const mat4 &a, const float f )
{
	float f_ = 1.0f/f;
	return mat4( a.m11*f_, a.m12*f_, a.m13*f_, a.m14*f_,
				 a.m21*f_, a.m22*f_, a.m23*f_, a.m24*f_,
				 a.m31*f_, a.m32*f_, a.m33*f_, a.m34*f_,
				 a.m41*f_, a.m42*f_, a.m43*f_, a.m44*f_ );
}
inline mat4 operator/ ( const float f, const mat4 &a )
{
	return f * !a;
}

inline vec operator*( const mat4 &m, const vec &v )
{															// m11 m12 m13 m14   v.x
	return vec( m.m11*v.x + m.m12*v.y + m.m13*v.z + m.m14,	// m21 m22 m23 m24 X v.y
				m.m21*v.x + m.m22*v.y + m.m23*v.z + m.m24,	// m31 m32 m33 m34   v.z
				m.m31*v.x + m.m32*v.y + m.m33*v.z + m.m34 );// m41 m42 m43 m44   1.0f
}

inline float mat4::Det2() 
{
	return m11 * m22 - m12 * m21; 
}

inline float mat4::Det3() 
{
	return	m11 * (m22 * m33 - m32 * m23) -
			m21 * (m12 * m33 - m32 * m13) +
			m31 * (m12 * m23 - m22 * m13);
}

inline float mat4::Det4()
{
	const float M3344 = m33 * m44 - m43 * m34;
	const float M2344 = m23 * m44 - m43 * m24;
	const float M2334 = m23 * m34 - m33 * m24;
	const float M1344 = m13 * m44 - m43 * m14;
	const float M1334 = m13 * m34 - m33 * m14;
	const float M1324 = m13 * m24 - m23 * m14;	
	return	m11 * (m22 * M3344 - m32 * M2344 + m42 * M2334) -
			m21 * (m12 * M3344 - m32 * M1344 + m42 * M1334) +
			m31 * (m12 * M2344 - m22 * M1344 + m42 * M1324) -
			m41 * (m12 * M2334 - m22 * M1334 + m32 * M1324);
}

inline void mat4::RotX( const float angle )
{
	const float c = (float) cos( PI180*angle );
	const float s = (float) sin( PI180*angle );
	mat4 m(*this);

	m.m12 =  c*m12 + s*m13;	m.m13 = -s*m12 + c*m13;		// 1  0  0  0
	m.m22 =  c*m22 + s*m23;	m.m23 = -s*m22 + c*m23;		// 0  c -s  0
	m.m32 =  c*m32 + s*m33;	m.m33 = -s*m32 + c*m33;		// 0  s  c  0
	m.m42 =  c*m42 + s*m43;	m.m43 = -s*m42 + c*m43;		// 0  0  0  1
	*this = m;
}

inline void mat4::RotY( const float angle )
{
	const float c = (float) cos( PI180*angle );
	const float s = (float) sin( PI180*angle );
	mat4 m(*this);

	m.m11 = c*m11 - s*m13;	m.m13 = s*m11 + c*m13;	// c  0  s  0
	m.m21 = c*m21 - s*m23;	m.m23 = s*m21 + c*m23;	// 0  1  0  0
	m.m31 = c*m31 - s*m33;	m.m33 = s*m31 + c*m33;	//-s  0  c  0
	m.m41 = c*m41 - s*m43;	m.m43 = s*m41 + c*m43;	// 0  0  0  1
	*this = m;
}

inline void mat4::RotZ( const float angle )
{
	const float c = (float) cos( PI180*angle );
	const float s = (float) sin( PI180*angle );
	mat4 m(*this);
	m.m11 = c*m11 + s*m12;	m.m12 = -s*m11 + c*m12;	// c -s  0  0
	m.m21 = c*m21 + s*m22;	m.m22 = -s*m21 + c*m22;	// s  c  0  0
	m.m31 = c*m31 + s*m32;	m.m32 = -s*m31 + c*m32;	// 0  0  1  0
	m.m41 = c*m41 + s*m42;	m.m42 = -s*m41 + c*m42;	// 0  0  0  1
	*this = m;
}

inline void mat4::Scale( const float x, const float y, const float z )
{
	m11 *= x;	m12 *= y;	m13 *= z;	// x 0 0 0
	m21 *= x;	m22 *= y;	m23 *= z;	// 0 y 0 0
	m31 *= x;	m32 *= y;	m33 *= z;	// 0 0 z 0
	m41 *= x;	m42 *= y;	m43 *= z;	// 0 0 0 1
}
inline void mat4::Scale( const vec &V )
{
	m11 *= V.x;	m12 *= V.y;	m13 *= V.z;	// x 0 0 0
	m21 *= V.x;	m22 *= V.y;	m23 *= V.z;	// 0 y 0 0
	m31 *= V.x;	m32 *= V.y;	m33 *= V.z;	// 0 0 z 0
	m41 *= V.x;	m42 *= V.y;	m43 *= V.z;	// 0 0 0 1
}

inline void mat4::Translate( const float x, const float y, const float z )
{
	m14 += m11*x + m12*y + m13*z;	// 1 0 0 x
	m24 += m21*x + m22*y + m23*z;	// 0 1 0 y
	m34 += m31*x + m32*y + m33*z;	// 0 0 1 z
	m44 += m41*x + m42*y + m43*z;	// 0 0 0 1
}
inline void mat4::Translate( const vec &V )
{
	m14 += m11*V.x + m12*V.y + m13*V.z;	// 1 0 0 V.x
	m24 += m21*V.x + m22*V.y + m23*V.z;	// 0 1 0 V.y
	m34 += m31*V.x + m32*V.y + m33*V.z;	// 0 0 1 V.z
	m44 += m41*V.x + m42*V.y + m43*V.z;	// 0 0 0 1
}

//inline void mat4::glGetMatrix(){glGetFloatv( GL_MODELVIEW_MATRIX, m);}
//inline void mat4::glSetMatrix() const {glLoadMatrixf( m);}
//inline void mat4::glMultMatrix() const {glMultMatrixf( m);}

// vytvori maticu m pre ktoru plati (v pripade, ze m14, m24, m34 == 0):
// m*vr = vec(1,0,0)
// m*vu = vec(0,1,0)
// m*ve = vec(0,0,1)
// m*vec(1,0,0) = vec(vr.x, vu.x, ve.x)
// m*vec(0,1,0) = vec(vr.y, vu.y, ve.y)
// m*vec(0,0,1) = vec(vr.z, vu.z, ve.z)
// matica otoci vektor vr na os x, vu na os y a ve na os z

// matica mT = ~m (transponovana matica) (v pripade ze m14, m24, m34 == 0) 
// nam otoci os x na vr, os y na vu a os z na ve
// mT*vec(1,0,0) = vr
// mT*vec(0,1,0) = vu
// mT*vec(0,0,1) = ve

// vr - vector right - vektor smerujuci vpravo
// vu - vector up    - vektor smerujuci hore
// ve - vector eye   - vektor smerujuci k ociam
// vektory musia byt normalizovane !

inline void mat4::MakeMatrix( const vec vr, const vec vu, const vec ve)
{
	m11 = vr.x;		m12 = vr.y;		m13 = vr.z;
	m21 = vu.x;		m22 = vu.y;		m23 = vu.z;
	m31 = ve.x;		m32 = ve.y;		m33 = ve.z;
}

inline void mat4::Rotate( const vec &p, const vec &vect, const float angle)
{
	vec v(vect);		// 2 krat rychlejsi, z opengl dokumentacie funkcie glRotatef
	v.Normalize();
	mat4 s,u,I;

	// u = vvT
	u.m11 = v.x*v.x;	u.m12 = v.x*v.y;	u.m13 = v.x*v.z;
	u.m21 = v.y*v.x;	u.m22 = v.y*v.y;	u.m23 = v.y*v.z;
	u.m31 = v.z*v.x;	u.m32 = v.z*v.y;	u.m33 = v.z*v.z;
	u.m44 = 0;

	v *= (float)sin( angle*PI180);

	I.m44 = 0;
	s.m11 =    0;	s.m12 = -v.z;	s.m13 =  v.y;
	s.m21 =  v.z;	s.m22 =    0;	s.m23 = -v.x;
	s.m31 = -v.y;	s.m32 =  v.x;	s.m33 =    0;
	s.m44 = 0;

	//	R = vvT + cos(uhol)*(I-vvT) + sin(uhol)*s
	*this = u + (float)cos(angle*PI180)*(I-u) + s;
	m44 = 1;

/*	mat4 T,Rx,Ry,Rz,Ryi,Rxi,Ti;
	vec v(vec);
	float a;

	T.Translate( -p );		// posunutie do bodu -p
	Ti.Translate( p );		// posunutie do bodu p

	v.Normalize();
	a = (float) sqrt( v.y*v.y + v.z*v.z);		// a - velkost vektora premietnutneho do yz

	if(a!=0)				// ak a==0 nema zmysel rotovat okolo x, pretoze v=(1,0,0)
	{
		// 1. rotacia v okolo x tak aby sa v dostal do roviny xz
		//  v\   | y   v sa ma dostat na z (xz), uhol pre rotaciu je uhol medzi v a z
		//    \  |     velkost v v tomto priemete je a
		//     \ |     sin = v.y/a
		//      \|     cos = v.z/a = [(0, v.y, v.z)dot(0, 0, v.z)]/[a*v.z]
		// z-----+                         (  1    0     0  )
		// rotacna matica pre rotaciu x :  (  0   cos  -sin )
		//                                 (  0   sin   cos )
		Rx.m22 = v.z/a; Rx.m23 =-v.y/a;
		Rx.m32 = v.y/a; Rx.m33 = v.z/a;

		Rxi.m22 = v.z/a; Rxi.m23 = v.y/a;
		Rxi.m32 =-v.y/a; Rxi.m33 = v.z/a;
	}
	// 2. rotacia v okolo osi y tak aby sa v stotoznil so z
	//        ^ y           vektor v je uz v rovine xz
	//        |             v budeme rotavat okolo osi y, tak aby sa stotoznil so z
	//        |             -----+----- x  vektor v rotujeme v smere pravotocivej skrutky okolo osi y
	//        +----- x     -x   /|         velkost vektora je v je 1 = (v.x^2+a^2)^0.5
	//       / \               / |         sin = -v.x
	//      /   \             /  |         cos = a
	//    z/     \v         v/   | z                           (  cos  0  sin )
	//                          rotacna matica pre rotaciu y : (   0   1   0  )
	//                                                         ( -sin  0  cos )
	Ry.m11 = a;    Ry.m13 = -v.x;
	Ry.m31 = v.x;  Ry.m33 = a;
	
	Ryi.m11 = a;    Ryi.m13 = v.x;
	Ryi.m31 = -v.x; Ryi.m33 = a;
	
	// 3. rotacia okolo z o uhol angle
	Rz.RotZ( angle);

	*this = Ti*Rxi*Ryi*Rz*Ry*Rx*T;*/
}

inline void mat4::MirrorMatrix( const vec &normal, const vec &pointOnPlane)
{
	MirrorMatrix( normal, -DOT3( normal, pointOnPlane) );
}

inline void mat4::MirrorMatrix( const vec &normal, float distance)
{
	m11 = 1 - 2*normal.x*normal.x;
	m12 =   - 2*normal.x*normal.y;
	m13 =   - 2*normal.x*normal.z;
	m14 =   - 2*normal.x*distance;

	m21 =   - 2*normal.y*normal.x;
	m22 = 1 - 2*normal.y*normal.y;
	m23 =   - 2*normal.y*normal.z;
	m24 =   - 2*normal.y*distance;

	m31 =   - 2*normal.z*normal.x;
	m32 =   - 2*normal.z*normal.y;
	m33 = 1 - 2*normal.z*normal.z;
	m34 =   - 2*normal.z*distance;

	m41 = 0;
	m42 = 0;
	m43 = 0;
	m44 = 1;
}
/*
inline void mat4::Invert()
{
    float a,b,c,d,e,f,g,h,i,j,k,l;
    float dW;

    a = m[ 0];	b = m[ 1];	c = m[ 2];
    d = m[ 4];	e = m[ 5];	f = m[ 6];
    g = m[ 8];	h = m[ 9];	i = m[10];
    j = m[12];	k = m[13];	l = m[14];

    dW = 1.0f / ( a*(e*i - f*h) - ( b*(d*i - f*g) + c*(e*g - d*h) ) );

    m[ 0]= (e*i - f*h) * dW;
    m[ 1]= (c*h - b*i) * dW;
    m[ 2]= (b*f - c*e) * dW;

    m[ 4]= (f*g - d*i) * dW;
    m[ 5]= (a*i - c*g) * dW;
    m[ 6]= (c*d - a*f) * dW;

    m[ 8]= (d*h - e*g) * dW;
    m[ 9]= (b*g - a*h) * dW;
    m[10]= (a*e - b*d) * dW;

    m[12]= ((e*(g*l - i*j) + f*(h*j - g*k) - d*(h*l - i*k)) * dW);
	m[13]= ((a*(h*l - i*k) + b*(i*j - g*l) + c*(g*k - h*j)) * dW);
	m[14]= ((b*(d*l - f*j) + c*(e*j - d*k) - a*(e*l - f*k)) * dW);
}
*/
/*
void mat4::GetEulerAngles( float &xT, float &yT, float &zT )
{
	float angleX, angleY, angleZ;
	angleY = arctan2( m13, FastSqrt(m23*m23+m33*m33) );
	float cosangleY = cosf( angleY );

	if ( absf(cosangleY) > EPSILON ) {
		angleZ = arctan2(-m12/cosangleY, m11/cosangleY);
		angleX = arctan2(-m23/cosangleY, m33/cosangleY);
	} else {
		if ( absf(PIhalf-angleY) < EPSILON )	{
			angleX = arctan2(m21 , m22);
			angleY = PIhalf;
			angleZ = 0.0f;
		} else {
			angleX = arctan2(-m21, m22);
			angleY = -PIhalf;
			angleZ = 0.0f;
		}
	}
	// We set the result:
	float PIUNDER180 = 180.0f / PI;
	xT = (float) ( -angleX*PIUNDER180 );
	yT = (float) ( -angleY*PIUNDER180 );
	zT = (float) ( -angleZ*PIUNDER180 );
}

void mat4::BuildMirrorPlane( const UPlane plane ) {
		const float fX = plane.normal.x;
		const float fY = plane.normal.y;
		const float fZ = plane.normal.z;
		const float fD = plane.dist;
		
		m11 = -(2.0f * fX * fX);
		m21 = -(2.0f * fX * fY);
		m31 = -(2.0f * fX * fZ);
		m41 = 0.0f;

		m12 = -(2.0f * fY * fX);
		m22 = -(2.0f * fY * fY);
		m32 = -(2.0f * fY * fZ);
		m42 = 0.0f;

		m13 = -(2.0f * fZ * fX);
		m23 = -(2.0f * fZ * fY);
		m33 = -(2.0f * fZ * fZ);
		m43 = 0.0f;

		m14 = -(2.0f * fD * fX);
		m24 = -(2.0f * fD * fY);
		m34 = -(2.0f * fD * fZ);
		m44 = 1.0f;
}
*/
/*
	void rotP(vector p, float x, float y, float z)
	{
		matrix trans;
	}

	void rotInv(float x, float y, float z)
	{
		float cx, cy, cz, sx, sy, sz;

		cx = (float)cos(x);
		sx = (float)sin(x);
		cy = (float)cos(y);
		sy = (float)sin(y);
		cz = (float)cos(z);
		sz = (float)sin(z);

		matrix mat;

		mat.m[0] = cy * cz;
		mat.m[4] = cy * sz;
		mat.m[8] = -sy;
		
		mat.m[1] = (sx * sy * cz) - (cx * sz);
		mat.m[5] = (sx * sy * sz) + (cx * cz);
		mat.m[9] = sx * cy;
		
		mat.m[2] = (cx * sy * cz) + (sx * sz);
		mat.m[6] = (cx * sy * sz) - (sx * cz);
		mat.m[10] = cx * cy;

		*this = mat * *this;
	}

	void rot(float x, float y, float z)
	{
		float cx, cy, cz, sx, sy, sz;

		cx = (float)cos(x);
		sx = (float)sin(x);
		cy = (float)cos(y);
		sy = (float)sin(y);
		cz = (float)cos(z);
		sz = (float)sin(z);

		matrix mat;

		mat.m[0] = cy * cz;
		mat.m[1] = cy * sz;
		mat.m[2] = -sy;
		
		mat.m[4] = (sx * sy * cz) - (cx * sz);
		mat.m[5] = (sx * sy * sz) + (cx * cz);
		mat.m[6] = sx * cy;
		
		mat.m[8] = (cx * sy * cz) + (sx * sz);
		mat.m[9] = (cx * sy * sz) - (sx * cz);
		mat.m[10] = cx * cy;

		*this = mat * *this;
	}
};
*/
#endif