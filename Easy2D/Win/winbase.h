#pragma once
#include "..\emacros.h"
#include <chrono>


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


HWND &GetHWnd();

ID2D1Factory * &GetFactory();

ID2D1HwndRenderTarget * &GetRenderTarget();

ID2D1SolidColorBrush * &GetSolidColorBrush();

IWICImagingFactory * &GetImagingFactory();

std::chrono::steady_clock::time_point &GetNow();

long long GetInterval(std::chrono::steady_clock::time_point tLast);


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