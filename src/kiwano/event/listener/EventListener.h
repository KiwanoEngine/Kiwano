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
#include <kiwano/core/IntrusiveList.hpp>
#include <kiwano/core/BaseObject.h>
#include <kiwano/event/Events.h>

namespace kiwano
{
class EventDispatcher;

/**
 * \~chinese
 * \defgroup EventListener 事件监听器
 */

/**
 * \addtogroup EventListener
 * @{
 */

/**
 * \~chinese
 * @brief 事件监听器
 */
class KGE_API EventListener
    : public BaseObject
    , protected IntrusiveListValue<RefPtr<EventListener>>
{
    friend class EventDispatcher;
    friend IntrusiveList<RefPtr<EventListener>>;

public:
    /// \~chinese
    /// @brief 监听器回调函数
    using Callback = Function<void(Event*)>;

    EventListener();

    virtual ~EventListener();

    /// \~chinese
    /// @brief 创建监听器
    /// @param callback 回调函数
    static RefPtr<EventListener> Create(const Callback& callback);

    /// \~chinese
    /// @brief 创建监听器
    /// @param name 监听器名称
    /// @param callback 回调函数
    static RefPtr<EventListener> Create(StringView name, const Callback& callback);

    /// \~chinese
    /// @brief 创建监听器
    /// @param type 监听的事件类型
    /// @param callback 回调函数
    static RefPtr<EventListener> Create(EventType type, const Callback& callback);

    /// \~chinese
    /// @brief 创建监听器
    /// @param name 监听器名称
    /// @param type 监听的事件类型
    /// @param callback 回调函数
    static RefPtr<EventListener> Create(StringView name, EventType type, const Callback& callback);

    /// \~chinese
    /// @brief 启动监听器
    void Start();

    /// \~chinese
    /// @brief 停止监听器
    void Stop();

    /// \~chinese
    /// @brief 移除监听器
    void Remove();

    /// \~chinese
    /// @brief 是否正在运行
    bool IsRunning() const;

    /// \~chinese
    /// @brief 是否可移除
    bool IsRemoveable() const;

    /// \~chinese
    /// @brief 是否开启消息吞没
    bool IsSwallowEnabled() const;

    /// \~chinese
    /// @brief 设置消息吞没功能
    /// @param enabled 是否启用
    void SetSwallowEnabled(bool enabled);

    /// \~chinese
    /// @brief 处理消息
    virtual void Handle(Event* evt) = 0;

private:
    bool running_;
    bool removeable_;
    bool swallow_;
};

/** @} */

inline void EventListener::Start()
{
    running_ = true;
}

inline void EventListener::Stop()
{
    running_ = false;
}

inline void EventListener::Remove()
{
    removeable_ = true;
}

inline bool EventListener::IsRunning() const
{
    return running_;
}

inline bool EventListener::IsRemoveable() const
{
    return removeable_;
}

inline bool EventListener::IsSwallowEnabled() const
{
    return swallow_;
}

inline void EventListener::SetSwallowEnabled(bool enabled)
{
    swallow_ = enabled;
}

}  // namespace kiwano
