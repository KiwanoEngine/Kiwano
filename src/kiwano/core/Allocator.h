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
#include <utility>  // std::forward
#include <limits>   // std::numeric_limits
#include <memory>   // std::addressof
#include <kiwano/macros.h>

namespace kiwano
{
namespace memory
{

/// \~chinese
/// @brief 内存分配器
class KGE_API MemoryAllocator
{
public:
    /// \~chinese
    /// @brief 申请内存
    virtual void* Alloc(size_t size) = 0;

    /// \~chinese
    /// @brief 释放内存
    virtual void Free(void* ptr) = 0;
};

/// \~chinese
/// @brief 获取当前内存分配器
MemoryAllocator* GetAllocator();

/// \~chinese
/// @brief 设置当前内存分配器
void SetAllocator(MemoryAllocator* allocator);

/// \~chinese
/// @brief 使用当前内存分配器分配内存
inline void* Alloc(size_t size)
{
    return memory::GetAllocator()->Alloc(size);
}

/// \~chinese
/// @brief 使用当前内存分配器释放内存
inline void Free(void* ptr)
{
    memory::GetAllocator()->Free(ptr);
}

}  // namespace memory

/// \~chinese
/// @brief 分配器
template <typename _Ty>
class Allocator
{
public:
    typedef _Ty        value_type;
    typedef _Ty*       pointer;
    typedef const _Ty* const_pointer;
    typedef _Ty&       reference;
    typedef const _Ty& const_reference;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <class _Other>
    struct rebind
    {
        using other = Allocator<_Other>;
    };

    Allocator() noexcept {}

    Allocator(const Allocator&) noexcept = default;

    template <class _Other>
    Allocator(const Allocator<_Other>&) noexcept
    {
    }

    inline _Ty* allocate(size_t count)
    {
        if (count > 0)
        {
            return static_cast<_Ty*>(memory::Alloc(sizeof(_Ty) * count));
        }
        return nullptr;
    }

    inline void* allocate(size_t count, const void*)
    {
        return allocate(count);
    }

    inline void deallocate(void* ptr, size_t count)
    {
        memory::Free(ptr /*, sizeof(_Ty) * count */);
    }

    template <typename _UTy, typename... _Args>
    inline void construct(_UTy* const ptr, _Args&&... args)
    {
        ::new (const_cast<void*>(static_cast<const volatile void*>(ptr))) _Ty(std::forward<_Args>(args)...);
    }

    template <typename _UTy>
    inline void destroy(_UTy* ptr)
    {
        ptr->~_UTy();
    }

    size_t max_size() const noexcept
    {
        return std::numeric_limits<size_t>::max() / sizeof(_Ty);
    }

    _Ty* address(_Ty& val) const noexcept
    {
        return std::addressof(val);
    }

    const _Ty* address(const _Ty& val) const noexcept
    {
        return std::addressof(val);
    }
};

// Allocator<void>
template <>
class Allocator<void>
{
public:
    using value_type = void;
    typedef void*       pointer;
    typedef const void* const_pointer;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <class _Other>
    struct rebind
    {
        using other = Allocator<_Other>;
    };
};

template <class _Ty, class _Other>
bool operator==(const Allocator<_Ty>&, const Allocator<_Other>&) noexcept
{
    return true;
}

template <class _Ty, class _Other>
bool operator!=(const Allocator<_Ty>&, const Allocator<_Other>&) noexcept
{
    return false;
}

}  // namespace kiwano
