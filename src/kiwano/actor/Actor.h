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
/// @brief ��ɫ�б�
typedef IntrusiveList<RefPtr<Actor>> ActorList;

/**
 * \~chinese
 * \defgroup Actors ������ɫ
 */

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ��ɫ
 * @details
 * ��ɫ����̨���������Ԫ�أ��������Ⱦ�����¡��¼��ַ��ȹ��ܵ���С��λ��Ҳ�Ƕ����������¼������ȹ��ܵ����� TODO
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
    /// @brief ÿ֡����ʱ����
    /// @param dt ����һ�θ��µ�ʱ����
    virtual void OnUpdate(Duration dt);

    /// \~chinese
    /// @brief ��ɫ������ʱ����
    virtual void OnActivated();

    /// \~chinese
    /// @brief ��ɫ��ͣ��ʱ����
    virtual void OnDeactivated();

    /// \~chinese
    /// @brief ��ɫ�Ƿ񼤻�
    bool IsActive() const;

    /// \~chinese
    /// @brief ���ý�ɫ����״̬
    void SetActive(bool active);

    /// \~chinese
    /// @brief ��ȡ������̨
    Stage* GetStage() const;

    /// \~chinese
    /// @brief ������̨
    void EnterStage(Stage* stage);

    /// \~chinese
    /// @brief ����̨���Ƴ� TODO ��һ֡�Ƴ�
    void RemoveFromStage();

    /// \~chinese
    /// @brief ��ȡ��ά�任
    const Transform& GetTransform() const;

    /// \~chinese
    /// @brief ��ȡ��ά�任
    Transform& GetTransform();

    /// \~chinese
    /// @brief ���ö�ά�任
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief ���ö�ά�任������һ��ɫ
    void FollowTransform(RefPtr<Actor> parent);

    /// \~chinese
    /// @brief ��ȡ���е�����
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    AbilityT* Get() const;

    /// \~chinese
    /// @brief ��ȡ���е�������TransformAbility ƫ�ػ���
    template <>
    inline TransformAbility* Get<TransformAbility>() const
    {
        return transform_ability_.Get();
    }

    /// \~chinese
    /// @brief ��ȡ���е�����
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    Vector<AbilityT*> GetAll() const;

    /// \~chinese
    /// @brief �������
    void Attach(RefPtr<Ability> ability);

    /// \~chinese
    /// @brief �Ƴ�����
    /// @param ability ���Ƴ�������
    /// @param immediate �Ƿ�ֱ���Ƴ���������ʹ�ó����˽����ĺ���
    void Detach(RefPtr<Ability> ability, bool immediate = false);

    /// \~chinese
    /// @brief �Ƴ�����
    template <class AbilityT, class = typename std::enable_if<std::is_base_of<Ability, AbilityT>::value>::type>
    void DetachAll();

    /// \~chinese
    /// @brief ��ȡ���е�����
    const AbilityList& GetAbilityList() const;

    /// \~chinese
    /// @brief ���л�
    void OnSerialize(Serializer* serializer) const override;

    /// \~chinese
    /// @brief �����л�
    void OnDeserialize(Deserializer* deserializer) override;

    /// \~chinese
    /// @brief ���½�ɫ������
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
