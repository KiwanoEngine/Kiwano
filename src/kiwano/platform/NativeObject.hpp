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
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 含有本地指针的对象
 */
class KGE_API NativeObject : public ObjectBase
{
public:
    NativeObject() = default;

    const Any& GetNative() const;

    template <class _Ty>
    _Ty GetNative() const;

    void SetNative(const Any& native);

    void ResetNative();

    bool IsValid() const override;

protected:
    Any native_;
};

inline const Any& NativeObject::GetNative() const
{
    return native_;
}

template <class _Ty>
inline _Ty NativeObject::GetNative() const
{
    if (native_.HasValue())
    {
        return native_.Cast<_Ty>();
    }
    return _Ty{};
}

inline void NativeObject::SetNative(const Any& native)
{
    native_ = native;
}

inline void NativeObject::ResetNative()
{
    native_.Clear();
}

inline bool NativeObject::IsValid() const
{
    return native_.HasValue() && ObjectBase::IsValid();
}

}  // namespace kiwano
