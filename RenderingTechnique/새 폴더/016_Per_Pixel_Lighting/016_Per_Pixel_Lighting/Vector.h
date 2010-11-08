#ifndef _VECTOR_H
#define _VECTOR_H
#include <math.h>

#ifndef PI180
#define PI180	0.0174532925199432957692369076848861f	// pi / 180
#endif

#ifndef PI
#define PI		3.1415926535897932384626433832795
#endif

#define MIN(x,y)		((x) < (y) ?  (x)      : (y))					// minimum
#define MAX(x,y)		((x) > (y) ?  (x)      : (y))					// maximum
#define ABS(x)			((x) < (0) ? -(x)      : (x))	

class vec4;

class vec  
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};

	vec(){}
	inline vec( const vec &a);
	inline vec( const vec4 &a);
	inline vec( const float a, const float b, const float c);
	inline void set( const float a, const float b, const float c);
	inline void clear();

	inline vec operator+() const;
	inline vec operator-() const;		// negative

	inline void operator+= ( const vec &a);
	inline void operator-= ( const vec &a);
	inline void operator*= ( const vec &a);
	inline void operator/= ( const vec &a);

	inline void operator+= ( const float f);
	inline void operator-= ( const float f);
	inline void operator*= ( const float f);
	inline void operator/= ( const float f);

	inline vec CROSS( const vec &a) const;
	inline float DOT3( const vec &a) const;
	inline float Length() const;
	inline float Length2() const;		// = Length ^ 2

	inline void RotX( const float angle);
	inline void RotY( const float angle);
	inline void RotZ( const float angle);
	
	inline void Normalize();
	inline void ArbitraryRotate(float theta, vec r);
/*
	inline friend vec operator+ ( const vec &a, const vec &b );
	inline friend vec operator- ( const vec &a, const vec &b );
	inline friend vec operator* ( const vec &a, const vec &b );
	inline friend vec operator/ ( const vec &a, const vec &b );

	inline friend vec operator+ ( const float f, const vec &a );
	inline friend vec operator- ( const float f, const vec &a );
	inline friend vec operator* ( const float f, const vec &a );
//	inline friend vec operator/ ( const float f, const vec &a );
	
	inline friend vec operator+ ( const vec &a, const float f );
	inline friend vec operator- ( const vec &a, const float f );
	inline friend vec operator* ( const vec &a, const float f );
	inline friend vec operator/ ( const vec &a, const float f );

	inline friend int operator< ( const vec &a, const vec &b );
	inline friend int operator<=( const vec &a, const vec &b );
	inline friend int operator> ( const vec &a, const vec &b );
	inline friend int operator>=( const vec &a, const vec &b );
	inline friend int operator==( const vec &a, const vec &b );
	inline friend int operator!=( const vec &a, const vec &b );

	inline friend vec CROSS ( const vec &a, const vec &b );
	inline friend float DOT3  ( const vec &a, const vec &b );
	inline friend vec Normalize( const vec &a );
	inline friend float Length( const vec &a );
	inline friend float Length2( const vec &a );
	inline friend float Distance ( const vec &a, const vec &b);
	inline friend float Distance2 ( const vec &a, const vec &b);
	inline friend vec MakeNormal( const vec &a, const vec &b, const vec &c);
	inline friend vec ReflectedRay( const vec &in, const vec &n);
	inline float PlaneDistance( const vec &normal, const vec &point);
	inline float PlanePointDelta( const vec &normal, float distance, const vec &point);
	inline float PlanePointDistance( const vec &normal, float distance, const vec &point);
	// Collision
	inline vec ClosestPointOnLine( const vec &a, const vec &b, const vec &point);
	inline int SpherePlaneCollision( const vec &center, float radius, const vec &normal, float distance, float *distanceCenterToPlane );
	double AngleBetweenVectors( const vec &a, const vec &b);
	inline int PointInsidePolygon( const vec &point, const vec &a, const vec &b, const vec &c);
	inline int PointInsidePolygon( const vec &point, const vec v[], unsigned int count);
	inline int EdgeSphereCollision( const vec &center, float radius, const vec &a, const vec &b, const vec &c);
	inline int EdgeSphereCollision( const vec &center, float radius, vec v[], unsigned int count);
	inline int SpherePolygonCollision(const vec &center, float radius, vec v[], unsigned int count, const vec &normal);
	inline int SpherePolygonCollision(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal);
	inline int LinePlaneCollision( const vec &a, const vec &b, const vec &normal, float distance);
	inline vec LinePlaneIntersection( const vec &a, const vec &b, const vec &normal, float distance);
	inline vec LinePlaneIntersectionDir( const vec &a, const vec &ab, const vec &normal, float distance);
	inline float LinePlaneIntersectionDirParameter( const vec &a, const vec &ab, const vec &normal, float distance);
	inline int LinePolygonCollision( const vec &a, const vec &b, vec v[], unsigned int count, const vec &normal);
	inline int LineSphereIntersection( const vec &p1, const vec &p2, const vec &center, float radius, double *t1, double *t2 );
	inline int LineSphereIntersectionDir( const vec &p1, const vec &p12, const vec &center, float radius, double *t1, double *t2 );
*/
};
inline char isInfPlus( const float &x);
inline char isInfMinus( const float &x);

