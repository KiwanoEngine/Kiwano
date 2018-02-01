#include "..\ebase.h"
#include "..\emanagers.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// 窗口句柄
static HWND s_HWnd = nullptr;
// 是否打开控制台
static bool s_bShowConsole = false;


bool e2d::EWindow::__init(LPCTSTR sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID /*= nullptr*/)
{
	// 注册窗口类
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = EWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"Easy2DApp";
	// 设置程序图标
	if (pIconID)
	{
		wcex.hIcon = (HICON)::LoadImage(
			GetModuleHandle(NULL),
			pIconID,
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}

	RegisterClassEx(&wcex);

	// 因为 CreateWindow 函数使用的是像素大小，获取系统的 DPI 以使它
	// 适应窗口缩放
	FLOAT dpiX, dpiY;

	// 工厂将返回当前的系统 DPI，这个值也将用来创建窗口
	ERenderer::getID2D1Factory()->GetDesktopDpi(&dpiX, &dpiY);

	nWidth = static_cast<UINT>(ceil(nWidth * dpiX / 96.f));
	nHeight = static_cast<UINT>(ceil(nHeight * dpiY / 96.f));

	// 获取屏幕分辨率
	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	// 当输入的窗口大小比分辨率大时，给出警告
	WARN_IF(screenWidth < nWidth || screenHeight < nHeight, "The window is larger than screen!");
	// 取最小值
	nWidth = min(nWidth, screenWidth);
	nHeight = min(nHeight, screenHeight);

	// 创建窗口
	s_HWnd = CreateWindow(
		L"Easy2DApp",
		sTitle,
		WS_OVERLAPPED | WS_SYSMENU,
		0, 0, nWidth, nHeight,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
	);

	HRESULT hr = s_HWnd ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// 禁用输入法
		EWindow::setTypewritingEnable(false);
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
		UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

void e2d::EWindow::__uninit()
{
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
}

void e2d::EWindow::__poll()
{
	static MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

float e2d::EWindow::getWidth()
{
	return ERenderer::getRenderTarget()->GetSize().width;
}

float e2d::EWindow::getHeight()
{
	return ERenderer::getRenderTarget()->GetSize().height;
}

e2d::ESize e2d::EWindow::getSize()
{
	D2D1_SIZE_F size = ERenderer::getRenderTarget()->GetSize();
	return ESize(size.width, size.height);
}

HWND e2d::EWindow::getHWnd()
{
	return s_HWnd;
}

void e2d::EWindow::setSize(UINT32 width, UINT32 height)
{
	// 获取屏幕分辨率
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小（包含菜单栏）
	tagRECT rcWindow;
	::GetWindowRect(s_HWnd, &rcWindow);
	// 获取客户区大小
	tagRECT rcClient;
	::GetClientRect(s_HWnd, &rcClient);
	// 计算边框大小
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// 修改窗口大小，并设置窗口在屏幕居中
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::EWindow::setTitle(const EString &title)
{
	// 设置窗口标题
	::SetWindowText(s_HWnd, title);
}

e2d::EString e2d::EWindow::getTitle()
{
	wchar_t wszTitle[MAX_PATH] = { 0 };
	::GetWindowText(s_HWnd, wszTitle, MAX_PATH);
	return wszTitle;
}

void e2d::EWindow::showConsole(bool show /* = true */)
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
				freopen_s(&stderrStream, "conerr$", "w+t", stderr);
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

void e2d::EWindow::setTypewritingEnable(bool bEnable)
{
	static HIMC hImc = nullptr;

	if (bEnable)
	{
		if (hImc != nullptr)
		{
			::ImmAssociateContext(EWindow::getHWnd(), hImc);
			hImc = nullptr;
		}
	}
	else
	{
		if (hImc == nullptr)
		{
			hImc = ::ImmAssociateContext(EWindow::getHWnd(), nullptr);
		}
	}
}


LRESULT e2d::EWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		ERenderer::getRenderTarget()->Resize(D2D1::SizeU(width, height));
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
		e2d::ERenderer::__render();
		ValidateRect(hWnd, NULL);
	}
	result = 0;
	break;

	// 窗口关闭消息
	case WM_CLOSE:
	{
		e2d::EScene * pCurrentScene = e2d::ESceneManager::getCurrentScene();
		if (!pCurrentScene || pCurrentScene->onCloseWindow())
		{
			e2d::EGame::quit();
			DestroyWindow(hWnd);
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