// Copyright (c) 2016-2018 Easy2D - Nomango
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
#	error Easy2D only supports C++ 
#endif

#ifndef _MSC_VER
#   error Easy2D only supports MSVC compiler
#endif

#ifndef VS_VER
#	define VS_VER _MSC_VER
#	define VS_2013 1800
#	define VS_2015 1900
#	define VS_2017 1900
#endif

#if VS_VER < VS_2013
#	error Easy2D only supports Visual Studio 2013 and above
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
#	define E2D_DEBUG
#endif


#if VS_VER >= VS_2015
#	define E2D_NOEXCEPT noexcept
#	define E2D_CONSTEXPR constexpr
#else
#	define E2D_NOEXCEPT throw()
#	define E2D_CONSTEXPR const
#endif


#ifndef E2D_ASSERT
#	ifdef E2D_DEBUG
#		define E2D_ASSERT(EXPR) assert(EXPR)
#	else
#		define E2D_ASSERT __noop
#	endif
#endif

#define E2D_NOT_USED(VAR) ((void)VAR)
