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

#include <kiwano/base/Module.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

ModuleContext::ModuleContext(ModuleList& modules)
    : index_(-1)
    , modules_(modules)
{
}

ModuleContext::~ModuleContext() {}

void ModuleContext::Next()
{
    index_++;
    for (; index_ < (int)modules_.size(); index_++)
    {
        this->Handle(modules_.at(index_));
    }
}

RenderModuleContext::RenderModuleContext(ModuleList& modules, RenderContext& ctx)
    : ModuleContext(modules)
    , render_ctx(ctx)
{
    render_ctx.BeginDraw();
}

RenderModuleContext::~RenderModuleContext()
{
    render_ctx.EndDraw();
}

void RenderModuleContext::Handle(Module* m)
{
    m->OnRender(*this);
}

UpdateModuleContext::UpdateModuleContext(ModuleList& modules, Duration dt)
    : ModuleContext(modules)
    , dt(dt)
{
}

void UpdateModuleContext::Handle(Module* m)
{
    m->OnUpdate(*this);
}

EventModuleContext::EventModuleContext(ModuleList& modules, Event* evt)
    : ModuleContext(modules)
    , evt(evt)
{
}

void EventModuleContext::Handle(Module* m)
{
    m->HandleEvent(*this);
}

Module::Module() {}

void Module::SetupModule() {}

void Module::DestroyModule() {}

void Module::OnRender(RenderModuleContext& ctx)
{
    ctx.Next();
}

void Module::OnUpdate(UpdateModuleContext& ctx)
{
    ctx.Next();
}

void Module::HandleEvent(EventModuleContext& ctx)
{
    ctx.Next();
}

}  // namespace kiwano
