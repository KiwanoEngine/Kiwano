#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME
#define REGISTER_CLASS	L"Easy2DApp"


e2d::Window * e2d::Window::_instance = nullptr;

e2d::Window::Window()
	: _hWnd(nullptr)
	, _size(640, 480)
	, _title(L"Easy2D Game")
	, _iconID(0)
	, _dpi(0.f)
{
}

e2d::Window::~Window()
{
	// 关闭控制台
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
	// 关闭窗口
	if (_hWnd)
	{
		::DestroyWindow(_hWnd);
		_hWnd = nullptr;
	}
}

e2d::Window * e2d::Window::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Window;
	return _instance;
}

void e2d::Window::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

bool e2d::Window::createMutex(const String & mutex)
{
	if (mutex.isEmpty())
		return false;

	HANDLE hMutex = ::CreateMutex(nullptr, TRUE, LPCWSTR(L"Easy2DApp-" + mutex));

	if (hMutex == nullptr)
	{
		WARN("CreateMutex Failed!");
		return false;
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// 关闭进程互斥体
		::CloseHandle(hMutex);
		// 打开游戏窗口
		if (!this->_title.isEmpty())
		{
			// 获取窗口句柄
			HWND hProgramWnd = ::FindWindow(REGISTER_CLASS, (LPCTSTR)_title);
			if (hProgramWnd)
			{
				// 获取窗口显示状态
				WINDOWPLACEMENT wpm;
				::GetWindowPlacement(hProgramWnd, &wpm);
				// 将运行的程序窗口还原成正常状态
				wpm.showCmd = SW_SHOW;
				::SetWindowPlacement(hProgramWnd, &wpm);
				::SetWindowPos(hProgramWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		return false;
	}
	return true;
}

HWND e2d::Window::__registerWindow()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpszClassName	= REGISTER_CLASS;
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= Window::WndProc;
	wcex.hIcon			= nullptr;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(LONG_PTR);
	wcex.hInstance		= HINST_THISCOMPONENT;
	wcex.hbrBackground	= nullptr;
	wcex.lpszMenuName	= nullptr;
	wcex.hCursor		= ::LoadCursor(nullptr, IDC_ARROW);

	if (_iconID != 0)
	{
		wcex.hIcon = (HICON)::LoadImage(
			HINST_THISCOMPONENT,
			MAKEINTRESOURCE(_iconID),
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);
	}

	// 注册窗口类
	RegisterClassEx(&wcex);

	// 计算窗口大小
	Rect wRect = __adjustWindow(int(_size.width), int(_size.height));

	// 创建窗口
	HWND hWnd = ::CreateWindowEx(
		NULL,
		REGISTER_CLASS,
		(LPCTSTR)_title,
		WINDOW_STYLE,
		int(wRect.origin.x),
		int(wRect.origin.y),
		int(wRect.size.width),
		int(wRect.size.height),
		nullptr,
		nullptr,
		HINST_THISCOMPONENT,
		nullptr
	);

	if (hWnd)
	{
		// 禁用输入法
		setTypewritingEnabled(false);
		// 禁用控制台关闭按钮
		HWND consoleHWnd = ::GetConsoleWindow();
		if (consoleHWnd)
		{
			HMENU hmenu = ::GetSystemMenu(consoleHWnd, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
		// 获取 DPI
		_dpi = static_cast<float>(::GetDpiForWindow(hWnd));
	}
	else
	{
		::UnregisterClass(REGISTER_CLASS, HINST_THISCOMPONENT);
	}
	return hWnd;
}

e2d::Rect e2d::Window::__adjustWindow(int width, int height)
{
	float dpiScaleX = 0.f, dpiScaleY = 0.f;
	Renderer::getFactory()->GetDesktopDpi(&dpiScaleX, &dpiScaleY);

	Rect result;
	RECT wRECT		= { 0, 0, LONG(ceil(width * dpiScaleX / 96.f)), LONG(ceil(height * dpiScaleY / 96.f)) };
	int maxWidth	= ::GetSystemMetrics(SM_CXSCREEN);
	int maxHeight	= ::GetSystemMetrics(SM_CYSCREEN);

	// 计算合适的窗口大小
	::AdjustWindowRectEx(&wRECT, WINDOW_STYLE, FALSE, NULL);
	width = static_cast<int>(wRECT.right - wRECT.left);
	height = static_cast<int>(wRECT.bottom - wRECT.top);

	// 当输入的窗口大小比分辨率大时，给出警告
	WARN_IF(maxWidth < width || maxHeight < height, "The window is larger than screen!");
	width = std::min(width, maxWidth);
	height = std::min(height, maxHeight);

	float x = float((maxWidth - width) / 2), y = float((maxHeight - height) / 2);
	return std::move(Rect(x, y, float(width), float(height)));
}

void e2d::Window::poll()
{
	while (::PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&_msg);
		::DispatchMessage(&_msg);
	}
}

float e2d::Window::getWidth()
{
	return _size.width;
}

float e2d::Window::getHeight()
{
	return _size.height;
}

e2d::Size e2d::Window::getSize()
{
	return _size;
}

float e2d::Window::getDpi()
{
	return _dpi;
}

e2d::String e2d::Window::getTitle()
{
	return _title;
}

HWND e2d::Window::getHWnd()
{
	if (!_hWnd)
	{
		_hWnd = __registerWindow();
		if (_hWnd == nullptr)
		{
			throw SystemException(L"注册窗口失败");
		}
	}
	return _hWnd;
}

void e2d::Window::setSize(int width, int height)
{
	_size = Size(static_cast<float>(width), static_cast<float>(height));
	if (_hWnd)
	{
		Rect wRect = __adjustWindow(width, height);
		::MoveWindow(
			_hWnd,
			int(wRect.origin.x),
			int(wRect.origin.y),
			int(wRect.size.width),
			int(wRect.size.height),
			TRUE
		);
	}
}

void e2d::Window::setTitle(const String& title)
{
	_title = title;
	if (_hWnd)
	{
		::SetWindowText(_hWnd, (LPCWSTR)title);
	}
}

void e2d::Window::setIcon(int iconID)
{
	this->_iconID = iconID;
	if (_hWnd)
	{
		HICON hIcon = (HICON)::LoadImage(HINST_THISCOMPONENT, MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		// 设置窗口的图标
		::SendMessage(_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		::SendMessage(_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
}

void e2d::Window::setCursor(Cursor cursor)
{
	LPCWSTR pCursorName = nullptr;
	switch (cursor)
	{
	case Cursor::Normal:
		pCursorName = IDC_ARROW;
		break;

	case Cursor::Hand:
		pCursorName = IDC_HAND;
		break;

	case Cursor::No:
		pCursorName = IDC_NO;
		break;

	case Cursor::Wait:
		pCursorName = IDC_WAIT;
		break;

	case Cursor::ArrowWait:
		pCursorName = IDC_APPSTARTING;
		break;

	default:
		break;
	}

	HCURSOR hCursor = ::LoadCursor(nullptr, pCursorName);
	::SetCursor(hCursor);
}

void e2d::Window::setConsoleEnabled(bool enabled)
{
	// 查找已存在的控制台句柄
	HWND hwnd = ::GetConsoleWindow();
	// 关闭控制台
	if (enabled)
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
				errno_t err = freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				WARN_IF(err != 0, "freopen stdout failed!");
				err = freopen_s(&stdinStream, "conin$", "r+t", stdin);
				WARN_IF(err != 0, "freopen stdin failed!");
				err = freopen_s(&stderrStream, "conout$", "w+t", stderr);
				WARN_IF(err != 0, "freopen stderr failed!");
				// 禁用控制台关闭按钮
				HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
				::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
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

void e2d::Window::setTypewritingEnabled(bool enabled)
{
	static HIMC hImc = nullptr;

	if (enabled)
	{
		if (hImc != nullptr)
		{
			::ImmAssociateContext(_hWnd, hImc);
			hImc = nullptr;
		}
	}
	else
	{
		if (hImc == nullptr)
		{
			hImc = ::ImmAssociateContext(_hWnd, nullptr);
		}
	}
}

bool e2d::Window::popup(const String & text, const String & title, Popup style, bool hasCancel)
{
	UINT type = 0;
	switch (style)
	{
	case e2d::Window::Popup::Information:
		type = MB_ICONINFORMATION;
		break;
	case e2d::Window::Popup::Warning:
		type = MB_ICONWARNING;
		break;
	case e2d::Window::Popup::Error:
		type = MB_ICONERROR;
		break;
	default:
		break;
	}

	if (hasCancel)
	{
		type |= MB_OKCANCEL;
	}

	Game::getInstance()->pause();
	int ret = ::MessageBox(_hWnd, (LPCWSTR)text, (LPCWSTR)title, type);
	Game::getInstance()->resume();
	return ret == IDOK;
}


LRESULT e2d::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	bool hasHandled = false;

	switch (message)
	{

	// 处理鼠标消息
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	{
		SceneManager::getInstance()->dispatch(MouseEvent(hWnd, message, wParam, lParam));
	}
	result = 0;
	hasHandled = true;
	break;

	// 处理按键消息
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		SceneManager::getInstance()->dispatch(KeyEvent(hWnd, message, wParam, lParam));
	}
	result = 0;
	hasHandled = true;
	break;

	// 处理窗口大小变化消息
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		if (wParam == SIZE_RESTORED)
		{
			_instance->_size.width = width * 96.f / _instance->_dpi;
			_instance->_size.height = height * 96.f / _instance->_dpi;
		}

		// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
		// 目标适当。它可能会调用失败，但是这里可以忽略有可能的
		// 错误，因为这个错误将在下一次调用 EndDraw 时产生
		auto pRT = Renderer::getInstance()->getRenderTarget();
		if (pRT)
		{
			pRT->Resize(D2D1::SizeU(width, height));
		}
	}
	break;

	// 处理窗口标题变化消息
	case WM_SETTEXT:
	{
		_instance->_title = (const wchar_t*)lParam;
	}
	break;

	// 处理分辨率变化消息
	case WM_DISPLAYCHANGE:
	{
		// 重绘客户区
		InvalidateRect(hWnd, nullptr, FALSE);
	}
	result = 0;
	hasHandled = true;
	break;

	// 重绘窗口
	case WM_PAINT:
	{
		e2d::Renderer::getInstance()->render();
		ValidateRect(hWnd, nullptr);
	}
	result = 0;
	hasHandled = true;
	break;

	// 窗口关闭消息
	case WM_CLOSE:
	{
		e2d::Scene * pCurrentScene = e2d::SceneManager::getInstance()->getCurrentScene();
		if (!pCurrentScene || pCurrentScene->onCloseWindow())
		{
			e2d::Game::getInstance()->quit();
		}
	}
	result = 0;
	hasHandled = true;
	break;

	// 窗口销毁消息
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	result = 1;
	hasHandled = true;
	break;

	}

	if (!hasHandled)
	{
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	return result;
}