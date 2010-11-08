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

#endif // !defined(AFX_UTIL_H__B9758181_7CAA_11D8_8A6F_E38D8D1F4F11__INCLUDED_)
