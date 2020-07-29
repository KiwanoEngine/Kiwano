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

#include <kiwano/base/RefObject.h>

namespace kiwano
{

RefObject::RefObject()
    : ref_count_(0)
{
}

RefObject::~RefObject() {}

void RefObject::Retain()
{
    ++ref_count_;
}

void RefObject::Release()
{
    --ref_count_;
    if (ref_count_ == 0)
    {
        delete this;
    }
}

uint32_t RefObject::GetRefCount() const
{
    return ref_count_.load();
}

void* RefObject::operator new(size_t size)
{
    void* ptr = memory::Alloc(size);
    if (!ptr)
    {
        throw std::bad_alloc();
    }
    return ptr;
}

void RefObject::operator delete(void* ptr)
{
    memory::Free(ptr);
}

void* RefObject::operator new(size_t size, std::nothrow_t const&)
{
    try
    {
        void* ptr = memory::Alloc(size);
        return ptr;
    }
    catch (...)
    {
    }
    return nullptr;
}

void RefObject::operator delete(void* ptr, std::nothrow_t const&)
{
    try
    {
        memory::Free(ptr);
    }
    catch (...)
    {
    }
}

void* RefObject::operator new(size_t size, void* ptr)
{
    return ::operator new(size, ptr);
}

void RefObject::operator delete(void* ptr, void* place) noexcept
{
    ::operator delete(ptr, place);
}

}  // namespace kiwano
