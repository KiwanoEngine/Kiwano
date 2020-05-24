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

#include <kiwano/2d/Canvas.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

CanvasPtr Canvas::Create(const Size& size)
{
    void*     mem = memory::Alloc<Canvas>();
    CanvasPtr ptr = ::new (mem) Canvas;
    if (ptr)
    {
        try
        {
            ptr->ResizeAndClear(size);
        }
        catch (std::exception)
        {
            return nullptr;
        }
    }
    return ptr;
}

Canvas::Canvas() {}

RenderContextPtr Canvas::GetContext2D() const
{
    return render_ctx_;
}

void Canvas::OnRender(RenderContext& ctx)
{
    if (texture_cached_)
    {
        ctx.DrawTexture(*texture_cached_, nullptr, &GetBounds());
    }
}

void Canvas::ResizeAndClear(Size size)
{
    texture_cached_ = memory::New<Texture>();
    render_ctx_     = RenderContext::Create(*texture_cached_, size);

    SetSize(render_ctx_->GetSize());
}

TexturePtr Canvas::ExportToTexture() const
{
    return texture_cached_;
}

}  // namespace kiwano
