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
class Module;

/// \~chinese
/// @brief 模块列表
typedef Vector<Module*> ModuleList;

/**
 * \addtogroup Module
 * @{
 */

/// \~chinese
/// @brief 模块上下文
class KGE_API ModuleContext
{
public:
    void Next();

    void Abort();

protected:
    ModuleContext(const ModuleList& modules);

    virtual ~ModuleContext();

    virtual void Handle(Module* m) = 0;

    void ResetIndex();

private:
    int               index_;
    const ModuleList& modules_;
};

/// \~chinese
/// @brief 更新模块上下文
class KGE_API UpdateModuleContext : public ModuleContext
{
public:
    Duration dt;

    UpdateModuleContext(const ModuleList& modules, Duration dt);

protected:
    void Handle(Module* m) override;
};

/// \~chinese
/// @brief 渲染模块上下文
class KGE_API RenderModuleContext : public ModuleContext
{
public:
    RenderContext& render_ctx;

    RenderModuleContext(const ModuleList& modules, RenderContext& ctx);

    virtual ~RenderModuleContext();

protected:
    void Handle(Module* m) override;

private:
    enum class Step
    {
        Before,
        Rendering,
        After,
    };

    Step step_;
};

/** @} */

}  // namespace kiwano
