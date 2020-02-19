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
#include <kiwano/core/EventListener.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief 事件分发系统
 */
class KGE_API EventDispatcher
{
public:
    /// \~chinese
    /// @brief 添加监听器
    EventListener* AddListener(EventListenerPtr listener);

    /// \~chinese
    /// @brief 添加监听器
    /// @param type 监听的事件类型
    /// @param callback 回调函数
    EventListener* AddListener(EventType type, EventListener::Callback callback);

    /// \~chinese
    /// @brief 添加监听器
    /// @param name 监听器名称
    /// @param type 监听的事件类型
    /// @param callback 回调函数
    EventListener* AddListener(const String& name, EventType type, EventListener::Callback callback);

    /// \~chinese
    /// @brief 添加监听器
    /// @tparam _EventTy 事件类型
    /// @param callback 回调函数
    template <typename _EventTy, typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type>
    EventListener* AddListener(EventListener::Callback callback)
    {
        return AddListener(KGE_EVENT(_EventTy), callback);
    }

    /// \~chinese
    /// @brief 添加监听器
    /// @tparam _EventTy 事件类型
    /// @param name 监听器名称
    /// @param callback 回调函数
    template <typename _EventTy, typename = typename std::enable_if<IsEvent<_EventTy>::value, int>>
    EventListener* AddListener(const String& name, EventListener::Callback callback)
    {
        return AddListener(name, KGE_EVENT(_EventTy), callback);
    }

    /// \~chinese
    /// @brief 启动监听器
    /// @param name 监听器名称
    void StartListeners(const String& name);

    /// \~chinese
    /// @brief 停止监听器
    /// @param name 监听器名称
    void StopListeners(const String& name);

    /// \~chinese
    /// @brief 移除监听器
    /// @param name 监听器名称
    void RemoveListeners(const String& name);

    /// \~chinese
    /// @brief 启动监听器
    /// @param type 监听的事件类型
    void StartListeners(const EventType& type);

    /// \~chinese
    /// @brief 停止监听器
    /// @param type 监听的事件类型
    void StopListeners(const EventType& type);

    /// \~chinese
    /// @brief 移除监听器
    /// @param type 监听的事件类型
    void RemoveListeners(const EventType& type);

    /// \~chinese
    /// @brief 启动所有监听器
    void StartAllListeners();

    /// \~chinese
    /// @brief 停止所有监听器
    void StopAllListeners();

    /// \~chinese
    /// @brief 移除所有监听器
    void RemoveAllListeners();

    /// \~chinese
    /// @brief 获取所有监听器
    const ListenerList& GetAllListeners() const;

    /// \~chinese
    /// @brief 分发事件
    /// @param evt 事件
    /// @return 是否继续分发该事件
    bool DispatchEvent(Event* evt);

private:
    ListenerList listeners_;
};
}  // namespace kiwano