inline vec::vec( const vec &a)
{	x = a.x; y = a.y; z = a.z;}
inline vec::vec( const float a, const float b, const float c )
{	x = a; y = b; z = c;}

inline void vec::set( const float a, const float b, const float c)
{	x = a; y = b; z = c;}

inline void vec::clear()
{	x = 0.f; y = 0.f; z = 0.f;}

inline vec vec::operator+() const	{	return *this;}
inline vec vec::operator-() const	{	return vec( -x, -y, -z);}

inline void vec::operator+= ( const vec &a){	x += a.x; y += a.y; z += a.z;}
inline void vec::operator-= ( const vec &a){	x -= a.x; y -= a.y; z -= a.z;}
inline void vec::operator*= ( const vec &a){	x *= a.x; y *= a.y; z *= a.z;}
inline void vec::operator/= ( const vec &a){	x /= a.x; y /= a.y; z /= a.z;}
inline void vec::operator+= ( const float f){	x += f; y += f; z += f; }
inline void vec::operator-= ( const float f){	x -= f; y -= f; z -= f; }
inline void vec::operator*= ( const float f){	x *= f; y *= f; z *= f; }
inline void vec::operator/= ( const float f){	x /= f; y /= f; z /= f; }

inline vec operator+ ( const vec &a, const vec &b ){return vec( a.x+b.x, a.y+b.y, a.z+b.z );}
inline vec operator- ( const vec &a, const vec &b ){return vec( a.x-b.x, a.y-b.y, a.z-b.z );}
inline vec operator* ( const vec &a, const vec &b ){return vec( a.x*b.x, a.y*b.y, a.z*b.z );}
inline vec operator/ ( const vec &a, const vec &b ){return vec( a.x/b.x, a.y/b.y, a.z/b.z );}

inline vec operator+ ( const float f, const vec &a ){return vec( f+a.x, f+a.y, f+a.z );}
inline vec operator- ( const float f, const vec &a ){return vec( f-a.x, f-a.y, f-a.z );}
inline vec operator* ( const float f, const vec &a ){return vec( f*a.x, f*a.y, f*a.z );}
inline vec operator+ ( const vec &a, const float f ){return vec( a.x+f, a.y+f, a.z+f );}
inline vec operator- ( const vec &a, const float f ){return vec( a.x-f, a.y-f, a.z-f );}
inline vec operator* ( const vec &a, const float f ){return vec( a.x*f, a.y*f, a.z*f );}
inline vec operator/ ( const vec &a, const float f ){float f_ = 1.0f/f;return vec( a.x*f_, a.y*f_, a.z*f_ );}

