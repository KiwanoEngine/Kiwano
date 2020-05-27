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

#if defined(KGE_PLATFORM_WINDOWS)

#include <memory>
#include <array>
#include <kiwano/core/Keys.h>
#include <kiwano/core/Exception.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/event/Events.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/render/DirectX/D3DDeviceResources.h>
#include <Windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <imm.h>       // ImmAssociateContext
#pragma comment(lib, "imm32.lib")
#include <timeapi.h>  // timeBeginPeriod, timeEndPeriod
#pragma comment(lib, "winmm.lib")

namespace kiwano
{

KGE_DECLARE_SMART_PTR(WindowWin32Impl);

class KGE_API WindowWin32Impl : public Window
{
public:
    WindowWin32Impl();

    virtual ~WindowWin32Impl();

    void Init(const String& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable);

    void SetTitle(const String& title) override;

    void SetIcon(uint32_t icon_resource) override;

    void SetMinimumSize(uint32_t width, uint32_t height) override;

    void SetMaximumSize(uint32_t width, uint32_t height) override;

    void SetCursor(CursorType cursor) override;

    void SetResolution(uint32_t width, uint32_t height, bool fullscreen) override;

    Vector<Resolution> GetResolutions() override;

    void PumpEvents() override;

    DWORD GetStyle() const;

    void UpdateCursor();

    LRESULT MessageProc(HWND, UINT32, WPARAM, LPARAM);

    static LRESULT CALLBACK StaticWndProc(HWND, UINT32, WPARAM, LPARAM);

private:
    bool       resizable_;
    bool       is_resizing_;
    bool       is_minimized_;
    CursorType mouse_cursor_;
    String     device_name_;

    Vector<Resolution>       resolutions_;
    std::array<KeyCode, 256> key_map_;
};

WindowPtr Window::Create(const String& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable)
{
    WindowWin32ImplPtr ptr = memory::New<WindowWin32Impl>();
    if (ptr)
    {
        ptr->Init(title, width, height, icon, resizable);
    }
    return ptr;
}

}  // namespace kiwano

namespace kiwano
{

#define WINDOW_FIXED_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_RESIZABLE_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX

namespace
{
MONITORINFOEXA GetMoniterInfoEx(HWND hwnd)
{
    HMONITOR       monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEXA monitor_info;

    memset(&monitor_info, 0, sizeof(monitor_info));
    monitor_info.cbSize = sizeof(monitor_info);
    ::GetMonitorInfoA(monitor, &monitor_info);

    return monitor_info;
}

void AdjustWindow(uint32_t width, uint32_t height, DWORD style, uint32_t* win_width, uint32_t* win_height)
{
    RECT rc;
    ::SetRect(&rc, 0, 0, (int)width, (int)height);
    ::AdjustWindowRect(&rc, style, false);

    *win_width  = rc.right - rc.left;
    *win_height = rc.bottom - rc.top;

    MONITORINFOEXA info = GetMoniterInfoEx(NULL);

    uint32_t screenw = info.rcWork.right - info.rcWork.left;
    uint32_t screenh = info.rcWork.bottom - info.rcWork.top;

    if (*win_width > screenw)
        *win_width = screenw;
    if (*win_height > screenh)
        *win_height = screenh;
}

}  // namespace

WindowWin32Impl::WindowWin32Impl()
    : resizable_(false)
    , is_resizing_(false)
    , is_minimized_(false)
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
    for (size_t i = 0; i < 10; ++i)
        key_map_['0' + i] = KeyCode(size_t(KeyCode::Num0) + i);

    // Numpad 0 - 9
    for (size_t i = 0; i < 10; ++i)
        key_map_[VK_NUMPAD0 + i] = KeyCode(size_t(KeyCode::Numpad0) + i);

    // F1 - F12
    for (size_t i = 0; i < 12; ++i)
        key_map_[VK_F1 + i] = KeyCode(size_t(KeyCode::F1) + i);

    ::timeBeginPeriod(0);
}

WindowWin32Impl::~WindowWin32Impl()
{
    if (handle_)
    {
        ::DestroyWindow(handle_);
        handle_ = nullptr;
    }

    ::timeEndPeriod(0);
}

