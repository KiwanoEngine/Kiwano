// Copyright (c) 2016-2019 Kiwano - Nomango
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

#include <kiwano/render/RenderContext.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

RefPtr<RenderContext> RenderContext::Create(RefPtr<Texture> texture, const PixelSize& size)
{
    return Renderer::GetInstance().CreateTextureRenderContext(texture, size);
}

RenderContext::RenderContext()
    : collecting_status_(false)
    , brush_opacity_(1.0f)
    , antialias_(true)
    , text_antialias_(TextAntialiasMode::GrayScale)
{
}

void RenderContext::BeginDraw()
{
    if (collecting_status_)
    {
        status_.start      = Time::Now();
        status_.primitives = 0;
    }
}

void RenderContext::EndDraw()
{
    if (collecting_status_)
    {
        status_.duration = Time::Now() - status_.start;
    }
}

void RenderContext::SetCollectingStatus(bool enable)
{
    collecting_status_ = enable;
}

void RenderContext::IncreasePrimitivesCount(uint32_t increase) const
{
    if (collecting_status_)
    {
        status_.primitives += increase;
    }
}

float RenderContext::GetBrushOpacity() const
{
    return brush_opacity_;
}

RefPtr<Brush> RenderContext::GetCurrentBrush() const
{
    return current_brush_;
}

void RenderContext::SetBrushOpacity(float opacity)
{
    brush_opacity_ = opacity;
}

void RenderContext::SetCurrentBrush(RefPtr<Brush> brush)
{
    current_brush_ = brush;
}

void RenderContext::SetCurrentStrokeStyle(RefPtr<StrokeStyle> stroke)
{
    current_stroke_ = stroke;
}

void RenderContext::DrawCircle(const Point& center, float radius)
{
    this->DrawEllipse(center, Vec2(radius, radius));
}

void RenderContext::FillCircle(const Point& center, float radius)
{
    this->FillEllipse(center, Vec2(radius, radius));
}

}  // namespace kiwano
