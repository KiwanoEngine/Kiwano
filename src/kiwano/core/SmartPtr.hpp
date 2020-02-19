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
#include <kiwano/core/RefCounter.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 默认的智能指针引用计数代理
 */
struct DefaultSmartPtrRefProxy
{
    static inline void Retain(RefCounter* ptr)
    {
        if (ptr)
            ptr->Retain();
    }

    static inline void Release(RefCounter* ptr)
    {
        if (ptr)
            ptr->Release();
    }
};

/**
 * \~chinese
 * @brief 侵入式智能指针
 */
template <typename _Ty, typename _ProxyTy = DefaultSmartPtrRefProxy>
class SmartPtr
{
public:
    using value_type           = _Ty;
    using pointer_type         = _Ty*;
    using const_pointer_type   = const _Ty*;
    using reference_type       = _Ty&;
    using const_reference_type = const _Ty&;

    SmartPtr() noexcept
        : ptr_(nullptr)
    {
    }

    SmartPtr(std::nullptr_t) noexcept
        : ptr_(nullptr)
    {
    }

    SmartPtr(pointer_type p)
        : ptr_(p)
    {
        typename _ProxyTy::Retain(ptr_);
    }

    SmartPtr(const SmartPtr& other)
        : ptr_(other.ptr_)
    {
        typename _ProxyTy::Retain(ptr_);
    }

    SmartPtr(SmartPtr&& other) noexcept
        : ptr_(nullptr)
    {
        Swap(other);
    }

    ~SmartPtr()
    {
        Tidy();
    }

    template <typename _UTy, typename std::enable_if<std::is_base_of<_Ty, _UTy>::value, int>::type = 0>
    SmartPtr(const SmartPtr<_UTy, _ProxyTy>& other)
    {
        ptr_ = const_cast<pointer_type>(dynamic_cast<const_pointer_type>(other.Get()));
        typename _ProxyTy::Retain(ptr_);
    }

    inline pointer_type Get() noexcept
    {
        return ptr_;
    }

    inline const_pointer_type Get() const noexcept
    {
        return ptr_;
    }

    inline void Reset(pointer_type ptr = nullptr)
    {
        if (ptr)
            SmartPtr(ptr).Swap(*this);
        else
            Tidy();
    }

    inline void Swap(SmartPtr& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }

    inline pointer_type operator->()
    {
        KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
        return ptr_;
    }

    inline const_pointer_type operator->() const
    {
        KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
        return ptr_;
    }

    inline reference_type operator*()
    {
        KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
        return *ptr_;
    }

    inline const_reference_type operator*() const
    {
        KGE_ASSERT(ptr_ != nullptr && "Invalid pointer");
        return *ptr_;
    }

    inline pointer_type* operator&()
    {
        KGE_ASSERT(ptr_ == nullptr && "Memory leak");
        return &ptr_;
    }

    inline operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }

    inline bool operator!() const noexcept
    {
        return ptr_ == 0;
    }

    inline SmartPtr& operator=(const SmartPtr& other)
    {
        if (other.ptr_ != ptr_)
            SmartPtr(other).Swap(*this);
        return (*this);
    }

    inline SmartPtr& operator=(SmartPtr&& other) noexcept
    {
        if (other.ptr_ != ptr_)
            other.Swap(*this);
        return (*this);
    }

    inline SmartPtr& operator=(pointer_type p)
    {
        if (p != ptr_)
            SmartPtr(p).Swap(*this);
        return (*this);
    }

    inline SmartPtr& operator=(std::nullptr_t)
    {
        Tidy();
        return *this;
    }

private:
    void Tidy()
    {
        typename _ProxyTy::Release(ptr_);
        ptr_ = nullptr;
    }

private:
    pointer_type ptr_;
};

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator==(const SmartPtr<_Ty, _ProxyTy>& lhs, const SmartPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(const SmartPtr<_Ty, _ProxyTy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator==(_Ty* lhs, const SmartPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return lhs == rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(const SmartPtr<_Ty, _ProxyTy>& lhs, std::nullptr_t) noexcept
{
    return !static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator==(std::nullptr_t, const SmartPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return !static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator!=(const SmartPtr<_Ty, _ProxyTy>& lhs, const SmartPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(const SmartPtr<_Ty, _ProxyTy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(_Ty* lhs, const SmartPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(const SmartPtr<_Ty, _ProxyTy>& lhs, std::nullptr_t) noexcept
{
    return static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(std::nullptr_t, const SmartPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator<(const SmartPtr<_Ty, _ProxyTy>& lhs, const SmartPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return lhs.Get() < rhs.Get();
}

// template class cannot specialize std::swap,
// so implement a Swap Function in kiwano namespace
template <class _Ty, class _ProxyTy>
inline void swap(SmartPtr<_Ty, _ProxyTy>& lhs, SmartPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    lhs.Swap(rhs);
}

}  // namespace kiwano

#ifndef KGE_DECLARE_SMART_PTR
#define KGE_DECLARE_SMART_PTR(CLASS) \
    class CLASS;                     \
    typedef ::kiwano::SmartPtr<CLASS> CLASS##Ptr;
#endif
