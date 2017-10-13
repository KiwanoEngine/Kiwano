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

void WindowCenter(HWND hWnd)
{
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小
	tagRECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	// 设置窗口在屏幕居中
	SetWindowPos(
		hWnd, 
		HWND_TOP,
		(screenWidth - (rcWindow.left - rcWindow.right)) / 2,
		(screenHeight - (rcWindow.top - rcWindow.bottom)) / 2,
		(rcWindow.left - rcWindow.right),
		(rcWindow.top - rcWindow.bottom),
		SWP_SHOWWINDOW | SWP_NOSIZE
	);
}
