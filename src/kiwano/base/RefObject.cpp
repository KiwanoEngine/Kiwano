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
#include <kiwano/base/ObjectPool.h>

namespace kiwano
{

autogc_t const autogc;

RefObject::RefObject()
    : ref_count_(1)
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

void RefObject::AutoRelease()
{
    ObjectPool::GetInstance().AddObject(this);
}

void* RefObject::operator new(std::size_t size)
{
    return memory::Alloc(size);
}

void* RefObject::operator new(std::size_t size, autogc_t const&)
{
    void* ptr = memory::Alloc(size);
    if (ptr)
    {
        ObjectPool::GetInstance().AddObject((ObjectBase*)ptr);
    }
    return ptr;
}

void RefObject::operator delete(void* ptr)
{
    memory::Free(ptr);
}

void RefObject::operator delete(void* ptr, autogc_t const&)
{
    memory::Free(ptr);
}

}  // namespace kiwano
