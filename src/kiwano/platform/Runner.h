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

namespace kiwano
{

class Application;

KGE_DECLARE_SMART_PTR(Runner);

/**
 * \~chinese
 * @brief 程序运行器
 */
class KGE_API Runner : public ObjectBase
{
    friend class Application;

public:
    /// \~chinese
    /// @brief 创建程序运行器
    /// @param main_window 主窗口
    static RunnerPtr Create(WindowPtr main_window);

    /// \~chinese
    /// @brief 创建程序运行器
    /// @param main_window 主窗口
    /// @param on_ready 应用程序初始化完成后执行的回调函数
    /// @param on_destroy 应用程序销毁时执行的回调函数
    static RunnerPtr Create(WindowPtr main_window, Function<void()> on_ready, Function<void()> on_destroy = nullptr);

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
    virtual bool OnClosing();

    /// \~chinese
    /// @brief 应用程序主循环
    /// @param dt 时间间隔
    /// @details 重载该函数以控制程序主循环
    /// @return 返回false退出主循环，否则继续运行主循环
    virtual bool MainLoop(Duration dt);

    /// \~chinese
    /// @brief 获取主窗口
    WindowPtr GetMainWindow() const;

    /// \~chinese
    /// @brief 设置主窗口
    void SetMainWindow(WindowPtr window);

private:
    WindowPtr main_window_;
};

inline void Runner::OnReady() {}

inline void Runner::OnDestroy() {}

inline bool Runner::OnClosing()
{
    return true;
}

inline WindowPtr Runner::GetMainWindow() const
{
    return main_window_;
}

inline void Runner::SetMainWindow(WindowPtr window)
{
    main_window_ = window;
}

}  // namespace kiwano
