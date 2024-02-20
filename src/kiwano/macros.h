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
#error Kiwano only supports C++
#endif

#ifdef _WIN32
#define KGE_PLATFORM_WINDOWS
#elif __ANDROID__
#define KGE_PLATFORM_ANDROID
#elif __linux__
#define KGE_PLATFORM_LINUX
#elif __APPLE__
#if TARGET_OS_IPHONE
#define KGE_PLATFORM_IPHONE
#elif TARGET_OS_MAC
#define KGE_PLATFORM_MACOS
#else
#error "Unsupported Apple platform"
#endif
#else
#error "Unsupported compiler"
#endif

// C++ RunTime Header Files
#include <cassert>

// Compile-time Config Header File
#include <kiwano/config.h>

#define KGE_MAJOR_VERSION 1
#define KGE_MINOR_VERSION 0
#define KGE_VERSION ((KGE_MAJOR_VERSION << 4) + KGE_MINOR_VERSION)

#define KGE_GET_MAJOR_VERSION(VERSION) ((VERSION & 0x00F0) >> 4)
#define KGE_GET_MINOR_VERSION(VERSION) (VERSION & 0x000F)

#if defined(DEBUG) || defined(_DEBUG)
#define KGE_DEBUG
#endif

#ifndef KGE_ASSERT
#define KGE_ASSERT(COND) assert(COND)
#endif

#define KGE_NOT_USED(VAR) ((void)VAR)

#define KGE_RENDER_ENGINE_NONE 0
#define KGE_RENDER_ENGINE_OPENGL 1
#define KGE_RENDER_ENGINE_OPENGLES 2
#define KGE_RENDER_ENGINE_DIRECTX 3
#define KGE_RENDER_ENGINE KGE_RENDER_ENGINE_NONE

/////////////////////////////////////////////////////////////
//
// Windows platform
//
/////////////////////////////////////////////////////////////

#if defined(KGE_PLATFORM_WINDOWS)

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_NONE
#undef KGE_RENDER_ENGINE
#define KGE_RENDER_ENGINE KGE_RENDER_ENGINE_DIRECTX
#endif

#define KGE_DEPRECATED(...) __declspec(deprecated(__VA_ARGS__))

#define KGE_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define KGE_SUPPRESS_WARNING(CODE) __pragma(warning(disable : CODE))
#define KGE_SUPPRESS_WARNING_POP __pragma(warning(pop))

#ifndef KGE_API
#if defined(KGE_USE_DLL)
#define KGE_API __declspec(dllimport)
#elif defined(KGE_EXPORT_DLL)
#define KGE_API __declspec(dllexport)
#endif
#endif

#ifndef KGE_API
/* Building or calling Kiwano as a static library */
#define KGE_API
#else
/*
 * C4251 can be ignored if you are deriving from a type in the
 * C++ Standard Library, compiling a debug release (/MTd) and
 * where the compiler error message refers to _Container_base.
 */
KGE_SUPPRESS_WARNING(4251)
#endif

#ifdef _MSC_VER
#ifndef KGE_VS_VER
#define KGE_VS_VER _MSC_VER
#define KGE_VS_2013 1800
#define KGE_VS_2015 1900
#define KGE_VS_2017 1900
#define KGE_VS_2019 1920
#endif

#if KGE_VS_VER < KGE_VS_2015
#error Kiwano only supports Visual Studio 2015 and above
#endif

#if defined(KGE_VS_VER) && KGE_VS_VER > KGE_VS_2013
#define KGE_HAS_LITERALS
#endif
#endif

#ifndef WINVER
#define WINVER 0x0700  // Allow use of features specific to Windows 7 or later
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0700  // Allow use of features specific to Windows 7 or later
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WIN7
#endif

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used items from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files
#include <wincodec.h>
#include <windows.h>

#elif defined(KGE_PLATFORM_MACOS)

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_NONE
#undef KGE_RENDER_ENGINE
#define KGE_RENDER_ENGINE KGE_RENDER_ENGINE_OPENGL
#endif

#define KGE_DEPRECATED(...)

#define KGE_SUPPRESS_WARNING_PUSH
#define KGE_SUPPRESS_WARNING(CODE)
#define KGE_SUPPRESS_WARNING_POP

#ifndef KGE_API
#if defined(KGE_USE_DLL)
#define KGE_API
#elif defined(KGE_EXPORT_DLL)
#define KGE_API
#endif
#endif

#ifndef KGE_API
/* Building or calling Kiwano as a static library */
#define KGE_API
#endif

#else

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#error "DirectX render engine is not supported on current platform"
#endif

#endif  // KGE_PLATFORM_WINDOWS
