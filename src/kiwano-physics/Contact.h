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
#include <kiwano-physics/Global.h>

namespace kiwano
{
namespace physics
{

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief 物理接触开始事件
class KGE_API ContactBeginEvent : public Event
{
public:
    b2Contact* contact;  ///< 产生的接触

    ContactBeginEvent()
        : ContactBeginEvent(nullptr)
    {
    }

    ContactBeginEvent(b2Contact* contact)
        : Event(KGE_EVENT(ContactBeginEvent))
        , contact(contact)
    {
    }
};

/// \~chinese
/// @brief 物理接触结束事件
class KGE_API ContactEndEvent : public Event
{
public:
    b2Contact* contact;  ///< 产生的接触

    ContactEndEvent()
        : ContactEndEvent(nullptr)
    {
    }

    ContactEndEvent(b2Contact* contact)
        : Event(KGE_EVENT(ContactEndEvent))
        , contact(contact)
    {
    }
};

/// \~chinese
/// @brief 物理接触列表
class ContactList
{
    template <typename _Ty>
    class IteratorImpl
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = _Ty;
        using pointer           = _Ty*;
        using reference         = _Ty&;
        using difference_type   = ptrdiff_t;

        IteratorImpl(const _Ty& elem)
            : elem_(elem)
        {
        }

        inline reference operator*() const
        {
            return const_cast<reference>(elem_);
        }

        inline pointer operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        inline IteratorImpl& operator++()
        {
            elem_ = elem_->GetNext();
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
    using value_type     = b2Contact*;
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
    value_type first_ = nullptr;
};

/// \~chinese
/// @brief 物理接触边列表
class ContactEdgeList
{
    template <typename _Ty>
    class IteratorImpl
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = _Ty;
        using pointer           = _Ty*;
        using reference         = _Ty&;
        using difference_type   = ptrdiff_t;

        inline IteratorImpl(const _Ty& elem)
            : elem_(elem)
        {
        }

        inline reference operator*() const
        {
            return const_cast<reference>(elem_);
        }

        inline pointer operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        inline IteratorImpl& operator++()
        {
            elem_ = elem_->next;
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
    using value_type     = b2ContactEdge*;
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

}  // namespace physics
}  // namespace kiwano
