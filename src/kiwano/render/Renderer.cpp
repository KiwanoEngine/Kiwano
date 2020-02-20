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

#include <kiwano/render/Renderer.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/platform/Application.h>

namespace kiwano
{

Renderer::Renderer()
    : vsync_(true)
    , clear_color_(Color::Black)
{
}

void Renderer::SetupModule()
{
    WindowPtr window = Application::GetInstance().GetMainWindow();
    MakeContextForWindow(window);
}

void Renderer::DestroyModule()
{
    Destroy();
}

void Renderer::HandleEvent(Event* evt)
{
    if (evt->IsType<WindowResizedEvent>())
    {
        auto window_evt = dynamic_cast<WindowResizedEvent*>(evt);
        Resize(window_evt->width, window_evt->height);
    }
}

void Renderer::BeginDraw()
{
    KGE_ASSERT(render_ctx_);

    render_ctx_->BeginDraw();
}

void Renderer::EndDraw()
{
    KGE_ASSERT(render_ctx_);

    render_ctx_->EndDraw();
}

}  // namespace kiwano
