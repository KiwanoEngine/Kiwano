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

#ifndef __E2D_MACROS_H__
#define __E2D_MACROS_H__


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

#ifndef DIRECTINPUT_VERSION
#	define DIRECTINPUT_VERSION 0x0800
#endif

#ifndef INITGUID
#	define INITGUID
#endif


// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dinput.h>
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

// C++ RunTime Header Files
#include <map>
#include <set>
#include <list>
#include <stack>
#include <vector>
#include <random>
#include <utility>
#include <chrono>
#include <sstream>
#include <functional>
#include <algorithm>

// Import Libraries
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "shlwapi.lib")


#ifndef HINST_THISCOMPONENT
	EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#	define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


#if _MSC_VER >= 1800
#	define E2D_OP_EXPLICIT explicit
#else
#	define E2D_OP_EXPLICIT
#endif


#if _MSC_VER >= 1900
#	define E2D_NOEXCEPT noexcept
#else
#	define E2D_NOEXCEPT throw()
#endif


#if _MSC_VER >= 1800
#	define E2D_DISABLE_COPY(Class) \
	Class(const Class &) = delete; \
	Class & operator= (const Class &) = delete
#else
#	define E2D_DISABLE_COPY(Class) \
	Class(const Class &); \
	Class & operator= (const Class &)
#endif


#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif


#ifdef UNICODE
#	define OutputDebugStringEx OutputDebugStringExW
#else
#	define OutputDebugStringEx OutputDebugStringExA
#endif


#ifndef E2D_WARNING
#	if defined( DEBUG ) || defined( _DEBUG )
#		define E2D_WARNING(msg) do { ::OutputDebugStringW(L"Warning: " _CRT_WIDE(msg) L"\r\n"); } while(0)
#	else
#		define E2D_WARNING(msg) ((void)0)
#	endif
#endif


#ifndef E2D_WARNING_IF
#	if defined( DEBUG ) || defined( _DEBUG )
#		define E2D_WARNING_IF(exp, msg) do { if (exp) { ::OutputDebugStringW(L"Warning: " _CRT_WIDE(msg) L"\r\n"); } } while(0)
#	else
#		define E2D_WARNING_IF(exp, msg) ((void)0)
#	endif
#endif


#endif // __E2D_MACROS_H__
