#pragma once
#include "..\emacros.h"


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


HWND &GetHWnd();

ID2D1Factory * &GetFactory();

ID2D1HwndRenderTarget * &GetRenderTarget();

ID2D1SolidColorBrush * &GetSolidColorBrush();

IWICImagingFactory * &GetImagingFactory();

IDWriteFactory * &GetDirectWriteFactory();

LARGE_INTEGER &GetFreq();

LARGE_INTEGER &GetNow();

bool IsIntervalFull(const LARGE_INTEGER& tLast, const LARGE_INTEGER& tInterval);

LONGLONG ToMilliseconds(LONGLONG tLast);

void SetInterval(LARGE_INTEGER& nInterval, LONGLONG nIntervalMS);


template<class Interface>
inline void SafeReleaseInterface(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = nullptr;
	}
}