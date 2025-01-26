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
 * \defgroup Ability ��ɫ����
 */

/**
 * \addtogroup Ability
 * @{
 */

/**
 * \~chinese
 * @brief ��ɫ����
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
    /// @brief �Ƿ�����
    bool IsEnabled() const;

    /// \~chinese
    /// @brief �������û����
    void SetEnabled(bool enabled);

    /// \~chinese
    /// @brief ��ȡ���ӵĽ�ɫ
    Actor* GetAttachedActor() const;

    /// \~chinese
    /// @brief �ӽ�ɫ�з���
    /// @param immediate �Ƿ�ֱ���Ƴ���������ʹ�ó����˽����ĺ���
    void Detach(bool immediate = false);

    /// \~chinese
    /// @brief ��ȡȨ��
    int16_t GetPriority() const;

    /// \~chinese
    /// @brief �������ȼ�
    /// @details ���ȼ�Խ��ִ��˳��Խ��ǰ��Ĭ��Ϊ 0
    void SetPriority(int16_t priority);

    /// \~chinese
    /// @brief ������ʼ��
    /// @details ������ Ability::OnInit ��������ʱ����ǿ������˳��
    void Initialize();

protected:
    Ability();

    /// \~chinese
    /// @brief �� Ability �� Actor ����ʱ����
    virtual void OnAttached(Actor* actor);

    /// \~chinese
    /// @brief �� Ability �� Actor ��������
    /// @details һ����֡ĩβ���ã�������ʱָ���� immediate�������̵���
    virtual void OnDetached();

    /// \~chinese
    /// @brief ��ʼ�� Ability
    /// @details ���ӵ� Actor ����һ֡��ʼǰ����
    virtual void OnInit();

    /// \~chinese
    /// @brief ÿ֡����ʱ����
    virtual void OnUpdate(Duration dt);

private:
    bool    enabled_;
    bool    initialized_;
    int16_t priority_;
    Actor*  actor_;
};

/// \~chinese
/// @brief ��ɫ�����б�
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
