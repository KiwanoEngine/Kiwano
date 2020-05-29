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
template <typename _Ty>
inline void* Alloc()
{
    return memory::GetAllocator()->Alloc(sizeof(_Ty));
}

/// \~chinese
/// @brief ʹ�õ�ǰ�ڴ�������ͷ��ڴ�
inline void Free(void* ptr, size_t size)
{
    return memory::GetAllocator()->Free(ptr, size);
}

/// \~chinese
/// @brief �������
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
/// @brief ʹ�õ�ǰ�ڴ��������������
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
/// @brief ȫ���ڴ��������ʹ��malloc��free�����ڴ�
class KGE_API GlobalAllocator : public MemoryAllocator
{
public:
    /// \~chinese
    /// @brief �����ڴ�
    virtual void* Alloc(size_t size) override;

    /// \~chinese
    /// @brief �ͷ��ڴ�
    virtual void Free(void* ptr, size_t size) override;
};

/// \~chinese
/// @brief ��ȡȫ���ڴ������
GlobalAllocator* GetGlobalAllocator();

}  // namespace memory
}  // namespace kiwano
