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
#include <kiwano/core/Time.h>

namespace kiwano
{
class RenderContext;
class Event;

template <typename _CompTy>
struct ModuleFlag;

/**
 * \~chinese
 * @brief 基础模块
 */
class KGE_API Module
{
public:
    /// \~chinese
    /// @brief 启动模块
    virtual void SetupModule() {}

    /// \~chinese
    /// @brief 销毁模块
    virtual void DestroyModule() {}

    template <typename _CompTy>
    _CompTy* Cast()
    {
        if (flag_ & ModuleFlag<_CompTy>::value)
            return dynamic_cast<_CompTy*>(this);
        return nullptr;
    }

protected:
    Module();

protected:
    int flag_;
};

/**
 * \~chinese
 * @brief 渲染模块
 */
class KGE_API RenderModule : public virtual Module
{
public:
    /// \~chinese
    /// @brief 渲染前
    virtual void BeforeRender() {}

    /// \~chinese
    /// @brief 渲染时
    /// @param ctx 渲染上下文
    virtual void OnRender(RenderContext& ctx) {}

    /// \~chinese
    /// @brief 渲染后
    virtual void AfterRender() {}

public:
    RenderModule();
};

/**
 * \~chinese
 * @brief 更新模块
 */
class KGE_API UpdateModule : public virtual Module
{
public:
    /// \~chinese
    /// @brief 更新前
    virtual void BeforeUpdate() {}

    /// \~chinese
    /// @brief 更新时
    /// @param dt 间隔时间
    virtual void OnUpdate(Duration dt) {}

    /// \~chinese
    /// @brief 更新后
    virtual void AfterUpdate() {}

public:
    UpdateModule();
};

/**
 * \~chinese
 * @brief 事件模块
 */
class KGE_API EventModule : public virtual Module
{
public:
    /// \~chinese
    /// @brief 事件处理
    /// @param evt 事件
    virtual void HandleEvent(Event* evt) {}

public:
    EventModule();
};

#define KGE_DEFINE_COMPONENT_FLAG(OFFSET) (0x01 << (OFFSET % 32))

template <>
struct ModuleFlag<RenderModule>
{
    static constexpr int value = KGE_DEFINE_COMPONENT_FLAG(0);
};

template <>
struct ModuleFlag<UpdateModule>
{
    static constexpr int value = KGE_DEFINE_COMPONENT_FLAG(1);
};

template <>
struct ModuleFlag<EventModule>
{
    static constexpr int value = KGE_DEFINE_COMPONENT_FLAG(2);
};

#undef KGE_DEFINE_COMPONENT_FLAG

}  // namespace kiwano
