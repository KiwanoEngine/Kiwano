// Copyright (c) 2018-2019 Kiwano - Nomango
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
#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/helper.h>

namespace kiwano
{
namespace physics
{
class Body;

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief ����Ӵ�
class KGE_API Contact
{
public:
    Contact();

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief �Ƿ��ǽӴ�
    bool IsTouching() const;

    /// \~chinese
    /// @brief ���û���� (��������һ��ʱ�䲽)
    void SetEnabled(bool flag);

    /// \~chinese
    /// @brief �Ƿ�����
    bool IsEnabled() const;

    /// \~chinese
    /// @brief ��ȡ����A�ļо�
    Fixture* GetFixtureA() const;

    /// \~chinese
    /// @brief ��ȡ����B�ļо�
    Fixture* GetFixtureB() const;

    /// \~chinese
    /// @brief ��ȡ����A
    Body* GetBodyA() const;

    /// \~chinese
    /// @brief ��ȡ����B
    Body* GetBodyB() const;

    /// \~chinese
    /// @brief ����Ħ����
    void SetFriction(float friction);

    /// \~chinese
    /// @brief ��ȡĦ����
    float GetFriction() const;

    /// \~chinese
    /// @brief ����Ħ����
    void ResetFriction();

    /// \~chinese
    /// @brief ���õ��Իָ�
    void SetRestitution(float restitution);

    /// \~chinese
    /// @brief ��ȡ���Իָ�
    float GetRestitution() const;

    /// \~chinese
    /// @brief ���õ��Իָ�
    void ResetRestitution();

    /// \~chinese
    /// @brief ���������ٶ�
    void SetTangentSpeed(float speed);

    /// \~chinese
    /// @brief ��ȡ�����ٶ�
    float GetTangentSpeed() const;

    b2Contact* GetB2Contact() const;

    void SetB2Contact(b2Contact* contact);

    bool operator==(const Contact& rhs) const;
    bool operator!=(const Contact& rhs) const;

private:
    b2Contact* contact_;
};

/// \~chinese
/// @brief ����Ӵ��б�
class ContactList
{
    template <typename _Ty>
    class IteratorImpl : public std::iterator<std::forward_iterator_tag, _Ty>
    {
        using herit = std::iterator<std::forward_iterator_tag, _Ty>;

    public:
        IteratorImpl(const _Ty& elem)
            : elem_(elem)
        {
        }

        inline typename herit::reference operator*() const
        {
            return const_cast<typename herit::reference>(elem_);
        }

        inline typename herit::pointer operator->() const
        {
            return std::pointer_traits<typename herit::pointer>::pointer_to(**this);
        }

        inline IteratorImpl& operator++()
        {
            elem_ = elem_.GetB2Contact()->GetNext();
            return *this;
        }

        inline IteratorImpl operator++(int)
        {
            IteratorImpl old = *this;
                         operator++();
            return old;
        }

        inline bool operator==(const IteratorImpl& rhs) const
        {
            return elem_ == rhs.elem_;
        }

        inline bool operator!=(const IteratorImpl& rhs) const
        {
            return !operator==(rhs);
        }

    private:
        _Ty elem_;
    };

public:
    using value_type     = Contact;
    using iterator       = IteratorImpl<value_type>;
    using const_iterator = IteratorImpl<const value_type>;

    inline ContactList() {}

    inline ContactList(const value_type& first)
        : first_(first)
    {
    }

    inline const value_type& front() const
    {
        return first_;
    }

    inline value_type& front()
    {
        return first_;
    }

    inline iterator begin()
    {
        return iterator(first_);
    }

    inline const_iterator begin() const
    {
        return cbegin();
    }

    inline const_iterator cbegin() const
    {
        return const_iterator(first_);
    }

    inline iterator end()
    {
        return iterator(Contact());
    }

    inline const_iterator end() const
    {
        return cend();
    }

    inline const_iterator cend() const
    {
        return const_iterator(Contact());
    }

private:
    value_type first_;
};

/** @} */

inline bool Contact::IsValid() const
{
    return contact_ != nullptr;
}
inline bool Contact::IsTouching() const
{
    KGE_ASSERT(contact_);
    return contact_->IsTouching();
}
inline void Contact::SetEnabled(bool flag)
{
    KGE_ASSERT(contact_);
    contact_->SetEnabled(flag);
}
inline bool Contact::IsEnabled() const
{
    KGE_ASSERT(contact_);
    return contact_->IsEnabled();
}
inline void Contact::SetFriction(float friction)
{
    KGE_ASSERT(contact_);
    contact_->SetFriction(friction);
}
inline float Contact::GetFriction() const
{
    KGE_ASSERT(contact_);
    return contact_->GetFriction();
}
inline void Contact::ResetFriction()
{
    KGE_ASSERT(contact_);
    contact_->ResetFriction();
}
inline void Contact::SetRestitution(float restitution)
{
    KGE_ASSERT(contact_);
    contact_->SetRestitution(restitution);
}
inline float Contact::GetRestitution() const
{
    KGE_ASSERT(contact_);
    return contact_->GetRestitution();
}
inline void Contact::ResetRestitution()
{
    KGE_ASSERT(contact_);
    contact_->ResetRestitution();
}
inline b2Contact* Contact::GetB2Contact() const
{
    return contact_;
}
inline void Contact::SetB2Contact(b2Contact* contact)
{
    contact_ = contact;
}
inline bool Contact::operator==(const Contact& rhs) const
{
    return contact_ == rhs.contact_;
}
inline bool Contact::operator!=(const Contact& rhs) const
{
    return contact_ != rhs.contact_;
}

}  // namespace physics
}  // namespace kiwano
