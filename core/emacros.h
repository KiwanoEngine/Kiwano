#pragma once


// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WIN7
#endif

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used items from Windows headers.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

// Windows Header Files:
#include <windows.h>
#include <assert.h>

// C RunTime Header Files
#include <stdio.h>
#include <wchar.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dinput.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


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