inline int operator< ( const vec &a, const vec &b ){return ((a.x< b.x)&&(a.y< b.y)&&(a.z< b.z));}
inline int operator<=( const vec &a, const vec &b ){return ((a.x<=b.x)&&(a.y<=b.y)&&(a.z<=b.z));}
inline int operator> ( const vec &a, const vec &b ){return ((a.x> b.x)&&(a.y> b.y)&&(a.z> b.z));}
inline int operator>=( const vec &a, const vec &b ){return ((a.x>=b.x)&&(a.y>=b.y)&&(a.z>=b.z));}
inline int operator==( const vec &a, const vec &b ){return ((a.x==b.x)&&(a.y==b.y)&&(a.z==b.z));}
inline int operator!=( const vec &a, const vec &b ){return ((a.x!=b.x)||(a.y!=b.y)||(a.z!=b.z));}

inline vec CROSS ( const vec &a, const vec &b )
{
	return vec( a.y*b.z - a.z*b.y,			//  i   j   k
					a.z*b.x - a.x*b.z,			// a.x a.y a.z
					a.x*b.y - a.y*b.x );		// b.x b.y b.z
}
inline vec vec::CROSS( const vec &a) const
{
	return vec( y*a.z - z*a.y,
					z*a.x - x*a.z,
					x*a.y - y*a.x );
}

inline float DOT3( const vec &a, const vec &b )
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}
inline float vec::DOT3( const vec &a) const
{
	return (x*a.x + y*a.y +z*a.z);
}

inline float vec::Length() const
{	
	return (float) sqrt(x*x + y*y + z*z);
}

inline float vec::Length2() const
{	
	return (float) x*x + y*y + z*z;
}

inline void vec::RotX( const float angle)
{
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float Y=y;
	y =  y*c - z*s;
	z =  Y*s + z*c;
}
inline void vec::RotY( const float angle)
{
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float X=x;
	x =  x*c + z*s;
	z = -X*s + z*c;
}
inline void vec::RotZ( const float angle)
{
	float s = (float) sin( PI180*angle );
	float c = (float) cos( PI180*angle );
	float X=x;
	x =  x*c - y*s;
	y =  X*s + y*c;
}
inline void vec::Normalize()
{
	float length;

	length = (float) sqrt(x*x + y*y + z*z);
	if (length != 0) {
		x /= length;
		y /= length;
		z /= length;
	} else {
		x = 0;
		y = 0;
		z = 0;
	}
}

//	Rotate a vector by angle theta around an arbitrary axis r
//	Positive angles are anticlockwise looking down the axis  towards the origin.
inline void vec::ArbitraryRotate(float theta, vec r)	// Rotate a vector by angle theta around an arbitrary axis r
{
	vec p(x,y,z);
	float costheta,sintheta;

	theta*=PI180;
	r.Normalize();
	costheta = (float)cos(theta);
	sintheta = (float)sin(theta);

	x  = (costheta + (1 - costheta) * r.x * r.x) * p.x;
	x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
	x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

	y  = ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
	y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
	y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

	z  = ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
	z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
	z += (costheta + (1 - costheta) * r.z * r.z) * p.z;
}

inline vec Normalize( const vec &a){	return a/a.Length();}
inline float Length( const vec &a ){	return a.Length();}
inline float Length2( const vec &a ){	return a.Length2();}
inline float Distance ( const vec &a, const vec &b){	return Length( a-b );}
inline float Distance2 ( const vec &a, const vec &b){	return Length2( a-b );}

inline vec MakeNormal( const vec &a, const vec &b, const vec &c)
{
	return Normalize(CROSS( b - a, c - a));
}

inline vec ReflectedRay( const vec &in, const vec &n)	// in from eye to vertex
{
	return (in - 2.f*n*DOT3( in, n));
}

