#pragma once
#include "..\emacros.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")

HWND &GetHWnd();

ID2D1Factory * &GetFactory();

ID2D1HwndRenderTarget * &GetRenderTarget();


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