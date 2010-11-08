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
	int LoadVS(char *filename, DWORD *pDeclaration, char* verprog=NULL);
	void SetVertexShader(){	pd->SetVertexShader( hVertexShader); }
	uVertexShader(){ hVertexShader=NULL;}
	virtual ~uVertexShader()
	{	if(hVertexShader!=NULL)pd->DeleteVertexShader( hVertexShader); }
};


class uPixelShader  
{
public:
	DWORD hPixelShader;
	int LoadPS(char *filename, char *pixelprog=NULL);
	void SetPixelShader(){ pd->SetPixelShader( hPixelShader); }
	uPixelShader(){ hPixelShader=NULL;}
	virtual ~uPixelShader()
	{	if(hPixelShader!=NULL) pd->DeletePixelShader( hPixelShader); }
};

#include "Font_bitmap.h"

class uStatistic
{
public:
	int DrawPrimitive_count;
	int vertex_count;
	int triangle_count;
	void Reset()
	{
		DrawPrimitive_count = 0;
		vertex_count = 0;
		triangle_count = 0;
	}
	void Draw(int x, int y, Font_bitmap* font, float fps)
	{
		font->Print_int( x, y+0, DrawPrimitive_count, "DrawPrimitive count:");
		font->Print_int( x, y+1, vertex_count, "vertex count:");
		font->Print_int( x, y+2, triangle_count, "triangle count:");
		font->Print_float( x+30, y+0, (float)floor(fps*DrawPrimitive_count), NULL, 9);
		font->Print_float( x+30, y+1, (float)floor(fps*vertex_count), NULL, 9);
		font->Print_float( x+30, y+2, (float)floor(fps*triangle_count), NULL, 9);
	}
};

extern uStatistic stat;

#include "AseFile.h"
// struct z_face{	vec	a,b,c,normal;};		// for Collision Detection

vec CheckCollision( vector<z_face> &collision, vec vp, vec vpold, float radius);
int CheckCollisionGround( vector<z_face> &collision, vec center, float radius, float angle, float mindist);

#include "Image.h"
#include <vector>
using namespace std;

class uPalette
{
public:
    vector<vec> colors;
    vec get(float t)
    {
        if(colors.size()<2)
        {
            if(colors.size()==1)return colors[0];
            else return vec(1,1,1);
        }
        int index = (int)floor( t*(float)(colors.size()-1) );
        if(index>=colors.size()-1)return colors[colors.size()-1];
        if(index<0)return colors[0];
        float d = t*(float)(colors.size()-1) - (float)index;
        
        return (1-d)*colors[index] + d*colors[index+1];
    }
    void add(vec color){ colors.push_back(color); }
    int load(char* filename)
    {
        Image a;
        vec col;
        if(!a.Load(filename))return 0;
        a.BGRtoRGB();
        for(int x=0; x<a.sizeX; x++)
        {
            col.x = (float)a.data[ (0*a.sizeX+x)*a.planes + 0]/255.f;
            col.y = (float)a.data[ (0*a.sizeX+x)*a.planes + 1]/255.f;
            col.z = (float)a.data[ (0*a.sizeX+x)*a.planes + 2]/255.f;
            colors.push_back(col);
        }
    }
    void set_fire()
    {
        colors.resize(12,vec(0,0,0));
        for(int i=0; i<4; i++)            // from humus, Particles.h, COLOR_SCHEME_FIRE 
        {
            colors[i    ].set(i / 4.0f, 0, 0);
            colors[i + 4].set(1, i / 4.0f, 0);
            colors[i + 8].set((3 - i) / 3.0f,(3 - i) / 3.0f, 1);
        }
        swap();
    }
    void set_ice()
    {
        colors.resize(12,vec(0,0,0));
        for (int i = 0; i < 6; i++)            // from humus, Particles.h, COLOR_SCHEME_ICE 
        {
            colors[i    ] = vec(0,0,i / 6.0f);
            colors[i + 6] = vec(i / 5.0f,1,1);
        }
        swap();
    }
    void set_smoke()
    {
        colors.resize(12,vec(0,0,0));
        for (int i = 0; i < 12; i++)        // from humus, Particles.h, COLOR_SCHEME_SMOKE 
        {
            float f = i / 44.0f;
            colors[i] = vec(f,f,f);
        }
        swap();
    }
    void set_rainbow()
    {
        colors.resize(12,vec(0,0,0));
        colors[0]  = vec(0,   0,   0);    // from humus, Particles.h, COLOR_SCHEME_RAINBOW 
        colors[1]  = vec(0,   0,   0.25f);
        colors[2]  = vec(0,   0,   0.5f);
        colors[3]  = vec(0,   0,   1);
        colors[4]  = vec(0,   0.5f,1);
        colors[5]  = vec(0,   1,   1);
        colors[6]  = vec(0,   1,   0.5f);
        colors[7]  = vec(0,   1,   0);
        colors[8]  = vec(0.5f,1,   0);
        colors[9]  = vec(1,   1,   0);
        colors[10] = vec(1,   0.5f,0);
        colors[11] = vec(1,   0,   0);
        swap();
    }
    void swap()
    {
        for(int i=0; i<colors.size()/2; i++)
        {
            vec temp =     colors[i];
            colors[i] = colors[colors.size()-1-i];
            colors[colors.size()-1-i] = temp;
        }
    }
};

#endif // !defined(AFX_UTIL_H__B9758181_7CAA_11D8_8A6F_E38D8D1F4F11__INCLUDED_)
