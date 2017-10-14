#pragma once
#include "..\emacros.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
//#pragma comment(lib, "wincodec.lib")

HWND &GetHWnd();

ID2D1Factory * &GetFactory();

ID2D1HwndRenderTarget * &GetRenderTarget();

ID2D1SolidColorBrush * &GetSolidColorBrush();


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