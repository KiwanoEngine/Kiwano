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


#include "emacros.h"
#include "etypedef.h"
#include "ecommon.h"
#include "ebase.h"
#include "emanagers.h"
#include "enodes.h"
#include "elisteners.h"
#include "etools.h"
#include "eactions.h"
#include "etransitions.h"
#include "egeometry.h"


#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "Easy2Ddw.lib")
#else
	#pragma comment(lib, "Easy2Dw.lib")
#endif


using namespace e2d;