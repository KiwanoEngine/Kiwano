// Copyright (c) 2016-2020 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano/platform/Window.h>

#if defined(KGE_WIN32)

#include <Windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <imm.h>       // ImmAssociateContext
#include <kiwano/core/Logger.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/platform/Application.h>
#pragma comment(lib, "imm32.lib")

#define WINDOW_FIXED_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_RESIZABLE_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX
#define WINDOW_FULLSCREEN_STYLE WS_CLIPCHILDREN | WS_POPUP
#define KGE_WND_CLASS_NAME L"KiwanoAppWnd"

namespace kiwano
{
namespace win32
{
namespace
{
MONITORINFOEX GetMoniterInfoEx(HWND hwnd)
{
    HMONITOR      monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEX monitor_info;

    memset(&monitor_info, 0, sizeof(MONITORINFOEX));
    monitor_info.cbSize = sizeof(MONITORINFOEX);
    ::GetMonitorInfoW(monitor, &monitor_info);

    return monitor_info;
}

void AdjustWindow(uint32_t width, uint32_t height, DWORD style, uint32_t* win_width, uint32_t* win_height)
{
    RECT rc;
    ::SetRect(&rc, 0, 0, (int)width, (int)height);
    ::AdjustWindowRect(&rc, style, false);

    *win_width  = rc.right - rc.left;
    *win_height = rc.bottom - rc.top;

    MONITORINFOEX info = GetMoniterInfoEx(NULL);

    uint32_t screenw = info.rcWork.right - info.rcWork.left;
    uint32_t screenh = info.rcWork.bottom - info.rcWork.top;

    if (*win_width > screenw)
        *win_width = screenw;
    if (*win_height > screenh)
        *win_height = screenh;
}

void ChangeFullScreenResolution(uint32_t width, uint32_t height, WCHAR* device_name)
{
    DEVMODE mode;

    memset(&mode, 0, sizeof(mode));
    mode.dmSize       = sizeof(DEVMODE);
    mode.dmBitsPerPel = ::GetDeviceCaps(::GetDC(0), BITSPIXEL);
    ;
    mode.dmPelsWidth  = width;
    mode.dmPelsHeight = height;
    mode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if (::ChangeDisplaySettingsExW(device_name, &mode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
        KGE_ERROR(L"ChangeDisplaySettings failed");
}

void RestoreResolution(WCHAR* device_name)
{
    ::ChangeDisplaySettingsExW(device_name, NULL, NULL, 0, NULL);
}
}  // namespace

class KGE_API WindowImpl : public kiwano::Window
{
public:
    WindowImpl();

    ~WindowImpl();

    void Create(String const& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable,
                bool fullscreen) override;

    WindowHandle GetHandle() const override;

    void SetTitle(String const& title) override;

    void SetIcon(uint32_t icon_resource) override;

    void Resize(uint32_t width, uint32_t height) override;

    void SetFullscreen(bool fullscreen) override;

    void SetCursor(CursorType cursor) override;

    void Destroy() override;

private:
    void PumpEvents() override;

    DWORD GetStyle() const;

    void UpdateCursor();

    void SetActive(bool actived);

    static LRESULT CALLBACK WndProc(HWND, UINT32, WPARAM, LPARAM);

private:
    bool         resizable_;
    bool         is_fullscreen_;
    wchar_t*     device_name_;
    WindowHandle handle_;
    CursorType   mouse_cursor_;

    std::array<KeyCode, 256> key_map_;
};

WindowImpl::WindowImpl()
    : handle_(nullptr)
    , device_name_(nullptr)
    , is_fullscreen_(false)
    , resizable_(false)
    , mouse_cursor_(CursorType::Arrow)
    , key_map_{}
{
    // Keys
    key_map_[VK_UP]      = KeyCode::Up;
    key_map_[VK_LEFT]    = KeyCode::Left;
    key_map_[VK_RIGHT]   = KeyCode::Right;
    key_map_[VK_DOWN]    = KeyCode::Down;
    key_map_[VK_RETURN]  = KeyCode::Enter;
    key_map_[VK_SPACE]   = KeyCode::Space;
    key_map_[VK_ESCAPE]  = KeyCode::Esc;
    key_map_[VK_CONTROL] = KeyCode::Ctrl;
    key_map_[VK_SHIFT]   = KeyCode::Shift;
    key_map_[VK_MENU]    = KeyCode::Alt;
    key_map_[VK_TAB]     = KeyCode::Tab;
    key_map_[VK_DELETE]  = KeyCode::Delete;
    key_map_[VK_BACK]    = KeyCode::Back;

    // VK_L* and VK_R*
    key_map_[VK_LCONTROL] = KeyCode::Ctrl;
    key_map_[VK_RCONTROL] = KeyCode::Ctrl;
    key_map_[VK_LSHIFT]   = KeyCode::Shift;
    key_map_[VK_RSHIFT]   = KeyCode::Shift;
    key_map_[VK_LMENU]    = KeyCode::Alt;
    key_map_[VK_RMENU]    = KeyCode::Alt;
    key_map_[VK_LWIN]     = KeyCode::Super;
    key_map_[VK_RWIN]     = KeyCode::Super;

    // A - Z
    for (size_t i = 0, size = ('Z' - 'A'); i <= size; ++i)
        key_map_['A' + i] = KeyCode(size_t(KeyCode::A) + i);

    // Num 0 - 9
    for (size_t i = 0; i < 9; ++i)
        key_map_['0' + i] = KeyCode(size_t(KeyCode::Num0) + i);

    // Numpad 0 - 9
    for (size_t i = 0; i < 9; ++i)
        key_map_[VK_NUMPAD0 + i] = KeyCode(size_t(KeyCode::Numpad0) + i);

    // F1 - F12
    for (size_t i = 0; i < 12; ++i)
        key_map_[VK_F1 + i] = KeyCode(size_t(KeyCode::F1) + i);
}

WindowImpl::~WindowImpl() {}

void WindowImpl::Create(String const& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable,
                        bool fullscreen)
{
    HINSTANCE  hinst   = GetModuleHandleW(nullptr);
    WNDCLASSEX wcex    = { 0 };
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.lpszClassName = KGE_WND_CLASS_NAME;
    wcex.style         = CS_HREDRAW | CS_VREDRAW /* | CS_DBLCLKS */;
    wcex.lpfnWndProc   = WindowImpl::WndProc;
    wcex.hIcon         = nullptr;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hinst;
    wcex.hbrBackground = (HBRUSH)(::GetStockObject(BLACK_BRUSH));
    wcex.lpszMenuName  = nullptr;
    wcex.hCursor       = ::LoadCursorW(hinst, IDC_ARROW);

    if (icon)
    {
        wcex.hIcon = (HICON)::LoadImageW(hinst, MAKEINTRESOURCE(icon), IMAGE_ICON, 0, 0,
                                         LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
    }

    ::RegisterClassExW(&wcex);

    // Get the nearest monitor to this window
    HMONITOR monitor = ::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

    // Get the target monitor info
    MONITORINFOEX monitor_info_ex;
    memset(&monitor_info_ex, 0, sizeof(MONITORINFOEX));
    monitor_info_ex.cbSize = sizeof(MONITORINFOEX);
    ::GetMonitorInfoW(monitor, &monitor_info_ex);

    // Save the device name
    int len = lstrlenW(monitor_info_ex.szDevice);

    device_name_ = new wchar_t[len + 1];
    lstrcpyW(device_name_, monitor_info_ex.szDevice);

    int left = -1, top = -1;

    resizable_     = resizable;
    is_fullscreen_ = fullscreen;

    if (is_fullscreen_)
    {
        top  = monitor_info_ex.rcMonitor.top;
        left = monitor_info_ex.rcMonitor.left;

        if (width > static_cast<uint32_t>(monitor_info_ex.rcWork.right - left))
            width = static_cast<uint32_t>(monitor_info_ex.rcWork.right - left);

        if (height > static_cast<uint32_t>(monitor_info_ex.rcWork.bottom - top))
            height = static_cast<uint32_t>(monitor_info_ex.rcWork.bottom - top);
    }
    else
    {
        uint32_t screenw = monitor_info_ex.rcWork.right - monitor_info_ex.rcWork.left;
        uint32_t screenh = monitor_info_ex.rcWork.bottom - monitor_info_ex.rcWork.top;

        uint32_t win_width, win_height;
        AdjustWindow(width, height, GetStyle(), &win_width, &win_height);

        left   = monitor_info_ex.rcWork.left + (screenw - win_width) / 2;
        top    = monitor_info_ex.rcWork.top + (screenh - win_height) / 2;
        width  = win_width;
        height = win_height;
    }

    handle_ = ::CreateWindowExW(is_fullscreen_ ? WS_EX_TOPMOST : 0, KGE_WND_CLASS_NAME, title.c_str(), GetStyle(), left,
                                top, width, height, nullptr, nullptr, hinst, nullptr);

    if (handle_ == nullptr)
    {
        ::UnregisterClass(KGE_WND_CLASS_NAME, hinst);

        KGE_ERROR(L"Failed with HRESULT of %08X", HRESULT_FROM_WIN32(GetLastError()));
        throw std::runtime_error("Create window failed");
    }

    width_ = width;
    height_ = height;

    // disable imm
    ::ImmAssociateContext(handle_, nullptr);

    // use Application instance in message loop
    ::SetWindowLongPtr(handle_, GWLP_USERDATA, LONG_PTR(this));

    ::ShowWindow(handle_, SW_SHOWNORMAL);
    ::UpdateWindow(handle_);

    if (is_fullscreen_)
    {
        ChangeFullScreenResolution(width, height, device_name_);
    }
}

WindowHandle WindowImpl::GetHandle() const
{
    return handle_;
}

void WindowImpl::PumpEvents()
{
    MSG msg;
    while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }
}

void WindowImpl::SetTitle(String const& title)
{
    if (handle_)
        ::SetWindowTextW(handle_, title.c_str());
}

void WindowImpl::SetIcon(uint32_t icon_resource)
{
    if (handle_)
    {
        HINSTANCE hinstance = GetModuleHandle(nullptr);
        HICON     icon      = (HICON)::LoadImageW(hinstance, MAKEINTRESOURCE(icon_resource), IMAGE_ICON, 0, 0,
                                         LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

        ::SendMessage(handle_, WM_SETICON, ICON_BIG, (LPARAM)icon);
        ::SendMessage(handle_, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    }
}

void WindowImpl::Resize(uint32_t width, uint32_t height)
{
    if (handle_ && !is_fullscreen_)
    {
        RECT rc = { 0, 0, LONG(width), LONG(height) };
        ::AdjustWindowRect(&rc, GetStyle(), false);

        width  = rc.right - rc.left;
        height = rc.bottom - rc.top;
        ::SetWindowPos(handle_, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void WindowImpl::SetFullscreen(bool fullscreen)
{
    if (is_fullscreen_ != fullscreen)
    {
        is_fullscreen_ = fullscreen;

        uint32_t width  = GetWidth();
        uint32_t height = GetHeight();

        if (is_fullscreen_)
        {
            // move window to (0, 0) before display switch
            ::SetWindowPos(handle_, HWND_TOPMOST, 0, 0, width, height, SWP_NOACTIVATE);

            ChangeFullScreenResolution(width, height, device_name_);

            MONITORINFOEX info = GetMoniterInfoEx(handle_);

            ::SetWindowLongPtr(handle_, GWL_STYLE, GetStyle());
            ::SetWindowPos(handle_, HWND_TOPMOST, info.rcMonitor.top, info.rcMonitor.left, width, height,
                           SWP_NOACTIVATE);
        }
        else
        {
            RestoreResolution(device_name_);

            MONITORINFOEX info = GetMoniterInfoEx(handle_);

            uint32_t screenw = uint32_t(info.rcWork.right - info.rcWork.left);
            uint32_t screenh = uint32_t(info.rcWork.bottom - info.rcWork.top);

            int left = screenw > width ? ((screenw - width) / 2) : 0;
            int top  = screenh > height ? ((screenh - height) / 2) : 0;

            ::SetWindowLongPtr(handle_, GWL_STYLE, GetStyle());
            ::SetWindowPos(handle_, HWND_NOTOPMOST, left, top, width, height, SWP_DRAWFRAME | SWP_FRAMECHANGED);
        }

        ::ShowWindow(handle_, SW_SHOWNORMAL);
    }
}

void WindowImpl::SetCursor(CursorType cursor)
{
    mouse_cursor_ = cursor;
}

void WindowImpl::Destroy()
{
    if (is_fullscreen_)
        RestoreResolution(device_name_);

    if (device_name_)
    {
        delete[] device_name_;
        device_name_ = nullptr;
    }

    if (handle_)
    {
        ::DestroyWindow(handle_);
        handle_ = nullptr;
    }

    Window::Destroy();
}

DWORD WindowImpl::GetStyle() const
{
    return is_fullscreen_ ? (WINDOW_FULLSCREEN_STYLE) : (resizable_ ? (WINDOW_RESIZABLE_STYLE) : (WINDOW_FIXED_STYLE));
}

void WindowImpl::UpdateCursor()
{
    LPTSTR win32_cursor = IDC_ARROW;
    switch (mouse_cursor_)
    {
    case CursorType::Arrow:
        win32_cursor = IDC_ARROW;
        break;
    case CursorType::TextInput:
        win32_cursor = IDC_IBEAM;
        break;
    case CursorType::SizeAll:
        win32_cursor = IDC_SIZEALL;
        break;
    case CursorType::SizeWE:
        win32_cursor = IDC_SIZEWE;
        break;
    case CursorType::SizeNS:
        win32_cursor = IDC_SIZENS;
        break;
    case CursorType::SizeNESW:
        win32_cursor = IDC_SIZENESW;
        break;
    case CursorType::SizeNWSE:
        win32_cursor = IDC_SIZENWSE;
        break;
    case CursorType::Hand:
        win32_cursor = IDC_HAND;
        break;
    }
    ::SetCursor(::LoadCursorW(nullptr, win32_cursor));
}

void WindowImpl::SetActive(bool actived)
{
    if (!handle_)
        return;

    if (is_fullscreen_)
    {
        if (actived)
        {
            ChangeFullScreenResolution(GetWidth(), GetHeight(), device_name_);

            MONITORINFOEX info = GetMoniterInfoEx(handle_);
            ::SetWindowPos(handle_, HWND_TOPMOST, info.rcMonitor.top, info.rcMonitor.left, GetWidth(), GetHeight(),
                           SWP_NOACTIVATE);
        }
        else
        {
            RestoreResolution(device_name_);

            ::SetWindowPos(handle_, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            ::ShowWindow(handle_, SW_MINIMIZE);
        }
    }
}

LRESULT CALLBACK WindowImpl::WndProc(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
{
    WindowImpl* window = reinterpret_cast<WindowImpl*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
    if (window == nullptr)
    {
        return ::DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        KeyCode key = window->key_map_[size_t(wparam)];
        if (key != KeyCode::Unknown)
        {
            KeyDownEventPtr evt = new KeyDownEvent;
            evt->code           = key;
            window->PushEvent(evt);
        }
    }
    break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        KeyCode key = window->key_map_[size_t(wparam)];
        if (key != KeyCode::Unknown)
        {
            KeyUpEventPtr evt = new KeyUpEvent;
            evt->code         = key;
            window->PushEvent(evt);
        }
    }
    break;

    case WM_CHAR:
    {
        KeyCharEventPtr evt = new KeyCharEvent;
        evt->value          = char(wparam);
        window->PushEvent(evt);
    }
    break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    {
        MouseDownEventPtr evt = new MouseDownEvent;
        evt->pos              = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));

        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
        {
            evt->button = MouseButton::Left;
        }
        else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
        {
            evt->button = MouseButton::Right;
        }
        else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK)
        {
            evt->button = MouseButton::Middle;
        }

        window->PushEvent(evt);
    }
    break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        MouseUpEventPtr evt = new MouseUpEvent;
        evt->pos            = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));

        if (msg == WM_LBUTTONUP)
        {
            evt->button = MouseButton::Left;
        }
        else if (msg == WM_RBUTTONUP)
        {
            evt->button = MouseButton::Right;
        }
        else if (msg == WM_MBUTTONUP)
        {
            evt->button = MouseButton::Middle;
        }

        window->PushEvent(evt);
    }
    break;

    case WM_MOUSEMOVE:
    {
        MouseMoveEventPtr evt = new MouseMoveEvent;
        evt->pos              = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
        window->PushEvent(evt);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        MouseWheelEventPtr evt = new MouseWheelEvent;
        evt->pos               = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
        evt->wheel             = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
        window->PushEvent(evt);
    }
    break;

    case WM_SIZE:
    {
        if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
        {
            KGE_SYS_LOG(L"Window minimized");
        }
        else
        {
            // KGE_SYS_LOG(L"Window resized");

            window->width_ = ((uint32_t)(short)LOWORD(lparam));
            window->height_ = ((uint32_t)(short)HIWORD(lparam));

            WindowResizedEventPtr evt = new WindowResizedEvent;
            evt->width                = window->GetWidth();
            evt->height               = window->GetHeight();
            window->PushEvent(evt);
        }
    }
    break;

    case WM_MOVE:
    {
        WindowMovedEventPtr evt = new WindowMovedEvent;
        evt->x                  = GET_X_LPARAM(lparam);
        evt->y                  = GET_Y_LPARAM(lparam);
        window->PushEvent(evt);
    }
    break;

    case WM_ACTIVATE:
    {
        bool active = (LOWORD(wparam) != WA_INACTIVE);

        window->SetActive(active);

        WindowFocusChangedEventPtr evt = new WindowFocusChangedEvent;
        evt->focus                     = active;
        window->PushEvent(evt);
    }
    break;

    case WM_SETTEXT:
    {
        KGE_SYS_LOG(L"Window title changed");

        window->title_ = String::cstr(reinterpret_cast<LPCWSTR>(lparam));

        WindowTitleChangedEventPtr evt = new WindowTitleChangedEvent;
        evt->title                     = window->title_;
        window->PushEvent(evt);
    }
    break;

    case WM_SETICON:
    {
        KGE_SYS_LOG(L"Window icon changed");
    }
    break;

    case WM_DISPLAYCHANGE:
    {
        KGE_SYS_LOG(L"The display resolution has changed");

        ::InvalidateRect(hwnd, nullptr, FALSE);
    }
    break;

    case WM_SETCURSOR:
    {
        window->UpdateCursor();
    }
    break;

    case WM_CLOSE:
    {
        KGE_SYS_LOG(L"Window is closing");

        WindowClosedEventPtr evt = new WindowClosedEvent;
        window->PushEvent(evt);
        window->Destroy();
    }
    break;

    case WM_DESTROY:
    {
        KGE_SYS_LOG(L"Window was destroyed");

        ::PostQuitMessage(0);
        return 0;
    }
    break;
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

}  // namespace win32
}  // namespace kiwano

namespace kiwano
{

Window& Window::Instance()
{
    static win32::WindowImpl instance;
    return instance;
}

}  // namespace kiwano

#endif
