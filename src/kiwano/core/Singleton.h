// Copyright (c) 2016-2020 Kiwano - Nomango
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
#include <memory>

namespace kiwano
{

template <typename _Ty>
class Singleton
{
protected:
    Singleton()                 = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    struct InstanceCreator
    {
        InstanceCreator()
        {
            (void)Singleton<_Ty>::GetInstancePtr();
        }

        inline void Dummy() const {}
    };
    static InstanceCreator creator_;

public:
    using object_type = _Ty;

    static std::unique_ptr<object_type> instance_ptr_;

    static inline object_type& GetInstance()
    {
        return *GetInstancePtr();
    }

    static inline object_type* GetInstancePtr()
    {
        creator_.Dummy();
        if (!instance_ptr_)
        {
            instance_ptr_.reset(new object_type);
        }
        return instance_ptr_.get();
    }

    static inline void DestroyInstance()
    {
        instance_ptr_.reset();
    }
};

template <typename _Ty>
typename Singleton<_Ty>::InstanceCreator Singleton<_Ty>::creator_;

template <typename _Ty>
typename std::unique_ptr<_Ty> Singleton<_Ty>::instance_ptr_;

}  // namespace kiwano
