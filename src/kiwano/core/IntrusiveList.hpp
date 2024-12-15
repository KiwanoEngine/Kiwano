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
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <kiwano/macros.h>

namespace kiwano
{

/// \~chinese
/// @brief 侵入式链表
template <typename _PtrTy>
class IntrusiveList
{
public:
    using value_type = typename std::pointer_traits<_PtrTy>::pointer;
    using pointer    = value_type*;
    using reference  = value_type&;

    IntrusiveList()
        : first_()
        , last_()
    {
    }

    ~IntrusiveList()
    {
        Clear();
    }

    /// \~chinese
    /// @brief 获取首元素
    const value_type& GetFirst() const
    {
        return first_;
    }

    /// \~chinese
    /// @brief 获取首元素
    value_type& GetFirst()
    {
        return first_;
    }

    /// \~chinese
    /// @brief 获取尾元素
    const value_type& GetLast() const
    {
        return last_;
    }

    /// \~chinese
    /// @brief 获取尾元素
    value_type& GetLast()
    {
        return last_;
    }

    /// \~chinese
    /// @brief 链表是否为空
    inline bool IsEmpty() const
    {
        return first_ == nullptr;
    }

    /// \~chinese
    /// @brief 在链表尾部添加对象
    void PushBack(reference child)
    {
        if (child->GetPrev())
            child->GetPrev()->GetNext() = child->GetNext();
        if (child->GetNext())
            child->GetNext()->GetPrev() = child->GetPrev();

        child->GetPrev() = last_;
        child->GetNext() = nullptr;

        if (first_)
        {
            last_->GetNext() = child;
        }
        else
        {
            first_ = child;
        }

        last_ = child;
    }

    /// \~chinese
    /// @brief 在链表头部添加对象
    void PushFront(reference child)
    {
        if (child->GetPrev())
            child->GetPrev()->GetNext() = child->GetNext();
        if (child->GetNext())
            child->GetNext()->GetPrev() = child->GetPrev();

        child->GetPrev() = nullptr;
        child->GetNext() = first_;

        if (first_)
        {
            first_->GetPrev() = child;
        }
        else
        {
            last_ = child;
        }

        first_ = child;
    }

    /// \~chinese
    /// @brief 在链表的对象前插入新对象
    void InsertBefore(reference child, reference before)
    {
        if (child->GetPrev())
            child->GetPrev()->GetNext() = child->GetNext();
        if (child->GetNext())
            child->GetNext()->GetPrev() = child->GetPrev();

        if (before->GetPrev())
            before->GetPrev()->GetNext() = child;
        else
            first_ = child;

        child->GetPrev()  = before->GetPrev();
        child->GetNext()  = before;
        before->GetPrev() = child;
    }

    /// \~chinese
    /// @brief 在链表的对象后插入新对象
    void InsertAfter(reference child, reference after)
    {
        if (child->GetPrev())
            child->GetPrev()->GetNext() = child->GetNext();
        if (child->GetNext())
            child->GetNext()->GetPrev() = child->GetPrev();

        if (after->GetNext())
            after->GetNext()->GetPrev() = child;
        else
            last_ = child;

        child->GetNext() = after->GetNext();
        child->GetPrev() = after;
        after->GetNext() = child;
    }

    /// \~chinese
    /// @brief 移除对象
    void Remove(reference child)
    {
        if (child->GetNext())
        {
            child->GetNext()->GetPrev() = child->GetPrev();
        }
        else
        {
            last_ = child->GetPrev();
        }

        if (child->GetPrev())
        {
            child->GetPrev()->GetNext() = child->GetNext();
        }
        else
        {
            first_ = child->GetNext();
        }

        child->GetPrev() = nullptr;
        child->GetNext() = nullptr;
    }

    /// \~chinese
    /// @brief 清空所有对象
    void Clear()
    {
        value_type p = first_;
        while (p)
        {
            value_type tmp = p;
            p              = p->GetNext();
            if (tmp)
            {
                tmp->GetNext() = nullptr;
                tmp->GetPrev() = nullptr;
            }
        }
        first_ = nullptr;
        last_  = nullptr;
    }

