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
#include <kiwano/render/NativeObject.h>
#include <kiwano/platform/win32/ComPtr.hpp>
#include <unknwn.h>

namespace kiwano
{

class KGE_API NativePtr
{
public:
    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
    static inline ComPtr<_Ty> Get(const NativeObject* object)
    {
        if (object)
        {
            ComPtr<IUnknown> ptr = object->GetNativePointer<ComPtr<IUnknown>>();
            if (ptr)
            {
                ComPtr<_Ty> native;
                if (SUCCEEDED(ptr->QueryInterface<_Ty>(&native)))
                    return native;
            }
        }
        return nullptr;
    }

    template <typename _Ty>
    static inline ComPtr<_Ty> Get(const NativeObject& object)
    {
        return NativePtr::Get<_Ty>(&object);
    }

    template <typename _Ty>
    static inline ComPtr<_Ty> Get(NativeObjectPtr object)
    {
        return NativePtr::Get<_Ty>(object.Get());
    }

    static inline void Set(NativeObject* object, ComPtr<IUnknown> com_ptr)
    {
        if (object)
        {
            object->SetNativePointer(com_ptr);
        }
    }

    static inline void Set(NativeObject& object, ComPtr<IUnknown> com_ptr)
    {
        NativePtr::Set(&object, com_ptr);
    }

    static inline void Set(NativeObjectPtr object, ComPtr<IUnknown> com_ptr)
    {
        NativePtr::Set(object.Get(), com_ptr);
    }
};

}  // namespace kiwano
