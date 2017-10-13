#include "winbase.h"
#include "..\emacros.h"

HWND hwnd = nullptr;
ID2D1Factory * pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget * pRenderTarget = nullptr;


HWND &GetHWnd()
{
	return hwnd;
}

ID2D1Factory * &GetFactory()
{
	return pDirect2dFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	return pRenderTarget;
}

