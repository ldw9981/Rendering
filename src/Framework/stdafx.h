// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <vector>
#include <list>
#include <map>

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <iostream> 


// TODO: reference additional headers your program requires here
