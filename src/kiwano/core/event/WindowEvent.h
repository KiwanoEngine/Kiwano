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
#include <kiwano/core/event/Event.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(WindowEvent);
KGE_DECLARE_SMART_PTR(WindowMovedEvent);
KGE_DECLARE_SMART_PTR(WindowResizedEvent);
KGE_DECLARE_SMART_PTR(WindowFocusChangedEvent);
KGE_DECLARE_SMART_PTR(WindowTitleChangedEvent);
KGE_DECLARE_SMART_PTR(WindowClosedEvent);
KGE_DECLARE_SMART_PTR(WindowFullscreenEvent);

/**
 * \addtogroup Events
 * @{
 */

/// \~chinese
/// @brief 窗口事件
class KGE_API WindowEvent : public Event
{
public:
    WindowEvent(const EventType& type);
};

/// \~chinese
/// @brief 窗口移动事件
class KGE_API WindowMovedEvent : public WindowEvent
{
public:
    int x;  ///< 窗口左上角 x 坐标
    int y;  ///< 窗口左上角 y 坐标

    WindowMovedEvent();
};

/// \~chinese
/// @brief 窗口大小变化事件
class KGE_API WindowResizedEvent : public WindowEvent
{
public:
    uint32_t width;   ///< 窗口宽度
    uint32_t height;  ///< 窗口高度

    WindowResizedEvent();
};

/// \~chinese
/// @brief 窗口焦点变化事件
class KGE_API WindowFocusChangedEvent : public WindowEvent
{
public:
    bool focus;  ///< 是否获取到焦点

    WindowFocusChangedEvent();
};

/// \~chinese
/// @brief 窗口标题更改事件
class KGE_API WindowTitleChangedEvent : public WindowEvent
{
public:
    String title;  ///< 标题

    WindowTitleChangedEvent();
};

/// \~chinese
/// @brief 窗口关闭事件
class KGE_API WindowClosedEvent : public WindowEvent
{
public:
    WindowClosedEvent();
};

/// \~chinese
/// @brief 窗口切换全屏事件
class KGE_API WindowFullscreenEvent : public WindowEvent
{
public:
    bool fullscreen;

    WindowFullscreenEvent();
};

/** @} */

template <>
struct IsEventType<WindowEvent>
{
    inline bool operator()(const Event* evt) const
    {
        return evt->GetType() == KGE_EVENT(WindowMovedEvent) || evt->GetType() == KGE_EVENT(WindowResizedEvent)
               || evt->GetType() == KGE_EVENT(WindowFocusChangedEvent)
               || evt->GetType() == KGE_EVENT(WindowTitleChangedEvent) || evt->GetType() == KGE_EVENT(WindowClosedEvent)
               || evt->GetType() == KGE_EVENT(WindowFullscreenEvent);
    }
};

}  // namespace kiwano
