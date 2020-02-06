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
#include <kiwano-physics/Contact.h>

namespace kiwano
{
namespace physics
{
/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief 接触边
class KGE_API ContactEdge
{
public:
    ContactEdge();

    ContactEdge(b2ContactEdge* edge);

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 获取接触物体
    Body* GetOtherBody() const;

    /// \~chinese
    /// @brief 获取接触
    Contact GetContact() const;

    b2ContactEdge* GetB2ContactEdge() const;

    void SetB2ContactEdge(b2ContactEdge* edge);

    bool operator==(const ContactEdge& rhs) const;
    bool operator!=(const ContactEdge& rhs) const;

private:
    b2ContactEdge* edge_;
};

/// \~chinese
/// @brief 物理接触边列表
class ContactEdgeList
{
    template <typename _Ty>
    class IteratorImpl : public std::iterator<std::forward_iterator_tag, _Ty>
    {
        using herit = std::iterator<std::forward_iterator_tag, _Ty>;

    public:
        inline IteratorImpl(const _Ty& elem)
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
            elem_ = elem_.GetB2ContactEdge()->next;
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
    using value_type     = ContactEdge;
    using iterator       = IteratorImpl<value_type>;
    using const_iterator = IteratorImpl<const value_type>;

    inline ContactEdgeList() {}

    inline ContactEdgeList(const value_type& first)
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
        return iterator(nullptr);
    }

    inline const_iterator end() const
    {
        return cend();
    }

    inline const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

private:
    value_type first_;
};

/** @} */

inline bool ContactEdge::IsValid() const
{
    return edge_ != nullptr;
}

inline Body* ContactEdge::GetOtherBody() const
{
    KGE_ASSERT(edge_);
    return static_cast<Body*>(edge_->other->GetUserData());
}

inline Contact ContactEdge::GetContact() const
{
    KGE_ASSERT(edge_);

    Contact contact;
    contact.SetB2Contact(edge_->contact);
    return contact;
}

inline b2ContactEdge* ContactEdge::GetB2ContactEdge() const
{
    return edge_;
}

inline void ContactEdge::SetB2ContactEdge(b2ContactEdge* edge)
{
    edge_ = edge;
}

inline bool ContactEdge::operator==(const ContactEdge& rhs) const
{
    return edge_ == rhs.edge_;
}

inline bool ContactEdge::operator!=(const ContactEdge& rhs) const
{
    return edge_ != rhs.edge_;
}

}  // namespace physics
}  // namespace kiwano
