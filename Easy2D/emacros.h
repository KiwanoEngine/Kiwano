#pragma once


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
#include <assert.h>

// C RunTime Header Files
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef ASSERT_IF
#if defined( DEBUG ) || defined( _DEBUG )
#define ASSERT(b, m) do {if (!(b)) { fprintf(stderr, "Assert: " #m "\n"); assert(b); }} while(0)
#else
	#define ASSERT(b, m) ((void)0)
#endif //DEBUG || _DEBUG
#endif

#ifndef WARN_IF
#if defined( DEBUG ) || defined( _DEBUG )
#define WARN_IF(b, m) do {if (b) { fprintf(stderr, "Warning: " #m "\n"); }} while(0)
#else
#define WARN_IF(b, m) ((void)0)
#endif //DEBUG || _DEBUG
#endif


template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }