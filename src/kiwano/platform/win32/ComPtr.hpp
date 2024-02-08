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
#include <type_traits>
#include <kiwano/core/Common.h>
#include <kiwano/core/RefBasePtr.hpp>
#include <kiwano/platform/NativeObject.hpp>
#include <Unknwnbase.h>

namespace kiwano
{
struct ComRefPolicy
{
    inline void Retain(IUnknown* ptr)
    {
        if (ptr)
            ptr->AddRef();
    }

    inline void Release(IUnknown* ptr)
    {
        if (ptr)
            ptr->Release();
    }
};

// ComPtr<> is a smart pointer for COM
template <typename _Ty, typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
using ComPtr = RefBasePtr<_Ty, ComRefPolicy>;

struct ComPolicy
{
    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
    static inline ComPtr<_Ty> Get(const NativeObject* object)
    {
        if (object)
        {
            const auto& native = object->GetNative();
            if (native.HasValue())
            {
                auto ptr = native.Cast<ComPtr<IUnknown>>();
                if (ptr)
                {
                    ComPtr<_Ty> native;
                    if (SUCCEEDED(ptr->QueryInterface<_Ty>(&native)))
                        return native;
                }
            }
        }
        return nullptr;
    }

    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
    static inline ComPtr<_Ty> Get(const NativeObject& object)
    {
        return ComPolicy::Get<_Ty>(&object);
    }

    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
    static inline ComPtr<_Ty> Get(RefPtr<NativeObject> object)
    {
        return ComPolicy::Get<_Ty>(object.Get());
    }

    static inline void Set(NativeObject* object, ComPtr<IUnknown> com_ptr)
    {
        if (object)
        {
            object->SetNative(Any{ com_ptr });
        }
    }

    static inline void Set(NativeObject* object, IUnknown* com_ptr)
    {
        ComPolicy::Set(object, ComPtr<IUnknown>(com_ptr));
    }

    static inline void Set(NativeObject& object, IUnknown* com_ptr)
    {
        ComPolicy::Set(&object, com_ptr);
    }

    static inline void Set(RefPtr<NativeObject> object, IUnknown* com_ptr)
    {
        ComPolicy::Set(object.Get(), com_ptr);
    }

    static inline void Set(NativeObject& object, ComPtr<IUnknown> com_ptr)
    {
        ComPolicy::Set(&object, com_ptr.Get());
    }

    static inline void Set(RefPtr<NativeObject> object, ComPtr<IUnknown> com_ptr)
    {
        ComPolicy::Set(object.Get(), com_ptr.Get());
    }
};

}  // namespace kiwano
