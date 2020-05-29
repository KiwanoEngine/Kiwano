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
    virtual void Free(void* ptr, size_t size) = 0;
};

/// \~chinese
/// @brief 获取当前内存分配器
MemoryAllocator* GetAllocator();

/// \~chinese
/// @brief 设置当前内存分配器
void SetAllocator(MemoryAllocator* allocator);

/// \~chinese
/// @brief 使用当前内存分配器分配内存
template <typename _Ty>
inline void* Alloc()
{
    return memory::GetAllocator()->Alloc(sizeof(_Ty));
}

/// \~chinese
/// @brief 使用当前内存分配器释放内存
inline void Free(void* ptr, size_t size)
{
    return memory::GetAllocator()->Free(ptr, size);
}

/// \~chinese
/// @brief 构造对象
template <typename _Ty>
inline _Ty* Construct(void* addr)
{
    if (addr)
    {
        return ::new (addr) _Ty;
    }
    return nullptr;
}

/// \~chinese
/// @brief 构造对象
template <typename _Ty, typename... _Args>
inline _Ty* Construct(void* ptr, _Args&&... args)
{
    return ::new (ptr) _Ty(std::forward<_Args>(args)...);
}

/// \~chinese
/// @brief 销毁对象
template <typename _Ty, typename... _Args>
inline void Destroy(_Ty* ptr)
{
    ptr->~_Ty();
}

/// \~chinese
/// @brief 使用当前内存分配器创建对象
template <typename _Ty>
inline _Ty* New()
{
    void* ptr = memory::Alloc<_Ty>();
    if (ptr)
    {
        return memory::Construct<_Ty>(ptr);
    }
    return nullptr;
}

/// \~chinese
/// @brief 使用当前内存分配器创建对象
template <typename _Ty, typename... _Args>
inline _Ty* New(_Args&&... args)
{
    void* ptr = memory::Alloc<_Ty>();
    if (ptr)
    {
        return memory::Construct<_Ty>(ptr, std::forward<_Args>(args)...);
    }
    return nullptr;
}

/// \~chinese
/// @brief 使用当前内存分配器销毁对象
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
/// @brief 全局内存分配器，使用malloc和free分配内存
class KGE_API GlobalAllocator : public MemoryAllocator
{
public:
    /// \~chinese
    /// @brief 申请内存
    virtual void* Alloc(size_t size) override;

    /// \~chinese
    /// @brief 释放内存
    virtual void Free(void* ptr, size_t size) override;
};

/// \~chinese
/// @brief 获取全局内存分配器
GlobalAllocator* GetGlobalAllocator();

}  // namespace memory
}  // namespace kiwano
