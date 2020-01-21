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
#include <kiwano/core/Common.h>
#include <kiwano/core/RefCounter.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief Ĭ�ϵ�����ָ�����
 */
struct DefaultSmartPtrRefProxy
{
    static inline void add_ref(RefCounter* ptr)
    {
        if (ptr)
            ptr->Retain();
    }

    static inline void release(RefCounter* ptr)
    {
        if (ptr)
            ptr->Release();
    }
};

/**
 * \~chinese
 * @brief ����ָ��
 */
template <typename _Ty>
using SmartPtr = IntrusivePtr<_Ty, DefaultSmartPtrRefProxy>;

}  // namespace kiwano

#ifndef KGE_DECLARE_SMART_PTR
#define KGE_DECLARE_SMART_PTR(CLASS) \
    class CLASS;                     \
    typedef ::kiwano::SmartPtr<CLASS> CLASS##Ptr;
#endif
