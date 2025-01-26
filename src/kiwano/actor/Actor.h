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
#include <kiwano/ability/Ability.h>
#include <kiwano/ability/TransformAbility.h>
#include <type_traits>

namespace kiwano
{
class Stage;
class Ability;

/// \~chinese
/// @brief 角色列表
typedef IntrusiveList<RefPtr<Actor>> ActorList;

/**
 * \~chinese
 * \defgroup Actors 基础角色
 */

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 角色
 * @details
 * 角色是舞台上最基本的元素，是完成渲染、更新、事件分发等功能的最小单位，也是动画、任务、事件监听等功能的载体 TODO
 */
class KGE_API Actor
    : public BaseObject
    , protected IntrusiveListValue<RefPtr<Actor>>
{
    friend class Ability;
    friend IntrusiveList<RefPtr<Actor>>;

public:
    Actor();

    virtual ~Actor();

    /// \~chinese
    /// @brief 每帧更新时调用
    /// @param dt 距上一次更新的时间间隔
    virtual void OnUpdate(Duration dt);

    /// \~chinese
    /// @brief 角色被激活时调用
    virtual void OnActivated();

    /// \~chinese
    /// @brief 角色被停用时调用
    virtual void OnDeactivated();

    /// \~chinese
    /// @brief 角色是否激活
    bool IsActive() const;

    /// \~chinese
    /// @brief 设置角色激活状态
    void SetActive(bool active);

    /// \~chinese
    /// @brief 获取所在舞台
    Stage* GetStage() const;

    /// \~chinese
    /// @brief 进入舞台
    void EnterStage(Stage* stage);

    /// \~chinese
    /// @brief 从舞台上移除 TODO 下一帧移除
    void RemoveFromStage();

    /// \~chinese
    /// @brief 获取二维变换
    const Transform& GetTransform() const;

    /// \~chinese
    /// @brief 获取二维变换
    Transform& GetTransform();

    /// \~chinese
    /// @brief 设置二维变换
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief 设置二维变换跟随另一角色
    void FollowTransform(RefPtr<Actor> parent);

    /// \~chinese
    /// @brief 获取具有的能力
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    AbilityT* Get() const;

    /// \~chinese
    /// @brief 获取具有的能力（TransformAbility 偏特化）
    template <>
    inline TransformAbility* Get<TransformAbility>() const
    {
        return transform_ability_.Get();
    }

    /// \~chinese
    /// @brief 获取具有的能力
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    Vector<AbilityT*> GetAll() const;

    /// \~chinese
    /// @brief 添加能力
    void Attach(RefPtr<Ability> ability);

    /// \~chinese
    /// @brief 移除能力
    /// @param ability 待移除的能力
    /// @param immediate 是否直接移除，不建议使用除非了解它的含义
    void Detach(RefPtr<Ability> ability, bool immediate = false);

    /// \~chinese
    /// @brief 移除能力
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    void DetachAll();

    /// \~chinese
    /// @brief 获取具有的能力
    const AbilityList& GetAbilityList() const;

    /// \~chinese
    /// @brief 序列化
    void OnSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief 反序列化
    void OnDeserialize(Deserializer* deserializer) override;

    /// \~chinese
    /// @brief 更新角色和能力
    virtual void Update(Duration dt);

private:
    void InsertAbilityByPriority(RefPtr<Ability> ability);

private:
    bool                     active_;
    Stage*                   stage_;
    RefPtr<TransformAbility> transform_ability_;
    AbilityList              ordered_abilities_;
};

/** @} */

inline void Actor::OnUpdate(Duration dt)
{
    KGE_NOT_USED(dt);
}

inline void Actor::OnActivated() {}

inline void Actor::OnDeactivated() {}

inline const Transform& Actor::GetTransform() const
{
    return transform_ability_->GetTransform();
}

inline Transform& Actor::GetTransform()
{
    return transform_ability_->GetTransform();
}

inline Stage* Actor::GetStage() const
{
    return stage_;
}

inline bool Actor::IsActive() const
{
    return active_;
}

template <class AbilityT, class>
AbilityT* Actor::Get() const
{
    for (const auto& a : ordered_abilities_)
    {
        auto ptr = dynamic_cast<AbilityT*>(a.Get());
        if (ptr)
            return ptr;
    }
    return nullptr;
}

template <class AbilityT, class>
Vector<AbilityT*> Actor::GetAll() const
{
    if (ordered_abilities_.IsEmpty())
        return {};

    Vector<AbilityT*> vec;
    for (const auto& a : ordered_abilities_)
    {
        auto ptr = dynamic_cast<AbilityT*>(a.Get());
        if (ptr)
            vec.push_back(ptr);
    }
    return std::move(vec);
}

inline const AbilityList& Actor::GetAbilityList() const
{
    return ordered_abilities_;
}

template <class AbilityT, class>
void Actor::DetachAll()
{
    for (auto iter = ordered_abilities_.begin(); iter != ordered_abilities_.end();)
    {
        RefPtr<Ability> a   = *(iter++);
        auto            ptr = dynamic_cast<AbilityT*>(a.Get());
        if (ptr)
            ordered_abilities_.Remove(a);
    }
}

}  // namespace kiwano
