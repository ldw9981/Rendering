#ifndef _INIT_H
#define _INIT_H

#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "Vector.h"					// my vector class

#define screen_x_font 800			// horizontal screen size for texts
#define screen_y_font 600			// vertical screen size for texts

#define FIELD_OF_VIEW 45.f			// angle in vertical direction
#define Z_NEAR_VIEW_PLANE 0.1f
#define Z_FAR_VIEW_PLANE 100.f

extern bool	keys[256];				// Array Used For The Keyboard Routine
extern bool	fullscreen;				// Fullscreen Flag
extern LPDIRECT3D8             pD3D;// Used to create the D3DDevice
extern LPDIRECT3DDEVICE8       pd;	// Our rendering device
extern D3DCAPS8 caps;				// the capabilities of the hardware
extern int software_vertexprocessing;
extern		HWND		hWnd;		// Holds Our Window Handle
extern		HINSTANCE	hInstance;	// Holds The Instance Of The Application
extern int	error;					// if is 1 then that is error and program will by close
extern int	screen_x;				// horizontal screen size
extern int	screen_y;				// vertical screen size
extern int	screen_bit;				// cout of bits on 1 pixel
extern int	english;

#endif