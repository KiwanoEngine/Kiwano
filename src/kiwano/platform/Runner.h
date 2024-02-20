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
#include <kiwano/utils/Ticker.h>

namespace kiwano
{

class Application;

/**
 * \~chinese
 * @brief 游戏设置
 */
struct Settings
{
    WindowConfig window;          ///< 窗口设置
    Color        bg_color;        ///< 背景色
    Duration     frame_interval;  ///< 帧间隔
    bool         vsync_enabled;   ///< 垂直同步
    bool         debug_mode;      ///< 调试模式

    Settings()
        : bg_color(Color::Black)
        , frame_interval(0)
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
    Runner();

    /// \~chinese
    /// @brief 创建程序运行器
    /// @param main_window 主窗口
    Runner(const Settings& settings);

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
    bool MainLoop(Duration dt);

    /// \~chinese
    /// @brief 获取窗口
    RefPtr<Window> GetWindow() const;

    /// \~chinese
    /// @brief 获取设置
    Settings GetSettings() const;

    /// \~chinese
    /// @brief 获取帧报时器
    RefPtr<Ticker> GetFrameTicker() const;

    /// \~chinese
    /// @brief 设置帧报时器
    void SetFrameTicker(RefPtr<Ticker> ticker);

protected:
    /// \~chinese
    /// @brief 修改设置
    void SetSettings(Settings settings);

    /// \~chinese
    /// @brief 设置窗口
    void SetWindow(RefPtr<Window> window);

private:
    friend class Application;

    void InitSettings();

private:
    Settings       settings_;
    RefPtr<Window> main_window_;
    RefPtr<Ticker> frame_ticker_;
};

inline void Runner::OnReady() {}

inline void Runner::OnDestroy() {}

inline bool Runner::OnClose()
{
    return true;
}

inline RefPtr<Window> Runner::GetWindow() const
{
    return main_window_;
}

inline void Runner::SetWindow(RefPtr<Window> window)
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

inline RefPtr<Ticker> Runner::GetFrameTicker() const
{
    return frame_ticker_;
}

inline void Runner::SetFrameTicker(RefPtr<Ticker> ticker)
{
    frame_ticker_ = ticker;
}

}  // namespace kiwano
