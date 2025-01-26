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

#include <kiwano/actor/Canvas.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{
Canvas::Canvas()
{
    render_comp_ = new TextureRenderAbility;
    AddComponent(render_comp_);
}

Canvas::Canvas(const PixelSize& size)
    : Canvas()
{
    ResizeAndClear(size);
}

RefPtr<CanvasRenderContext> Canvas::GetContext2D() const
{
    RefPtr<CanvasRenderContext> ctx = new CanvasRenderContext(render_ctx_);
    return ctx;
}

void Canvas::ResizeAndClear(const PixelSize& size)
{
    texture_cached_ = MakePtr<Texture>();
    render_ctx_     = RenderContext::Create(texture_cached_, size);

    render_comp_->SetTexture(texture_cached_);
    if (render_ctx_)
    {
        SetSize(render_ctx_->GetSize());
    }
    else
    {
        Fail("Canvas::ResizeAndClear failed");
    }
}

CanvasRenderContext::CanvasRenderContext(RefPtr<RenderContext> ctx)
    : ctx_(ctx)
{
    if (!ctx_)
    {
        Fail("CanvasRenderContext constructor failed");
    }
}

CanvasRenderContext::~CanvasRenderContext() {}

}  // namespace kiwano
