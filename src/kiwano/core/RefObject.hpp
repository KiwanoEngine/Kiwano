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
#include <atomic>
#include <kiwano/core/Common.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 引用计数器
 */
class KGE_API RefObject : protected Noncopyable
{
public:
    virtual ~RefObject() = default;

    /// \~chinese
    /// @brief 增加引用计数
    inline void Retain()
    {
        ++ref_count_;
    }

    /// \~chinese
    /// @brief 减少引用计数
    inline void Release()
    {
        --ref_count_;
        if (ref_count_ == 0)
            delete this;
    }

    /// \~chinese
    /// @brief 获取引用计数
    inline uint32_t GetRefCount() const
    {
        return ref_count_.load();
    }

    static inline void* operator new(size_t size)
    {
        void* ptr = memory::Alloc(size);
        if (!ptr)
            throw std::bad_alloc();
        return ptr;
    }

    static inline void operator delete(void* ptr)
    {
        memory::Free(ptr);
    }

    static inline void* operator new(size_t size, std::nothrow_t const&) noexcept
    {
        try
        {
            void* ptr = memory::Alloc(size);
            return ptr;
        }
        catch (...)
        {
            return nullptr;
        }
    }

    static inline void operator delete(void* ptr, std::nothrow_t const&) noexcept
    {
        try
        {
            memory::Free(ptr);
        }
        catch (...)
        {
        }
    }

    static inline void* operator new(size_t size, void* ptr) noexcept
    {
        return ::operator new(size, ptr);
    }

    static inline void operator delete(void* ptr, void* place) noexcept
    {
        ::operator delete(ptr, place);
    }

protected:
    inline RefObject()
        : ref_count_(0)
    {
    }

private:
    std::atomic<uint32_t> ref_count_;
};

}  // namespace kiwano
