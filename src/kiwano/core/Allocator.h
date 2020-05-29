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
#include <limits>  // std::numeric_limits
#include <memory>  // std::addressof
#include <kiwano/macros.h>

namespace kiwano
{
namespace memory
{

/// \~chinese
/// @brief �ڴ������
class KGE_API MemoryAllocator
{
public:
    /// \~chinese
    /// @brief �����ڴ�
    virtual void* Alloc(size_t size) = 0;

    /// \~chinese
    /// @brief �ͷ��ڴ�
    virtual void Free(void* ptr, size_t size) = 0;
};

/// \~chinese
/// @brief ��ȡ��ǰ�ڴ������
MemoryAllocator* GetAllocator();

/// \~chinese
/// @brief ���õ�ǰ�ڴ������
void SetAllocator(MemoryAllocator* allocator);

/// \~chinese
/// @brief ʹ�õ�ǰ�ڴ�����������ڴ�
inline void* Alloc(size_t size)
{
    return memory::GetAllocator()->Alloc(size);
}

/// \~chinese
/// @brief ʹ�õ�ǰ�ڴ�������ͷ��ڴ�
inline void Free(void* ptr, size_t size)
{
    memory::GetAllocator()->Free(ptr, size);
}

/// \~chinese
/// @brief �������
template <typename _Ty, typename... _Args>
inline _Ty* Construct(void* ptr, _Args&&... args)
{
    return ::new (ptr) _Ty(std::forward<_Args>(args)...);
}

/// \~chinese
/// @brief ���ٶ���
template <typename _Ty, typename... _Args>
inline void Destroy(_Ty* ptr)
{
    ptr->~_Ty();
}

/// \~chinese
/// @brief ʹ�õ�ǰ�ڴ��������������
template <typename _Ty, typename... _Args>
inline _Ty* New(_Args&&... args)
{
    void* ptr = memory::Alloc(sizeof(_Ty));
    if (ptr)
    {
        return memory::Construct<_Ty>(ptr, std::forward<_Args>(args)...);
    }
    return nullptr;
}

/// \~chinese
/// @brief ʹ�õ�ǰ�ڴ���������ٶ���
template <typename _Ty>
inline void Delete(_Ty* ptr)
{
    if (ptr)
    {
        memory::Destroy<_Ty>(ptr);
        memory::Free(ptr, sizeof(_Ty));
    }
}


/// \~chinese
/// @brief ������
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
        memory::Free(ptr, sizeof(_Ty) * count);
    }

    template <typename _UTy, typename... _Args>
    inline void construct(_UTy* ptr, _Args&&... args)
    {
        memory::Construct<_UTy>(ptr, std::forward<_Args>(args)...);
    }

    template <typename _UTy>
    inline void destroy(_UTy* ptr)
    {
        memory::Destroy<_UTy>(ptr);
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

}  // namespace memory
}  // namespace kiwano