inline float PlaneDistance( const vec &normal, const vec &point)
{
	return -DOT3( normal, point);	//	D = - Ax + By + Cz;   plane equation is Ax + By + Cz + D = 0
}
inline float PlanePointDelta( const vec &normal, float distance, const vec &point)
{
	// if point is on normal side, return positive value, else return negative value
	return DOT3( normal, point) + distance;			// d = Ax + By + Cz + D
}
inline float PlanePointDistance( const vec &normal, float distance, const vec &point)
{	
	// return always positive value
	return ABS(DOT3( normal, point) + distance);			// distance = ABS(Ax + By + Cz + D)
}

// najblisi bod na usecke a,b k bodu point
inline vec ClosestPointOnLine( const vec &a, const vec &b, const vec &point)
{
	vec ap = point - a;			// vector from a to point
	vec ab = b-a;				// vector from a to b
	float d = ab.Length();		// = Distance( a, b);  // length Line
	ab.Normalize();				// normalized direction vector from a to b
	float t = DOT3( ab, ap);	// ab is unit vector, t is distance from a to point projected on line ab
	if( t<= 0) return a;		// point projected on line ab is out of line, closest point on line is a
	if( t>= d) return b;		// point projected on line ab is out of line, closest point on line is b
	return a+t*ab;				// point away from a on t length in direction ab
}
inline int SpherePlaneCollision( const vec &center, float radius, const vec &normal, float distance, float *distanceCenterToPlane )
{
	*distanceCenterToPlane = DOT3( normal, center) + distance;			// d = Ax + By + Cz + D
	if( fabs(*distanceCenterToPlane) < radius) return 1;	// sphere intersected plane
//	if( *distanceCenterToPlane<= -radius) return 2;			// sphere behind plane
//	else return 0;											// sphere is front plane
	return 0;												// sphere is front or behind plane
}

inline double AngleBetweenVectors( const vec &a, const vec &b)
{							
	double angle = acos( DOT3( a, b) / ( a.Length()*b.Length() ) );
//	if(_isnan(angle))return 0;		// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate
	if( isInfPlus( (float)angle)||isInfMinus( (float)angle) )return 0;
	return( angle );
}

// return, t=0 a, t=1 b
inline vec Interpolate( vec a, const vec &b, float t)
{
	if(a==b)return a;
	float angle = (float)AngleBetweenVectors( a, b);
	a.ArbitraryRotate( angle*t/PI180, CROSS( a, b));
	return a;
}

