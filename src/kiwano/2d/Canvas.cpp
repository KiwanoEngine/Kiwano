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
#include <kiwano/core/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

CanvasPtr Canvas::Create(Size const& size)
{
    CanvasPtr ptr = new (std::nothrow) Canvas;
    if (ptr)
    {
        try
        {
            ptr->ctx_          = TextureRenderContext::Create();
            ptr->stroke_brush_ = Brush::Create(Color::White);
            ptr->fill_brush_   = Brush::Create(Color::White);

            ptr->SetSize(ptr->ctx_->GetSize());
        }
        catch (std::exception)
        {
            return nullptr;
        }
    }
    return ptr;
}

Canvas::Canvas()
    : cache_expired_(false)
    , stroke_width_(1.0f)
    , stroke_style_()
{
}

void Canvas::BeginDraw()
{
    KGE_ASSERT(ctx_);
    ctx_->BeginDraw();
}

void Canvas::EndDraw()
{
    KGE_ASSERT(ctx_);
    ctx_->EndDraw();
    cache_expired_ = true;
}

void Canvas::OnRender(RenderContext& ctx)
{
    UpdateCache();

    if (texture_cached_ && texture_cached_->IsValid())
    {
        PrepareToRender(ctx);
        ctx.DrawTexture(*texture_cached_, nullptr, &GetBounds());
    }
}

void Canvas::SetBrush(BrushPtr brush)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(brush);
}

void Canvas::SetBrushTransform(Transform const& transform)
{
    KGE_ASSERT(ctx_);
    ctx_->SetTransform(transform.ToMatrix());
}

void Canvas::SetBrushTransform(Matrix3x2 const& transform)
{
    KGE_ASSERT(ctx_);
    ctx_->SetTransform(transform);
}

void Canvas::PushLayer(Layer& layer)
{
    KGE_ASSERT(ctx_);
    ctx_->PushLayer(layer);
}

void Canvas::PopLayer()
{
    KGE_ASSERT(ctx_);
    ctx_->PopLayer();
}

void Canvas::PushClipRect(Rect const& clip_rect)
{
    KGE_ASSERT(ctx_);
    ctx_->PushClipRect(clip_rect);
}

void Canvas::PopClipRect()
{
    KGE_ASSERT(ctx_);
    ctx_->PopClipRect();
}

void Canvas::DrawShape(ShapePtr shape)
{
    KGE_ASSERT(ctx_);
    if (shape)
    {
        ctx_->SetCurrentBrush(stroke_brush_);
        ctx_->DrawShape(*shape, stroke_style_, stroke_width_);
        cache_expired_ = true;
    }
}

void Canvas::DrawLine(Point const& begin, Point const& end)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawLine(begin, end, stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::DrawCircle(Point const& center, float radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawEllipse(center, Vec2(radius, radius), stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::DrawEllipse(Point const& center, Vec2 const& radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawEllipse(center, radius, stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::DrawRect(Rect const& rect)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawRectangle(rect, stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::DrawRoundedRect(Rect const& rect, Vec2 const& radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawRoundedRectangle(rect, radius, stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::FillShape(ShapePtr shape)
{
    KGE_ASSERT(ctx_);
    if (shape)
    {
        ctx_->SetCurrentBrush(fill_brush_);
        ctx_->FillShape(*shape);
        cache_expired_ = true;
    }
}

void Canvas::FillCircle(Point const& center, float radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillEllipse(center, Vec2(radius, radius));
    cache_expired_ = true;
}

void Canvas::FillEllipse(Point const& center, Vec2 const& radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillEllipse(center, radius);
    cache_expired_ = true;
}

void Canvas::FillRect(Rect const& rect)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillRectangle(rect);
    cache_expired_ = true;
}

void Canvas::FillRoundedRect(Rect const& rect, Vec2 const& radius)
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillRoundedRectangle(rect, radius);
    cache_expired_ = true;
}

void Canvas::DrawTexture(TexturePtr texture, const Rect* src_rect, const Rect* dest_rect)
{
    KGE_ASSERT(ctx_);
    if (texture)
    {
        ctx_->DrawTexture(*texture, src_rect, dest_rect);
        cache_expired_ = true;
    }
}

void Canvas::DrawTextLayout(String const& text, Point const& point)
{
    if (text.empty())
        return;

    TextLayout layout;
    layout.SetStyle(text_style_);
    layout.SetText(text);
    DrawTextLayout(layout, point);
}

void Canvas::DrawTextLayout(TextLayout const& layout, Point const& point)
{
    KGE_ASSERT(ctx_);
    ctx_->DrawTextLayout(layout, point);
}

void Canvas::BeginPath(Point const& begin_pos)
{
    shape_sink_.BeginPath(begin_pos);
}

void Canvas::EndPath(bool closed)
{
    shape_sink_.EndPath(closed);
}

void Canvas::AddLine(Point const& point)
{
    shape_sink_.AddLine(point);
}

void Canvas::AddLines(Vector<Point> const& points)
{
    shape_sink_.AddLines(points);
}

void Canvas::AddBezier(Point const& point1, Point const& point2, Point const& point3)
{
    shape_sink_.AddBezier(point1, point2, point3);
}

void Canvas::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
{
    shape_sink_.AddArc(point, radius, rotation, clockwise, is_small);
}

void Canvas::StrokePath()
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->DrawShape(*shape_sink_.GetShape(), stroke_style_, stroke_width_);
    cache_expired_ = true;
}

void Canvas::FillPath()
{
    KGE_ASSERT(ctx_);
    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillShape(*shape_sink_.GetShape());
    cache_expired_ = true;
}

void Canvas::Clear()
{
    KGE_ASSERT(ctx_);
    ctx_->Clear();
    cache_expired_ = true;
}

void Canvas::Clear(Color const& clear_color)
{
    KGE_ASSERT(ctx_);
    ctx_->Clear(clear_color);
    cache_expired_ = true;
}

void Canvas::ResizeAndClear(Size size)
{
    ctx_ = TextureRenderContext::Create(size);
}

TexturePtr Canvas::ExportToTexture() const
{
    UpdateCache();
    return texture_cached_;
}

void Canvas::UpdateCache() const
{
    if (cache_expired_ && ctx_)
    {
        if (!texture_cached_)
        {
            texture_cached_ = new Texture;
        }

        if (ctx_->GetOutput(*texture_cached_))
        {
            cache_expired_ = false;
        }
    }
}

}  // namespace kiwano
