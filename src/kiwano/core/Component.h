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
#include <kiwano/macros.h>

namespace kiwano
{
class RenderContext;
class Event;

/**
 * \~chinese
 * @brief 基础功能组件
 */
class KGE_API ComponentBase
{
public:
    /// \~chinese
    /// @brief 启动组件
    virtual void SetupComponent() = 0;

    /// \~chinese
    /// @brief 销毁组件
    virtual void DestroyComponent() = 0;

    bool Check(const int flag);

protected:
    ComponentBase();

protected:
    int flag_;
};

/**
 * \~chinese
 * @brief 渲染支持组件
 */
class KGE_API RenderComponent : public virtual ComponentBase
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
    static const int flag;

    RenderComponent();
};

/**
 * \~chinese
 * @brief 更新支持组件
 */
class KGE_API UpdateComponent : public virtual ComponentBase
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
    static const int flag;

    UpdateComponent();
};

/**
 * \~chinese
 * @brief 事件支持组件
 */
class KGE_API EventComponent : public virtual ComponentBase
{
public:
    /// \~chinese
    /// @brief 事件处理
    /// @param evt 事件
    virtual void HandleEvent(Event* evt) {}

public:
    static const int flag;

    EventComponent();
};
}  // namespace kiwano