inline int PointInsidePolygon( const vec &point, const vec &a, const vec &b, const vec &c)
{
	vec pa = a-point;
	vec pb = b-point;
	vec pc = c-point;
	double angle  = AngleBetweenVectors( pa, pb);
	angle += AngleBetweenVectors( pb, pc);
	angle += AngleBetweenVectors( pc, pa);

	const double MATCH_FACTOR = 0.99;			// Used to cover up the error in floating point
	if(angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
		return 1;							// The point is inside of the polygon
	return 0;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
}

inline int PointInsidePolygon( const vec &point, const vec v[], unsigned int count)
{
	double angle = 0.0;
	
	for (int i = 0; (unsigned int)i<count; i++)				// Go in a circle to each vertex and get the angle between
		angle += AngleBetweenVectors( v[i] - point, v[(i+1)%count] - point);
											
	const double MATCH_FACTOR = 0.99;			// Used to cover up the error in floating point
	if(angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
		return 1;							// The point is inside of the polygon
	return 0;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
}

inline int EdgeSphereCollision( const vec &center, float radius, const vec &a, const vec &b, const vec &c)
{
	if( Distance(center, ClosestPointOnLine( a, b, center)) < radius) return 1;
	if( Distance(center, ClosestPointOnLine( b, c, center)) < radius) return 1;
	if( Distance(center, ClosestPointOnLine( c, a, center)) < radius) return 1;
	return 0;
}

inline int EdgeSphereCollision( const vec &center, float radius, vec v[], unsigned int count)
{
	for(int i = 0; (unsigned int)i<count; i++)
		if( Distance(center, ClosestPointOnLine(v[i], v[(i+1)%count], center)) < radius) return 1;
	return 0;
}

inline int SpherePolygonCollision(const vec &center, float radius, vec v[], unsigned int count, const vec &normal)
{
	float distance = PlaneDistance( normal, v[0] );
	float distanceCenterToPlane;

	if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
	{
		vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

		if( PointInsidePolygon( centerProjOnPlane, v, count ) ||
			EdgeSphereCollision( center, radius, v, count ) )
		{
			return 1;
		}
	}
	return 0;
}

inline int SpherePolygonCollision(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal)
{
	float distance = PlaneDistance( normal, a );
	float distanceCenterToPlane;

	if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
	{
		vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

		if( PointInsidePolygon( centerProjOnPlane, a, b, c )  ||
			EdgeSphereCollision( center, radius, a, b, c ) )
		{
			return 1;
		}
	}
	return 0;
}

inline int SpherePolygonCollisionRadius05(const vec &center, float radius, const vec &a, const vec &b, const vec &c, const vec &normal)
{
	float distance = PlaneDistance( normal, a );
	float distanceCenterToPlane;

	if(SpherePlaneCollision( center, radius, normal, distance, &distanceCenterToPlane) == 1) 
	{
		vec centerProjOnPlane = center - distanceCenterToPlane*normal;			// center project on plane

		if( PointInsidePolygon( centerProjOnPlane, a, b, c )  ||
			EdgeSphereCollision( center, 0.5f*radius, a, b, c ) )
		{
			return 1;
		}
	}
	return 0;
}

inline int LinePlaneCollision( const vec &a, const vec &b, const vec &normal, float distance)
{
	float distance_a_plane = DOT3( normal, a) + distance;
	float distance_b_plane = DOT3( normal, b) + distance;
	if( distance_a_plane*distance_b_plane < 0) return 1;	// must have differently sign
	return 0;
}

inline vec LinePlaneIntersectionDir( const vec &a, const vec &ab, const vec &normal, float distance)
{
	vec ab_norm = Normalize( ab);
	float distance_plane_a = DOT3( normal, a) + distance;
	float divisor = DOT3( normal, ab_norm);
	if(divisor==0.f) return a;				// line lie on plane
	float t = distance_plane_a/divisor;
	return a-t*ab_norm;
}

inline float LinePlaneIntersectionDirParameter( const vec &a, const vec &ab, const vec &normal, float distance)
{
	vec ab_norm = Normalize( ab);
	float distance_plane_a = DOT3( normal, a) + distance;
	float divisor = DOT3( normal, ab_norm);
	if(divisor==0.f) return 0.f;				// line lie on plane
	float t = distance_plane_a/divisor;
	return -t/ab.Length();						// for point a t=0, for a+ab t=1.0f 
}

inline vec LinePlaneIntersection( const vec &a, const vec &b, const vec &normal, float distance)
{
	return LinePlaneIntersectionDir( a, b-a, normal, distance );
}

inline int LinePolygonCollision( const vec &a, const vec &b, vec v[], unsigned int count, const vec &normal)
{
	float distance = PlaneDistance( normal, v[0]);
	if( !LinePlaneCollision( a, b, normal, distance) ) return 0;

	vec Intersection = LinePlaneIntersection( a, b, normal, distance);
	if( PointInsidePolygon( Intersection, v, count) ) return 1;
	return 0;
}

inline int LineSphereIntersectionDir( const vec &p1, const vec &p12, const vec &center, float radius, double *t1, double *t2 )
{
//	vec	p12 = p2-p1;
	double a = DOT3(p12, p12);		// a is always positive
	double b = 2.0*DOT3( p12, p1 - center);
	double c = DOT3( center, center) + DOT3( p1, p1) - 2.0*DOT3( center, p1) - radius*radius;
	double diskriminant = b*b - 4.0*a*c;

	if(diskriminant<0 || a==0){	*t1 = 0;*t2 = 0;return 0;}
	if(diskriminant==0)
	{
		*t1 = ( -b/(2.0*a) );
		*t2 = *t1;
		return 1;
	}
	double s_diskriminant = sqrt(diskriminant);
	*t1 = ( (-b + s_diskriminant)/(2.0*a) );
	*t2 = ( (-b - s_diskriminant)/(2.0*a) );
	// because a is always positive, t1 > t2
	return 2;
}

inline int LineSphereIntersection( const vec &p1, const vec &p2, const vec &center, float radius, double *t1, double *t2 )
{
	return LineSphereIntersectionDir( p1, p2-p1, center, radius, t1, t2);
}
/*  angle1
   in\  A n-nomala
      \ |     n1 - absolute refraction index of material 1
       V|                        sin(angle1)   v1   c/n1   n2        1.0
   -----+------  nr = n2/n1      ----------- = -- = ---- = -- = nr = ---
         \                       sin(angle2)   v2   c/n2   n1        nri
         |    n2 - absolute refraction index of material 2
         \                       n1*sin(angle1) = n2*sin(angle2)
   angle2 | T - transmission vector
in - vector from eye to vertex, must be normalized
n - normal vector
nr - relative refraction index, water nr = 1.33
nri - nri = 1.f/nr
  T = nri*in - nri*(in*n)*n - sqrt( 1.0-nri^2*(1.0-(in,n)^2) )*n   */
// *i - call with nri = 1/relative refraction index
// *n - vector in is Normalized
inline vec RefractedRayin(const vec &in, const vec &n, float nri)	// in from eye to vertex
{
//	float nri=1.f/nr;
	float dot_in_n = DOT3(in,n);
	float a = (float)sqrt( 1.f-nri*nri*(1.f-dot_in_n*dot_in_n) );
	return nri*in - (a+nri*dot_in_n)*n;
}
inline vec RefractedRayi( vec &in, const vec &n, float nri)		// normalize in
{	return RefractedRayin( Normalize(in), n, nri);}
inline vec RefractedRay( vec &in, const vec &n, float nr)
{	return RefractedRayin( Normalize(in), n, 1.f/nr);}

// work also if DOT3( in, n) > 0.f
inline vec RefractedRayic( vec &in, const vec &n, float nri)	// in from eye to vertex
{
	if( DOT3( in, n) > 0.f)
		return RefractedRayi( in, -n, 1.f/nri);
	else 
		return RefractedRayi( in, n, nri);
}

inline float FresnelF0( float nr)
{
	float nri=1.f/nr;
	float f0,f0_;
	f0 = 1-nri;		//      (1-nri)^2
	f0 *= f0;		// f0 = ----------
	f0_ = 1+nri;	//      (1+nri)^2
	f0_ *= f0_;
	f0 /= f0_;
	return f0;
}
// Fresnel
inline float Fresnelin( const vec &in, const vec &n, float nri, float f0)
{
	float dot = DOT3( n, -in);
	float dot2 = dot*dot;
	dot *= dot2*dot2;		// dot = (n*v)^5
	return f0+(1.f-f0)*dot;
}
inline float Fresneli( const vec &in, const vec &n, float nri, float f0)
{	return Fresnelin( Normalize(in), n, nri, f0);}
inline float Fresnel( const vec &in, const vec &n, float nr, float f0)
{	return Fresnelin( Normalize(in), n, 1.f/nr, f0);}

inline float Fresnelin_1( const vec &in, const vec &n, float nri, float f0)
{
	float dot = DOT3( n, -in);	// dot = (n*v)^1
	return f0+(1.f-f0)*dot;
}
inline float Fresnelin_3( const vec &in, const vec &n, float nri, float f0)
{
	float dot = DOT3( n, -in);
	dot *= dot*dot;				// dot = (n*v)^3
	return f0+(1.f-f0)*dot;
}

inline char isInfPlus( const float &x)
{
	float a;
	*(unsigned int*)&a = (unsigned int)0x7f800000;
	return ( x==a);
//	return (*(unsigned int*) &x==0x7f800000);
}

inline float Fresnel2in( const vec &in, const vec &n, float nri)
{
	float k,g;
	
	k = DOT3( in, n);
	g = (float)sqrt(nri*nri+k*k-1);

	float gmk = g-k;
	float gpk = g+k;

	float a = k*gpk-1;
	float b = k*gmk+1;
	return (gmk*gmk/(2*gpk*gpk)) *(1+(a*a)/(b*b));
}
inline float Fresnel2( const vec &in, const vec &n, float nr)
{	return Fresnel2in( Normalize(in), n, 1.f/nr);}

inline char isInfMinus( const float &x)
{
	float a;
	*(unsigned int*)&a = (unsigned int)0xff800000;
	return ( x==a);
//	return (*(unsigned int*) &x==0xff800000);
}


class vec2  
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	void set( float x_, float y_){ x=x_; y=y_;}
};

static inline void tangent( const vec &v0, const vec &v1, const vec &v2, const vec2 &t0, const vec2 &t1, const vec2 &t2, vec &tg)
{
	vec edge0,edge1;
	vec2 cross;

	edge0.x = v1.x - v0.x;
	edge0.y = t1.x - t0.x;
	edge0.z = t1.y - t0.y;

	edge1.x = v2.x - v0.x;
	edge1.y = t2.x - t0.x;
	edge1.z = t2.y - t0.y;

//	cross = CROSS( edge0, edge1);
	cross.x = edge0.y*edge1.z - edge0.z*edge1.y;
	cross.y = edge0.z*edge1.x - edge0.x*edge1.z;
	tg.x = - cross.y / cross.x;

	edge0.x = v1.y - v0.y;
	edge1.x = v2.y - v0.y;
	cross.x = edge0.y*edge1.z - edge0.z*edge1.y;
	cross.y = edge0.z*edge1.x - edge0.x*edge1.z;
	tg.y = - cross.y / cross.x;

	edge0.x = v1.z - v0.z;
	edge1.x = v2.z - v0.z;
	cross.x = edge0.y*edge1.z - edge0.z*edge1.y;
	cross.y = edge0.z*edge1.x - edge0.x*edge1.z;
	tg.z = - cross.y / cross.x;
}

class vec4 
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float v[4];
	};

	vec4(){}
	inline vec4( const vec4 &a);
	inline vec4( const vec &in);
	inline vec4( const float a, const float b, const float c, const float d = 1.f);
	inline void set( const float a, const float b, const float c, const float d = 1.f);
	inline void clear();
	inline unsigned getRGBA()
	{
		unsigned int col;
		int tmp;

		tmp = int(x * 255);
		if (tmp < 0  ) tmp = 0;
		if (tmp > 255) tmp = 255;
		col = tmp;
		tmp = int(y * 255);
		if (tmp < 0  ) tmp = 0;
		if (tmp > 255) tmp = 255;
		col |= tmp << 8;
		tmp = int(z * 255);
		if (tmp < 0  ) tmp = 0;
		if (tmp > 255) tmp = 255;
		col |= tmp << 16;
		tmp = int(w * 255);
		if (tmp < 0  ) tmp = 0;
		if (tmp > 255) tmp = 255;
		col |= tmp << 24;
		return col;
	}
};

