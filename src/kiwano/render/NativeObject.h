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

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/D2DDeviceResources.h>
#endif

namespace kiwano
{

KGE_DECLARE_SMART_PTR(NativeObject);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 含有本机指针的对象
 */
class KGE_API NativeObjectBase : public ObjectBase
{
public:
    NativeObjectBase();

    bool IsValid() const override;

    void* GetNativePointer() const;

    template <typename _NativeTy>
    _NativeTy* GetNativePointer() const;

    virtual void ResetNativePointer(void* native_pointer = nullptr);

protected:
    void* native_pointer_;
};

#if defined(KGE_PLATFORM_WINDOWS)

class KGE_API NativeObject : public NativeObjectBase
{
public:
    virtual ~NativeObject();

    void ResetNativePointer(void* native_pointer = nullptr) override;
};

#else

typedef NativeObjectBase NativeObject;

#endif

/** @} */

inline void* NativeObjectBase::GetNativePointer() const
{
    return native_pointer_;
}

template <typename _NativeTy>
inline _NativeTy* NativeObjectBase::GetNativePointer() const
{
    if (native_pointer_ != nullptr)
    {
        return static_cast<_NativeTy*>(native_pointer_);
    }
    return nullptr;
}

}  // namespace kiwano
