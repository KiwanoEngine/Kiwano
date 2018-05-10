#pragma once

#ifndef WINVER
#define WINVER 0x0700       // Allow use of features specific to Windows 7 or later
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0700 // Allow use of features specific to Windows 7 or later
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

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#define INITGUID


// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dinput.h>
#include <d2d1helper.h>
#include <xaudio2.h>

// C RunTime Header Files
#include <stdio.h>
#include <wchar.h>

// Import Libraries
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xaudio2.lib")


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


#ifndef ASSERT
#if defined( DEBUG ) || defined( _DEBUG )
	#define ASSERT(expression, message, ...) do {if (!(expression)) { fwprintf(stderr, L"Assert: " _CRT_WIDE(#message) L"\n", __VA_ARGS__); abort(); }} while(0)
#else
	#define ASSERT(expression, message, ...) ((void)0)
#endif
#endif

#ifndef WARN_IF
#if defined( DEBUG ) || defined( _DEBUG )
	#define WARN_IF(expression, message, ...) do {if (expression) { fwprintf(stderr, L"Warning: " _CRT_WIDE(#message) L"\n", __VA_ARGS__); }} while(0)
#else
	#define WARN_IF(expression, message, ...) ((void)0)
#endif
#endif