inline vec4::vec4( const vec4 &a)
{	x = a.x; y = a.y; z = a.z; w = a.w;}
inline vec4::vec4( const vec &in)
{	x = in.x; y = in.y; z = in.z; w = 1.f;}
inline vec4::vec4( const float a, const float b, const float c, const float d)
{	x = a; y = b; z = c; w = d;}
inline void vec4::set( const float a, const float b, const float c, const float d)
{	x = a; y = b; z = c; w = d;}
inline void vec4::clear()
{	x = 0; y = 0; z = 0; w = 1;}

inline vec::vec( const vec4 &a)
{	x = a.x; y = a.y; z = a.z;}


inline vec Bspline( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
{
	float t2=t*t;		// t^2
	float c3=t2*t;		// t^3
	float c0,c1,c2;

	c0 = 1.f - t;	c0 *= c0*c0;		// c0 = (1-t)^3
	c1 = 3.f*c3 - 6.f*t2 + 4;			// c1 = 3*t^3 - 6*t^2 + 4
	c2 =-3.f*c3 + 3.f*t2 + 3.f*t + 1.f;	// c2 =-3*t^3 + 3*t^2 + 3*t + 1
//	c3 = t*t*t;
//	vec v = c0*p0 + c1*p1 + c2*p2 + c3*p3;
//	v *= 0.166666666666666666666666666666667f;	// 1/6
	return 0.166666666666666666666666666666667f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
}

inline vec BsplineDeriv( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
{
	float t2=t*t;		// t^2
	float c0,c1,c2,c3;
//	(1-t)^3 = (1-t)*(1-t-t+t^2) = (1-t)*(1-2*t+t^2) = (1-2*t+t^2 -t +2*t^2 - t^3) = (1-3*t+3*t^2-t^3)
	c0 =-3.f*t2 + 6.f*t - 3;			// c0 =-3*t^2 + 6*t - 3
	c1 = 9.f*t2 -12.f*t;				// c1 = 9*t^2 -12*t
	c2 =-9.f*t2 + 6.f*t + 3;			// c2 =-9*t^2 + 6*t + 3
	c3 = 3.f*t*t;						// c3 = 3*t^2
	return 0.166666666666666666666666666666667f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
}

inline vec HermiteCurve( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t)
{
	float t2 = t*t;
	float t3 = t2*t;
	float h1 =  2*t3 - 3*t2 + 1;
	float h2 = -2*t3 + 3*t2;
	float h3 =    t3 - 2*t2 + t;
	float h4 =    t3 -   t2;
	return h1*p0 + h2*p1 + h3*t0 + h4*t1;
}

inline vec HermiteCurveDeriv( const vec &p0, const vec &p1, const vec &t0, const vec &t1, float t)
{
	float t2 = t*t;
	float h1 =  6*t2 - 6*t;
	float h2 = -6*t2 + 6*t;
	float h3 =  3*t2 - 4*t + 1;
	float h4 =  3*t2 - 2*t;
	return h1*p0 + h2*p1 + h3*t0 + h4*t1;
} 


inline vec Catmull_Rom_spline( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
{
	float t2=t*t;		// t^2
	float t3=t2*t;		// t^3
	float c0,c1,c2,c3;

	c0 = -     t3 + 2.f*t2 - t;
	c1 =   3.f*t3 - 5.f*t2     + 2;
	c2 = - 3.f*t3 + 4.f*t2 + t;
	c3 =       t3 -     t2;

	return 0.5f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
}

inline vec Catmull_Rom_splineDeriv( const vec &p0, const vec &p1, const vec &p2, const vec &p3, float t)
{
	float t2=t*t;		// t^2
	float c0,c1,c2,c3;

	c0 = - 3.f*t2 + 4.f*t - 1.0f;
	c1 =   9.f*t2 -10.f*t;
	c2 = - 9.f*t2 + 8.f*t + 1.0f;
	c3 =   3.f*t2 - 2.f*t;

	return 0.5f*(c0*p0 + c1*p1 + c2*p2 + c3*p3);
}

#endif
