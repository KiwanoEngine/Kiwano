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

#include <kiwano/core/Allocator.h>

namespace kiwano
{
namespace memory
{

MemoryAllocator* current_allocator_ = nullptr;

MemoryAllocator* GetGlobalAllocator()
{
    class KGE_API GlobalAllocator : public MemoryAllocator
    {
    public:
        virtual void* Alloc(size_t size) override
        {
            return ::operator new(size);
        }

        virtual void Free(void* ptr) override
        {
            ::operator delete(ptr);
        }
    };

    static GlobalAllocator global_allocator;
    return &global_allocator;
}

MemoryAllocator* GetAllocator()
{
    if (!current_allocator_)
    {
        current_allocator_ = GetGlobalAllocator();
    }
    return current_allocator_;
}

void SetAllocator(MemoryAllocator* allocator)
{
    KGE_ASSERT(allocator);
    current_allocator_ = allocator;
}

}  // namespace memory
}  // namespace kiwano
