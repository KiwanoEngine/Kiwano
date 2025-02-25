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
#include <kiwano/platform/Keys.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/event/Events.h>
#include <kiwano/platform/Application.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/render/Renderer.h>
#include <Windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <imm.h>       // ImmAssociateContext
#pragma comment(lib, "imm32.lib")
#include <timeapi.h>  // timeBeginPeriod, timeEndPeriod
#pragma comment(lib, "winmm.lib")

namespace kiwano
{

void  EnableDpiAwareness();
float GetDpiForHwnd(HWND hwnd);

class KGE_API WindowWin32Impl : public Window
{
public:
    WindowWin32Impl();

    virtual ~WindowWin32Impl();

    void Init(const WindowConfig& config);

    void SetTitle(StringView title) override;

    void SetIcon(Icon icon) override;

    void SetMinimumSize(uint32_t width, uint32_t height) override;

    void SetMaximumSize(uint32_t width, uint32_t height) override;

    void SetCursor(CursorType cursor) override;

    void SetResolution(uint32_t width, uint32_t height, bool fullscreen) override;

    Vector<Resolution> GetResolutions() override;

    void PumpEvents() override;

    void SetImmEnabled(bool enable) override;

    DWORD GetStyle() const;

    void SetActive(bool active);

    void UpdateCursor();

    LRESULT MessageProc(HWND, UINT32, WPARAM, LPARAM);

    static LRESULT CALLBACK StaticWndProc(HWND, UINT32, WPARAM, LPARAM);

private:
    bool       resizable_;
    bool       is_moving_or_resizing_;
    bool       is_minimized_;
    CursorType mouse_cursor_;
    HIMC       imc_restore_;
    int        mouse_flag_;
    String     device_name_;

    Vector<Resolution>       resolutions_;
    std::array<KeyCode, 256> key_map_;
};

RefPtr<Window> Window::Create(const WindowConfig& config)
{
    RefPtr<WindowWin32Impl> ptr = MakePtr<WindowWin32Impl>();
    if (ptr)
    {
        ptr->Init(config);
    }
    return ptr;
}

}  // namespace kiwano

