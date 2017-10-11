#include "winbase.h"
#include <Windows.h>
#include <tchar.h>
#include <atltypes.h>

static steady_clock::time_point nNow;

steady_clock::time_point GetNow()
{
	return nNow;
}

void FlushSteadyClock()
{
	nNow = steady_clock::now();	// 获取当前时间
}

LONGLONG GetInterval(steady_clock::time_point nLast)
{
	return duration_cast<milliseconds>(nNow - nLast).count();
}

void WindowCenter(HWND hWnd)
{
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小
	CRect rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	// 设置窗口在屏幕居中
	SetWindowPos(hWnd, HWND_TOP,
		(screenWidth - rcWindow.Size().cx) / 2,
		(screenHeight - rcWindow.Size().cy) / 2,
		rcWindow.Size().cx,
		rcWindow.Size().cy,
		SWP_SHOWWINDOW | SWP_NOSIZE);
}

void CreateGameWindow()
{
	HWND _wnd;
	HINSTANCE s_hInstance;
	if (!s_hInstance)
	{
		s_hInstance = GetModuleHandle(NULL);		// Grab An Instance For Our Window

		WNDCLASS  wc;								// Windows Class Structure

													// Redraw On Size, And Own DC For Window.
		wc.style = 0;
		wc.lpfnWndProc = _SoundPlayProc;			// WndProc Handles Messages
		wc.cbClsExtra = 0;							// No Extra Window Data
		wc.cbWndExtra = 0;							// No Extra Window Data
		wc.hInstance = s_hInstance;					// Set The Instance
		wc.hIcon = 0;								// Load The Default Icon
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
		wc.hbrBackground = NULL;					// No Background Required For GL
		wc.lpszMenuName = NULL;						// We Don't Want A Menu
		wc.lpszClassName = _T("Easy2dCallbackWnd");		// Set The Class Name

		if (!RegisterClass(&wc)						// Register Our Class
			&& GetLastError() != 1410)				// Class is Already Existent
		{
			return;
		}
	}

	_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,							// Extended Style For The Window
		_T("Easy2dCallbackWnd"),							// Class Name
		NULL,										// Window Title
		WS_POPUPWINDOW,								// Defined Window Style
		0, 0,										// Window Position
		0, 0,										// Window Width And Height
		NULL,										// No Parent Window
		NULL,										// No Menu
		s_hInstance,								// Instance
		NULL);										// No Param

	if (_wnd)
	{
		//SetWindowLongPtr(_wnd, GWLP_USERDATA, (LONG_PTR)this);
	}
}