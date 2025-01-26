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
#include <kiwano/module/ModuleContext.h>

namespace kiwano
{

/**
 * \~chinese
 * \defgroup Module ģ��
 */

/**
 * \addtogroup Module
 * @{
 */

/**
 * \~chinese
 * @brief ����ģ��
 */
class KGE_API Module : Noncopyable
{
public:
    /// \~chinese
    /// @brief ����ģ��
    virtual void SetupModule();

    /// \~chinese
    /// @brief ����ģ��
    virtual void DestroyModule();

    /// \~chinese
    /// @brief ����ʱ
    /// @param ctx ����������
    virtual void OnUpdate(UpdateModuleContext& ctx);

    /// \~chinese
    /// @brief ��Ⱦǰ
    /// @param ctx ��Ⱦ������
    virtual void BeforeRender(RenderModuleContext& ctx);

    /// \~chinese
    /// @brief ��Ⱦʱ
    /// @param ctx ��Ⱦ������
    virtual void OnRender(RenderModuleContext& ctx);

    /// \~chinese
    /// @brief ��Ⱦ��
    /// @param ctx ��Ⱦ������
    virtual void AfterRender(RenderModuleContext& ctx);

protected:
    Module() = default;
};

/** @} */

inline void Module::SetupModule() {}

inline void Module::DestroyModule() {}

inline void Module::OnRender(RenderModuleContext& ctx)
{
    KGE_NOT_USED(ctx);
}

inline void Module::OnUpdate(UpdateModuleContext& ctx)
{
    KGE_NOT_USED(ctx);
}

inline void Module::BeforeRender(RenderModuleContext& ctx)
{
    KGE_NOT_USED(ctx);
}

inline void Module::AfterRender(RenderModuleContext& ctx)
{
    KGE_NOT_USED(ctx);
}

}  // namespace kiwano
