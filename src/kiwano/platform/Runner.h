// Copyright (c) 2016-2018 Kiwano - Nomango
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

#pragma once
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>
#include <kiwano/platform/Window.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

class Application;

KGE_DECLARE_SMART_PTR(Runner);

/**
 * \~chinese
 * @brief 游戏设置
 */
struct Settings
{
    uint32_t width;          ///< 窗口宽度
    uint32_t height;         ///< 窗口高度
    String   title;          ///< 窗口标题
    uint32_t icon;           ///< 窗口图标
    bool     resizable;      ///< 窗口大小可调整
    Color    bg_color;       ///< 窗口背景色
    bool     vsync_enabled;  ///< 垂直同步
    bool     debug_mode;     ///< 调试模式

    Settings()
        : width(800)
        , height(600)
        , title("Kiwano")
        , icon()
        , resizable(false)
        , bg_color(Color::Black)
        , vsync_enabled(true)
        , debug_mode(false)
    {
    }
};

/**
 * \~chinese
 * @brief 程序运行器
 */
class KGE_API Runner : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 创建程序运行器
    /// @param main_window 主窗口
    static RunnerPtr Create(Settings settings);

    /// \~chinese
    /// @brief 创建程序运行器
    /// @param main_window 主窗口
    /// @param on_ready 应用程序初始化完成后执行的回调函数
    /// @param on_destroy 应用程序销毁时执行的回调函数
    static RunnerPtr Create(Settings settings, Function<void()> on_ready, Function<void()> on_destroy = nullptr);

    Runner();

    virtual ~Runner();

    /// \~chinese
    /// @brief 初始化完成处理
    /// @details 重载该函数以在应用程序初始化完成后自动执行
    virtual void OnReady();

    /// \~chinese
    /// @brief 应用程序销毁处理
    /// @details 重载该函数以处理应用程序销毁时的行为，如完成资源回收等
    virtual void OnDestroy();

    /// \~chinese
    /// @brief 应用程序关闭处理
    /// @details 重载该函数以处理用户关闭应用程序时的行为，如保存用户数据等
    /// @return 返回true允许用户关闭程序，否则阻止程序关闭
    virtual bool OnClose();

    /// \~chinese
    /// @brief 应用程序主循环
    /// @param dt 时间间隔
    /// @details 重载该函数以控制程序主循环
    /// @return 返回false退出主循环，否则继续运行主循环
    virtual bool MainLoop(Duration dt);

    /// \~chinese
    /// @brief 获取窗口
    WindowPtr GetWindow() const;

    /// \~chinese
    /// @brief 设置窗口
    void SetWindow(WindowPtr window);

    /// \~chinese
    /// @brief 获取设置
    Settings GetSettings() const;

protected:
    /// \~chinese
    /// @brief 修改设置
    void SetSettings(Settings settings);

private:
    friend class Application;

    void InitSettings();

private:
    Settings  settings_;
    WindowPtr main_window_;
};

inline void Runner::OnReady() {}

inline void Runner::OnDestroy() {}

inline bool Runner::OnClose()
{
    return true;
}

inline WindowPtr Runner::GetWindow() const
{
    return main_window_;
}

inline void Runner::SetWindow(WindowPtr window)
{
    main_window_ = window;
}

inline Settings Runner::GetSettings() const
{
    return settings_;
}

inline void Runner::SetSettings(Settings settings)
{
    settings_ = settings;
}

}  // namespace kiwano
