// Frustum.h: interface for the Frustum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRUSTUM_H__B211D401_6AB4_11D8_8A6F_981030398C0E__INCLUDED_)
#define AFX_FRUSTUM_H__B211D401_6AB4_11D8_8A6F_981030398C0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"			// zakladne nastavenia a zakladne hlavickove subory
//#include <vector>
//using namespace std;

class Frustum  
{
public:
	int CubeInFrustum(const vec &c, float size);
	int BoxInFrustum(const vec &min, const vec &max);
	int SphereInFrustum(const vec &p, float radius);
	void Init();
	int PointInFrustum(const vec &p);
	void Init(const mat4 &m);
	Frustum();
	virtual ~Frustum();
	vec n[6];
	float d[6];
};

#endif // !defined(AFX_FRUSTUM_H__B211D401_6AB4_11D8_8A6F_981030398C0E__INCLUDED_)
