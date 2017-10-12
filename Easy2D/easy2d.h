/******************************************************
* Easy2D Game Engine
* 
* Website: http://www.easy2d.cn
* Source Code: https://gitee.com/werelone/Easy2D
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used items from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <wchar.h>

#include "emacros.h"
#include "ecommon.h"
#include "ebase.h"


#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib, "Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib, "Easy2D.lib")
#endif


// Functions Declare

using namespace e2d;