void WindowWin32Impl::Init(const String& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable)
{
    HINSTANCE  hinst   = GetModuleHandle(nullptr);
    WNDCLASSEXA wcex   = { 0 };
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.lpszClassName = "KiwanoAppWnd";
    wcex.style         = CS_HREDRAW | CS_VREDRAW /* | CS_DBLCLKS */;
    wcex.lpfnWndProc   = WindowWin32Impl::StaticWndProc;
    wcex.hIcon         = nullptr;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hinst;
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = nullptr;
    wcex.hCursor       = ::LoadCursor(hinst, IDC_ARROW);

    if (icon)
    {
        wcex.hIcon = (HICON)::LoadImage(hinst, MAKEINTRESOURCE(icon), IMAGE_ICON, 0, 0,
                                        LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
    }

    ::RegisterClassExA(&wcex);

    // Get the nearest monitor to this window
    HMONITOR monitor = ::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

    // Get the target monitor info
    MONITORINFOEXA monitor_info_ex;
    memset(&monitor_info_ex, 0, sizeof(MONITORINFOEXA));
    monitor_info_ex.cbSize = sizeof(MONITORINFOEXA);
    ::GetMonitorInfoA(monitor, &monitor_info_ex);

    // Save the device name
    device_name_ = monitor_info_ex.szDevice;

    uint32_t screenw = monitor_info_ex.rcWork.right - monitor_info_ex.rcWork.left;
    uint32_t screenh = monitor_info_ex.rcWork.bottom - monitor_info_ex.rcWork.top;

    uint32_t win_width, win_height;
    AdjustWindow(width, height, GetStyle(), &win_width, &win_height);

    int left = monitor_info_ex.rcWork.left + (screenw - win_width) / 2;
    int top  = monitor_info_ex.rcWork.top + (screenh - win_height) / 2;
    width  = win_width;
    height = win_height;

    width_     = width;
    height_    = height;
    resizable_ = resizable;
    handle_    = ::CreateWindowExA(0, "KiwanoAppWnd", title.c_str(), GetStyle(), left, top, width, height, nullptr,
                                nullptr, hinst, nullptr);

    if (handle_ == nullptr)
    {
        ::UnregisterClassA("KiwanoAppWnd", hinst);
        KGE_THROW_SYSTEM_ERROR(HRESULT_FROM_WIN32(GetLastError()), "Create window failed");
    }

    // disable imm
    ::ImmAssociateContext(handle_, nullptr);

    // use Application instance in message loop
    ::SetWindowLongPtrA(handle_, GWLP_USERDATA, LONG_PTR(this));

    ::ShowWindow(handle_, SW_SHOWNORMAL);
    ::UpdateWindow(handle_);

    // Initialize Direct3D resources
    auto d3d_res = graphics::directx::GetD3DDeviceResources();

    HRESULT hr = d3d_res->Initialize(handle_);

    // Initialize Direct2D resources
    if (SUCCEEDED(hr))
    {
        auto d2d_res = graphics::directx::GetD2DDeviceResources();

        hr = d2d_res->Initialize(d3d_res->GetDXGIDevice(), d3d_res->GetDXGISwapChain());
    }

    KGE_THROW_IF_FAILED(hr, "Create DirectX resources failed");
}

void WindowWin32Impl::PumpEvents()
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}

void WindowWin32Impl::SetTitle(const String& title)
{
    KGE_ASSERT(handle_);
    ::SetWindowTextA(handle_, title.c_str());
}

