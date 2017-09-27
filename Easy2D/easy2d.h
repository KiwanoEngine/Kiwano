/******************************************************
* Easy2D Game Engine (v1.1.0)
* http://www.easy2d.cn
* 
* Depends on EasyX (Ver:20170827(beta))
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


#include <e2dbase.h>
#include "e2dmsg.h"
#include "e2dtool.h"
#include "e2dstyle.h"
#include "e2dobj.h"
#include "e2daction.h"

#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib,"Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib,"Easy2D.lib")
#endif

using namespace easy2d;