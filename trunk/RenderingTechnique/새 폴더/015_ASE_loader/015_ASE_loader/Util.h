// Util.h: interface for the Util class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTIL_H__B9758181_7CAA_11D8_8A6F_E38D8D1F4F11__INCLUDED_)
#define AFX_UTIL_H__B9758181_7CAA_11D8_8A6F_E38D8D1F4F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "init.h"

class uFPS_counter
{
public:
	float time;
	float frames;
	float avg_fps;
	float GetAvgFps(float timeframe, float reflesh_time)
	{
		if(timeframe>0.04f)  // fps < 25
		{
			time = 0;
			frames = 0;
			avg_fps = 1.f/timeframe;
		}
		else
		{
			time += timeframe;
			frames++;
			if(time>reflesh_time)
			{
				avg_fps = frames/time;
				time = 0;
				frames = 0;
			}
		}
		return avg_fps;
	}
	uFPS_counter()
	{
		time = 0;
		frames = 0;
		avg_fps = 0;
	}
};

class uVertexShader  
{
public:
    DWORD hVertexShader;
    int Load(char *filename, DWORD *pDeclaration, char* verprog=NULL);
    uVertexShader(){ hVertexShader=NULL;}
    virtual ~uVertexShader()
    {    if(hVertexShader!=NULL)pd->DeleteVertexShader( hVertexShader); }
};

class uPixelShader  
{
public:
    DWORD hPixelShader;
    int LoadPixelShader(char *filename, char *pixelprog=NULL);
    uPixelShader(){ hPixelShader=NULL;}
    virtual ~uPixelShader()
    {    if(hPixelShader!=NULL) pd->DeletePixelShader( hPixelShader); }
};

#include "ase.h"
// struct z_face{	vec	a,b,c,normal;};		// for Collision Detection

vec CheckCollision( vector<z_face> &collision, vec vp, vec vpold, float radius);
int CheckCollisionGround( vector<z_face> &collision, vec center, float radius, float angle, float mindist);

#endif // !defined(AFX_UTIL_H__B9758181_7CAA_11D8_8A6F_E38D8D1F4F11__INCLUDED_)
