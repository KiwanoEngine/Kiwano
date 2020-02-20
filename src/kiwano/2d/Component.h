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
#include <kiwano/core/Time.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/IntrusiveList.h>

namespace kiwano
{

class Actor;
class Event;

KGE_DECLARE_SMART_PTR(Component);

/// \~chinese
/// @brief 组件列表
typedef IntrusiveList<ComponentPtr> ComponentList;

/**
 * \~chinese
 * @brief 组件
 */
class KGE_API Component
    : public ObjectBase
    , protected IntrusiveListValue<ComponentPtr>
{
    friend class Actor;
    friend IntrusiveList<ComponentPtr>;

public:
    /// \~chinese
    /// @brief 是否启用组件
    bool IsEnable() const;

    /// \~chinese
    /// @brief 设置组件启用或禁用
    void SetEnabled(bool enabled);

    /// \~chinese
    /// @brief 获取绑定的角色
    Actor* GetBoundActor() const;

    /// \~chinese
    /// @brief 从角色中移除
    void RemoveFromActor();

protected:
    Component();

    virtual ~Component();

    /// \~chinese
    /// @brief 初始化组件
    virtual void InitComponent(Actor* actor);

    /// \~chinese
    /// @brief 销毁组件
    virtual void DestroyComponent();

    /// \~chinese
    /// @brief 更新组件
    virtual void OnUpdate(Duration dt);

    /// \~chinese
    /// @brief 处理角色事件
    virtual void HandleEvent(Event* evt);

private:
    bool   enabled_;
    Actor* actor_;
};

inline bool Component::IsEnable() const
{
    return enabled_;
}

inline void Component::SetEnabled(bool enabled)
{
    enabled_ = enabled;
}

inline Actor* Component::GetBoundActor() const
{
    return actor_;
}

inline void Component::OnUpdate(Duration dt)
{
    KGE_NOT_USED(dt);
}

inline void Component::HandleEvent(Event* event)
{
    KGE_NOT_USED(event);
}

}  // namespace kiwano
