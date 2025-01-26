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
#include <kiwano/core/BaseObject.h>
#include <kiwano/core/IntrusiveList.hpp>

namespace kiwano
{

class Actor;
class Director;

/**
 * \~chinese
 * \defgroup Ability 角色能力
 */

/**
 * \addtogroup Ability
 * @{
 */

/**
 * \~chinese
 * @brief 角色能力
 */
class KGE_API Ability
    : public BaseObject
    , protected IntrusiveListValue<RefPtr<Ability>>
{
    friend class Actor;
    friend class Director;
    friend IntrusiveList<RefPtr<Ability>>;

public:
    /// \~chinese
    /// @brief 是否启用
    bool IsEnabled() const;

    /// \~chinese
    /// @brief 设置启用或禁用
    void SetEnabled(bool enabled);

    /// \~chinese
    /// @brief 获取附加的角色
    Actor* GetAttachedActor() const;

    /// \~chinese
    /// @brief 从角色中分离
    /// @param immediate 是否直接移除，不建议使用除非了解它的含义
    void Detach(bool immediate = false);

    /// \~chinese
    /// @brief 获取权重
    int16_t GetPriority() const;

    /// \~chinese
    /// @brief 设置优先级
    /// @details 优先级越大，执行顺序越靠前，默认为 0
    void SetPriority(int16_t priority);

    /// \~chinese
    /// @brief 主动初始化
    /// @details 在其他 Ability::OnInit 主动调用时可以强制依赖顺序
    void Initialize();

protected:
    Ability();

    /// \~chinese
    /// @brief 当 Ability 被 Actor 附加时调用
    virtual void OnAttached(Actor* actor);

    /// \~chinese
    /// @brief 当 Ability 被 Actor 分离后调用
    /// @details 一般在帧末尾调用，若分离时指定了 immediate，则立刻调用
    virtual void OnDetached();

    /// \~chinese
    /// @brief 初始化 Ability
    /// @details 附加到 Actor 后，下一帧开始前调用
    virtual void OnInit();

    /// \~chinese
    /// @brief 每帧更新时调用
    virtual void OnUpdate(Duration dt);

private:
    bool    enabled_;
    bool    initialized_;
    int16_t priority_;
    Actor*  actor_;
};

/// \~chinese
/// @brief 角色能力列表
typedef IntrusiveList<RefPtr<Ability>> AbilityList;

/** @} */

inline bool Ability::IsEnabled() const
{
    return enabled_;
}

inline void Ability::SetEnabled(bool enabled)
{
    enabled_ = enabled;
}

inline Actor* Ability::GetAttachedActor() const
{
    return actor_;
}

inline int16_t Ability::GetPriority() const
{
    return priority_;
}

inline void Ability::OnInit() {}

inline void Ability::OnUpdate(Duration dt)
{
    KGE_NOT_USED(dt);
}

}  // namespace kiwano
