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

Canvas::Canvas()
{
}

void Canvas::BeginDraw()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->BeginDraw();
}

void Canvas::EndDraw()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->EndDraw();
}

void Canvas::OnRender(RenderContext& ctx)
{
    if (texture_cached_)
    {
        ctx.DrawTexture(*texture_cached_, nullptr, &GetBounds());
    }
}

void Canvas::SetBrush(BrushPtr brush)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(brush);
}

void Canvas::SetBrushTransform(const Transform& transform)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetTransform(transform.ToMatrix());
}

void Canvas::SetBrushTransform(const Matrix3x2& transform)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetTransform(transform);
}

void Canvas::PushLayer(LayerPtr layer)
{
    KGE_ASSERT(render_ctx_);
    if (layer)
    {
        render_ctx_->PushLayer(*layer);
    }
}

void Canvas::PopLayer()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->PopLayer();
}

void Canvas::PushClipRect(const Rect& clip_rect)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->PushClipRect(clip_rect);
}

void Canvas::PopClipRect()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->PopClipRect();
}

void Canvas::DrawShape(ShapePtr shape)
{
    KGE_ASSERT(render_ctx_);
    if (shape)
    {
        render_ctx_->SetCurrentBrush(stroke_brush_);
        render_ctx_->SetCurrentStrokeStyle(stroke_style_);
        render_ctx_->DrawShape(*shape);
    }
}

void Canvas::DrawLine(const Point& begin, const Point& end)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawLine(begin, end);
}

void Canvas::DrawCircle(const Point& center, float radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawEllipse(center, Vec2(radius, radius));
}

void Canvas::DrawEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawEllipse(center, radius);
}

void Canvas::DrawRect(const Rect& rect)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawRectangle(rect);
}

void Canvas::DrawRoundedRect(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawRoundedRectangle(rect, radius);
}

void Canvas::FillShape(ShapePtr shape)
{
    KGE_ASSERT(render_ctx_);
    if (shape)
    {
        render_ctx_->SetCurrentBrush(fill_brush_);
        render_ctx_->FillShape(*shape);
    }
}

void Canvas::FillCircle(const Point& center, float radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->FillEllipse(center, Vec2(radius, radius));
}

void Canvas::FillEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->FillEllipse(center, radius);
}

void Canvas::FillRect(const Rect& rect)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->FillRectangle(rect);
}

void Canvas::FillRoundedRect(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->FillRoundedRectangle(rect, radius);
}

void Canvas::DrawFrame(FramePtr frame, const Point& pos)
{
    KGE_ASSERT(render_ctx_);
    if (frame && frame->IsValid())
    {
        render_ctx_->DrawTexture(*frame->GetTexture(), &frame->GetCropRect(), &Rect(pos, frame->GetSize()));
    }
}

void Canvas::DrawFrame(FramePtr frame, const Point& pos, const Size& size)
{
    KGE_ASSERT(render_ctx_);
    if (frame && frame->IsValid())
    {
        render_ctx_->DrawTexture(*frame->GetTexture(), &frame->GetCropRect(), &Rect(pos, size));
    }
}

void Canvas::DrawTextLayout(const String& text, const TextStyle& style, const Point& point)
{
    if (text.empty())
        return;

    DrawTextLayout(TextLayout::Create(text, style), point);
}

void Canvas::DrawTextLayout(TextLayoutPtr layout, const Point& point)
{
    KGE_ASSERT(render_ctx_);
    if (layout)
    {
        render_ctx_->DrawTextLayout(*layout, point);
    }
}

void Canvas::BeginPath(const Point& begin_pos)
{
    shape_maker_.BeginPath(begin_pos);
}

void Canvas::EndPath(bool closed)
{
    shape_maker_.EndPath(closed);
}

void Canvas::AddLine(const Point& point)
{
    shape_maker_.AddLine(point);
}

void Canvas::AddLines(const Vector<Point>& points)
{
    shape_maker_.AddLines(points);
}

void Canvas::AddBezier(const Point& point1, const Point& point2, const Point& point3)
{
    shape_maker_.AddBezier(point1, point2, point3);
}

void Canvas::AddArc(const Point& point, const Size& radius, float rotation, bool clockwise, bool is_small)
{
    shape_maker_.AddArc(point, radius, rotation, clockwise, is_small);
}

void Canvas::StrokePath()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(stroke_brush_);
    render_ctx_->SetCurrentStrokeStyle(stroke_style_);
    render_ctx_->DrawShape(*shape_maker_.GetShape());
}

void Canvas::FillPath()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->FillShape(*shape_maker_.GetShape());
}

void Canvas::Clear()
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->Clear();
}

void Canvas::Clear(const Color& clear_color)
{
    KGE_ASSERT(render_ctx_);
    render_ctx_->Clear(clear_color);
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
