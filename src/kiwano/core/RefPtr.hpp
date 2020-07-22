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
#include <kiwano/base/RefObject.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 默认的智能指针引用计数代理
 */
struct DefaultRefPtrRefProxy
{
    static inline void Retain(RefObject* ptr)
    {
        if (ptr)
            ptr->Retain();
    }

    static inline void Release(RefObject* ptr)
    {
        if (ptr)
            ptr->Release();
    }
};

/**
 * \~chinese
 * @brief 引用计数智能指针
 */
template <typename _Ty, typename _ProxyTy = DefaultRefPtrRefProxy>
class RefPtr
{
public:
    using value_type           = _Ty;
    using pointer_type         = _Ty*;
    using const_pointer_type   = const _Ty*;
    using reference_type       = _Ty&;
    using const_reference_type = const _Ty&;

    RefPtr() noexcept
        : ptr_(nullptr)
    {
    }

    RefPtr(std::nullptr_t) noexcept
        : ptr_(nullptr)
    {
    }

    RefPtr(pointer_type p)
        : ptr_(p)
    {
        _ProxyTy::Retain(ptr_);
    }

    RefPtr(const RefPtr& other)
        : ptr_(other.ptr_)
    {
        _ProxyTy::Retain(ptr_);
    }

    RefPtr(RefPtr&& other) noexcept
        : ptr_(nullptr)
    {
        Swap(other);
    }

    ~RefPtr()
    {
        Tidy();
    }

    template <typename _UTy, typename std::enable_if<std::is_convertible<_UTy*, _Ty*>::value, int>::type = 0>
    RefPtr(const RefPtr<_UTy, _ProxyTy>& other)
    {
        ptr_ = dynamic_cast<pointer_type>(other.Get());
        _ProxyTy::Retain(ptr_);
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
            RefPtr(ptr).Swap(*this);
        else
            Tidy();
    }

    inline void Swap(RefPtr& other) noexcept
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

    inline RefPtr& operator=(const RefPtr& other)
    {
        if (other.ptr_ != ptr_)
            RefPtr(other).Swap(*this);
        return (*this);
    }

    inline RefPtr& operator=(RefPtr&& other) noexcept
    {
        if (other.ptr_ != ptr_)
            other.Swap(*this);
        return (*this);
    }

    inline RefPtr& operator=(pointer_type p)
    {
        if (p != ptr_)
            RefPtr(p).Swap(*this);
        return (*this);
    }

    template <typename _UTy, typename std::enable_if<std::is_convertible<_UTy*, _Ty*>::value, int>::type = 0>
    inline RefPtr& operator=(const RefPtr<_UTy, _ProxyTy>& other)
    {
        if (other.Get() != ptr_)
            RefPtr(dynamic_cast<pointer_type>(other.Get())).Swap(*this);
        return (*this);
    }

    inline RefPtr& operator=(std::nullptr_t)
    {
        Tidy();
        return *this;
    }

private:
    void Tidy()
    {
        _ProxyTy::Release(ptr_);
        ptr_ = nullptr;
    }

private:
    pointer_type ptr_;
};

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator==(const RefPtr<_Ty, _ProxyTy>& lhs, const RefPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(const RefPtr<_Ty, _ProxyTy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator==(_Ty* lhs, const RefPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return lhs == rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator==(const RefPtr<_Ty, _ProxyTy>& lhs, std::nullptr_t) noexcept
{
    return !static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator==(std::nullptr_t, const RefPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return !static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator!=(const RefPtr<_Ty, _ProxyTy>& lhs, const RefPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(const RefPtr<_Ty, _ProxyTy>& lhs, _Ty* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(_Ty* lhs, const RefPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(const RefPtr<_Ty, _ProxyTy>& lhs, std::nullptr_t) noexcept
{
    return static_cast<bool>(lhs);
}

template <class _Ty, class _ProxyTy>
inline bool operator!=(std::nullptr_t, const RefPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    return static_cast<bool>(rhs);
}

template <class _Ty, class _UTy, class _ProxyTy>
inline bool operator<(const RefPtr<_Ty, _ProxyTy>& lhs, const RefPtr<_UTy, _ProxyTy>& rhs) noexcept
{
    return lhs.Get() < rhs.Get();
}

// template class cannot specialize std::swap,
// so implement a swap function in kiwano namespace
template <class _Ty, class _ProxyTy>
inline void swap(RefPtr<_Ty, _ProxyTy>& lhs, RefPtr<_Ty, _ProxyTy>& rhs) noexcept
{
    lhs.Swap(rhs);
}

template <typename _Ty, typename... _Args>
inline RefPtr<_Ty> MakePtr(_Args&&... args)
{
    static_assert(std::is_base_of<RefObject, _Ty>::value, "_Ty must be derived from RefObject");

    RefPtr<_Ty> ptr;

    _Ty** pptr = ptr.GetAddressOfAndRelease();
    (*pptr)    = new _Ty(std::forward<_Args>(args)...);
    return ptr;
}

}  // namespace kiwano

#ifndef KGE_DECLARE_SMART_PTR
#define KGE_DECLARE_SMART_PTR(CLASS) \
    class CLASS;                     \
    typedef ::kiwano::RefPtr<CLASS> CLASS##Ptr;
#endif
