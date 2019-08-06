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

#ifndef INITGUID
#	define INITGUID
#endif


// Windows Header Files
#include <windows.h>
#include <wincodec.h>

// C++ RunTime Header Files
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <algorithm>


#if _MSC_VER >= 1900  // >= Visual Studio 2015
#	define E2D_NOEXCEPT noexcept
#else
#	define E2D_NOEXCEPT throw()
#endif


#define E2D_DISABLE_COPY(Class)					\
	Class(const Class &) = delete;				\
	Class & operator= (const Class &) = delete


#if defined( DEBUG ) || defined( _DEBUG )
#	define E2D_WARNING(msg) do { ::OutputDebugStringW(L"[easy2d] Warning: " _CRT_WIDE(msg) L"\r\n"); } while(0)
#else
#	define E2D_WARNING(msg) ((void)0)
#endif


#if defined( DEBUG ) || defined( _DEBUG )
#	define E2D_WARNING_IF(exp, msg) do { if (exp) { ::OutputDebugStringW(L"[easy2d] Warning: " _CRT_WIDE(msg) L"\r\n"); } } while(0)
#else
#	define E2D_WARNING_IF(exp, msg) ((void)0)
#endif
