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
    using value_type = typename std::pointer_traits<_PtrTy>::element_type;
    using pointer    = typename std::pointer_traits<_PtrTy>::pointer;
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
    const pointer GetFirst() const
    {
        return first_;
    }

    /// \~chinese
    /// @brief 获取首元素
    pointer GetFirst()
    {
        return first_;
    }

    /// \~chinese
    /// @brief 获取尾元素
    const pointer GetLast() const
    {
        return last_;
    }

    /// \~chinese
    /// @brief 获取尾元素
    pointer GetLast()
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
    void PushBack(pointer child)
    {
        if (child->prev_)
            child->prev_->next_ = child->next_;
        if (child->next_)
            child->next_->prev_ = child->prev_;

        child->prev_ = last_;
        child->next_ = nullptr;

        if (first_)
        {
            last_->next_ = child;
        }
        else
        {
            first_ = child;
        }

        last_ = child;
    }

    /// \~chinese
    /// @brief 在链表头部添加对象
    void PushFront(pointer child)
    {
        if (child->prev_)
            child->prev_->next_ = child->next_;
        if (child->next_)
            child->next_->prev_ = child->prev_;

        child->prev_ = nullptr;
        child->next_ = first_;

        if (first_)
        {
            first_->prev_ = child;
        }
        else
        {
            last_ = child;
        }

        first_ = child;
    }

    /// \~chinese
    /// @brief 在链表的对象前插入新对象
    void InsertBefore(pointer child, pointer before)
    {
        if (child->prev_)
            child->prev_->next_ = child->next_;
        if (child->next_)
            child->next_->prev_ = child->prev_;

        if (before->prev_)
            before->prev_->next_ = child;
        else
            first_ = child;

        child->prev_ = before->prev_;
        child->next_ = before;
        before->prev_ = child;
    }

    /// \~chinese
    /// @brief 在链表的对象后插入新对象
    void InsertAfter(pointer child, pointer after)
    {
        if (child->prev_)
            child->prev_->next_ = child->next_;
        if (child->next_)
            child->next_->prev_ = child->prev_;

        if (after->next_)
            after->next_->prev_ = child;
        else
            last_ = child;

        child->next_ = after->next_;
        child->prev_ = after;
        after->next_ = child;
    }

    /// \~chinese
    /// @brief 移除对象
    void Remove(pointer child)
    {
        if (child->next_)
        {
            child->next_->prev_ = child->prev_;
        }
        else
        {
            last_ = child->prev_;
        }

        if (child->prev_)
        {
            child->prev_->next_ = child->next_;
        }
        else
        {
            first_ = child->next_;
        }

        child->prev_ = nullptr;
        child->next_ = nullptr;
    }

    /// \~chinese
    /// @brief 清空所有对象
    void Clear()
    {
        pointer p = first_;
        while (p)
        {
            pointer tmp = p;
            p = p->next_;
            if (tmp)
            {
                tmp->next_ = nullptr;
                tmp->prev_ = nullptr;
            }
        }
        first_ = nullptr;
        last_ = nullptr;
    }

    /// \~chinese
    /// @brief 检查链表是否有效
    bool CheckValid()
    {
        if (!first_)
            return true;

        int pos = 0;
        pointer p = first_;
        pointer tmp = p;
        do
        {
            tmp = p;
            p = p->next_;
            ++pos;

            if (p)
            {
                if (p->prev_ != tmp)
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
    template <typename _PTy>
    struct Iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using pointer           = _PTy;
        using reference         = typename IntrusiveList::reference;
        using difference_type   = ptrdiff_t;

        inline Iterator(pointer ptr = nullptr, bool is_end = false)
            : base_(ptr)
            , is_end_(is_end)
        {
        }

        inline pointer Get() const
        {
            KGE_ASSERT(!is_end_);
            return const_cast<pointer&>(base_);
        }

        inline reference operator*() const
        {
            KGE_ASSERT(base_ && !is_end_);
            return const_cast<reference>(*base_);
        }

        inline pointer operator->() const
        {
            KGE_ASSERT(base_ && !is_end_);
            return const_cast<pointer&>(base_);
        }

        inline Iterator& operator++()
        {
            KGE_ASSERT(base_ && !is_end_);
            pointer next = base_->GetNext();
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

        inline bool operator==(Iterator const& other) const
        {
            return base_ == other.base_ && is_end_ == other.is_end_;
        }

        inline bool operator!=(Iterator const& other) const
        {
            return !(*this == other);
        }

        inline operator bool() const
        {
            return base_ != nullptr && !is_end_;
        }

    private:
        bool is_end_;

        typename IntrusiveList::pointer base_;
    };

public:
    using iterator               = Iterator<pointer>;
    using const_iterator         = Iterator<const pointer>;
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

    inline pointer front()
    {
        if (IsEmpty())
            throw std::out_of_range("front() called on empty list");
        return first_;
    }

    inline const pointer front() const
    {
        if (IsEmpty())
            throw std::out_of_range("front() called on empty list");
        return first_;
    }

    inline pointer back()
    {
        if (IsEmpty())
            throw std::out_of_range("back() called on empty list");
        return last_;
    }

    inline const pointer back() const
    {
        if (IsEmpty())
            throw std::out_of_range("back() called on empty list");
        return last_;
    }

private:
    pointer first_;
    pointer last_;
};


/// \~chinese
/// @brief 侵入式链表元素
template <typename _PtrTy>
class IntrusiveListValue
{
public:
    using pointer = typename std::pointer_traits<_PtrTy>::pointer;

    IntrusiveListValue()
        : prev_(nullptr)
        , next_(nullptr)
    {
    }

    IntrusiveListValue(pointer rhs)
        : prev_(nullptr)
        , next_(nullptr)
    {
        if (rhs)
        {
            prev_ = rhs->prev_;
            next_ = rhs->next_;
        }
    }

    /// \~chinese
    /// @brief 获取前一元素
    const pointer GetPrev() const
    {
        return prev_;
    }

    /// \~chinese
    /// @brief 获取前一元素
    pointer GetPrev()
    {
        return prev_;
    }

    /// \~chinese
    /// @brief 获取下一元素
    const pointer GetNext() const
    {
        return next_;
    }

    /// \~chinese
    /// @brief 获取下一元素
    pointer GetNext()
    {
        return next_;
    }

private:
    pointer prev_;
    pointer next_;

    friend class IntrusiveList<_PtrTy>;
};

}  // namespace kiwano
