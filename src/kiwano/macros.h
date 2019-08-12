// Copyright (c) 2016-2018 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#ifndef __cplusplus
#	error Kiwano only supports C++ 
#endif

#ifndef _MSC_VER
#   error Kiwano only supports MSVC compiler
#endif

#ifndef KGE_VS_VER
#	define KGE_VS_VER _MSC_VER
#	define KGE_VS_2013 1800
#	define KGE_VS_2015 1900
#	define KGE_VS_2017 1900
#	define KGE_VS_2019 1920
#endif

#if KGE_VS_VER < KGE_VS_2015
#	error Kiwano only supports Visual Studio 2015 and above
#endif

#ifndef WINVER
#	define WINVER 0x0700	   // Allow use of features specific to Windows 7 or later
#endif

#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0700 // Allow use of features specific to Windows 7 or later
#endif

#ifndef NTDDI_VERSION
#	define NTDDI_VERSION NTDDI_WIN7
#endif

#ifndef UNICODE
#	define UNICODE
#endif

// Exclude rarely-used items from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#	define NOMINMAX
#endif


// Windows Header Files
#include <windows.h>
#include <wincodec.h>

// C++ RunTime Header Files
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

// Compile-time Config Header File
#include "config.h"


#if defined(DEBUG) || defined(_DEBUG)
#	define KGE_DEBUG
#endif


#ifndef KGE_ASSERT
#	ifdef KGE_DEBUG
#		define KGE_ASSERT(EXPR) assert(EXPR)
#	else
#		define KGE_ASSERT __noop
#	endif
#endif


#ifndef KGE_API
#	if defined(KGE_USE_DLL)
#		define KGE_API __declspec( dllimport )
#	elif defined(KGE_EXPORT_DLL)
#		define KGE_API __declspec( dllexport )
#	endif
#endif


#ifndef KGE_API
/* Building or calling Kiwano as a static library */
#	define KGE_API
#else
/*
 * C4251 can be ignored if you are deriving from a type in the 
 * C++ Standard Library, compiling a debug release (/MTd) and 
 * where the compiler error message refers to _Container_base.
 */
#	pragma warning (disable: 4251)
#endif

#define KGE_UNUSED(VAR) ((void)VAR)

#define KGE_DEPRECATED(...) __declspec(deprecated(__VA_ARGS__))
