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
#include <utility>
#include <type_traits>
#include <kiwano/core/Common.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 引用计数智能指针
 */
template <typename _Ty, typename _RefPolicy>
class RefBasePtr : protected _RefPolicy
{
public:
    using value_type           = _Ty;
    using pointer_type         = _Ty*;
    using const_pointer_type   = const _Ty*;
    using reference_type       = _Ty&;
    using const_reference_type = const _Ty&;

    RefBasePtr() noexcept
        : ptr_(nullptr)
    {
    }

    RefBasePtr(std::nullptr_t) noexcept
        : ptr_(nullptr)
    {
    }

    RefBasePtr(pointer_type p)
        : ptr_(p)
    {
        _RefPolicy::Retain(ptr_);
    }

    RefBasePtr(const RefBasePtr& other)
        : ptr_(other.ptr_)
    {
        _RefPolicy::Retain(ptr_);
    }

    RefBasePtr(RefBasePtr&& other) noexcept
        : ptr_(nullptr)
    {
        Swap(other);
    }

    ~RefBasePtr()
    {
        Tidy();
    }

    template <typename _UTy, typename std::enable_if<std::is_convertible<_UTy*, _Ty*>::value, int>::type = 0>
    RefBasePtr(const RefBasePtr<_UTy, _RefPolicy>& other)
    {
        ptr_ = dynamic_cast<pointer_type>(other.Get());
        _RefPolicy::Retain(ptr_);
    }

    inline pointer_type Get() const noexcept
    {
        return ptr_;
    }

    inline pointer_type* GetAddressOfAndRelease()
    {
        Tidy();
        return &ptr_;
    }

    inline void Reset(pointer_type ptr = nullptr)
    {
        if (ptr)
            RefBasePtr(ptr).Swap(*this);
        else
            Tidy();
    }

    inline void Swap(RefBasePtr& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }

    inline pointer_type operator->()
    {
        return ptr_;
    }

    inline const_pointer_type operator->() const
    {
        return ptr_;
    }

    inline reference_type operator*()
    {
        return *ptr_;
    }

    inline const_reference_type operator*() const
    {
        return *ptr_;
    }

    inline pointer_type* operator&()
    {
        return this->GetAddressOfAndRelease();
    }

    inline operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }

    inline bool operator!() const noexcept
    {
        return ptr_ == 0;
    }

    inline RefBasePtr& operator=(const RefBasePtr& other)
    {
        if (other.ptr_ != ptr_)
            RefBasePtr(other).Swap(*this);
        return (*this);
    }

    inline RefBasePtr& operator=(RefBasePtr&& other) noexcept
    {
        if (other.ptr_ != ptr_)
            other.Swap(*this);
        return (*this);
    }

    inline RefBasePtr& operator=(pointer_type p)
    {
        if (p != ptr_)
            RefBasePtr(p).Swap(*this);
        return (*this);
    }

    template <typename _UTy, typename std::enable_if<std::is_convertible<_UTy*, _Ty*>::value, int>::type = 0>
    inline RefBasePtr& operator=(const RefBasePtr<_UTy, _RefPolicy>& other)
    {
        if (other.Get() != ptr_)
            RefBasePtr(dynamic_cast<pointer_type>(other.Get())).Swap(*this);
        return (*this);
    }

    inline RefBasePtr& operator=(std::nullptr_t)
    {
        Tidy();
        return *this;
    }

private:
    void Tidy()
    {
        _RefPolicy::Release(ptr_);
        ptr_ = nullptr;
    }

private:
    pointer_type ptr_;
};

template <class _Ty, class _UTy, class _RefPolicy>
inline bool operator==(const RefBasePtr<_Ty, _RefPolicy>& lhs, const RefBasePtr<_UTy, _RefPolicy>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <class _Ty, class _RefPolicy>
inline bool operator==(const RefBasePtr<_Ty, _RefPolicy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class _Ty, class _RefPolicy>
inline bool operator==(_Ty* lhs, const RefBasePtr<_Ty, _RefPolicy>& rhs) noexcept
{
    return lhs == rhs.Get();
}

template <class _Ty, class _RefPolicy>
inline bool operator==(const RefBasePtr<_Ty, _RefPolicy>& lhs, std::nullptr_t) noexcept
{
    return !static_cast<bool>(lhs);
}

template <class _Ty, class _RefPolicy>
inline bool operator==(std::nullptr_t, const RefBasePtr<_Ty, _RefPolicy>& rhs) noexcept
{
    return !static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _RefPolicy>
inline bool operator!=(const RefBasePtr<_Ty, _RefPolicy>& lhs, const RefBasePtr<_UTy, _RefPolicy>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <class _Ty, class _RefPolicy>
inline bool operator!=(const RefBasePtr<_Ty, _RefPolicy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class _Ty, class _RefPolicy>
inline bool operator!=(_Ty* lhs, const RefBasePtr<_Ty, _RefPolicy>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <class _Ty, class _RefPolicy>
inline bool operator!=(const RefBasePtr<_Ty, _RefPolicy>& lhs, std::nullptr_t) noexcept
{
    return static_cast<bool>(lhs);
}

template <class _Ty, class _RefPolicy>
inline bool operator!=(std::nullptr_t, const RefBasePtr<_Ty, _RefPolicy>& rhs) noexcept
{
    return static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _RefPolicy>
inline bool operator<(const RefBasePtr<_Ty, _RefPolicy>& lhs, const RefBasePtr<_UTy, _RefPolicy>& rhs) noexcept
{
    return lhs.Get() < rhs.Get();
}

// template class cannot specialize std::swap,
// so implement a swap function in kiwano namespace
template <class _Ty, class _RefPolicy>
inline void swap(RefBasePtr<_Ty, _RefPolicy>& lhs, RefBasePtr<_Ty, _RefPolicy>& rhs) noexcept
{
    lhs.Swap(rhs);
}

}  // namespace kiwano