namespace kiwano
{

#define WINDOW_FIXED_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_RESIZABLE_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX
#define WINDOW_FULLSCREEN_STYLE WS_CLIPCHILDREN | WS_POPUP

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

HICON Icon2HIcon(const Icon& icon)
{
    HICON hicon = NULL;
    if (icon.resource_id != 0 && IS_INTRESOURCE(icon.resource_id))
    {
        HINSTANCE hinstance = ::GetModuleHandle(nullptr);

        hicon = (HICON)::LoadImage(hinstance, MAKEINTRESOURCE(icon.resource_id), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
    }
    else
    {
        String full_path = FileSystem::GetInstance().GetFullPathForFile(icon.file_path);
        if (!full_path.empty())
        {
            hicon = (HICON)::LoadImageA(NULL, full_path.c_str(), IMAGE_ICON, 0, 0,
                                        LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
        }
    }
    return hicon;
}

}  // namespace

WindowWin32Impl::WindowWin32Impl()
    : resizable_(false)
    , is_moving_or_resizing_(false)
    , is_minimized_(false)
    , mouse_cursor_(CursorType::Arrow)
    , key_map_{}
    , imc_restore_(nullptr)
    , mouse_flag_{}
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

void WindowWin32Impl::Init(const WindowConfig& config)
{
    EnableDpiAwareness();

    HINSTANCE   hinst  = GetModuleHandle(nullptr);
    WNDCLASSEXA wcex   = { 0 };
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.lpszClassName = "KiwanoAppWnd";
    wcex.style         = CS_HREDRAW | CS_VREDRAW /* | CS_DBLCLKS */;
    wcex.lpfnWndProc   = WindowWin32Impl::StaticWndProc;
    wcex.hIcon         = Icon2HIcon(config.icon);
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hinst;
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = nullptr;
    wcex.hCursor       = ::LoadCursor(hinst, IDC_ARROW);

    ::RegisterClassExA(&wcex);

    // Get the nearest monitor to this window
    HMONITOR monitor = ::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

    // Get the target monitor info
    MONITORINFOEXA monitor_info;
    memset(&monitor_info, 0, sizeof(MONITORINFOEXA));
    monitor_info.cbSize = sizeof(MONITORINFOEXA);
    ::GetMonitorInfoA(monitor, &monitor_info);

    // Save the device name
    device_name_ = monitor_info.szDevice;

    logical_size_  = Size((float)config.width, (float)config.height);
    resizable_     = config.resizable;
    is_fullscreen_ = config.fullscreen;

    // In terms of using the correct DPI, to create a window at a specific size
    // like this, the procedure is to first create the window hidden. Then we get
    // the actual DPI from the HWND (which will be assigned by whichever monitor
    // the window is created on). Then we use SetWindowPos to resize it to the
    // correct DPI-scaled size, then we use ShowWindow to show it.
    handle_ = ::CreateWindowExA(NULL, "KiwanoAppWnd", config.title.c_str(), GetStyle(), 0, 0, 0, 0, nullptr, nullptr,
                                hinst, nullptr);

    if (handle_ == nullptr)
    {
        ::UnregisterClassA("KiwanoAppWnd", hinst);
        KGE_THROW_SYSTEM_ERROR(HRESULT_FROM_WIN32(GetLastError()), "Create window failed");
    }

    dpi_ = GetDpiForHwnd(handle_);

    // use Application instance in message loop
    ::SetWindowLongPtrA(handle_, GWLP_USERDATA, LONG_PTR(this));

    SetResolution(config.width, config.height, config.fullscreen);

    ::UpdateWindow(handle_);

    // disable imm
    SetImmEnabled(false);
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

void WindowWin32Impl::SetImmEnabled(bool enable)
{
    const bool enabled = imc_restore_ == nullptr;
    if (enable != enabled)
    {
        if (enable)
        {
            ::ImmAssociateContext(handle_, imc_restore_);
            imc_restore_ = nullptr;
        }
        else
        {
            imc_restore_ = ::ImmAssociateContext(handle_, nullptr);
        }
    }
}

void WindowWin32Impl::SetTitle(StringView title)
{
    KGE_ASSERT(handle_);
    ::SetWindowTextA(handle_, title.data());
}

void WindowWin32Impl::SetIcon(Icon icon)
{
    KGE_ASSERT(handle_);

    HICON hicon = Icon2HIcon(icon);
    ::SendMessage(handle_, WM_SETICON, ICON_BIG, (LPARAM)hicon);
    ::SendMessage(handle_, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
}

void WindowWin32Impl::SetMinimumSize(uint32_t width, uint32_t height)
{
    AdjustWindow(width, height, GetStyle(), &min_width_, &min_height_);
}

void WindowWin32Impl::SetMaximumSize(uint32_t width, uint32_t height)
{
    AdjustWindow(width, height, GetStyle(), &max_width_, &max_height_);
}

void WindowWin32Impl::SetCursor(CursorType cursor)
{
    mouse_cursor_ = cursor;
}

void WindowWin32Impl::SetResolution(uint32_t width, uint32_t height, bool fullscreen)
{
    if (is_fullscreen_ != fullscreen)
    {
        is_fullscreen_ = fullscreen;

        // Reset window style
        ::SetWindowLongPtrA(handle_, GWL_STYLE, GetStyle());
    }

    if (is_fullscreen_)
    {
        MONITORINFOEXA info = GetMoniterInfoEx(handle_);
        int            x    = (int)info.rcMonitor.left;
        int            y    = (int)info.rcMonitor.top;
        int            cx   = (int)(info.rcMonitor.right - info.rcMonitor.left);
        int            cy   = (int)(info.rcMonitor.bottom - info.rcMonitor.top);

        // Top the window
        ::SetWindowPos(handle_, HWND_TOPMOST, x, y, cx, cy, SWP_NOACTIVATE);
    }
    else
    {
        // Adjust the rect of client area
        RECT rc = { 0, 0, LONG(float(width) * GetDPIScale()), LONG(float(height) * GetDPIScale()) };
        ::AdjustWindowRectExForDpi(&rc, GetStyle(), FALSE, NULL, UINT(dpi_));

        auto client_width  = uint32_t(rc.right - rc.left);
        auto client_height = uint32_t(rc.bottom - rc.top);

        MONITORINFOEXA info    = GetMoniterInfoEx(handle_);
        uint32_t       screenw = uint32_t(info.rcWork.right - info.rcWork.left);
        uint32_t       screenh = uint32_t(info.rcWork.bottom - info.rcWork.top);
        int            left    = screenw > client_width ? ((screenw - client_width) / 2) : 0;
        int            top     = screenh > client_height ? ((screenh - client_height) / 2) : 0;

        // Unpin the window
        ::SetWindowPos(handle_, HWND_NOTOPMOST, left, top, client_width, client_height,
                       SWP_DRAWFRAME | SWP_FRAMECHANGED);
    }

    ::ShowWindow(handle_, SW_SHOWNORMAL);

    resolution_ = Resolution{ width, height, 0 };
}

Vector<Resolution> WindowWin32Impl::GetResolutions()
{
    if (resolutions_.empty())
    {
        Set<String> resolution_list;

        DEVMODEA dmi;
        ZeroMemory(&dmi, sizeof(dmi));
        dmi.dmSize = sizeof(dmi);

        DWORD mode_count = 0;
        while (EnumDisplaySettingsA(device_name_.c_str(), mode_count++, &dmi) != 0)
        {
            StringStream ss;
            ss << dmi.dmPelsWidth << 'x' << dmi.dmPelsHeight << ':' << dmi.dmDisplayFrequency;

            if (resolution_list.find(ss.str()) == resolution_list.end())
            {
                resolution_list.insert(ss.str());
                resolutions_.push_back(Resolution{ uint32_t(dmi.dmPelsWidth), uint32_t(dmi.dmPelsHeight),
                                                   uint32_t(dmi.dmDisplayFrequency) });
            }
            ZeroMemory(&dmi, sizeof(dmi));
        }
    }
    return resolutions_;
}

DWORD WindowWin32Impl::GetStyle() const
{
    if (is_fullscreen_)
        return WINDOW_FULLSCREEN_STYLE;
    if (resizable_)
        return WINDOW_RESIZABLE_STYLE;
    return WINDOW_FIXED_STYLE;
}

void WindowWin32Impl::SetActive(bool active)
{
    if (!handle_)
        return;

    if (is_fullscreen_)
    {
        // Hide window when it is not active
        if (active)
        {
            ::SetWindowPos(handle_, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
        else
        {
            ::SetWindowPos(handle_, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
    }
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
    case CursorType::No:
        win32_cursor = IDC_NO;
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
        if (key != KeyCode::Unknown && !ignore_key_events_)
        {
            RefPtr<KeyDownEvent> evt = new KeyDownEvent;
            evt->code                = key;
            this->PushEvent(evt);
        }
    }
    break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        KeyCode key = this->key_map_[size_t(wparam)];
        if (key != KeyCode::Unknown && !ignore_key_events_)
        {
            RefPtr<KeyUpEvent> evt = new KeyUpEvent;
            evt->code              = key;
            this->PushEvent(evt);
        }
    }
    break;

    case WM_CHAR:
    {
        if (!ignore_key_events_)
        {
            RefPtr<KeyCharEvent> evt = new KeyCharEvent;
            evt->value               = char(wparam);
            this->PushEvent(evt);
        }
    }
    break;

    case WM_IME_CHAR:
    {
        // no need to handle
        return TRUE;
    }

    case WM_IME_COMPOSITION:
    {
        if (lparam & GCS_RESULTSTR)
        {
            const auto hIMC = ::ImmGetContext(hwnd);
            if (hIMC)
            {
                // Get the size of the result string.
                auto dwSize = ::ImmGetCompositionStringA(hIMC, GCS_RESULTSTR, NULL, 0);

                // increase buffer size for terminating null character
                dwSize += sizeof(char);

                // Get the result strings that is generated by IME.
                String buf(size_t(dwSize), 0);
                ::ImmGetCompositionStringA(hIMC, GCS_RESULTSTR, const_cast<char*>(buf.data()), dwSize);
                ::ImmReleaseContext(hwnd, hIMC);

                RefPtr<IMEInputEvent> evt = new IMEInputEvent;
                evt->value                = std::move(buf);
                this->PushEvent(evt);
                return TRUE;
            }
        }
    }

    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    {
        RefPtr<MouseDownEvent> evt = new MouseDownEvent;
        evt->pos                   = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam)) / GetDPIScale();

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

        if (mouse_flag_ == 0 && ::GetCapture() == nullptr)
            ::SetCapture(hwnd);
        mouse_flag_ |= 1 << int(evt->button);

        this->PushEvent(evt);
    }
    break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        RefPtr<MouseUpEvent> evt = new MouseUpEvent;
        evt->pos                 = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam)) / GetDPIScale();

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

        mouse_flag_ &= ~(1 << int(evt->button));
        if (mouse_flag_ == 0 && ::GetCapture() == hwnd)
            ::ReleaseCapture();

        this->PushEvent(evt);
    }
    break;

    case WM_MOUSEMOVE:
    {
        RefPtr<MouseMoveEvent> evt = new MouseMoveEvent;
        evt->pos                   = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam)) / GetDPIScale();
        this->PushEvent(evt);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
        ::ScreenToClient(hwnd, &pt);

