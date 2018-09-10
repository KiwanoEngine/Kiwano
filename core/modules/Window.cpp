#include "..\e2dmodule.h"
#include "..\e2dmanager.h"
#include "..\e2dobject.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME
#define REGISTER_CLASS	L"Easy2DApp"


e2d::Window * e2d::Window::instance_ = nullptr;

e2d::Window * e2d::Window::GetInstance()
{
	if (!instance_)
		instance_ = new (std::nothrow) Window;
	return instance_;
}

void e2d::Window::DestroyInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

e2d::Size e2d::Window::GetScreenSize()
{
	int screen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int screen_height = ::GetSystemMetrics(SM_CYSCREEN);
	Size screen_size(
		static_cast<float>(screen_width),
		static_cast<float>(screen_height)
	);
	return std::move(screen_size);
}

e2d::Window::Window()
	: hWnd_(nullptr)
	, width_(640)
	, height_(480)
	, title_(L"Easy2D Game")
	, icon_id_(0)
	, dpi_(0.f)
{
	::CoInitialize(nullptr);

	// 获取系统 DPI
	dpi_ = static_cast<float>(::GetDpiForSystem());
}

e2d::Window::~Window()
{
	if (::GetConsoleWindow())
		::FreeConsole();

	if (hWnd_)
		::DestroyWindow(hWnd_);

	::CoUninitialize();
}

bool e2d::Window::CheckUnique()
{
	HANDLE mutex = ::CreateMutex(nullptr, TRUE, LPCWSTR(L"Easy2DApp-" + title_));

	if (mutex == nullptr)
	{
		WARN("CreateMutex Failed!");
		return false;
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(mutex);
		return false;
	}
	return true;
}

e2d::Rect e2d::Window::Locate(int width, int height)
{
	Rect result;
	RECT wRECT		= { 0, 0, LONG(ceil(width * dpi_ / 96.f)), LONG(ceil(height * dpi_ / 96.f)) };
	int max_width	= ::GetSystemMetrics(SM_CXSCREEN);
	int max_height	= ::GetSystemMetrics(SM_CYSCREEN);

	// 计算合适的窗口大小
	::AdjustWindowRectEx(&wRECT, WINDOW_STYLE, FALSE, NULL);
	width = static_cast<int>(wRECT.right - wRECT.left);
	height = static_cast<int>(wRECT.bottom - wRECT.top);

	// 当输入的窗口大小比分辨率大时，给出警告
	WARN_IF(max_width < width || max_height < height, "The window Is larger than screen!");
	width = std::min(width, max_width);
	height = std::min(height, max_height);

	float x = float((max_width - width) / 2), y = float((max_height - height) / 2);
	return std::move(Rect(x, y, float(width), float(height)));
}

void e2d::Window::Poll()
{
	while (::PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg_);
		::DispatchMessage(&msg_);
	}
}

int e2d::Window::GetWidth() const
{
	return width_;
}

int e2d::Window::GetHeight() const
{
	return height_;
}

e2d::Size e2d::Window::GetSize() const
{
	return e2d::Size(float(width_), float(height_));
}

float e2d::Window::GetDpi() const
{
	return dpi_;
}

const e2d::String& e2d::Window::GetTitle() const
{
	return title_;
}

HWND e2d::Window::GetHWnd()
{
	if (!hWnd_)
	{
		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpszClassName = REGISTER_CLASS;
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = Window::WndProc;
		wcex.hIcon = nullptr;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);

		if (icon_id_ != 0)
		{
			wcex.hIcon = (HICON)::LoadImage(
				HINST_THISCOMPONENT,
				MAKEINTRESOURCE(icon_id_),
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
			);
		}

		// 注册窗口类
		RegisterClassEx(&wcex);

		// 计算窗口大小
		Rect clientRect = Locate(width_, height_);

		// 创建窗口
		hWnd_ = ::CreateWindowEx(
			NULL,
			REGISTER_CLASS,
			(LPCTSTR)title_,
			WINDOW_STYLE,
			int(clientRect.origin.x),
			int(clientRect.origin.y),
			int(clientRect.size.width),
			int(clientRect.size.height),
			nullptr,
			nullptr,
			HINST_THISCOMPONENT,
			this
		);

		if (hWnd_)
		{
			// 禁用输入法
			SetTypewritingEnabled(false);
			// 禁用控制台关闭按钮
			HWND consoleHWnd = ::GetConsoleWindow();
			if (consoleHWnd)
			{
				HMENU hmenu = ::GetSystemMenu(consoleHWnd, FALSE);
				::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			}
		}
		else
		{
			::UnregisterClass(REGISTER_CLASS, HINST_THISCOMPONENT);
			throw RuntimeException("Create window failed");
		}
	}
	return hWnd_;
}

void e2d::Window::SetSize(int width, int height)
{
	if (width_ == width && height_ == height)
		return;

	width_ = width;
	height_ = height;

	if (hWnd_)
	{
		Rect wRect = Locate(width, height);
		::MoveWindow(
			hWnd_,
			int(wRect.origin.x),
			int(wRect.origin.y),
			int(wRect.size.width),
			int(wRect.size.height),
			TRUE
		);
	}
}

