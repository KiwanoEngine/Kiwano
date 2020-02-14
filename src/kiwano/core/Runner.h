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

namespace kiwano
{
/**
 * \~chinese
 * @brief 程序运行器
 */
class KGE_API Runner
{
public:
    typedef Function<void()> Callback;

    Runner();

    /// \~chinese
    /// @brief 构造程序运行器
    /// @param on_ready 应用程序初始化完成后执行的回调函数
    /// @param on_destroy 应用程序销毁时执行的回调函数
    Runner(const Callback& on_ready, const Callback& on_destroy = nullptr);

    /// \~chinese
    /// @brief 初始化完成处理
    /// @details 重载该函数以在应用程序初始化完成后自动执行
    virtual void OnReady();

    /// \~chinese
    /// @brief 应用程序销毁处理
    /// @details 重载该函数以处理应用程序销毁时的行为，如完成资源回收等
    virtual void OnDestroy();

    /// \~chinese
    /// @brief 窗口关闭处理
    /// @details 重载该函数以处理用户关闭窗口时的行为，如保存用户数据等
    /// @return 返回true允许用户关闭窗口，否则阻止窗口关闭
    virtual bool OnClosing();

private:
    Callback on_ready_;
    Callback on_destroy_;
};


inline Runner::Runner() {}

inline Runner::Runner(const Callback& on_ready, const Callback& on_destroy)
    : on_ready_(on_ready)
    , on_destroy_(on_destroy)
{
}

inline void Runner::OnReady()
{
    if (on_ready_)
    {
        on_ready_();
    }
}

inline void Runner::OnDestroy()
{
    if (on_destroy_)
    {
        on_destroy_();
    }
}

inline bool Runner::OnClosing()
{
    return true;
}

}  // namespace kiwano