        RefPtr<MouseWheelEvent> evt = new MouseWheelEvent;
        evt->pos                    = Point((float)pt.x, (float)pt.y) / GetDPIScale();
        evt->wheel                  = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
        this->PushEvent(evt);
    }
    break;

    case WM_SIZE:
    {
        bool resized = false;
        if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
        {
            KGE_DEBUG_LOGF("Window minimized");

            is_minimized_ = true;
            // Pause game when window is minimized
            Application::GetInstance().Pause();
        }
        else if (SIZE_MAXIMIZED == wparam)
        {
            KGE_DEBUG_LOGF("Window maximized");

            if (is_minimized_)
            {
                is_minimized_ = false;
                Application::GetInstance().Resume();
            }
            else
            {
                resized = true;
            }
        }
        else if (wparam == SIZE_RESTORED)
        {
            if (is_minimized_)
            {
                KGE_DEBUG_LOGF("Window restored");

                // the window was restored and was previously minimized
                is_minimized_ = false;
                Application::GetInstance().Resume();
            }
            else if (is_moving_or_resizing_)
            {
                // DO NOTHING until the dragging / resizing has stopped.
            }
            else
            {
                resized = true;
            }
        }
        if (resized)
        {
            this->real_width_  = ((uint32_t)(short)LOWORD(lparam));
            this->real_height_ = ((uint32_t)(short)HIWORD(lparam));

            RefPtr<WindowResizedEvent> evt = new WindowResizedEvent;
            evt->window                    = this;
            evt->real_width                = this->GetRealWidth();
            evt->real_height               = this->GetRealHeight();
            this->PushEvent(evt);

            KGE_DEBUG_LOGF("Window resized to (%d, %d)", this->real_width_, this->real_height_);
        }
    }
    break;

    case WM_ENTERSIZEMOVE:
    {
        is_moving_or_resizing_ = true;
        Application::GetInstance().Pause();
        return 0;
    }

    case WM_EXITSIZEMOVE:
    {
        is_moving_or_resizing_ = false;
        Application::GetInstance().Resume();

        // Send window resized event when client size changed
        RECT client_rect = { 0 };
        ::GetClientRect(hwnd, &client_rect);

        uint32_t client_width  = uint32_t(client_rect.right - client_rect.left);
        uint32_t client_height = uint32_t(client_rect.bottom - client_rect.top);
        if (client_width != this->GetRealWidth() || client_height != this->GetRealHeight())
        {
            KGE_DEBUG_LOGF("Window resized to (%d, %d)", client_width, client_height);

            this->real_width_  = client_width;
            this->real_height_ = client_height;

            RefPtr<WindowResizedEvent> evt = new WindowResizedEvent;
            evt->window                    = this;
            evt->real_width                = client_width;
            evt->real_height               = client_height;
            this->PushEvent(evt);
        }

        RECT window_rect = { 0 };
        ::GetWindowRect(hwnd, &window_rect);

        int window_x = int(window_rect.left);
        int window_y = int(window_rect.top);
        if (window_x != this->GetPosX() || window_y != this->GetPosY())
        {
            KGE_DEBUG_LOGF("Window moved to (%d, %d)", window_x, window_y);

            this->pos_x_ = window_x;
            this->pos_y_ = window_y;

            RefPtr<WindowMovedEvent> evt = new WindowMovedEvent;
            evt->window                  = this;
            evt->x                       = window_x;
            evt->y                       = window_y;
            this->PushEvent(evt);
        }
        return 0;
    }

    case WM_GETMINMAXINFO:
    {
        if (min_width_ && min_height_)
        {
            ((MINMAXINFO*)lparam)->ptMinTrackSize = POINT{ LONG(min_width_), LONG(min_height_) };
        }
        if (max_width_ && max_height_)
        {
            ((MINMAXINFO*)lparam)->ptMaxTrackSize = POINT{ LONG(max_width_), LONG(max_height_) };
            ((MINMAXINFO*)lparam)->ptMaxSize      = POINT{ LONG(max_width_), LONG(max_height_) };
        }
        return 0;
    }

    case WM_MOVE:
    {
        if (is_moving_or_resizing_)
        {
            // DO NOTHING until the dragging / resizing has stopped.
        }
        else
        {
            int window_x = GET_X_LPARAM(lparam);
            int window_y = GET_Y_LPARAM(lparam);

            KGE_DEBUG_LOGF("Window moved to (%d, %d)", window_x, window_y);

            this->pos_x_ = window_x;
            this->pos_y_ = window_y;

            RefPtr<WindowMovedEvent> evt = new WindowMovedEvent;
            evt->window                  = this;
            evt->x                       = window_x;
            evt->y                       = window_y;
            this->PushEvent(evt);
        }
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

        RefPtr<WindowFocusChangedEvent> evt = new WindowFocusChangedEvent;
        evt->window                         = this;
        evt->focus                          = active;
        this->PushEvent(evt);
    }
    break;

    case WM_SETFOCUS:
    {
        if (is_fullscreen_)
        {
            SetActive(true);
        }
    }
    break;

    case WM_KILLFOCUS:
    {
        if (is_fullscreen_)
        {
            SetActive(false);
        }
    }
    break;

    case WM_SETTEXT:
    {
        KGE_DEBUG_LOGF("Window title changed");

        this->title_ = strings::WideToNarrow(reinterpret_cast<LPCWSTR>(lparam));

        RefPtr<WindowTitleChangedEvent> evt = new WindowTitleChangedEvent;
        evt->window                         = this;
        evt->title                          = this->title_;
        this->PushEvent(evt);
    }
    break;

    case WM_SETICON:
    {
        KGE_DEBUG_LOGF("Window icon changed");
    }
    break;

    case WM_DISPLAYCHANGE:
    {
        KGE_DEBUG_LOGF("The display resolution has changed");
    }
    break;

    case WM_SETCURSOR:
    {
        this->UpdateCursor();
    }
    break;

    case WM_CLOSE:
    {
        KGE_DEBUG_LOGF("Window is closing");

        RefPtr<WindowClosedEvent> evt = new WindowClosedEvent;
        evt->window                   = this;
        this->PushEvent(evt);
        this->SetShouldClose(true);
        return 0;
    }
    break;

    case WM_DESTROY:
    {
        KGE_DEBUG_LOGF("Window was destroyed");

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

//
// DPI-related helpers
//

typedef enum
{
    PROCESS_DPI_UNAWARE           = 0,
    PROCESS_SYSTEM_DPI_AWARE      = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum
{
    MDT_EFFECTIVE_DPI = 0,
    MDT_ANGULAR_DPI   = 1,
    MDT_RAW_DPI       = 2,
    MDT_DEFAULT       = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

typedef HRESULT(WINAPI* PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);  // Shcore.lib + dll, Windows 8.1+
typedef HRESULT(WINAPI* PFN_GetDpiForMonitor)(HMONITOR, MONITOR_DPI_TYPE, UINT*,
                                              UINT*);  // Shcore.lib + dll, Windows 8.1+
typedef DPI_AWARENESS_CONTEXT(WINAPI* PFN_SetThreadDpiAwarenessContext)(
    DPI_AWARENESS_CONTEXT);  // User32.lib + dll, Windows 10 v1607+ (Creators Update)

static BOOL _IsWindowsVersionOrGreater(WORD major, WORD minor, WORD)
{
    typedef LONG(WINAPI * PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);
    static PFN_RtlVerifyVersionInfo RtlVerifyVersionInfoFn = nullptr;
    if (RtlVerifyVersionInfoFn == nullptr)
        if (HMODULE ntdllModule = ::GetModuleHandleA("ntdll.dll"))
            RtlVerifyVersionInfoFn = (PFN_RtlVerifyVersionInfo)GetProcAddress(ntdllModule, "RtlVerifyVersionInfo");
    if (RtlVerifyVersionInfoFn == nullptr)
        return FALSE;

    RTL_OSVERSIONINFOEXW versionInfo   = {};
    ULONGLONG            conditionMask = 0;
    versionInfo.dwOSVersionInfoSize    = sizeof(RTL_OSVERSIONINFOEXW);
    versionInfo.dwMajorVersion         = major;
    versionInfo.dwMinorVersion         = minor;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    return (RtlVerifyVersionInfoFn(&versionInfo, VER_MAJORVERSION | VER_MINORVERSION, conditionMask) == 0) ? TRUE
                                                                                                           : FALSE;
}

#define _IsWindowsVistaOrGreater() _IsWindowsVersionOrGreater(HIBYTE(0x0600), LOBYTE(0x0600), 0)  // _WIN32_WINNT_VISTA
#define _IsWindows8OrGreater() _IsWindowsVersionOrGreater(HIBYTE(0x0602), LOBYTE(0x0602), 0)      // _WIN32_WINNT_WIN8
#define _IsWindows8Point1OrGreater() \
    _IsWindowsVersionOrGreater(HIBYTE(0x0603), LOBYTE(0x0603), 0)  // _WIN32_WINNT_WINBLUE
#define _IsWindows10OrGreater() \
    _IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0)  // _WIN32_WINNT_WINTHRESHOLD / _WIN32_WINNT_WIN10

// Helper function to enable DPI awareness without setting up a manifest
static void EnableDpiAwareness()
{
    if (_IsWindows10OrGreater())
    {
        static HINSTANCE user32_dll = ::LoadLibraryA("user32.dll");  // Reference counted per-process
        if (PFN_SetThreadDpiAwarenessContext SetThreadDpiAwarenessContextFn =
                (PFN_SetThreadDpiAwarenessContext)::GetProcAddress(user32_dll, "SetThreadDpiAwarenessContext"))
        {
            SetThreadDpiAwarenessContextFn(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll");  // Reference counted per-process
        if (PFN_SetProcessDpiAwareness SetProcessDpiAwarenessFn =
                (PFN_SetProcessDpiAwareness)::GetProcAddress(shcore_dll, "SetProcessDpiAwareness"))
        {
            SetProcessDpiAwarenessFn(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }
#if _WIN32_WINNT >= 0x0600
    ::SetProcessDPIAware();
#endif
}

static float GetDpiForMonitor(HMONITOR monitor)
{
    UINT xdpi = 96, ydpi = 96;
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE            shcore_dll         = ::LoadLibraryA("shcore.dll");  // Reference counted per-process
        static PFN_GetDpiForMonitor GetDpiForMonitorFn = nullptr;
        if (GetDpiForMonitorFn == nullptr && shcore_dll != nullptr)
            GetDpiForMonitorFn = (PFN_GetDpiForMonitor)::GetProcAddress(shcore_dll, "GetDpiForMonitor");
        if (GetDpiForMonitorFn != nullptr)
        {
            GetDpiForMonitorFn(monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
            return (float)xdpi;
        }
    }

    const HDC dc = ::GetDC(nullptr);
    xdpi         = ::GetDeviceCaps(dc, LOGPIXELSX);
    ydpi         = ::GetDeviceCaps(dc, LOGPIXELSY);
    ::ReleaseDC(nullptr, dc);
    return (float)xdpi;
}

static float GetDpiForHwnd(HWND hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    return GetDpiForMonitor(monitor);
}

}  // namespace kiwano

#endif