void WindowWin32Impl::SetIcon(uint32_t icon_resource)
{
    KGE_ASSERT(handle_);

    HINSTANCE hinstance = ::GetModuleHandle(nullptr);
    HICON     icon      = (HICON)::LoadImage(hinstance, MAKEINTRESOURCE(icon_resource), IMAGE_ICON, 0, 0,
                                    LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

    ::SendMessage(handle_, WM_SETICON, ICON_BIG, (LPARAM)icon);
    ::SendMessage(handle_, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

void WindowWin32Impl::SetMinimumSize(uint32_t width, uint32_t height)
{
    min_width_  = width;
    min_height_ = height;
}

void WindowWin32Impl::SetMaximumSize(uint32_t width, uint32_t height)
{
    max_width_  = width;
    max_height_ = height;
}

void WindowWin32Impl::SetCursor(CursorType cursor)
{
    mouse_cursor_ = cursor;
}

void WindowWin32Impl::SetResolution(uint32_t width, uint32_t height, bool fullscreen)
{
    auto d3d = kiwano::graphics::directx::GetD3DDeviceResources();

    if (fullscreen)
    {
        HRESULT hr = d3d->ResizeTarget(width, height);
        KGE_THROW_IF_FAILED(hr, "DXGI ResizeTarget failed!");

        hr = d3d->SetFullscreenState(fullscreen);
        KGE_THROW_IF_FAILED(hr, "DXGI SetFullscreenState failed!");
    }
    else
    {
        HRESULT hr = d3d->SetFullscreenState(fullscreen);
        KGE_THROW_IF_FAILED(hr, "DXGI SetFullscreenState failed!");

        hr = d3d->ResizeTarget(width, height);
        KGE_THROW_IF_FAILED(hr, "DXGI ResizeTarget failed!");
    }

    is_fullscreen_ = fullscreen;
}

Vector<Resolution> WindowWin32Impl::GetResolutions()
{
    if (resolutions_.empty())
    {
        auto d3d = kiwano::graphics::directx::GetD3DDeviceResources();

        DXGI_MODE_DESC* mode_descs = nullptr;
        int             mode_num   = 0;

        HRESULT hr = d3d->GetDisplaySettings(&mode_descs, &mode_num);
        if (SUCCEEDED(hr))
        {
            std::unique_ptr<DXGI_MODE_DESC[]> mode_list(mode_descs);

            if (mode_list)
            {
                for (int i = 0; i < mode_num; i++)
                {
                    Resolution res;
                    res.width        = mode_descs[i].Width;
                    res.height       = mode_descs[i].Height;
                    res.refresh_rate = 0;

                    if (mode_descs[i].RefreshRate.Denominator > 0)
                    {
                        res.refresh_rate = mode_descs[i].RefreshRate.Numerator / mode_descs[i].RefreshRate.Denominator;
                    }

                    if (!resolutions_.empty())
                    {
                        auto& back = resolutions_.back();
                        if (back.width == res.width && back.height == res.height
                            && back.refresh_rate == res.refresh_rate)
                            continue;
                    }

                    resolutions_.push_back(res);
                }
            }
        }
        else
        {
            KGE_THROW_IF_FAILED(hr, "DXGI GetDisplaySettings failed!");
        }
    }
    return resolutions_;
}

DWORD WindowWin32Impl::GetStyle() const
{
    return (resizable_ ? (WINDOW_RESIZABLE_STYLE) : (WINDOW_FIXED_STYLE));
}

void WindowWin32Impl::UpdateCursor()
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
    ::SetCursor(::LoadCursor(nullptr, win32_cursor));
}

LRESULT WindowWin32Impl::MessageProc(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        KeyCode key = this->key_map_[size_t(wparam)];
        if (key != KeyCode::Unknown)
        {
            KeyDownEventPtr evt = new KeyDownEvent;
            evt->code           = key;
            this->PushEvent(evt);
        }
    }
    break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        KeyCode key = this->key_map_[size_t(wparam)];
        if (key != KeyCode::Unknown)
        {
            KeyUpEventPtr evt = new KeyUpEvent;
            evt->code         = key;
            this->PushEvent(evt);
        }
    }
    break;

    case WM_CHAR:
    {
        KeyCharEventPtr evt = new KeyCharEvent;
        evt->value          = char(wparam);
        this->PushEvent(evt);
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

        this->PushEvent(evt);
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

        this->PushEvent(evt);
    }
    break;

    case WM_MOUSEMOVE:
    {
        MouseMoveEventPtr evt = new MouseMoveEvent;
        evt->pos              = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
        this->PushEvent(evt);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        MouseWheelEventPtr evt = new MouseWheelEvent;
        evt->pos               = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
        evt->wheel             = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
        this->PushEvent(evt);
    }
    break;

    case WM_SIZE:
    {
        if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
        {
            KGE_SYS_LOG("Window minimized");

            is_minimized_ = true;
            // Pause game when window is minimized
            Application::GetInstance().Pause();
        }
        else if (SIZE_MAXIMIZED == wparam)
        {
            KGE_SYS_LOG("Window maximized");

            if (is_minimized_)
            {
                is_minimized_ = false;
                Application::GetInstance().Resume();
            }
        }
        else if (wparam == SIZE_RESTORED)
        {
            if (is_minimized_)
            {
                KGE_SYS_LOG("Window restored");

                // the window was restored and was previously minimized
                is_minimized_ = false;
                Application::GetInstance().Resume();
            }
            else if (is_resizing_)
            {
                // DO NOTHING until the dragging / resizing has stopped.
            }
            else
            {
                this->width_  = ((uint32_t)(short)LOWORD(lparam));
                this->height_ = ((uint32_t)(short)HIWORD(lparam));

                WindowResizedEventPtr evt = new WindowResizedEvent;
                evt->width                = this->GetWidth();
                evt->height               = this->GetHeight();
                this->PushEvent(evt);

                KGE_SYS_LOG("Window resized to (%d, %d)", this->width_, this->height_);
            }
        }
    }
    break;

    case WM_ENTERSIZEMOVE:
    {
        is_resizing_ = true;
        Application::GetInstance().Pause();
        return 0;
    }

    case WM_EXITSIZEMOVE:
    {
        is_resizing_ = false;
        Application::GetInstance().Resume();

        // Send window resized event when client size changed
        RECT client_rect = { 0 };
        ::GetClientRect(hwnd, &client_rect);

        uint32_t client_width  = uint32_t(client_rect.right - client_rect.left);
        uint32_t client_height = uint32_t(client_rect.bottom - client_rect.top);
        if (client_width != this->GetWidth() || client_height != this->GetHeight())
        {
            KGE_SYS_LOG("Window resized to (%d, %d)", client_width, client_height);

            this->width_  = client_width;
            this->height_ = client_height;

            WindowResizedEventPtr evt = new WindowResizedEvent;
            evt->width                = this->GetWidth();
            evt->height               = this->GetHeight();
            this->PushEvent(evt);
        }
        return 0;
    }

    case WM_GETMINMAXINFO:
    {
        if (min_width_ || min_height_)
        {
            ((MINMAXINFO*)lparam)->ptMinTrackSize = POINT{ LONG(min_width_), LONG(min_height_) };
        }
        if (max_width_ || max_height_)
        {
            ((MINMAXINFO*)lparam)->ptMaxTrackSize = POINT{ LONG(max_width_), LONG(max_height_) };
        }
        return 0;
    }

    case WM_MOVE:
    {
        WindowMovedEventPtr evt = new WindowMovedEvent;
        evt->x                  = GET_X_LPARAM(lparam);
        evt->y                  = GET_Y_LPARAM(lparam);
        this->PushEvent(evt);
    }
    break;

    case WM_MENUCHAR:
    {
        // Disables the crazy beeping sound when pressing a mnemonic key.
        // Simply tell Windows that we want the menu closed.
        return MAKELRESULT(0, MNC_CLOSE);
    }

    case WM_ACTIVATE:
    {
        bool active = (LOWORD(wparam) != WA_INACTIVE);

        WindowFocusChangedEventPtr evt = new WindowFocusChangedEvent;
        evt->focus                     = active;
        this->PushEvent(evt);
    }
    break;

    case WM_SETFOCUS:
    {
        if (is_fullscreen_)
        {
            // TODO restore to fullscreen mode
            // SetResolution();
        }
    }
    break;

    case WM_KILLFOCUS:
    {
        if (is_fullscreen_)
        {
            // TODO exit fullscreen mode
            // ::ShowWindow(handle_, SW_MINIMIZE);
        }
    }
    break;

    case WM_SETTEXT:
    {
        KGE_SYS_LOG("Window title changed");

        this->title_ = strings::WideToNarrow(reinterpret_cast<LPCWSTR>(lparam));

        WindowTitleChangedEventPtr evt = new WindowTitleChangedEvent;
        evt->title                     = this->title_;
        this->PushEvent(evt);
    }
    break;

    case WM_SETICON:
    {
        KGE_SYS_LOG("Window icon changed");
    }
    break;

    case WM_DISPLAYCHANGE:
    {
        KGE_SYS_LOG("The display resolution has changed");

        // Check fullscreen state
        auto d3d_res = graphics::directx::GetD3DDeviceResources();
        auto swap_chain = d3d_res->GetDXGISwapChain();
        if (swap_chain)
        {
            BOOL is_fullscreen = FALSE;
            if (SUCCEEDED(swap_chain->GetFullscreenState(&is_fullscreen, nullptr)))
            {
                is_fullscreen_ = !!is_fullscreen;
            }
        }
    }
    break;

    case WM_SETCURSOR:
    {
        this->UpdateCursor();
    }
    break;

    case WM_CLOSE:
    {
        KGE_SYS_LOG("Window is closing");

        WindowClosedEventPtr evt = new WindowClosedEvent;
        this->PushEvent(evt);
        this->SetShouldClose(true);
        return 0;
    }
    break;

    case WM_DESTROY:
    {
        KGE_SYS_LOG("Window was destroyed");

        ::PostQuitMessage(0);
        return 0;
    }
    break;
    }

    return ::DefWindowProcA(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK WindowWin32Impl::StaticWndProc(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
{
    LONG_PTR ptr = static_cast<LONG_PTR>(::GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    if (ptr)
    {
        WindowWin32Impl* window = reinterpret_cast<WindowWin32Impl*>(ptr);
        return window->MessageProc(hwnd, msg, wparam, lparam);
    }
    return ::DefWindowProcA(hwnd, msg, wparam, lparam);
}

}  // namespace kiwano

#endif