void e2d::Window::SetTitle(const String& title)
{
	title_ = title;
	if (hWnd_)
	{
		::SetWindowText(hWnd_, (LPCWSTR)title);
	}
}

void e2d::Window::SetIcon(int resource_id)
{
	this->icon_id_ = resource_id;
	if (hWnd_)
	{
		HICON icon = (HICON)::LoadImage(
			HINST_THISCOMPONENT,
			MAKEINTRESOURCE(resource_id),
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);
		// 设置窗口的图标
		::SendMessage(hWnd_, WM_SETICON, ICON_BIG, (LPARAM)icon);
		::SendMessage(hWnd_, WM_SETICON, ICON_SMALL, (LPARAM)icon);
	}
}

void e2d::Window::SetCursor(Cursor cursor)
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

void e2d::Window::SetConsoleEnabled(bool enabled)
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
				freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				freopen_s(&stdinStream, "conin$", "r+t", stdin);
				freopen_s(&stderrStream, "conout$", "w+t", stderr);
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

void e2d::Window::SetTypewritingEnabled(bool enabled)
{
	static HIMC hImc = nullptr;

	if (enabled)
	{
		if (hImc != nullptr)
		{
			::ImmAssociateContext(GetHWnd(), hImc);
			hImc = nullptr;
		}
	}
	else
	{
		if (hImc == nullptr)
		{
			hImc = ::ImmAssociateContext(GetHWnd(), nullptr);
		}
	}
}

bool e2d::Window::Popup(const String & text, const String & title, PopupStyle style, bool has_cancel)
{
	UINT type = 0;
	switch (style)
	{
	case PopupStyle::Info:
		type = MB_ICONINFORMATION;
		break;
	case PopupStyle::Warning:
		type = MB_ICONWARNING;
		break;
	case PopupStyle::Error:
		type = MB_ICONERROR;
		break;
	default:
		break;
	}

	if (has_cancel)
	{
		type |= MB_OKCANCEL;
	}

	Game::GetInstance()->Pause();
	int ret = ::MessageBox(hWnd_, (LPCWSTR)text, (LPCWSTR)title, type);
	Game::GetInstance()->Resume();
	return ret == IDOK;
}


LRESULT e2d::Window::WndProc(HWND hWnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)l_param;
		Window *window = (Window *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(window)
		);

		result = 1;
	}
	else
	{
		bool hasHandled = false;
		Window *window = reinterpret_cast<Window *>(
			static_cast<LONG_PTR>(
				::GetWindowLongPtrW(hWnd, GWLP_USERDATA)
			)
		);

		switch (msg)
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
			auto game = Game::GetInstance();
			if (game->IsTransitioning())
				break;

			if (game->GetCurrentScene())
			{
				game->GetCurrentScene()->Dispatch(MouseEvent(msg, w_param, l_param), false);
			}
		}
		result = 0;
		hasHandled = true;
		break;

		// 处理按键消息
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			auto game = Game::GetInstance();
			if (game->IsTransitioning())
				break;

			if (game->GetCurrentScene())
			{
				game->GetCurrentScene()->Dispatch(KeyEvent(msg, w_param, l_param), false);
			}
		}
		result = 0;
		hasHandled = true;
		break;

		// 处理窗口大小变化消息
		case WM_SIZE:
		{
			UINT width = LOWORD(l_param);
			UINT height = HIWORD(l_param);

			if (w_param == SIZE_RESTORED)
			{
				window->width_ = static_cast<int>(width * 96.f / window->dpi_);
				window->height_ = static_cast<int>(height * 96.f / window->dpi_);
			}

			// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
			// 目标适当。它可能会调用失败，但是这里可以忽略有可能的
			// 错误，因为这个错误将在下一次调用 EndDraw 时产生
			auto render_target = Renderer::GetInstance()->GetRenderTarget();
			if (render_target)
			{
				render_target->Resize(D2D1::SizeU(width, height));
			}
		}
		break;

		// 处理窗口标题变化消息
		case WM_SETTEXT:
		{
			window->title_ = (const wchar_t*)l_param;
		}
		break;

		// 处理分辨率变化消息
		case WM_DISPLAYCHANGE:
		{
			// 重绘客户区
			::InvalidateRect(hWnd, nullptr, FALSE);
		}
		result = 0;
		hasHandled = true;
		break;

		// 重绘窗口
		case WM_PAINT:
		{
			Game::GetInstance()->DrawScene();
			::ValidateRect(hWnd, nullptr);
		}
		result = 0;
		hasHandled = true;
		break;

		// 窗口关闭消息
		case WM_CLOSE:
		{
			auto game = Game::GetInstance();
			auto currScene = game->GetCurrentScene();
			if (!currScene || currScene->OnCloseWindow())
			{
				game->Quit();
			}
		}
		result = 0;
		hasHandled = true;
		break;

		// 窗口销毁消息
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
		}
		result = 1;
		hasHandled = true;
		break;

		}

		if (!hasHandled)
		{
			result = ::DefWindowProc(hWnd, msg, w_param, l_param);
		}
	}
	return result;
}