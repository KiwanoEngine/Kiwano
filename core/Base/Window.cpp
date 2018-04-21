#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// 窗口句柄
static HWND s_HWnd = nullptr;
// 是否打开控制台
static bool s_bShowConsole = false;


bool e2d::Window::__init()
{
	// 注册窗口类
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"Easy2DApp";
	wcex.hIcon = NULL;

	RegisterClassEx(&wcex);

	// 因为 CreateWindow 函数使用的是像素大小，获取系统的 DPI 以使它
	// 适应窗口缩放
	float dpiX = Renderer::getDpiScaleX();
	float dpiY = Renderer::getDpiScaleY();

	UINT nWidth = static_cast<UINT>(ceil(640 * dpiX / 96.f));
	UINT nHeight = static_cast<UINT>(ceil(480 * dpiY / 96.f));

	// 获取屏幕分辨率
	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	// 当输入的窗口大小比分辨率大时，给出警告
	WARN_IF(screenWidth < nWidth || screenHeight < nHeight, "The window is larger than screen!");
	// 取最小值
	nWidth = min(nWidth, screenWidth);
	nHeight = min(nHeight, screenHeight);

	// 计算窗口大小
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX;
	RECT wr = { 0, 0, static_cast<LONG>(nWidth), static_cast<LONG>(nHeight) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// 获取新的宽高
	nWidth = wr.right - wr.left;
	nHeight = wr.bottom - wr.top;

	// 创建窗口
	s_HWnd = ::CreateWindowEx(
		NULL,
		L"Easy2DApp",
		L"Easy2D Game",
		dwStyle,
		(screenWidth - nWidth) / 2, (screenHeight - nHeight) / 2, 
		nWidth, nHeight,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
	);

	HRESULT hr = s_HWnd ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// 禁用输入法
		Window::setTypewritingEnable(false);
		// 查找是否存在控制台
		HWND hwnd = ::GetConsoleWindow();
		if (hwnd)
		{
			// 禁用控制台关闭按钮
			HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			// 默认隐藏控制台
			if (!s_bShowConsole)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
	}
	else
	{
		::UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		::MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

void e2d::Window::__uninit()
{
	// 关闭控制台
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
	// 关闭窗口
	if (s_HWnd)
	{
		::DestroyWindow(s_HWnd);
		s_HWnd = nullptr;
	}
}

void e2d::Window::__poll()
{
	static MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

double e2d::Window::getWidth()
{
	if (s_HWnd)
	{
		// 获取客户区大小
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return rcClient.right - rcClient.left;
	}
	return 0;
}

double e2d::Window::getHeight()
{
	if (s_HWnd)
	{
		// 获取客户区大小
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return rcClient.bottom - rcClient.top;
	}
	return 0;
}

e2d::Size e2d::Window::getSize()
{
	if (s_HWnd)
	{
		// 获取客户区大小
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return Size(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	}
	return Size();
}

HWND e2d::Window::getHWnd()
{
	return s_HWnd;
}

void e2d::Window::setSize(UINT32 width, UINT32 height)
{
	// 计算窗口大小
	DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// 获取新的宽高
	width = wr.right - wr.left;
	height = wr.bottom - wr.top;
	// 获取屏幕分辨率
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// 修改窗口大小，并设置窗口在屏幕居中
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::Window::setTitle(String title)
{
	// 设置窗口标题
	::SetWindowText(s_HWnd, title);
}

void e2d::Window::setIcon(LPCTSTR pIconID)
{
	
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HICON hIcon = (HICON)::LoadImage(hInstance, pIconID, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// 设置窗口的图标
	::SendMessage(s_HWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(s_HWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

e2d::String e2d::Window::getTitle()
{
	wchar_t wszTitle[MAX_PATH] = { 0 };
	::GetWindowText(s_HWnd, wszTitle, MAX_PATH);
	return wszTitle;
}

void e2d::Window::showConsole(bool show /* = true */)
{
	s_bShowConsole = show;
	// 查找已存在的控制台句柄
	HWND hwnd = ::GetConsoleWindow();
	// 关闭控制台
	if (show)
	{
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		else
		{
			// 显示一个新控制台
			if (::AllocConsole())
			{
				hwnd = ::GetConsoleWindow();
				// 重定向输入输出
				FILE * stdoutStream, * stdinStream, * stderrStream;
				freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				freopen_s(&stdinStream, "conin$", "r+t", stdin);
				freopen_s(&stderrStream, "conout$", "w+t", stderr);
				// 禁用控制台关闭按钮
				HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
				::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			}
			else
			{
				MessageBox(nullptr, L"Alloc Console Failed!", L"Error", MB_OK);
			}
		}
	}
	else 
	{
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
}

void e2d::Window::setTypewritingEnable(bool bEnable)
{
	static HIMC hImc = nullptr;

	if (bEnable)
	{
		if (hImc != nullptr)
		{
			::ImmAssociateContext(Window::getHWnd(), hImc);
			hImc = nullptr;
		}
	}
	else
	{
		if (hImc == nullptr)
		{
			hImc = ::ImmAssociateContext(Window::getHWnd(), nullptr);
		}
	}
}


LRESULT e2d::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
	// 处理窗口大小变化消息
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
		// 目标适当。它可能会调用失败，但是这里可以忽略有可能的
		// 错误，因为这个错误将在下一次调用 EndDraw 时产生
		auto pRT = Renderer::getRenderTarget();
		if (pRT) pRT->Resize(D2D1::SizeU(width, height));
	}
	break;

	// 处理分辨率变化消息
	case WM_DISPLAYCHANGE:
	{
		// 重绘客户区
		InvalidateRect(hWnd, NULL, FALSE);
	}
	result = 0;
	break;

	// 重绘窗口
	case WM_PAINT:
	{
		e2d::Renderer::__render();
		ValidateRect(hWnd, NULL);
	}
	result = 0;
	break;

	// 窗口关闭消息
	case WM_CLOSE:
	{
		e2d::Scene * pCurrentScene = e2d::SceneManager::getCurrentScene();
		if (!pCurrentScene || pCurrentScene->onCloseWindow())
		{
			e2d::Game::quit();
		}
	}
	result = 0;
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	result = 1;
	break;

	default:
	{
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	}

	return result;
}