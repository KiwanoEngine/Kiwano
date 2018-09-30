#include "..\e2dmodule.h"


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

void e2d::Window::SetCursor(Cursor cursor)
{
	LPCWSTR cursor_name = nullptr;
	switch (cursor)
	{
	case Cursor::Normal:
		cursor_name = IDC_ARROW;
		break;

	case Cursor::Hand:
		cursor_name = IDC_HAND;
		break;

	case Cursor::No:
		cursor_name = IDC_NO;
		break;

	case Cursor::Wait:
		cursor_name = IDC_WAIT;
		break;

	case Cursor::ArrowWait:
		cursor_name = IDC_APPSTARTING;
		break;

	default:
		break;
	}

	HCURSOR hcursor = ::LoadCursor(nullptr, cursor_name);
	if (hcursor)
	{
		::SetCursor(hcursor);
	}
}

void e2d::Window::ShowConsole(bool enabled)
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

	int ret = ::MessageBoxW(nullptr, (LPCWSTR)text, (LPCWSTR)title, type);
	return ret == IDOK;
}
