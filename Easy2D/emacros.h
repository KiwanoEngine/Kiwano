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



#ifndef ASSERT_IF
#if defined( DEBUG ) || defined( _DEBUG )
	#define ASSERT(b) do {if (!(b)) { OutputDebugStringA("Assert: " #b "\n"); }} while(0)
#else
	#define ASSERT(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef WARN_IF
#define WARN_IF(b, m) do {if (b) { fprintf(stderr, "Warning: " #m "/n"); }} while(0)
#endif


template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }