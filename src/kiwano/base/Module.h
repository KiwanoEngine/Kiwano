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
class Module;

/// \~chinese
/// @brief 模块列表
typedef Vector<Module*> ModuleList;

/// \~chinese
/// @brief 模块上下文
class KGE_API ModuleContext
{
public:
    void Next();

protected:
    ModuleContext(ModuleList& modules);

    virtual ~ModuleContext();

    virtual void Handle(Module* m) = 0;

private:
    int         index_;
    ModuleList& modules_;
};

/// \~chinese
/// @brief 渲染模块上下文
class KGE_API RenderModuleContext : public ModuleContext
{
public:
    RenderContext& render_ctx;

    RenderModuleContext(ModuleList& modules, RenderContext& ctx);

    virtual ~RenderModuleContext();

protected:
    void Handle(Module* m) override;
};

/// \~chinese
/// @brief 更新模块上下文
class KGE_API UpdateModuleContext : public ModuleContext
{
public:
    Duration dt;

    UpdateModuleContext(ModuleList& modules, Duration dt);

protected:
    void Handle(Module* m) override;
};

/// \~chinese
/// @brief 时间模块上下文
class KGE_API EventModuleContext : public ModuleContext
{
public:
    Event* evt;

    EventModuleContext(ModuleList& modules, Event* evt);

protected:
    void Handle(Module* m) override;
};

/**
 * \~chinese
 * @brief 基础模块
 */
class KGE_API Module : Noncopyable
{
public:
    /// \~chinese
    /// @brief 启动模块
    virtual void SetupModule();

    /// \~chinese
    /// @brief 销毁模块
    virtual void DestroyModule();

    /// \~chinese
    /// @brief 渲染时
    /// @param ctx 渲染上下文
    virtual void OnRender(RenderModuleContext& ctx);

    /// \~chinese
    /// @brief 更新时
    /// @param ctx 更新上下文
    virtual void OnUpdate(UpdateModuleContext& ctx);

    /// \~chinese
    /// @brief 事件处理
    /// @param ctx 事件上下文
    virtual void HandleEvent(EventModuleContext& ctx);

protected:
    Module();
};

}  // namespace kiwano