    /// \~chinese
    /// @brief 检查链表是否有效
    bool CheckValid()
    {
        if (!first_)
            return true;

        int pos = 0;

        value_type p   = first_;
        value_type tmp = p;
        do
        {
            tmp = p;
            p   = p->GetNext();
            ++pos;

            if (p)
            {
                if (p->GetPrev() != tmp)
                    return false;
            }
            else
            {
                if (tmp != last_)
                    return false;
            }
        } while (p);
        return true;
    }

public:
    template <typename _IterPtrTy>
    struct Iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = _IterPtrTy;
        using pointer           = _IterPtrTy*;
        using reference         = _IterPtrTy&;
        using difference_type   = ptrdiff_t;

        inline Iterator(value_type ptr = nullptr, bool is_end = false)
            : base_(ptr)
            , is_end_(is_end)
        {
        }

        inline reference operator*() const
        {
            KGE_ASSERT(base_ && !is_end_);
            return const_cast<reference>(base_);
        }

        inline pointer operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        inline Iterator& operator++()
        {
            KGE_ASSERT(base_ && !is_end_);
            value_type next = base_->GetNext();
            if (next)
                base_ = next;
            else
                is_end_ = true;
            return (*this);
        }

        inline Iterator operator++(int)
        {
            Iterator old = (*this);
            ++(*this);
            return old;
        }

        inline Iterator& operator--()
        {
            KGE_ASSERT(base_);
            if (is_end_)
                is_end_ = false;
            else
                base_ = base_->GetPrev();
            return (*this);
        }

        inline Iterator operator--(int)
        {
            Iterator old = (*this);
            --(*this);
            return old;
        }

        inline bool operator==(const Iterator& other) const
        {
            return base_ == other.base_ && is_end_ == other.is_end_;
        }

        inline bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

        inline operator bool() const
        {
            return base_ != nullptr && !is_end_;
        }

    private:
        bool is_end_;

        typename std::remove_const<value_type>::type base_;
    };

public:
    using iterator               = Iterator<value_type>;
    using const_iterator         = Iterator<const value_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    inline iterator begin()
    {
        return iterator(first_, first_ == nullptr);
    }

    inline const_iterator begin() const
    {
        return const_iterator(first_, first_ == nullptr);
    }

    inline const_iterator cbegin() const
    {
        return begin();
    }

    inline iterator end()
    {
        return iterator(last_, true);
    }

    inline const_iterator end() const
    {
        return const_iterator(last_, true);
    }

    inline const_iterator cend() const
    {
        return end();
    }

    inline reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    inline const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    inline const_reverse_iterator crbegin() const
    {
        return rbegin();
    }

    inline reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    inline const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    inline const_reverse_iterator crend() const
    {
        return rend();
    }

    inline value_type& front()
    {
        if (IsEmpty())
            throw std::out_of_range("front() called on empty list");
        return first_;
    }

    inline const value_type& front() const
    {
        if (IsEmpty())
            throw std::out_of_range("front() called on empty list");
        return first_;
    }

    inline value_type& back()
    {
        if (IsEmpty())
            throw std::out_of_range("back() called on empty list");
        return last_;
    }

    inline const value_type& back() const
    {
        if (IsEmpty())
            throw std::out_of_range("back() called on empty list");
        return last_;
    }

private:
    value_type first_;
    value_type last_;
};

/// \~chinese
/// @brief 侵入式链表元素
template <typename _PtrTy>
class IntrusiveListValue
{
public:
    using value_type = typename std::pointer_traits<_PtrTy>::pointer;
    using reference  = value_type&;
    using pointer    = value_type*;

    IntrusiveListValue()
        : prev_(nullptr)
        , next_(nullptr)
    {
    }

    IntrusiveListValue(value_type rhs)
        : prev_(nullptr)
        , next_(nullptr)
    {
        if (rhs)
        {
            prev_ = rhs->GetPrev();
            next_ = rhs->GetNext();
        }
    }

    /// \~chinese
    /// @brief 获取前一元素
    const value_type& GetPrev() const
    {
        return prev_;
    }

    /// \~chinese
    /// @brief 获取前一元素
    value_type& GetPrev()
    {
        return prev_;
    }

    /// \~chinese
    /// @brief 获取下一元素
    const value_type& GetNext() const
    {
        return next_;
    }

    /// \~chinese
    /// @brief 获取下一元素
    value_type& GetNext()
    {
        return next_;
    }

private:
    value_type prev_;
    value_type next_;

    friend class IntrusiveList<_PtrTy>;
};

}  // namespace kiwano
